#include "component.h"
#include <string.h>

GeneralLayoutSettingsRef general_settings;

#define CHECK_AND_DRAW(name, ident) \
    if(strcmp(name, type) == 0) { \
        draw_##ident(ctx, LayoutState_component_as_##ident(state, index)); \
        return; \
    }

void draw(struct nk_context* ctx, LayoutStateRef state, size_t index) {
    const char* type = LayoutState_component_type(state, index);

    CHECK_AND_DRAW("DetailedTimer", detailed_timer);
    CHECK_AND_DRAW("KeyValue", key_value);
    CHECK_AND_DRAW("Splits", splits);
    CHECK_AND_DRAW("Text", text);
    CHECK_AND_DRAW("Timer", timer);
    CHECK_AND_DRAW("Title", title);
}
#undef CHECK_AND_DRAW
