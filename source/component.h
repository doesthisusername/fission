#ifndef FIS_COMPONENT_H
#define FIS_COMPONENT_H
#include "backend.h"
#include <livesplit_core.h>

// not ideal, but forgetting this isn't good either.
// best case is probably if there's a way to get the number of drawn rows from nuklear itself.
#define nk_layout_row_end(ctx) nk_layout_row_end(ctx); render_state.row_count++

extern GeneralLayoutSettingsRef general_settings;

/// Draw the `index`th element of the `state`.
void draw(struct nk_context* ctx, LayoutStateRef state, size_t index);

void draw_detailed_timer(struct nk_context* ctx, DetailedTimerComponentStateRef state);
void draw_key_value(struct nk_context* ctx, KeyValueComponentStateRef state);
void draw_splits(struct nk_context* ctx, SplitsComponentStateRef state);
void draw_text(struct nk_context* ctx, TextComponentStateRef state);
void draw_timer(struct nk_context* ctx, TimerComponentStateRef state);
void draw_title(struct nk_context* ctx, TitleComponentStateRef state);

#endif // FIS_COMPONENT_H
