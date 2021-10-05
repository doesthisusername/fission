#include "../component.h"
#include "../font.h"
#include "../text_buf.h"

#define NAME_ALIGN NK_TEXT_LEFT
#define VALUE_ALIGN NK_TEXT_RIGHT

void draw_splits(struct nk_context* ctx, SplitsComponentStateRef state) {
    const size_t split_n = SplitsComponentState_len(state);

    for(size_t i = 0; i < split_n; i++) {
        const size_t col_n = SplitsComponentState_columns_len(state, i);
        const double ratio = 1.0 / (col_n + 1); // split name isn't included in `col_n`

        nk_layout_row_begin(ctx, NK_DYNAMIC, render_state.row_height, col_n);
        {
            // otherwise it's just a blank padding line, and it's just, well, padding
            if(col_n > 0) {
                nk_layout_row_push(ctx, ratio);

                const struct nk_color text_color = nk_rgba_u32(GeneralLayoutSettings_text_color(general_settings));
                nk_label_colored(ctx, SplitsComponentState_name(state, i), NAME_ALIGN, text_color);

                // column values are given right-to-left,
                // so iterate in reverse to make it left-to-right.
                // continuation condition works because of unsigned overflow.
                // NOTE: does not draw the actual split name (i.e. the leftmost column)
                for(size_t j = col_n - 1; j < col_n; j--) {
                    const char* text = SplitsComponentState_column_value(state, i, j);
                    const struct nk_color color = nk_rgba_u32(SplitsComponentState_column_color(state, i, j));

                    nk_label_colored(ctx, text, VALUE_ALIGN, color);
                }
            }
        }
        nk_layout_row_end(ctx);
    }
}
