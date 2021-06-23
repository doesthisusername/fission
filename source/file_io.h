#ifndef FIS_FILE_IO_H
#define FIS_FILE_IO_H
#include <stdbool.h>
#include <livesplit_core.h>

Layout load_layout(const char* path, bool fallback_to_default);
bool save_layout(const Layout layout, const char* path);

SharedTimer load_splits(const char* path);
bool save_splits(const TimerRef timer, const char* path);

HotkeyConfig load_config(const char* path, bool fallback_to_default);
bool save_config(const HotkeyConfig cfg, const char* path);

#endif // FIS_FILE_IO_H
