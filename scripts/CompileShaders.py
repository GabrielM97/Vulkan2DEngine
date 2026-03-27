import os
import subprocess
import sys

# Paths
PROJECT_ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
SHADER_DIR = os.path.join(PROJECT_ROOT, "Assets", "Shaders")
OUTPUT_DIR = os.path.join(PROJECT_ROOT, "Build","Debug","Assets", "Shaders")

# GLSL compiler
GLSLC = "glslc"  # Must be in PATH

# Ensure output folder exists
os.makedirs(OUTPUT_DIR, exist_ok=True)

# Compile shaders
for file in os.listdir(SHADER_DIR):
    if file.endswith(".vert") or file.endswith(".frag"):
        shader_path = os.path.join(SHADER_DIR, file)
        spv_path = os.path.join(OUTPUT_DIR, file + ".spv")
        print(f"[Shader] Compiling {file} -> {spv_path}")
        try:
            subprocess.check_call([GLSLC, shader_path, "-o", spv_path])
        except subprocess.CalledProcessError:
            print(f"[Error] Failed to compile {file}")
            sys.exit(1)

# Copy to runtime folder
RUNTIME_DIR = os.path.join(PROJECT_ROOT, "Build", "Sandbox", "Assets", "Shaders")
os.makedirs(RUNTIME_DIR, exist_ok=True)
for spv_file in os.listdir(OUTPUT_DIR):
    src = os.path.join(OUTPUT_DIR, spv_file)
    dst = os.path.join(RUNTIME_DIR, spv_file)
    subprocess.run(["copy" if os.name == "nt" else "cp", src, dst], shell=True)

print("[Shader] All shaders compiled and copied successfully.")