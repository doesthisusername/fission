#include "font.h"
#include "backend.h"

bool reload_fonts = false;
struct nk_font* fonts[FONT_NUM];

// TODO: unhardcode, and add proper fallbacks for dashes and minus
// TODO: add mechanism for loading needed glyphs, especially for non-ASCII split names
#define FONT_PATH "data/font/TitilliumWeb-SemiBold.ttf"
void load_fonts(struct nk_context* ctx, struct nk_vec2 dim) {
    // ASCII, en and em dashes, and mathematical minus
    static const nk_rune range[] = {0x20, 0xFF, 0x2013, 0x2014, 0x2212, 0x2212, 0};
    static struct nk_font_atlas* atlas = NULL;

    // avoid memory leak
    if(atlas != NULL) {
        nk_font_atlas_cleanup(atlas);
        nk_font_atlas_clear(atlas);
    }

    nk_font_stash_begin(&atlas);
    for(size_t i = 0; i < FONT_NUM; i++) {
        const float height = render_state.row_height;
        struct nk_font_config cfg = nk_font_config(height);
        cfg.range = range;

        fonts[i] = nk_font_atlas_add_from_file(atlas, FONT_PATH, height, &cfg);
    }
    nk_font_stash_end();

    reload_fonts = false;

    set_font(ctx, FONT_NORMAL);
}

void set_font(struct nk_context* ctx, enum font_sizes size) {
    nk_style_set_font(ctx, &fonts[size]->handle);
}
