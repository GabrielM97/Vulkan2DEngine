param(
    [int]$Port = 8088,
    [switch]$NoBrowser
)

$repoRoot = Split-Path -Parent $PSScriptRoot
$listener = [System.Net.HttpListener]::new()
$prefix = "http://localhost:$Port/"
$listener.Prefixes.Add($prefix)
$listener.Start()

$contentTypes = @{
    ".html" = "text/html; charset=utf-8"
    ".css"  = "text/css; charset=utf-8"
    ".js"   = "application/javascript; charset=utf-8"
    ".md"   = "text/markdown; charset=utf-8"
    ".rtf"  = "application/rtf"
    ".json" = "application/json; charset=utf-8"
    ".png"  = "image/png"
    ".jpg"  = "image/jpeg"
    ".jpeg" = "image/jpeg"
    ".svg"  = "image/svg+xml"
    ".ico"  = "image/x-icon"
    ".txt"  = "text/plain; charset=utf-8"
}

if (-not $NoBrowser) {
    Start-Process ($prefix + "DocsSite/index.html") | Out-Null
}

Write-Host ""
Write-Host "Vulkan2DRenderer docs site is running."
Write-Host ("Open: " + $prefix + "DocsSite/index.html")
Write-Host "Press Ctrl+C to stop."
Write-Host ""

try {
    while ($listener.IsListening) {
        $context = $listener.GetContext()
        $response = $context.Response

        try {
            $relativePath = [System.Uri]::UnescapeDataString($context.Request.Url.AbsolutePath.TrimStart('/'))
            if ([string]::IsNullOrWhiteSpace($relativePath)) {
                $relativePath = "DocsSite/index.html"
            }

            $fullPath = [System.IO.Path]::GetFullPath((Join-Path $repoRoot $relativePath))
            $rootPath = [System.IO.Path]::GetFullPath($repoRoot)

            if (-not $fullPath.StartsWith($rootPath, [System.StringComparison]::OrdinalIgnoreCase)) {
                $response.StatusCode = 403
                $buffer = [System.Text.Encoding]::UTF8.GetBytes("Forbidden")
                $response.OutputStream.Write($buffer, 0, $buffer.Length)
                continue
            }

            if ((Test-Path $fullPath) -and (Get-Item $fullPath).PSIsContainer) {
                $fullPath = Join-Path $fullPath "index.html"
            }

            if (-not (Test-Path $fullPath -PathType Leaf)) {
                $response.StatusCode = 404
                $buffer = [System.Text.Encoding]::UTF8.GetBytes("Not found")
                $response.OutputStream.Write($buffer, 0, $buffer.Length)
                continue
            }

            $extension = [System.IO.Path]::GetExtension($fullPath).ToLowerInvariant()
            $response.ContentType = $contentTypes[$extension]
            if (-not $response.ContentType) {
                $response.ContentType = "application/octet-stream"
            }

            $bytes = [System.IO.File]::ReadAllBytes($fullPath)
            $response.ContentLength64 = $bytes.Length
            $response.OutputStream.Write($bytes, 0, $bytes.Length)
        }
        catch {
            $response.StatusCode = 500
            $buffer = [System.Text.Encoding]::UTF8.GetBytes($_.Exception.Message)
            $response.OutputStream.Write($buffer, 0, $buffer.Length)
        }
        finally {
            $response.OutputStream.Close()
        }
    }
}
finally {
    $listener.Stop()
    $listener.Close()
}
