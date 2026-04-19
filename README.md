# Vulkan2DEngine

Documentation:

- `Docs/EngineDocumentation.md` for architecture, workflows, and examples
- `Docs/HeaderReference.md` for the header-by-header API reference
- `Docs/EngineFunctionReference.md` for the categorized function-by-function breakdown
- `Docs/EngineFunctionReference.rtf` for a Word-compatible version
- `Docs/Generate-EngineFunctionReference.ps1` to regenerate the standardized function reference from the current headers

Docs website:

- `DocsSite/index.html` is the static documentation app
- `DocsSite/launch-docs.bat` launches the local docs website on Windows
- `DocsSite/launch-docs.ps1` runs the local HTTP server directly

## Setup
1. Install the Vulkan SDK and make sure `VULKAN_SDK` is set.
2. Install or clone `vcpkg` and set `VCPKG_ROOT`.
   If you keep it at `C:\vcpkg`, the setup script will find it automatically.
3. Run one of these:

```powershell
.\scripts\setup.ps1 -Build
```

```bat
scripts\setup.bat -Build
```

This project uses vcpkg manifest mode, so CMake will automatically install the
repo's declared dependencies during configure. Runtime DLLs are copied into the
build output automatically after build.

You can also run the steps manually:

```powershell
cmake --preset windows-debug
cmake --build --preset build-debug
```
