#define _GNU_SOURCE 1
#include "ahit_autosplitter.h"
#include "backend.h"
#include "types.h"
#include <livesplit_core.h>
#include <dirent.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

// 1000Hz (ish) is overkill but without throttling it can hit 3MHz, so doesn't matter
#define TIMER_SLEEP_US 1000
#define PROCFS_DIR "/proc"
#define INVALID_PID -1

#define IL_SPLIT 0

enum perms {
    READ = 1,
    WRITE = 2,
    EXECUTE = 4,
    PERM_N = 3,
};

struct map {
    size_t base;
    size_t end;
    enum perms perms;
};

// little endian
#define TIMR_MAGIC 0x524D4954
#define END_MAGIC  0x20444E45

struct __attribute__((packed)) hat_timer {
    char timr_magic[4];
    u32 state;
    double unpause_time;
    u32 game_timer_paused;
    u32 act_timer_paused;
    u32 act_timer_visible;
    u32 unpause_time_dirty;
    u32 just_got_timepiece;
    double game_time;
    double act_time;
    double real_game_time;
    double real_act_time;
    s32 timepiece_count;
    char end_magic[4];
};

struct game {
    struct hat_timer timer;
};

static struct game current;
static struct game old;
static size_t timr_addr;

static pid_t pid = INVALID_PID;
static pid_t get_pid_from_name(const char* name);
static bool get_maps_from_pid(pid_t pid, struct map** out, size_t* out_n);

static bool read_mem(size_t addr, size_t size, void* buf);
static size_t scan_mem_u32(const struct map* maps, size_t map_n, enum perms perms, u32 needle);

static bool init(const struct map* maps, size_t map_n);

static bool update();
static bool is_running();
static bool should_start();
static bool should_split();
static bool should_reset();

#define LOCK_START(shared_timer, T, t, e) \
    { \
        Timer##T##Lock lock = SharedTimer_##t(shared_timer); \
        Timer##e timer = Timer##T##Lock_timer(lock)
//  } omitted on purpose

#define LOCK_END(T) \
        Timer##T##Lock_drop(lock); \
    }

#define RLOCK_START(shared_timer) LOCK_START(shared_timer, Read, read, Ref)
#define RLOCK_END LOCK_END(Read)
#define WLOCK_START(shared_timer) LOCK_START(shared_timer, Write, write, RefMut)
#define WLOCK_END LOCK_END(Write)

void run(SharedTimer shared_timer) {
    enum {
        RUNNING = 1,
        STARTING = 2,
        SPLITTING = 4,
        RESETTING = 8,
    } update_state;

    size_t map_n;
    struct map* maps = NULL;

    while(still_running()) {
        if(pid == INVALID_PID) {
            // comm only shows 15 chars
            pid = get_pid_from_name("HatinTimeGame.e");

            if(pid != INVALID_PID) {
                if(maps) {
                    free(maps);
                }

                if(!get_maps_from_pid(pid, &maps, &map_n) || !init(maps, map_n)) {
                    pid = INVALID_PID;
                }
            }
        }

        // stop spam
        if(pid == INVALID_PID) {
            sleep(1);
            continue;
        }

        if(!update()) {
            pid = INVALID_PID;
            continue;
        }

        // check what to update, then update at once to reduce lock usage
        update_state = 0;
        update_state |= is_running() ? RUNNING : 0;
        update_state |= should_start() ? STARTING : 0;
        update_state |= should_split() ? SPLITTING : 0;
        update_state |= should_reset() ? RESETTING : 0;

        if(update_state) {
            WLOCK_START(shared_timer);
            
#if IL_SPLIT
#define timer_to_read real_act_time
#else
#define timer_to_read real_game_time
#endif

            if((update_state & RUNNING) && current.timer.timer_to_read != old.timer.timer_to_read) {
                TimeSpan time = TimeSpan_from_seconds(current.timer.timer_to_read);
                Timer_set_game_time(timer, time);
                TimeSpan_drop(time);
            }
            if(update_state & STARTING) {
                Timer_initialize_game_time(timer);
                Timer_pause_game_time(timer);
                Timer_set_current_timing_method(timer, 1);
                Timer_start(timer);
            }
            if(update_state & SPLITTING) {
                Timer_split(timer);
            }
            if(update_state & RESETTING) {
                Timer_reset(timer, true);
            }

            WLOCK_END;
        }

        usleep(TIMER_SLEEP_US);
    }

    if(maps) {
        free(maps);
    }
}

static bool update() {
    memcpy(&old, &current, sizeof(old));
    return read_mem(timr_addr, sizeof(current.timer), &current.timer);
}

static bool is_running() {
    return current.timer.state == 1;
}

static bool should_start() {
#if IL_SPLIT
    return current.timer.act_timer_visible && current.timer.real_act_time > 0.0f && old.timer.real_act_time == 0.0f;
#else
    return current.timer.state == 1 && old.timer.state == 0 && current.timer.timepiece_count < 40;
#endif
}

static bool should_split() {
#if IL_SPLIT
    return current.timer.just_got_timepiece;
#else
    return 
        (current.timer.state == 1 && current.timer.timepiece_count == old.timer.timepiece_count + 1) ||
        (current.timer.state == 2 && old.timer.state == 1)
    ;
#endif
}

static bool should_reset() {
#if IL_SPLIT
    return current.timer.act_timer_paused && current.timer.act_timer_visible && current.timer.real_act_time == 0.0f && old.timer.real_act_time > 0.0f;
#else
    return current.timer.state == 0 && old.timer.state == 1;
#endif
}

static bool init(const struct map* maps, size_t map_n) {
    timr_addr = scan_mem_u32(maps, map_n, READ | WRITE, TIMR_MAGIC);
    if(timr_addr == 0) {
        return false;
    }

    u32 end_magic;
    read_mem(timr_addr + sizeof(current.timer) - sizeof(current.timer.end_magic), sizeof(end_magic), &end_magic);

    if(end_magic != END_MAGIC) {
        fprintf(stderr, "end magic not %08X like expected, was %08X\n", END_MAGIC, end_magic);
        return false;
    }

    return true;
}

static bool read_mem(size_t addr, size_t size, void* buf) {
    if(pid == INVALID_PID) {
        return false;
    }

    struct iovec local = {
        .iov_base = buf,
        .iov_len = size,
    };
    struct iovec remote = {
        .iov_base = (void*)addr,
        .iov_len = size,
    };

    return process_vm_readv(pid, &local, 1, &remote, 1, 0) == size;
}

static size_t scan_mem_u32(const struct map* maps, size_t map_n, enum perms perms, u32 needle) {
    for(size_t i = 0; i < map_n; i++) {
        if(maps[i].perms == perms) {
            const size_t size = maps[i].end - maps[i].base;
            u32* buf = malloc(size);

            if(!read_mem(maps[i].base, size, buf)) {
                free(buf);
                return 0;
            }

            for(size_t j = 0; j < size / sizeof(*buf); j++) {
                if(buf[j] == needle) {
                    free(buf);
                    return maps[i].base + j * sizeof(*buf);
                }
            }

            free(buf);
        }
    }

    return 0;
}

static bool is_numeric(const char* str, pid_t* out) {
    if(*str == '\0') {
        return false;
    }

    pid_t num = 0;
    size_t i = 0;
    while(str[i]) {
        const char c = str[i] - '0';
        if(c > 9) {
            return false;
        }

        num *= 10;
        num += c;

        i++;
    }

    *out = num;
    return true;
}

static pid_t get_pid_from_name(const char* name) {
    DIR* dir = opendir(PROCFS_DIR);
    struct dirent* entry;

    const size_t name_len = strlen(name);
    pid_t pid;
    while((entry = readdir(dir))) {
        if(is_numeric(entry->d_name, &pid)) {
            char path[32];
            char comm[16];
            
            snprintf(path, sizeof(path), PROCFS_DIR "/%s/comm", entry->d_name);

            FILE* f = fopen(path, "r");
            if(f == NULL) {
                continue;
            }

            fread(comm, sizeof(comm), 1, f);
            fclose(f);

            comm[sizeof(comm) - 1] = '\0';
            if(strcmp(name, comm) == 0) {
                closedir(dir);
                return pid;
            }
        }
    }

    closedir(dir);
    return INVALID_PID;
}

static bool parse_map(const char* line, struct map* out) {
    char perms[PERM_N];

    if(sscanf(line, "%zX-%zX %c%c%c", &out->base, &out->end, &perms[0], &perms[1], &perms[2]) != 5) {
        return false;
    }

    out->perms = 0;
    out->perms |= perms[0] == 'r' ? READ : 0;
    out->perms |= perms[1] == 'w' ? WRITE : 0;
    out->perms |= perms[2] == 'x' ? EXECUTE : 0;

    return true;
}

static bool get_maps_from_pid(pid_t pid, struct map** out, size_t* out_n) {
    char path[32];
    snprintf(path, sizeof(path), PROCFS_DIR "/%d/maps", pid);

    FILE* f = fopen(path, "r");
    if(f == NULL) {
        return false;
    }

    size_t n = 0;
    size_t max = 0;
    struct map* maps = NULL;

    char line[256];
    size_t i = 0;
    while(true) {
        const char c = fgetc(f);
        if(c == EOF) {
            break;
        }

        if(c == '\n') {
            line[i] = '\0';
            if(n == max) {
                max = max == 0 ? 16 : max * 2;
                maps = realloc(maps, sizeof(*maps) * max);
            }

            if(!parse_map(line, &maps[n])) {
                fprintf(stderr, "map parsing error!!! quitting function\n");

                return false;
            }
            
            n++;
            i = ~0; // will roll over to 0
        }
        else {
            line[i] = c;
        }

        i++;
        if(i >= sizeof(line)) {
            fprintf(stderr, "line too long!! quitting function\n");

            fclose(f);
            return false;
        }
    }

    fclose(f);

    *out_n = n;
    *out = maps;
    return true;
}
