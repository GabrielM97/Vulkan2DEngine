import os
import time
import subprocess
import shutil

PROJECT_ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))

SRC_DIR = os.path.join(PROJECT_ROOT, "Assets", "Shaders")
BUILD_DIR = os.path.join(PROJECT_ROOT, "Build","Debug", "Assets", "Shaders")
RUNTIME_DIR = os.path.join(PROJECT_ROOT,"Assets", "Shaders")

GLSLC = "glslc"

os.makedirs(BUILD_DIR, exist_ok=True)
os.makedirs(RUNTIME_DIR, exist_ok=True)

last_write_times = {}

def compile_shader(path):
    filename = os.path.basename(path)
    out_file = os.path.join(BUILD_DIR, filename + ".spv")
    runtime_file = os.path.join(RUNTIME_DIR, filename + ".spv")

    print(f"[Shader] Compiling: {filename}")

    try:
        subprocess.check_call([GLSLC, path, "-o", out_file])
        shutil.copy2(out_file, runtime_file)
        print(f"[Shader] Updated: {filename}.spv")
    except subprocess.CalledProcessError:
        print(f"[Error] Failed: {filename}")

def scan():
    for root, _, files in os.walk(SRC_DIR):
        for f in files:
            if not f.endswith((".vert", ".frag")):
                continue

            full_path = os.path.join(root, f)
            mod_time = os.path.getmtime(full_path)

            if full_path not in last_write_times:
                last_write_times[full_path] = mod_time
                compile_shader(full_path)
            elif last_write_times[full_path] != mod_time:
                last_write_times[full_path] = mod_time
                compile_shader(full_path)

def main():
    print("[ShaderWatcher] Running...")
    while True:
        scan()
        time.sleep(0.5)

if __name__ == "__main__":
    main()