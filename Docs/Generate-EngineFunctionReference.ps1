param(
    [string]$OutputPath = 'Docs/EngineFunctionReference.md'
)

Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'

function Get-SystemName {
    param([string]$RelativePath)

    switch -Regex ($RelativePath.Replace('\', '/')) {
        '^Engine/Core/' { return 'Core' }
        '^Engine/Platform/' { return 'Platform' }
        '^Engine/Math/' { return 'Math' }
        '^Engine/Reflection/' { return 'Reflection' }
        '^Engine/Component/' { return 'Component System' }
        '^Engine/Scene/' { return 'Scene' }
        '^Engine/Object/' { return 'Object Layer' }
        '^Engine/Renderer/' { return 'Renderer' }
        '^Engine/Editor/' { return 'Editor' }
        '^Sandbox/' { return 'Sandbox' }
        default { return 'Miscellaneous' }
    }
}

function New-TypeDoc {
    param(
        [string]$Name,
        [string]$Kind,
        [string]$Path,
        [string]$System
    )

    [pscustomobject]@{
        Name = $Name
        Kind = $Kind
        Path = $Path
        System = $System
        Functions = New-Object System.Collections.Generic.List[object]
    }
}

function New-FunctionDoc {
    param(
        [string]$Signature,
        [string]$Access
    )

    [pscustomobject]@{
        Signature = $Signature
        Access = $Access
    }
}

function Get-DefaultAccess {
    param([string]$Kind)

    if ($Kind -eq 'struct') {
        return 'public'
    }

    return 'private'
}

function To-RelativeDocPath {
    param([string]$FullPath)

    $root = (Get-Location).Path
    $relative = Resolve-Path -LiteralPath $FullPath | ForEach-Object { $_.Path.Substring($root.Length).TrimStart('\') }
    return $relative.Replace('\', '/')
}

function Normalize-Signature {
    param([string]$Text)

    $signature = ($Text -replace '\s+', ' ').Trim()
    $signature = $signature -replace '\)\s*:\s*.*$', ')'
    $signature = $signature -replace '\s*\{.*$', ''
    $signature = $signature -replace '\s*\\$', ''
    $signature = $signature -replace ';+$', ''
    $signature = $signature -replace '\s*=\s*(default|delete)$', ''
    $signature = $signature.Trim()

    if (-not $signature.Contains('(')) {
        return $null
    }

    return $signature
}

function Looks-LikeCallable {
    param([string]$Line)

    $trimmed = $Line.Trim()

    if (-not $trimmed.Contains('(')) { return $false }
    if ($trimmed -match '^[A-Z][A-Z0-9_]*\s*\(') { return $false }
    if ($trimmed -match '^(if|for|while|switch|return|static_assert)\b') { return $false }
    if ($trimmed -match '^using\b') { return $false }
    if ($trimmed -match '^\#') { return $false }
    if ($trimmed -match '^friend\b') { return $false }
    if ($trimmed -match '^\s*$') { return $false }

    return $true
}

function Extract-MethodName {
    param([string]$Signature)

    $match = [regex]::Match($Signature, '([~A-Za-z_][A-Za-z0-9_:<>]*)\s*\(')
    if (-not $match.Success) {
        return ''
    }

    $fullName = $match.Groups[1].Value
    $parts = $fullName.Split('::')
    return $parts[$parts.Length - 1]
}

function Extract-ArgumentList {
    param([string]$Signature)

    $match = [regex]::Match($Signature, '\((.*)\)')
    if (-not $match.Success) {
        return @()
    }

    $inner = $match.Groups[1].Value.Trim()
    if ([string]::IsNullOrWhiteSpace($inner) -or $inner -eq 'void') {
        return @()
    }

    return $inner.Split(',') | ForEach-Object { $_.Trim() }
}

function To-VariableName {
    param([string]$Name)

    $clean = ($Name -replace '[^A-Za-z0-9]', '')
    if ([string]::IsNullOrWhiteSpace($clean)) {
        return 'value'
    }

    if ($clean.Length -eq 1) {
        return $clean.ToLowerInvariant()
    }

    return $clean.Substring(0, 1).ToLowerInvariant() + $clean.Substring(1)
}

function Build-ArgumentExample {
    param([string[]]$Arguments)

    $values = foreach ($arg in $Arguments) {
        if ($arg -match '\bbool\b') { 'true'; continue }
        if ($arg -match '\bfloat\b') { '1.0f'; continue }
        if ($arg -match '\bdouble\b') { '1.0'; continue }
        if ($arg -match '\b(int|uint|size_t|uint32_t|int32_t|uint64_t)\b') { '0'; continue }
        if ($arg -match 'std::string|const char') { '"example"'; continue }
        if ($arg -match 'glm::vec2') { '{0.0f, 0.0f}'; continue }
        if ($arg -match 'glm::vec4') { '{1.0f, 1.0f, 1.0f, 1.0f}'; continue }
        if ($arg -match 'glm::ivec2') { '{0, 0}'; continue }
        if ($arg -match 'Transform2D') { '{}'; continue }
        if ($arg -match 'GameObjectID') { '0'; continue }
        if ($arg -match 'Scene\b') { 'scene'; continue }
        if ($arg -match 'Entity\b') { 'entity'; continue }
        if ($arg -match 'VulkanRenderer|IRenderer2D') { 'renderer'; continue }
        if ($arg -match 'InputState') { 'input'; continue }
        if ($arg -match 'SceneViewportState') { 'viewportState'; continue }
        if ($arg -match 'std::vector') { '{}'; continue }
        if ($arg -match 'const .*&') { '{}'; continue }

        '/* ... */'
    }

    return ($values -join ', ')
}

function Build-InvocationTarget {
    param(
        [string]$Signature,
        [pscustomobject]$TypeDoc
    )

    if ($TypeDoc.Kind -eq 'namespace') {
        return $TypeDoc.Name
    }

    if ($Signature -match '^static ') {
        return $TypeDoc.Name
    }

    return (To-VariableName $TypeDoc.Name)
}

function Build-FunctionExample {
    param(
        [string]$Signature,
        [pscustomobject]$TypeDoc,
        [string]$Access = 'public'
    )

    $methodName = Extract-MethodName $Signature
    if ([string]::IsNullOrWhiteSpace($methodName)) {
        return @('// Example unavailable for this declaration.')
    }

    $args = Extract-ArgumentList $Signature
    $argsExample = Build-ArgumentExample $args
    $target = Build-InvocationTarget $Signature $TypeDoc

    if ($methodName -eq $TypeDoc.Name) {
        if ([string]::IsNullOrWhiteSpace($argsExample)) {
            return @($TypeDoc.Name + ' ' + (To-VariableName $TypeDoc.Name) + ';')
        }

        return @($TypeDoc.Name + ' ' + (To-VariableName $TypeDoc.Name) + '(' + $argsExample + ');')
    }

    if ($methodName.StartsWith('~')) {
        return @('// Destructor calls are usually automatic when the object leaves scope.')
    }

    if ($Access -eq 'private') {
        return @('// This helper is intended to be called from inside ' + $TypeDoc.Name + ' itself.')
    }

    if ($Access -eq 'protected') {
        return @('// Call this from ' + $TypeDoc.Name + ' or a derived type when extending behavior.')
    }

    if ($methodName -match '^(Is|Has|Can|Was)') {
        return @(
            'if (' + $target + '.' + $methodName + '(' + $argsExample + '))',
            '{',
            '    // React to the returned state here.',
            '}'
        )
    }

    if ($methodName -match '^Get') {
        return @('auto value = ' + $target + '.' + $methodName + '(' + $argsExample + ');')
    }

    return @($target + '.' + $methodName + '(' + $argsExample + ');')
}

function Get-TypeSummary {
    param([pscustomobject]$TypeDoc)

    $name = $TypeDoc.Name
    $path = $TypeDoc.Path

    if ($TypeDoc.Kind -eq 'namespace') {
        return 'Provides grouped helper functions for the ' + $name + ' subsystem so related math or utility calls stay in one discoverable place.'
    }

    if ($name -match 'Scene') {
        return 'Owns or organizes scene state for this part of the engine, including object lifetime, transforms, and editor or runtime coordination.'
    }
    if ($name -match 'Renderer|Vulkan|Framebuffer|Pipeline|Buffer|Texture|SwapChain|RenderPass|Context|Device') {
        return 'Wraps a renderer-facing system resource so higher-level engine code can perform drawing or GPU setup through a focused API.'
    }
    if ($name -match 'Editor|Panel|Inspector|Controller') {
        return 'Packages editor-facing behavior for inspecting, editing, or interacting with engine data inside the tooling workflow.'
    }
    if ($name -match 'Component') {
        return 'Represents a component or component-related helper used to attach data and behavior to game objects in a consistent way.'
    }
    if ($name -match 'Camera') {
        return 'Stores camera state and exposes the operations needed to move between world space and viewport space.'
    }
    if ($name -match 'Animation') {
        return 'Defines animation data or playback helpers used to drive sprite motion and clip selection.'
    }
    if ($name -match 'Tile') {
        return 'Handles tile-map or tile-set data so levels, tiles, and per-cell state can be edited and rendered consistently.'
    }
    if ($name -match 'Entity|Actor|Object|Player') {
        return 'Acts as a gameplay-facing wrapper around engine state so game code can work with objects through a clear, typed surface.'
    }
    if ($name -match 'Application|Window|Input') {
        return 'Supports the engine lifecycle by handling application flow, platform integration, or per-frame input capture.'
    }
    if ($name -match 'Property|Reflection') {
        return 'Supplies reflection and serialization metadata so editor and save systems can discover values automatically.'
    }
    if ($TypeDoc.Kind -eq 'struct') {
        return 'Carries related engine data in a compact type so other systems can pass, store, or serialize it consistently.'
    }

    return 'Provides a focused engine type declared in `' + $path + '` so this subsystem has a clear place for the behavior it exposes.'
}

function Get-TypeUseCaseText {
    param([pscustomobject]$TypeDoc)

    $name = $TypeDoc.Name

    if ($name -eq 'Scene') { return 'Use `Scene` when you need to create entities, step gameplay state, and render or serialize the current level.' }
    if ($name -eq 'Entity') { return 'Use `Entity` when gameplay code needs to query or mutate a single game object through a convenient handle.' }
    if ($name -eq 'Application') { return 'Use `Application` as the base class for a game or sandbox app that owns startup, per-frame updates, rendering, and shutdown.' }
    if ($name -eq 'Camera2D') { return 'Use `Camera2D` when you need to pan, zoom, or convert coordinates between screen space and world space.' }
    if ($TypeDoc.Kind -eq 'namespace') { return 'Use `' + $name + '` when you want stateless helpers instead of creating an object just to perform a calculation.' }
    if ($name -match 'Editor|Panel|Controller') { return 'Use `' + $name + '` while building or extending the editor workflow for inspecting and changing engine data.' }
    if ($name -match 'Renderer|Vulkan') { return 'Use `' + $name + '` when renderer code needs direct access to a specific GPU-side resource or rendering stage.' }
    if ($name -match 'Component') { return 'Use `' + $name + '` when an entity or editor feature needs this specific piece of attached state.' }
    if ($name -match 'Tile') { return 'Use `' + $name + '` when you are authoring or consuming tile data, tile assets, or tile editing tools.' }

    return 'Use `' + $name + '` when this subsystem is the natural owner of the behavior or data you are working with.'
}

function Build-TypeExample {
    param([pscustomobject]$TypeDoc)

    $name = $TypeDoc.Name
    $varName = To-VariableName $name

    switch ($name) {
        'Application' {
            return @(
                'class MyGameApp : public Application',
                '{',
                'protected:',
                '    void OnInit() override',
                '    {',
                '        // Create startup content here.',
                '    }',
                '};'
            )
        }
        'Scene' {
            return @(
                'Scene scene;',
                'Entity player = scene.CreateEntity("Player");',
                'scene.Update(deltaTime);',
                'scene.Render(renderer);'
            )
        }
        'Entity' {
            return @(
                'Entity entity = scene.CreateEntity("Crate");',
                'entity.SetPosition({128.0f, 64.0f});',
                'entity.EnsureBoxCollider();'
            )
        }
        'Camera2D' {
            return @(
                'Camera2D camera;',
                'camera.SetTopLeftPosition({0.0f, 0.0f});',
                'camera.SetZoom(1.25f);'
            )
        }
        'TransformMath2D' {
            return @(
                'glm::vec2 rotated = TransformMath2D::RotateVector({1.0f, 0.0f}, 90.0f);'
            )
        }
        default {
            if ($TypeDoc.Kind -eq 'namespace') {
                $lines = New-Object System.Collections.Generic.List[string]
                $lines.Add('// Example helper call')
                $lines.Add('auto result = ' + $name + '::' + '/* function */' + ';')
                return $lines.ToArray()
            }

            $lines = New-Object System.Collections.Generic.List[string]
            $lines.Add('// Example setup for ' + $name)
            $lines.Add($name + ' ' + $varName + ';')
            return $lines.ToArray()
        }
    }
}

function Get-FunctionDescription {
    param(
        [string]$Signature,
        [pscustomobject]$TypeDoc
    )

    $methodName = Extract-MethodName $Signature
    if ([string]::IsNullOrWhiteSpace($methodName)) {
        return 'Declares callable behavior exposed by `' + $TypeDoc.Name + '`.'
    }

    if ($methodName -match '^Get') { return 'Returns data from `' + $TypeDoc.Name + '` without requiring the caller to reach into its internal state directly.' }
    if ($methodName -match '^Set') { return 'Updates engine state through the supported API for `' + $TypeDoc.Name + '`.' }
    if ($methodName -match '^(Create|Spawn|Place|Add)') { return 'Creates or attaches something through `' + $TypeDoc.Name + '` and returns or applies the new result.' }
    if ($methodName -match '^(Load|Save|Serialize|Deserialize)') { return 'Moves data into or out of `' + $TypeDoc.Name + '` so it can be persisted or restored.' }
    if ($methodName -match '^(Init|Begin|End|Shutdown)') { return 'Participates in lifecycle flow for `' + $TypeDoc.Name + '`.' }
    if ($methodName -match '^(Update|Render|Draw)') { return 'Advances or displays `' + $TypeDoc.Name + '` as part of the current frame.' }
    if ($methodName -match '^(Move|Resolve)') { return 'Applies movement or spatial resolution logic associated with `' + $TypeDoc.Name + '`.' }
    if ($methodName -match '^(Is|Has|Can)') { return 'Answers a yes or no question about the current state of `' + $TypeDoc.Name + '`.' }
    if ($methodName -match '^(Play|Stop|Reset)') { return 'Controls runtime playback or reset behavior exposed by `' + $TypeDoc.Name + '`.' }
    if ($methodName -match '^(Remove|Destroy|Clear)') { return 'Removes, clears, or tears down data owned by `' + $TypeDoc.Name + '`.' }

    return 'Exposes a focused operation on `' + $TypeDoc.Name + '` for code that needs this specific behavior.'
}

function Get-FunctionNotes {
    param(
        [string]$Signature,
        [pscustomobject]$TypeDoc,
        [string]$Access
    )

    $visibility = if ([string]::IsNullOrWhiteSpace($Access)) { 'declared' } else { $Access + ' member' }
    return 'This is a ' + $visibility + ' in `' + $TypeDoc.Path + '`. Use it when `' + $TypeDoc.Name + '` is the right owner for this operation.'
}

function Parse-Headers {
    $headerFiles = Get-ChildItem -Path 'Engine', 'Sandbox' -Recurse -Include *.h | Sort-Object FullName
    $allTypes = New-Object System.Collections.Generic.List[object]

    foreach ($file in $headerFiles) {
        $relativePath = To-RelativeDocPath $file.FullName
        $system = Get-SystemName $relativePath
        $lines = Get-Content -LiteralPath $file.FullName

        $stack = New-Object System.Collections.Generic.List[object]
        $pendingType = $null
        $pendingTemplate = $null
        $currentStatement = $null
        $statementOwner = $null
        $statementAccess = $null
        $braceDepth = 0

        foreach ($line in $lines) {
            $trimmed = $line.Trim()

            if ($pendingType -and $trimmed.Contains('{')) {
                $pendingType.Level = $braceDepth + 1
                $stack.Add($pendingType)
                $allTypes.Add($pendingType.Doc)
                $pendingType = $null
            }

            $owner = if ($stack.Count -gt 0) { $stack[$stack.Count - 1] } else { $null }

            if ($owner -and $owner.Kind -ne 'namespace' -and $trimmed -match '^(public|private|protected)\s*:\s*$') {
                $owner.Access = $matches[1]
            }

            if ($currentStatement) {
                $currentStatement += ' ' + $trimmed
                if ($trimmed.Contains(';') -or $trimmed.Contains('{')) {
                    $signature = Normalize-Signature $currentStatement
                    if ($signature) {
                        $statementOwner.Doc.Functions.Add((New-FunctionDoc -Signature $signature -Access $statementAccess))
                    }

                    $currentStatement = $null
                    $statementOwner = $null
                    $statementAccess = $null
                }
            }
            elseif ($trimmed -match '^template\s*<') {
                $pendingTemplate = $trimmed
            }
            elseif ($trimmed -match '^namespace\s+([A-Za-z_][A-Za-z0-9_]*)\s*$') {
                $namespaceName = $matches[1]
                $pendingType = [pscustomobject]@{
                    Name = $namespaceName
                    Kind = 'namespace'
                    Access = 'public'
                    Level = 0
                    Doc = (New-TypeDoc -Name $namespaceName -Kind 'namespace' -Path $relativePath -System $system)
                }
            }
            elseif ($trimmed -match '^(class|struct)\s+([A-Za-z_][A-Za-z0-9_]*)\b' -and $trimmed -notmatch ';\s*$') {
                $kind = $matches[1]
                $name = $matches[2]
                $fullName = if ($owner -and $owner.Kind -ne 'namespace') { $owner.Doc.Name + '::' + $name } else { $name }
                $newType = [pscustomobject]@{
                    Name = $fullName
                    Kind = $kind
                    Access = (Get-DefaultAccess $kind)
                    Level = 0
                    Doc = (New-TypeDoc -Name $fullName -Kind $kind -Path $relativePath -System $system)
                }

                if ($trimmed.Contains('{')) {
                    $newType.Level = $braceDepth + 1
                    $stack.Add($newType)
                    $allTypes.Add($newType.Doc)
                }
                else {
                    $pendingType = $newType
                }
            }
            elseif ($owner -and $braceDepth -eq $owner.Level -and (Looks-LikeCallable $trimmed)) {
                $statementText = if ($pendingTemplate) { $pendingTemplate + ' ' + $trimmed } else { $trimmed }
                if ($trimmed.Contains(';') -or $trimmed.Contains('{')) {
                    $signature = Normalize-Signature $statementText
                    if ($signature) {
                        $owner.Doc.Functions.Add((New-FunctionDoc -Signature $signature -Access $owner.Access))
                    }
                }
                else {
                    $currentStatement = $statementText
                    $statementOwner = $owner
                    $statementAccess = $owner.Access
                }

                $pendingTemplate = $null
            }
            else {
                $pendingTemplate = $null
            }

            $openCount = ([regex]::Matches($line, '\{')).Count
            $closeCount = ([regex]::Matches($line, '\}')).Count
            $braceDepth += ($openCount - $closeCount)

            while ($stack.Count -gt 0 -and $stack[$stack.Count - 1].Level -gt $braceDepth) {
                $stack.RemoveAt($stack.Count - 1)
            }
        }

        if ($allTypes.Where({ $_.Path -eq $relativePath }).Count -eq 0) {
            $fallbackName = [System.IO.Path]::GetFileNameWithoutExtension($relativePath)
            $allTypes.Add((New-TypeDoc -Name $fallbackName -Kind 'header' -Path $relativePath -System $system))
        }
    }

    return (Merge-TypeDocs $allTypes)
}

function Merge-TypeDocs {
    param([System.Collections.Generic.List[object]]$Types)

    $merged = New-Object System.Collections.Generic.List[object]
    $byKey = @{}

    foreach ($typeDoc in $Types) {
        $key = $typeDoc.System + '|' + $typeDoc.Path + '|' + $typeDoc.Name
        if (-not $byKey.ContainsKey($key)) {
            $clone = New-TypeDoc -Name $typeDoc.Name -Kind $typeDoc.Kind -Path $typeDoc.Path -System $typeDoc.System
            foreach ($fn in $typeDoc.Functions) {
                $clone.Functions.Add((New-FunctionDoc -Signature $fn.Signature -Access $fn.Access))
            }

            $byKey[$key] = $clone
            $merged.Add($clone)
            continue
        }

        $existing = $byKey[$key]
        if ($existing.Kind -eq 'header' -and $typeDoc.Kind -ne 'header') {
            $existing.Kind = $typeDoc.Kind
        }

        foreach ($fn in $typeDoc.Functions) {
            $alreadyPresent = $existing.Functions | Where-Object {
                $_.Signature -eq $fn.Signature -and $_.Access -eq $fn.Access
            } | Select-Object -First 1

            if (-not $alreadyPresent) {
                $existing.Functions.Add((New-FunctionDoc -Signature $fn.Signature -Access $fn.Access))
            }
        }
    }

    return $merged
}

function Write-Reference {
    param(
        [System.Collections.Generic.List[object]]$Types,
        [string]$Destination
    )

    $tick = [char]96
    $output = New-Object System.Collections.Generic.List[string]
    $output.Add('# Engine Function Reference')
    $output.Add('')
    $output.Add('## Format')
    $output.Add('This reference is organized by subsystem and then by declared type. Each type uses the same pattern: a short summary, a practical example use case, a few highlights, and then a function-by-function breakdown with notes and example calls.')
    $output.Add('')

    $systemOrder = @('Core', 'Platform', 'Math', 'Reflection', 'Component System', 'Scene', 'Object Layer', 'Renderer', 'Editor', 'Sandbox', 'Miscellaneous')

    foreach ($system in $systemOrder) {
        $systemTypes = $Types | Where-Object { $_.System -eq $system } | Sort-Object Path, Name
        if (-not $systemTypes) {
            continue
        }

        $output.Add('## ' + $system)
        $output.Add('')

        foreach ($typeDoc in $systemTypes) {
            $output.Add('### ' + $tick + $typeDoc.Name + $tick)
            $output.Add('')
            $output.Add('**Summary**')
            $output.Add((Get-TypeSummary $typeDoc))
            $output.Add('')
            $output.Add('**Example Use Case**')
            $output.Add((Get-TypeUseCaseText $typeDoc))
            $output.Add('```cpp')
            foreach ($exampleLine in (Build-TypeExample $typeDoc)) {
                $output.Add($exampleLine)
            }
            $output.Add('```')
            $output.Add('')
            $output.Add('**Highlights**')
            $output.Add('- Declared in ' + $tick + $typeDoc.Path + $tick + '.')
            $output.Add('- Type kind: ' + $typeDoc.Kind + '.')
            $output.Add('- Documented functions: ' + $typeDoc.Functions.Count + '.')

            if ($typeDoc.Functions.Count -eq 0) {
                $output.Add('- This type is primarily a data holder, marker, or grouping construct in the header.')
                $output.Add('')
                continue
            }

            $output.Add('')

            foreach ($functionDoc in $typeDoc.Functions) {
                $output.Add($tick + $functionDoc.Signature + $tick)
                $output.Add('- What it does: ' + (Get-FunctionDescription -Signature $functionDoc.Signature -TypeDoc $typeDoc))
                $output.Add('- Notes: ' + (Get-FunctionNotes -Signature $functionDoc.Signature -TypeDoc $typeDoc -Access $functionDoc.Access))
                $output.Add('- Example use:')
                $output.Add('```cpp')
                foreach ($exampleLine in (Build-FunctionExample -Signature $functionDoc.Signature -TypeDoc $typeDoc -Access $functionDoc.Access)) {
                    $output.Add($exampleLine)
                }
                $output.Add('```')
                $output.Add('')
            }
        }
    }

    $output.Add('## Notes')
    $output.Add('- This file is generated from the current header surface in the repo.')
    $output.Add('- Regenerate it with ' + $tick + 'powershell -ExecutionPolicy Bypass -File Docs/Generate-EngineFunctionReference.ps1' + $tick + '.')
    $output.Add('- The docs site in ' + $tick + 'DocsSite/' + $tick + ' reads this file as one of its primary sources.')

    $output | Set-Content -LiteralPath $Destination -Encoding UTF8
}

$types = Parse-Headers
Write-Reference -Types $types -Destination $OutputPath
