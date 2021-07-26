#ifndef FIS_TEXT_BUF_H
#define FIS_TEXT_BUF_H
#include <stdlib.h>

/// Very simple self-expanding buffer.
/// Does not free `text` itself.
struct text_buf {
    char* text;
    size_t max;
};

/// Copies the string content of `text` to `buf->text`, overwriting any prior text contents.
/// Will also expand backing memory if necessary.
/// `text` can safely be deallocated afterwards, as a `text_buf` has its own buffer.
void buf_set(struct text_buf* buf, const char* text);

#endif // FIS_TEXT_BUF_H
