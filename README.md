# Touhou Danmakufu ph3sx

Touhou Danmakufu ph3sx is a modified version of the Danmakufu ph3 bullet-hell
engine. This fork focuses on performance improvements and additional scripting
features. It was originally used for *Sapphire Panlogism* and
*Treasure Castle Labyrinth*.

## Project status

- The checked-in build system targets **32-bit Windows (Win32)** and
  **DirectX 9**.
- A native macOS build is **not currently available**. macOS/Apple Silicon
  support is being explored; there is no supported macOS release yet.
- The planned cross-platform architecture is SDL3 for windowing and input,
  Direct3D 11 for Windows x64, and Metal for macOS on Apple Silicon. The
  executor, configuration utility, and archive utility are all in scope.
  This is a target, not an implemented or supported build.
- SDL3 event polling and input are integrated into the Windows game executor.
  The executor still uses its legacy Win32 window and DirectX 9 renderer; this
  integration is an intermediate migration step, not a cross-platform build.
- DnhViewer is not included or supported.
- The project may still contain bugs. Please report reproducible issues using
  the [issue tracker](https://github.com/MaxGog/Touhou-Danmakufu-ph3sx-2/issues).

## Features

- Performance improvements over the original ph3 engine.
- Additional scripting-language features and script functions.
- A revised `.dat` archive format with compression and encryption.

## Documentation

- [Documentation home](./docs/README.md)
- [Engine architecture and platform migration](./docs/architecture.md)
- [Scripting language features](./docs/scripting-language.md)
- [Script API reference](./docs/script-api.md)
- [Script style guide](./docs/style-guide.md)

## Compatibility

ph3sx is not fully compatible with vanilla Danmakufu ph3. In particular,
replays, saved common data, and `.dat` archives are not interchangeable between
the two engines. Do not assume that files created by one version can be read by
the other.

## Build on Windows

The solution currently defines `Win32` configurations only. The checked-in
projects use the **v142** Visual C++ toolset, the Windows 10 SDK, and DirectX 9
including the legacy D3DX9 components.

### Prerequisites

- Visual Studio 2019, or Visual Studio 2022 with the **v142 build tools**
  installed.
- The **Desktop development with C++** workload and a Windows 10 SDK.
- DirectX SDK (June 2010), with `DXSDK_DIR` configured so the projects can find
  the DirectX 9/D3DX9 headers and libraries.
- vcpkg manifest support in Visual Studio for the dependencies listed in
  [`vcpkg.json`](./vcpkg.json).
- Git, including support for submodules.

### Clone and build

Clone the repository and its submodules:

```sh
git clone --recurse-submodules https://github.com/MaxGog/Touhou-Danmakufu-ph3sx-2.git
cd Touhou-Danmakufu-ph3sx-2
```

If you already cloned without submodules, initialize them with:

```sh
git submodule update --init --recursive
```

Open `GcProject.sln` in Visual Studio, select a `Win32` configuration such as
`Release`, and build the solution. The projects include the game executor,
configuration utility, and archive utility. Build outputs are written to
`bin_th_dnh/` and `bin_ext/`.

## SDL3 platform layer (migration work in progress)

The game executor now polls SDL3 events and uses SDL3 for keyboard, mouse, and
joystick/gamepad input. Its Windows build wraps the existing Win32 game window;
the renderer and other Win32/DirectX 9 facilities are still legacy code. The
standalone CMake target builds the SDL3 platform library and its input tests,
not the full game executor. For the current component boundaries, frame flow,
and planned rendering backends, see the
[engine architecture guide](./docs/architecture.md).

Initialize `SDLPlatform` before creating `SDLWindow` and `SDLInput`; each frame,
call `SDLPlatform::PollEvents` before `SDLInput::Update`. The executor bridges
SDL input into the existing key-state and virtual-key interfaces, preserving
the legacy DirectInput scan-code IDs used by scripts and saved bindings.

Install CMake and vcpkg, and set `VCPKG_ROOT` to the vcpkg checkout.
Configure and build for Apple Silicon on macOS:

```sh
cmake -S . -B build/macos-arm64 \
  -DCMAKE_TOOLCHAIN_FILE="$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake" \
  -DVCPKG_TARGET_TRIPLET=arm64-osx
cmake --build build/macos-arm64 --target ph3sx_sdl_platform
ctest --test-dir build/macos-arm64 --output-on-failure
```

Configure and build for Windows 11 x64 from PowerShell:

```powershell
cmake -S . -B build/windows-x64 `
  -DCMAKE_TOOLCHAIN_FILE="$env:VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake" `
  -DVCPKG_TARGET_TRIPLET=x64-windows
cmake --build build/windows-x64 --target ph3sx_sdl_platform
ctest --test-dir build/windows-x64 --output-on-failure
```

## Working with this fork

This repository is the fork (`origin`); the original project is `upstream`.
After cloning your own fork, configure and verify the original remote once:

```sh
git remote add upstream https://github.com/Natashi/Touhou-Danmakufu-ph3sx-2.git
git remote -v
```

Fetch upstream changes and merge them into your local base branch before
publishing the updated branch to your fork:

```sh
git fetch upstream
git switch master
git merge upstream/master
git push origin master
```

Create feature branches from the appropriate base branch, push them to your
fork, and open a pull request against the original repository. Avoid force
pushing shared branches.

## Reporting issues

When filing an issue, include the engine version, Windows version, steps to
reproduce the problem, and relevant logs or scripts. You can also discuss ph3sx
in the [development Discord](https://discord.gg/f9KFujKGEx).

## Credits and related projects

- [Original Danmakufu ph3 source](https://github.com/james7132/Danmakufu-ph3)
- [Danmakufu Woo Edition](https://github.com/WishMakers0/Danmakufu-Woo-Edition)
- [ph3sx zlabel fork](https://github.com/nazjun/Touhou-Danmakufu-ph3sx-zlabel)
- Thanks to WishMakers for the Woo Edition reference and to Naudiz and other
  contributors for testing, bug reports, and feature requests.

## License

This project is distributed under the [MIT License](./LICENSE.md). Third-party
components in `submodules/` and dependencies managed by vcpkg retain their own
licenses.
