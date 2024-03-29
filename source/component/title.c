#include "../component.h"
#include "../font.h"
#include "../text_buf.h"
#include "../types.h"

#define ATTEMPT_COUNTER_ALIGN NK_TEXT_RIGHT
#define TITLE_ALIGN NK_TEXT_LEFT

static struct text_buf lines[] = {
    {.text = NULL, .max = 0},
    {.text = NULL, .max = 0},
};

struct attempt_counter_info {
    u32 attempts;
    u32 finished;
    bool show_attempts;
    bool show_finished;
};

// assumes at least one of info->show_attempts and info->show_finished are true
static void draw_attempts(struct nk_context* ctx, const struct attempt_counter_info* info, struct nk_color color) {
    const bool show_both = info->show_attempts && info->show_finished;

    if(show_both) {
        nk_labelf_colored(ctx, ATTEMPT_COUNTER_ALIGN, color, "%u/%u", info->finished, info->attempts);
    }
    else {
        const u32 number = info->show_attempts ? info->attempts : info->finished;
        nk_labelf_colored(ctx, ATTEMPT_COUNTER_ALIGN, color, "%u", number);
    }
}

void draw_title(struct nk_context* ctx, TitleComponentStateRef state) {
    const char* line2 = TitleComponentState_line2(state);
    const bool is_single_line = line2 == NULL;
    const struct nk_color color = nk_rgba_u32(TitleComponentState_text_color_or_default(state, general_settings));

    struct attempt_counter_info ac_info = {
        .show_attempts = TitleComponentState_shows_attempts(state),
        .show_finished = TitleComponentState_shows_finished_runs(state),
    };
    if(ac_info.show_attempts) {
        ac_info.attempts = TitleComponentState_attempts(state);
    }
    if(ac_info.show_finished) {
        ac_info.finished = TitleComponentState_finished_runs(state);
    }

    // keep same amount of columns for both rows,
    // so having two center-aligned lines does not look terrible
    const int cols = 1 + (ac_info.show_attempts || ac_info.show_finished);

    // second element is not read if `cols` is 1
    const float ratio[] = {cols == 1 ? 1.0f : 0.8f, 0.2f};

    if(!is_single_line) {
        buf_set(&lines[1], line2);
    }

    buf_set(&lines[0], TitleComponentState_line1(state));

    set_font(ctx, FONT_TEXT);
    nk_layout_row_begin(ctx, NK_DYNAMIC, render_state.row_height, cols);
    {
        nk_layout_row_push(ctx, ratio[0]);
        nk_label_colored(ctx, lines[0].text, TITLE_ALIGN, color);
        if(is_single_line && cols > 1) {
            nk_layout_row_push(ctx, ratio[1]);
            draw_attempts(ctx, &ac_info, color);
        }
    }
    nk_layout_row_end(ctx);

    if(!is_single_line) {
        nk_layout_row_begin(ctx, NK_DYNAMIC, render_state.row_height, cols);
        {
            nk_layout_row_push(ctx, ratio[0]);
            nk_label_colored(ctx, lines[1].text, TITLE_ALIGN, color);
            if(cols > 1) {
                nk_layout_row_push(ctx, ratio[1]);
                draw_attempts(ctx, &ac_info, color);
            }
        }
        nk_layout_row_end(ctx);
    }
}
