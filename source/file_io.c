#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <livesplit_core.h>
#include "file_io.h"

// not pretty, but reduces #ifdefs below
#ifndef _WIN32
    #define FD_T int
    #define INVALID_FD -1

    #define OPEN_RO(path) open(path, O_RDONLY)
    #define CLOSE_FD(fd) close(fd)
#else
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>

    #define FD_T HANDLE
    #define INVALID_FD INVALID_HANDLE_VALUE

    #define OPEN_RO(path) CreateFile(path, GENERIC_READ, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)
    #define CLOSE_FD(fd) CloseHandle(fd)
#endif

// if open succeeds, save return value of load(fd) to *dst
// if it fails, do not touch *dst, and return false
static bool load_file(const char* path, void** dst, void*(*load)(int64_t handle)) {
    const FD_T fd = OPEN_RO(path);
    if(fd == INVALID_FD) {
        fprintf(stderr, "failed to open %s for reading\n", path);
        return false;
    }
    else {
        *dst = load((int64_t)fd);
        CLOSE_FD(fd);
        return true;
    }
}

// data is assumed to be null-terminated text
static bool save_file(const char* path, const char* data) {
    FILE* f = fopen(path, "w");
    if(f == NULL) {
        fprintf(stderr, "failed to open %s for saving\n", path);
        return false;
    }

    const size_t data_len = strlen(data);
    const size_t nwritten = fwrite(data, data_len, 1, f);
    fclose(f);

    return nwritten == 1;
}

// remove need for (void**) and function pointer casts for load_file...
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"
Layout load_layout(const char* path, bool fallback_to_default) {
    Layout layout = NULL;
    load_file(path, &layout, Layout_parse_file_handle);

    if(layout == NULL) {
        if(fallback_to_default) {
            layout = Layout_default_layout();
        }
        else {
            fprintf(stderr, "failed to parse layout in %s\n", path);
        }
    }

    return layout;
}

bool save_layout(const Layout layout, const char* path) {
    const char* json = Layout_settings_as_json(layout);
    return save_file(path, json);
}

SharedTimer load_splits(const char* path) {
    const FD_T fd = OPEN_RO(path);
    if(fd == INVALID_FD) {
        fprintf(stderr, "failed to open %s for reading\n", path);
        return NULL;
    }

    ParseRunResult maybe_run = Run_parse_file_handle((int64_t)fd, path, true);
    CLOSE_FD(fd);

    if(!ParseRunResult_parsed_successfully(maybe_run)) {
        fprintf(stderr, "failed to parse %s\n", path);
        return NULL;
    }

    Run run = ParseRunResult_unwrap(maybe_run);
    return Timer_into_shared(Timer_new(run));
}

bool save_splits(const TimerRef timer, const char* path) {
    const char* lss = Timer_save_as_lss(timer);
    return save_file(path, lss);
}

HotkeyConfig load_config(const char* path, bool fallback_to_default) {
    HotkeyConfig cfg = NULL;
    load_file(path, &cfg, HotkeyConfig_parse_file_handle);

    if(cfg == NULL) {
        if(fallback_to_default) {
            cfg = HotkeyConfig_new();
        }
        else {
            fprintf(stderr, "failed to parse hotkeys in %s\n", path);
        }
    }

    return cfg;
}

bool save_config(const HotkeyConfig cfg, const char* path) {
    const char* json = HotkeyConfig_as_json(cfg);
    return save_file(path, json);
}

#pragma GCC diagnostic pop // Wincompatible-pointer-types
