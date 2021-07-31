# fission
Hopefully performant, cross-platform LiveSplit One alternative.

## Status
Early. Standard `CMake` procedure should just work (after cloning with `--recursive`).

Only renders a few components at the moment, layouting is barely adhered to, and file paths are hardcoded. Thus, it's not usable for actual runs, yet.

Builds on Linux, both native and to Windows with `mingw`, and builds natively on Windows, even with MSVC (please stay that way). Mac status is unknown.

## Build Requirements
### Common
- CMake.
- C99 compiler (actually just `c_restrict` in CMake, for the generated `livesplit_core.h` file; might be able to circumvent with `-Drestrict=__restrict`).
- Rust (`rustup` usually works; for `livesplit-core` dependency).

### Cross-compilation
- Target added in `rustup`.
- `set(CARGO_TARGET <rust triple for target>)` in the toolchain file for passing along to `livesplit-core`.

## Build Notes
- Red squigglies in `livesplit_core.h` in Visual Studio are due to it lacking a C99 standard option. Change to C11 in `fission`'s project settings to make them go away. C99 still builds fine: this is for squigglies in the IDE.
- Visual Studio adds a `Debug`/`Release` folder in the output, which breaks the placement of `data`. I'll look at fixing this, but in the meantime you can copy `fission.exe` into `bin`.
- `glfw` is built as a submodule and statically linked instead of linking to the system library, as I was having issues with `mingw`. Static linking also made application startup a lot faster, for whatever reason.
- If it looks like `glfw` is taking a while to build, it's actually `livesplit-core` blocking it.
- The `data` folder is hopefully temporary, but I don't know what to replace it with.

## License
Probably MIT. Dependency licenses are in `data/license` -- please let me know if those are missing anything.
