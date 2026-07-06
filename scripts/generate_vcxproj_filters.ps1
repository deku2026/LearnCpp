# Generates vs/learn_cpp.vcxproj.filters so Solution Explorer mirrors the
# src/ and include/ directory trees (same effect as CMake source_group(TREE)).
#
# Usage: pwsh -File scripts/generate_vcxproj_filters.ps1
# Run from the repo root.  Commit the generated file so VS is correct on open.

$ErrorActionPreference = 'Stop'
Set-Location (Split-Path $PSScriptRoot -Parent)

$vcxprojDir = 'vs'
$filtersFile = Join-Path $vcxprojDir 'learn_cpp.vcxproj.filters'

# ------------------------------------------------------------------
# Collect files
# ------------------------------------------------------------------
$srcFiles = Get-ChildItem -Path src -Recurse -Filter *.cpp | ForEach-Object {
    @{
        RelativePath = $_.FullName.Substring((Get-Location).Path.Length + 1)
        FilterPath   = 'Source Files\src' + ($_.DirectoryName.Substring((Get-Location).Path.Length + 1) -replace '^src', '') -replace '\\', '\'
    }
}

$includeFiles = Get-ChildItem -Path include -Recurse -Include *.hpp, *.h | ForEach-Object {
    @{
        RelativePath = $_.FullName.Substring((Get-Location).Path.Length + 1)
        FilterPath   = 'Header Files\include' + ($_.DirectoryName.Substring((Get-Location).Path.Length + 1) -replace '^include', '') -replace '\\', '\'
    }
}

# ------------------------------------------------------------------
# Collect unique filter paths
# ------------------------------------------------------------------
$filterPaths = [System.Collections.Generic.HashSet[string]]::new()
foreach ($f in $srcFiles)    { [void]$filterPaths.Add($f.FilterPath) }
foreach ($f in $includeFiles) { [void]$filterPaths.Add($f.FilterPath) }

# Sort by depth then alphabetically so parent filters come first
$sortedFilters = $filterPaths | Sort-Object { $_.Split('\').Count }, { $_ }

# Generate deterministic GUIDs from filter path (MD5 → GUID format)
function Get-FilterGuid([string]$path) {
    $hash = [System.Security.Cryptography.MD5]::Create().ComputeHash(
        [System.Text.Encoding]::UTF8.GetBytes($path)
    )
    $hex = [System.BitConverter]::ToString($hash).Replace('-', '')
    $guid = '{' + $hex.Substring(0,8) + '-' + $hex.Substring(8,4) + '-' +
            $hex.Substring(12,4) + '-' + $hex.Substring(16,4) + '-' +
            $hex.Substring(20,12) + '}'
    return $guid.ToUpper()
}

# ------------------------------------------------------------------
# Generate XML
# ------------------------------------------------------------------
$xml = [System.Text.StringBuilder]::new()
[void]$xml.AppendLine('<?xml version="1.0" encoding="utf-8"?>')
[void]$xml.AppendLine('<Project ToolsVersion="4.0" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">')

# --- Filter definitions ---
[void]$xml.AppendLine('  <ItemGroup>')
foreach ($filter in $sortedFilters) {
    $guid = Get-FilterGuid $filter
    [void]$xml.AppendLine("    <Filter Include=`"$filter`">")
    [void]$xml.AppendLine("      <UniqueIdentifier>$guid</UniqueIdentifier>")
    [void]$xml.AppendLine('    </Filter>')
}
[void]$xml.AppendLine('  </ItemGroup>')

# --- Source files ---
[void]$xml.AppendLine('  <ItemGroup>')
$srcFilesSorted = $srcFiles | Sort-Object RelativePath
foreach ($f in $srcFilesSorted) {
    $rel = $f.RelativePath -replace '\\', '\'
    [void]$xml.AppendLine("    <ClCompile Include=`"..\$rel`">")
    [void]$xml.AppendLine("      <Filter>$($f.FilterPath)</Filter>")
    [void]$xml.AppendLine('    </ClCompile>')
}
[void]$xml.AppendLine('  </ItemGroup>')

# --- Header files ---
[void]$xml.AppendLine('  <ItemGroup>')
$includeFilesSorted = $includeFiles | Sort-Object RelativePath
foreach ($f in $includeFilesSorted) {
    $rel = $f.RelativePath -replace '\\', '\'
    [void]$xml.AppendLine("    <ClInclude Include=`"..\$rel`">")
    [void]$xml.AppendLine("      <Filter>$($f.FilterPath)</Filter>")
    [void]$xml.AppendLine('    </ClInclude>')
}
[void]$xml.AppendLine('  </ItemGroup>')

[void]$xml.AppendLine('</Project>')

# ------------------------------------------------------------------
# Write output
# ------------------------------------------------------------------
Set-Content -Path $filtersFile -Value $xml.ToString() -Encoding UTF8

$cppCount = $srcFiles.Count
$hdrCount = $includeFiles.Count
$filterCount = $sortedFilters.Count
Write-Host "Generated $filtersFile"
Write-Host "  $cppCount .cpp files under Source Files\src\"
Write-Host "  $hdrCount .hpp/.h files under Header Files\include\"
Write-Host "  $filterCount unique filters"
