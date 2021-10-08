# fission
Hopefully performant, cross-platform speedrun timer built with [livesplit-core](https://github.com/LiveSplit/livesplit-core). Name might change but it works for now.

# This Branch
Personal branch with a hardcoded AHiT autosplitter, until proper autosplitting is merged into `livesplit-core` (it's going to work a lot better than I could've made it for this).

You can use it for a reference if you'd like I suppose, it's not commented much, but I tried doing the out-of-process memory I/O somewhat properly.

## Status
Quite early. Might be able to use for runs, but you'll be on your own. Standard `CMake` procedure should just work (after cloning with `--recursive`).

Supports just about the most essential components, layouting isn't adhered to much outside of text color, and file paths are hardcoded.

Builds on Linux, both native and to Windows with `mingw`, and builds natively on Windows, even with MSVC (please stay that way). Mac status is unknown.

## Build Requirements
### Common
- CMake.
- C99 compiler (actually just `c_restrict` in CMake, for the generated `livesplit_core.h` file; might be able to circumvent with `-Drestrict=__restrict`).
- Rust (`rustup` usually works; for `livesplit-core` dependency).

### Cross-compilation
- Target added in `rustup`.
- `set(CARGO_TARGET <rust triple for target>)` in your CMake toolchain file for passing along to `livesplit-core`.

## Build Notes
- Visual Studio adds a `Debug`/`Release` folder in the output, which breaks the placement of `data`. I'll look at fixing this, but in the meantime you can copy `fission.exe` into `bin`.
- `glfw` is built as a submodule and statically linked instead of linking to the system library, as I was having issues with `mingw`. Static linking also made application startup a lot faster, for whatever reason.
- If it looks like `glfw` is taking a while to build, it's actually `livesplit-core` blocking it (`livesplit-core` is fairly big).
- The `data` folder is hopefully temporary, but I don't know what to replace it with.

## License
Probably MIT. Dependency licenses are in `data/license` -- please let me know if those are missing anything.

## Bugs
For some reason, it uses an entire CPU core on Windows. On Linux (my main dev platform), it only uses about 2% at 165Hz.
