#include "../component.h"
#include "../text_buf.h"
#include "../font.h"

#define TEXT_HEIGHT 60.0f
#define KEY_ALIGN NK_TEXT_LEFT
#define VALUE_ALIGN NK_TEXT_RIGHT

void draw_key_value(struct nk_context* ctx, KeyValueComponentStateRef state) {
    static struct text_buf key = {.text = NULL, .max = 0};
    static struct text_buf val = {.text = NULL, .max = 0};

    buf_set(&key, KeyValueComponentState_key(state));
    buf_set(&val, KeyValueComponentState_value(state));

    set_font(ctx, FONT_M);
    nk_layout_row_begin(ctx, NK_DYNAMIC, TEXT_HEIGHT, 2);
    {
        nk_layout_row_push(ctx, 0.5f);
        nk_label(ctx, key.text, KEY_ALIGN);
        nk_label(ctx, val.text, VALUE_ALIGN);
    }
    nk_layout_row_end(ctx);
}
