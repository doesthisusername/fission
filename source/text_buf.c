#include "text_buf.h"
#include <string.h>
#include <stdlib.h>

// realloc in increments of this number of bytes
#define SIZE_STEP 8

void buf_set(struct text_buf* buf, const char* text) {
    const size_t text_len = strlen(text);

    // >= because of \0 not being counted in strlen
    if(text_len >= buf->max) {
        const size_t new_max = (text_len + SIZE_STEP) & ~(SIZE_STEP - 1);
        
        buf->text = realloc(buf->text, new_max);
        buf->max = new_max;
    }

    memcpy(buf->text, text, text_len + 1);
}
