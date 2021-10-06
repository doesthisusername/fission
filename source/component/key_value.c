#include "../component.h"
#include "../font.h"

#define KEY_ALIGN NK_TEXT_LEFT
#define VALUE_ALIGN NK_TEXT_RIGHT

void draw_key_value(struct nk_context* ctx, KeyValueComponentStateRef state) {
    nk_layout_row_begin(ctx, NK_DYNAMIC, render_state.row_height, 2);
    {
        nk_layout_row_push(ctx, 0.5f);

        const char* key = KeyValueComponentState_key(state);
        set_font(ctx, FONT_TEXT);
        nk_label_colored(ctx, key, KEY_ALIGN, nk_rgba_u32(KeyValueComponentState_key_color_or_default(state, general_settings)));

        const char* val = KeyValueComponentState_value(state);
        set_font(ctx, FONT_TIME);
        nk_label_colored(ctx, val, VALUE_ALIGN, nk_rgba_u32(KeyValueComponentState_value_color_or_default(state, general_settings)));
    }
    nk_layout_row_end(ctx);
}
