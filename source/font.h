#ifndef FIS_FONT_H
#define FIS_FONT_H

#include <stdbool.h>
#include "backend.h"

#define FONT_S_SCALE 0.5f
#define FONT_M_SCALE 0.7f
#define FONT_L_SCALE 1.0f
#define FONT_SCALE(x) ((float[FONT_NUM]){FONT_S_SCALE, FONT_M_SCALE, FONT_L_SCALE}[x])

enum font_sizes {
    FONT_S,
    FONT_M,
    FONT_L,
    FONT_NUM,
};

extern bool reload_fonts;
extern struct nk_font* fonts[FONT_NUM];

void load_fonts(struct nk_context* ctx, struct nk_vec2 dim);
void set_font(struct nk_context* ctx, enum font_sizes size);

#endif // FIS_FONT_H
