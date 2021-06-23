#include "../component.h"
#include <stdlib.h>
#include <stdio.h>

struct text_buf {
    char* text;
    size_t max;
};

static void buf_set(struct text_buf* buf, const char* text) {
    const size_t text_len = strlen(text);

    // >= because of \0 not being counted in strlen
    if(text_len >= buf->max) {
        buf->text = realloc(buf->text, (text_len + 8) & ~7);
    }

    memcpy(buf->text, text, text_len + 1);
}

void draw_timer(struct nk_context* ctx, TimerComponentStateRef state) {
    static struct text_buf secs = {.text = NULL, .max = 0};
    static struct text_buf frac = {.text = NULL, .max = 0};

    buf_set(&secs, TimerComponentState_time(state));
    buf_set(&frac, TimerComponentState_fraction(state));

    nk_layout_row_begin(ctx, NK_LAYOUT_DYNAMIC_ROW, 120.0f, 1);
    nk_labelf(ctx, NK_TEXT_ALIGN_RIGHT, "%s%s", secs.text, frac.text);
    nk_layout_row_end(ctx);
}
