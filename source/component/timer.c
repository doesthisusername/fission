#include "../component.h"
#include "../text_buf.h"

#define TIMER_ALIGN NK_TEXT_RIGHT
#define TIMER_HEIGHT 60.0f

void draw_timer(struct nk_context* ctx, TimerComponentStateRef state) {
    static struct text_buf secs = {.text = NULL, .max = 0};
    static struct text_buf frac = {.text = NULL, .max = 0};

    buf_set(&secs, TimerComponentState_time(state));
    buf_set(&frac, TimerComponentState_fraction(state));

    nk_layout_row_begin(ctx, NK_DYNAMIC, TIMER_HEIGHT, 1);
    {
        nk_layout_row_push(ctx, 1.0f);
        nk_labelf(ctx, TIMER_ALIGN, "%s%s", secs.text, frac.text);
    }
    nk_layout_row_end(ctx);
}
