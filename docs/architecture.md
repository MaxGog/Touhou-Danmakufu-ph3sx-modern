# Engine architecture and platform migration

This guide describes the current ph3sx architecture, the SDL3 migration work
already integrated, and the intended platform direction. It distinguishes
working code from the target design: a successful build of the SDL platform
library does not mean that the complete engine runs on macOS or Windows 11 x64.

## Current architecture

The repository currently contains a Windows-only engine and tools:

- **Game executor:** `DnhExecutor` runs scripts, updates game state, and
  renders frames.
- **Graphics:** `EDirectGraphics` and related graphics classes use Win32 and
  DirectX 9, including legacy D3DX9 facilities.
- **Input:** the legacy DirectInput implementation remains available. In the
  executor, SDL3 is now the active keyboard, mouse, and joystick/gamepad input
  source and feeds the existing input interfaces.
- **Other platform services:** window management, audio, logging windows, and
  parts of the engine still depend on Windows APIs and DirectX-era code.
- **Builds:** the Visual Studio projects target Win32. A separate CMake build
  compiles and tests the SDL3 platform library; it does not build the full
  executor or replace the Visual Studio solution.

The intended platform direction is SDL3 for windowing, events, and input;
Direct3D 11 for Windows x64 rendering; and Metal for macOS on Apple Silicon.
The configuration and archive utilities are also in the overall porting
scope. These rendering backends and full application targets are planned work,
not implemented features.

## Current SDL3 integration

The SDL3 abstraction lives in
[`source/GcLib/platform/SDLPlatform.hpp`](../source/GcLib/platform/SDLPlatform.hpp)
and
[`source/GcLib/platform/SDLPlatform.cpp`](../source/GcLib/platform/SDLPlatform.cpp):

- `SDLPlatform` initializes SDL and polls events.
- `SDLWindow` creates an SDL window or wraps a native window. The executor
  currently wraps its existing Win32 window; it does not create the main
  rendering window through SDL.
- `SDLInput` tracks keyboard, mouse, joystick, and SDL gamepad state, and
  reports transitions as `Push`, `Hold`, `Pull`, and `Free`.
- `EDirectInput` bridges these states into the legacy DirectInput-compatible
  state arrays and virtual-key manager. Existing bindings and scripts keep
  using the legacy scan-code identifiers.

The executor initializes SDL after creating the DirectX 9 window. Each
executor-loop iteration polls SDL events before advancing the existing
update/render controller. A close request ends the executor loop. Input focus
loss releases held keyboard and mouse states, and queued key/button
transitions preserve short presses that occur between logic updates.

The native-window wrapper includes platform property handling for Windows and
Cocoa, but only the existing Windows/DX9 executor calls it today. That does
not make the executor runnable on macOS: its graphics initialization still
requires Win32 and DirectX 9.

## Component flow

```text
SDL event queue
      |
      v
SDLPlatform::PollEvents
      +------> SDLWindow (focus, size, close state)
      |
      +------> SDLInput (keyboard, mouse, joystick/gamepad)
                        |
                        v
               EDirectInput SDL bridge
                        |
                        v
        Legacy key states and virtual-key map
                        |
                        v
          Existing executor update and scripts

Existing executor renderer: Win32 window + DirectX 9 (unchanged)
```

The platform library is kept separate from the legacy DirectX input
implementation so the SDL types can be built and tested independently. The
legacy backend is not removed; other existing code can continue to use it
until each application is migrated.

## Build and test the SDL platform library

The CMake target uses SDL3 from vcpkg. Set `VCPKG_ROOT` to the local vcpkg
checkout, then configure and build for Apple Silicon on macOS:

```sh
cmake -S . -B build/macos-arm64 \
  -DCMAKE_TOOLCHAIN_FILE="$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake" \
  -DVCPKG_TARGET_TRIPLET=arm64-osx
cmake --build build/macos-arm64 --target ph3sx_sdl_platform_tests
ctest --test-dir build/macos-arm64 --output-on-failure
```

For Windows 11 x64, run the equivalent configuration from PowerShell:

```powershell
cmake -S . -B build/windows-x64 `
  -DCMAKE_TOOLCHAIN_FILE="$env:VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake" `
  -DVCPKG_TARGET_TRIPLET=x64-windows
cmake --build build/windows-x64 --target ph3sx_sdl_platform_tests
ctest --test-dir build/windows-x64 --output-on-failure
```

These commands build only `ph3sx_sdl_platform` and its tests. On Windows, the
Visual Studio executor obtains SDL3 through the vcpkg manifest integration in
[`vcpkg.json`](../vcpkg.json). The existing Win32/DX9 requirements still apply
to building the full executor.

Tests use SDL's dummy video driver and cover legacy scan-code mapping, key
transitions (including a queued quick press/release), mouse input, focus loss,
and quit events. Hardware controller behavior is not covered by these tests.

## Migration sequence

The intended work can proceed in increments without replacing the script
runtime all at once:

1. **Platform foundation:** keep platform APIs behind reusable window, event,
   and input boundaries. The SDL3 library and executor input bridge are the
   first slice of this stage.
2. **Window and event ownership:** migrate the executor from a wrapped Win32
   window to a window created and owned through SDL3; remove direct Win32
   message-loop assumptions from the executor.
3. **Rendering backends:** introduce a renderer boundary and implement
   Direct3D 11 for Windows x64 and Metal for macOS. Replace DirectX 9/D3DX9
   dependencies and validate rendering behavior separately on both targets.
4. **Remaining platform services and tools:** migrate audio, dialogs,
   configuration UI, archive utility, and other Win32-dependent services.
5. **Platform builds and validation:** define supported macOS arm64 and
   Windows x64 application builds, then test packaging, input devices,
   display scaling, focus/lifecycle, scripts, and representative games.

This is an architectural roadmap, not a promise that all listed subsystems
already have abstraction interfaces. Keep documentation and build status
explicit about which migration step is implemented.

## Porting constraints

- Preserve the legacy input scan-code and virtual-key behavior while migrating
  scripts and saved configuration data.
- Keep SDL event polling on the executor's main thread and update input state
  before consuming it in game logic.
- Avoid tying the future renderer interface to either Direct3D or Metal types.
- Treat native window handles as platform-specific implementation details;
  do not assume an `HWND` exists in shared engine code.
- Keep platform builds and tests independent where practical, but do not claim
  full engine support based only on a platform-library build.
