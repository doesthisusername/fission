#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <livesplit_core.h>
#include "backend.h"
#include "component.h"
#include "file_io.h"
#include "types.h"

static SharedTimer shared_timer = NULL;
static Layout layout = NULL;
static HotkeySystem hk_sys;
static HotkeyConfig hk_cfg;

static void load_fonts(struct nk_context* ctx) {
    struct nk_font_atlas* atlas;

    nk_font_stash_begin(&atlas);
    const struct nk_font* titillium = nk_font_atlas_add_from_file(atlas, "data/font/TitilliumWeb-SemiBold.ttf", 72, NULL);
    nk_font_stash_end();

    nk_style_set_font(ctx, &titillium->handle);
}

s32 main(s32 argc, char** argv) {
    // TODO: check if this works on windows
    // cd to dir with executable in it
    const size_t path_len = strrchr(argv[0], '/') - argv[0];
    char* target_dir = malloc(path_len + 1 /* \0 */);
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
    load_fonts(ctx);

    // init livesplit-core (splits and timer)
    shared_timer = load_splits("data/lss/default.lss");
    hk_cfg = HotkeyConfig_new();
    hk_sys = HotkeySystem_with_config(SharedTimer_share(shared_timer), hk_cfg);

    // (layout)
    layout = load_layout("data/lsl/default.lsl", true);
    LayoutState lstate = LayoutState_new();

    // main loop
    while(still_running()) {
        start_frame();

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
    save_layout(layout, "data/lsl/default.lsl");

    TimerReadLock rlock = SharedTimer_read(shared_timer);
    TimerRef save_timer = TimerReadLock_timer(rlock);
    save_splits(save_timer, "data/lss/default.lss");
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
