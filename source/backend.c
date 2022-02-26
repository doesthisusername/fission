#include "backend.h"

struct window_state win_info = {
    .width = INIT_WINDOW_WIDTH,
    .height = INIT_WINDOW_HEIGHT,
};

struct render_state render_state = {
    .row_count = 0,
    .row_height = INIT_ROW_HEIGHT,
    .timer_height_offset = 0.0f,
};
