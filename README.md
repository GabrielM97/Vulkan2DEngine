# Vulkan2DEngine

## Setup
1. Install the Vulkan SDK and make sure `VULKAN_SDK` is set.
2. Install or clone `vcpkg` and set `VCPKG_ROOT`.
   If you keep it at `C:\vcpkg`, the setup script will find it automatically.
3. Run:

```powershell
.\scripts\setup.ps1 -Build
```

This project uses vcpkg manifest mode, so CMake will automatically install the
repo's declared dependencies during configure. Runtime DLLs are copied into the
build output automatically after build.

You can also run the steps manually:

```powershell
cmake --preset windows-debug
cmake --build --preset build-debug
```
