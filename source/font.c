#include "font.h"
#include "backend.h"

bool reload_fonts = false;
struct nk_font* fonts[FONT_NUM];

// just calculates FONT_L height
// subject to change
#define FONT_IDEAL_ROWS 16
#define TYPICAL_RATIO (1.0f / 2.0f)
#define MIN_RATIO (1.0f / 1.5f)
static float calc_font_height(struct nk_vec2 dim) {
    const float ratio = NK_MAX(dim.x / dim.y, MIN_RATIO);

    return dim.y / (ratio * FONT_IDEAL_ROWS);
}

// todo: unhardcode
#define FONT_PATH "data/font/TitilliumWeb-SemiBold.ttf"
void load_fonts(struct nk_context* ctx, struct nk_vec2 dim) {
    const float l_size = calc_font_height(dim);
    struct nk_font_atlas* atlas;

    nk_font_stash_begin(&atlas);
    for(size_t i = 0; i < FONT_NUM; i++) {
        fonts[i] = nk_font_atlas_add_from_file(atlas, FONT_PATH, FONT_SCALE(i) * l_size, NULL);
    }
    nk_font_stash_end();

    reload_fonts = false;

    // take biggest; might change in the future
    nk_style_set_font(ctx, &fonts[FONT_NUM - 1]->handle);
}

inline void set_font(struct nk_context* ctx, enum font_sizes size) {
    nk_style_set_font(ctx, &fonts[size]->handle);
}
