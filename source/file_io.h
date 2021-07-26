#ifndef FIS_FILE_IO_H
#define FIS_FILE_IO_H
#include <stdbool.h>
#include <livesplit_core.h>

/// Loads the livesplit-core layout file from `path`.
/// If this fails, and `fallback_to_default` is true, returns a default layout.
/// Else, returns `NULL`.
Layout load_layout(const char* path, bool fallback_to_default);

/// Saves `layout` to the file at `path`.
/// Returns true on success.
bool save_layout(const Layout layout, const char* path);

/// Loads the splits from the file at `path`.
/// Returns `NULL` on failure.
SharedTimer load_splits(const char* path);

/// Saves `timer` to the file at `path`.
/// Returns true on success.
bool save_splits(const TimerRef timer, const char* path);

HotkeyConfig load_config(const char* path, bool fallback_to_default);
bool save_config(const HotkeyConfig cfg, const char* path);

#endif // FIS_FILE_IO_H
