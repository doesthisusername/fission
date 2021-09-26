#include "../component.h"
#include "../text_buf.h"
#include "../font.h"

#define TIMER_ALIGN NK_TEXT_RIGHT

void draw_timer(struct nk_context* ctx, TimerComponentStateRef state) {
    static struct text_buf secs = {.text = NULL, .max = 0};
    static struct text_buf frac = {.text = NULL, .max = 0};

    buf_set(&secs, TimerComponentState_time(state));
    buf_set(&frac, TimerComponentState_fraction(state));

    set_font(ctx, FONT_TIMER);
    nk_layout_row_begin(ctx, NK_DYNAMIC, render_state.row_height, 1);
    {
        nk_layout_row_push(ctx, 1.0f);
        const struct nk_color color = nk_rgba_u32(TimerComponentState_color(state));

        nk_labelf_colored(ctx, TIMER_ALIGN, color, "%s%s", secs.text, frac.text);
    }
    nk_layout_row_end(ctx);

    set_font(ctx, FONT_NORMAL);
}
