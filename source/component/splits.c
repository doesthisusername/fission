#include "../component.h"
#include "../text_buf.h"
#include "../font.h"

#define ROW_HEIGHT 60.0f
#define NAME_ALIGN NK_TEXT_LEFT
#define VALUE_ALIGN NK_TEXT_RIGHT

void draw_splits(struct nk_context* ctx, SplitsComponentStateRef state) {
    const size_t split_n = SplitsComponentState_len(state);

    set_font(ctx, FONT_M);
    for(size_t i = 0; i < split_n; i++) {
        const size_t col_n = SplitsComponentState_columns_len(state, i);
        const double ratio = 1.0 / (col_n + 1); // split name isn't included above

        nk_layout_row_begin(ctx, NK_DYNAMIC, ROW_HEIGHT, col_n);
        {
            nk_layout_row_push(ctx, ratio);
            nk_label(ctx, SplitsComponentState_name(state, i), NAME_ALIGN);

            // column values are given right-to-left,
            // so iterate in reverse to make it left-to-right.
            // continuation condition works because of unsigned overflow.
            for(size_t j = col_n - 1; j < col_n; j--) {
                nk_label(ctx, SplitsComponentState_column_value(state, i, j), VALUE_ALIGN);
            }
        }
        nk_layout_row_end(ctx);
    }
}
