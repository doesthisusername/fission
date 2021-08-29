#include "../component.h"
#include "../font.h"

#define TEXT_HEIGHT 60.0f
#define LEFT_ALIGN NK_TEXT_LEFT
#define RIGHT_ALIGN NK_TEXT_RIGHT
#define CENTER_ALIGN NK_TEXT_CENTERED

void draw_text(struct nk_context* ctx, TextComponentStateRef state) {
    set_font(ctx, FONT_M);

    const bool is_split = TextComponentState_is_split(state);
    if(is_split) {
        nk_layout_row_begin(ctx, NK_DYNAMIC, TEXT_HEIGHT, 2);
        {
            nk_layout_row_push(ctx, 0.5f);

            const struct nk_color left_color = nk_rgba_u32(TextComponentState_left_center_color_or_default(state, general_settings));
            nk_label_colored(ctx, TextComponentState_left(state), LEFT_ALIGN, left_color);

            const struct nk_color right_color = nk_rgba_u32(TextComponentState_right_color_or_default(state, general_settings));
            nk_label_colored(ctx, TextComponentState_right(state), RIGHT_ALIGN, right_color);
        }
        nk_layout_row_end(ctx);
    }
    else {
        nk_layout_row_begin(ctx, NK_DYNAMIC, TEXT_HEIGHT, 1);
        {
            const struct nk_color center_color = nk_rgba_u32(TextComponentState_left_center_color_or_default(state, general_settings));
            nk_label_colored(ctx, TextComponentState_center(state), CENTER_ALIGN, center_color);
        }
        nk_layout_row_end(ctx);
    }
}
