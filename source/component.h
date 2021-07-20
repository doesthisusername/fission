#ifndef FIS_COMPONENT_H
#define FIS_COMPONENT_H
#include "backend.h"
#include <livesplit_core.h>

void draw(struct nk_context* ctx, LayoutStateRef state, size_t index);

void draw_timer(struct nk_context* ctx, TimerComponentStateRef state);
void draw_title(struct nk_context* ctx, TitleComponentStateRef state);
void draw_key_value(struct nk_context* ctx, KeyValueComponentStateRef state);

#endif // FIS_COMPONENT_H
