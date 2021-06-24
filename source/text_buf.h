#ifndef FIS_TEXT_BUF_H
#define FIS_TEXT_BUF_H
#include <stdlib.h>

struct text_buf {
    char* text;
    size_t max;
};

/// @brief Sets the string content of the text_buf, allocating memory if necessary.
/// The input text can safely be deallocated afterwards, as text_buf has its own buffer.
/// @param buf the text_buf whose string shall be set.
/// @param text the string to copy to buf.
void buf_set(struct text_buf* buf, const char* text);

#endif // FIS_TEXT_BUF_H
