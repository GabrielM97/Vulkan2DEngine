# Vulkan2DEngine

## Setup
1. Install vcpkg
2. Create a Build directory
3. Paste the glfw3.dll into Build.
4. Edit the CMake Debug Profile Settings:
   - Set the Generator to Ninja;
   - Set CMake Options to -G Ninja -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake;
   - Set build directory to Build;
