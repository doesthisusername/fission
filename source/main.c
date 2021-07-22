#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <livesplit_core.h>
#include "backend.h"
#include "component.h"
#include "file_io.h"
#include "font.h"
#include "types.h"

#ifndef _WIN32
    // for `chdir()`
    #include <unistd.h>
#endif

static SharedTimer shared_timer;
static Layout layout;
static HotkeySystem hk_sys;
static HotkeyConfig hk_cfg;

#ifdef _WIN32
    #define SEP '\\'
#else
    #define SEP '/'
#endif

s32 main(s32 argc, char** argv) {
    // if SEP is '/' on windows, it won't find it, 
    // and the length calculation is likely to overflow,
    // causing the malloc to fail.
    // NOTE: '/' works for linux, even on ntfs filesystems.
    const size_t path_len = strrchr(argv[0], SEP) - argv[0];
    char* target_dir = malloc(path_len + 1 /* \0 */);
    if(target_dir == NULL) {
        fprintf(stderr, "failed to allocate memory to change dir\n");
        return 1;
    }

    memcpy(target_dir, argv[0], path_len);
    target_dir[path_len] = '\0';

    if(chdir(target_dir)) {
        fprintf(stderr, "failed to change working dir to %s\n", target_dir);
        return 1;
    }
    free(target_dir);

    // nuklear
    struct nk_context* ctx;

    // init nuklear with backend
    if(!init_nk(&ctx)) {
        fprintf(stderr, "failed to init nk_context\n");
        return 1;
    }
    
    // init livesplit-core (splits and timer)
    shared_timer = load_splits("data/lss/default.lss");
    if(shared_timer == NULL) {
        fprintf(stderr, "failed to load splits\n");
        return 1;
    }

    // (hotkeys)
    hk_cfg = HotkeyConfig_new();
    hk_sys = HotkeySystem_with_config(SharedTimer_share(shared_timer), hk_cfg);

    // (layout)
    layout = load_layout("data/lsl/default.lsl", true);
    if(layout == NULL) {
        fprintf(stderr, "failed to load layout\n");
        return 1;
    }
    LayoutState lstate = LayoutState_new();

    // main loop
    while(still_running()) {
        start_frame();
        if(reload_fonts) {
            load_fonts(ctx, win_info.scale);
        }

        if(nk_begin(ctx, "fission", nk_rect(0, 0, win_info.width, win_info.height), 0)) {
            TimerWriteLock wlock = SharedTimer_write(shared_timer);
            TimerRefMut timer = TimerWriteLock_timer(wlock);

            Layout_update_state(layout, lstate, timer);
            TimerWriteLock_drop(wlock);

            const size_t elem_n = LayoutState_len(lstate);
            for(size_t i = 0; i < elem_n; i++) {
                draw(ctx, lstate, i);
            }
        }
        nk_end(ctx);

        end_frame();
    }

    // save
    if(!save_layout(layout, "data/lsl/default.lsl")) {
        fprintf(stderr, "ERR: unable to save layout!\n");
        return 2;
    }

    TimerReadLock rlock = SharedTimer_read(shared_timer);
    TimerRef save_timer = TimerReadLock_timer(rlock);
    if(!save_splits(save_timer, "data/lss/default.lss")) {
        fprintf(stderr, "ERR: unable to save splits!\n");
        return 2;
    }
    TimerReadLock_drop(rlock);

    // free
    LayoutState_drop(lstate);
    SharedTimer_drop(shared_timer);
    Layout_drop(layout);
    if(hk_sys != NULL) {
        HotkeySystem_drop(hk_sys);
    }

    shutdown_nk();
    return 0;
}
