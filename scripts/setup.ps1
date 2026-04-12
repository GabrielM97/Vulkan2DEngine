param(
    [switch]$Build
)

$ErrorActionPreference = "Stop"

$repoRoot = Split-Path -Parent $PSScriptRoot
$defaultVcpkgRoot = "C:\vcpkg"

if (-not $env:VCPKG_ROOT -or [string]::IsNullOrWhiteSpace($env:VCPKG_ROOT)) {
    if (Test-Path $defaultVcpkgRoot) {
        $env:VCPKG_ROOT = $defaultVcpkgRoot
    } else {
        throw "VCPKG_ROOT is not set and C:\vcpkg was not found. Install vcpkg or set VCPKG_ROOT first."
    }
}

$toolchain = Join-Path $env:VCPKG_ROOT "scripts\buildsystems\vcpkg.cmake"
if (-not (Test-Path $toolchain)) {
    throw "Could not find vcpkg toolchain at '$toolchain'."
}

$vcpkgExe = Join-Path $env:VCPKG_ROOT "vcpkg.exe"
$bootstrapScript = Join-Path $env:VCPKG_ROOT "bootstrap-vcpkg.bat"
if (-not (Test-Path $vcpkgExe)) {
    if (-not (Test-Path $bootstrapScript)) {
        throw "Could not find vcpkg.exe or bootstrap-vcpkg.bat in '$env:VCPKG_ROOT'."
    }

    & $bootstrapScript
    if ($LASTEXITCODE -ne 0) {
        throw "Failed to bootstrap vcpkg."
    }
}

Push-Location $repoRoot
try {
    & cmake --preset windows-debug
    if ($LASTEXITCODE -ne 0) {
        throw "CMake configure failed."
    }

    if ($Build) {
        & cmake --build --preset build-debug
        if ($LASTEXITCODE -ne 0) {
            throw "CMake build failed."
        }
    }
}
finally {
    Pop-Location
}
