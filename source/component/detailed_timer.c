#include "../component.h"
#include "../font.h"
#include "../text_buf.h"

#define ROW_COUNT 2
#define LEFT_ALIGN NK_TEXT_LEFT
#define TIMER_ALIGN NK_TEXT_RIGHT

static struct text_buf secs = {.text = NULL, .max = 0};
static struct text_buf frac = {.text = NULL, .max = 0};

static struct text_buf name = {.text = NULL, .max = 0};
static struct text_buf time = {.text = NULL, .max = 0};

static void draw_row(struct nk_context* ctx, bool left_visible, struct nk_color right_color) {
    if(left_visible) {
        const struct nk_color text_color = nk_rgba_u32(GeneralLayoutSettings_text_color(general_settings));

        nk_layout_row_begin(ctx, NK_DYNAMIC, render_state.row_height, 2);
        {
            nk_layout_row_push(ctx, 0.5f);

            set_font(ctx, FONT_TEXT);
            nk_labelf_colored(ctx, LEFT_ALIGN, text_color, "%s: %s", name.text, time.text);

            set_font(ctx, FONT_TIME);
            nk_labelf_colored(ctx, TIMER_ALIGN, right_color, "%s%s", secs.text, frac.text);
        }
        nk_layout_row_end(ctx);
    }
    else {
        nk_layout_row_begin(ctx, NK_DYNAMIC, render_state.row_height, 1);
        {
            nk_layout_row_push(ctx, 1.0f);

            set_font(ctx, FONT_TIME);
            nk_labelf_colored(ctx, TIMER_ALIGN, right_color, "%s%s", secs.text, frac.text);
        }
        nk_layout_row_end(ctx);
    }
}

void draw_detailed_timer(struct nk_context* ctx, DetailedTimerComponentStateRef state) {
    const bool comparison_visible[ROW_COUNT] = {
        DetailedTimerComponentState_comparison1_visible(state),
        DetailedTimerComponentState_comparison2_visible(state),
    };
    const struct nk_color timer_colors[ROW_COUNT] = {
        nk_rgba_u32(DetailedTimerComponentState_timer_color(state)),
        nk_rgba_u32(DetailedTimerComponentState_segment_timer_color(state)),
    };

    #define DRAW_ROW(i, type, comparison) \
        buf_set(&secs, DetailedTimerComponentState_##type##_time(state)); \
        buf_set(&frac, DetailedTimerComponentState_##type##_fraction(state)); \
\
        if(comparison_visible[i]) { \
            buf_set(&name, DetailedTimerComponentState_##comparison##_name(state)); \
            buf_set(&time, DetailedTimerComponentState_##comparison##_time(state)); \
        } \
\
        draw_row(ctx, comparison_visible[i], timer_colors[i])

    // actually draw
    DRAW_ROW(0, timer, comparison1);
    DRAW_ROW(1, segment_timer, comparison2);

#undef DRAW_ROW

    set_font(ctx, FONT_TEXT);
}
