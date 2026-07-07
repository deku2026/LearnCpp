<#
.SYNOPSIS
  Generates explicit VS C++ project items and matching Solution Explorer filters.

.DESCRIPTION
  The LearnCpp.slnx + vs/learn_cpp.vcxproj build is independent of CMake. Visual
  Studio's C++ IDE project system expects source/header files to be listed as
  individual MSBuild items; wildcard project items can build, but do not reliably
  appear in Solution Explorer or bind to .vcxproj.filters entries.

  This script scans src/ and include/, rewrites the generated item block inside
  vs/learn_cpp.vcxproj, and emits vs/learn_cpp.vcxproj.filters. The generated
  filters mirror CMake's source_group(TREE ...) layout:

      source_group(TREE "${CMAKE_SOURCE_DIR}/src"     PREFIX "Topics"  FILES ...)
      source_group(TREE "${CMAKE_SOURCE_DIR}/include"  PREFIX "Headers" FILES ...)

  i.e. each src/<stage>/<section>/<item>.cpp lives under Topics\<stage>\<section>,
  src/<dir>/<file>.cpp under Topics\<dir>, src/<file>.cpp under Topics, and every
  include/ header under Headers\<...>.

  The .filters entries use the exact same Include strings as the generated
  vcxproj items (for example, ..\src\...\file.cpp). Each ClCompile item also gets
  an explicit ObjectFileName under $(IntDir)src\... because explicit MSBuild
  items do not populate %(RecursiveDir), and this repo has many duplicate source
  basenames.

  Re-run this script after adding or removing source/header files so both the
  build item list and the VS Solution Explorer tree stay in sync.

  Filter <UniqueIdentifier>s are derived deterministically from each filter name
  (MD5 -> Guid) so the filters file is byte-stable across regenerations.

.PARAMETER RepoRoot
  Repository root. Defaults to the parent of this script's directory (scripts/ -> repo).

.EXAMPLE
  pwsh scripts/generate-vs-filters.ps1
#>
param(
  [string]$RepoRoot = (Split-Path -Parent $PSScriptRoot)
)

$ErrorActionPreference = 'Stop'

$srcDir      = Join-Path $RepoRoot 'src'
$incDir      = Join-Path $RepoRoot 'include'
$projectFile = Join-Path $RepoRoot 'vs\learn_cpp.vcxproj'
$filtersFile = Join-Path $RepoRoot 'vs\learn_cpp.vcxproj.filters'

$beginMarker = '  <!-- BEGIN GENERATED LEARNCPP ITEMS: run scripts/generate-vs-filters.ps1 -->'
$endMarker   = '  <!-- END GENERATED LEARNCPP ITEMS -->'

if (-not (Test-Path -LiteralPath $srcDir)) { throw "src/ not found at $srcDir" }
if (-not (Test-Path -LiteralPath $incDir)) { throw "include/ not found at $incDir" }
if (-not (Test-Path -LiteralPath $projectFile)) { throw "vcxproj not found at $projectFile" }

# Return backslash paths of files under $root. $root is expected to be the src/
# or include/ directory itself, so returned paths are relative to that directory.
function Get-RelFiles([string]$root, [string[]]$extensions) {
  Get-ChildItem -LiteralPath $root -Recurse -File |
    Where-Object { $extensions -contains $_.Extension.ToLower() } |
    ForEach-Object {
      ($_.FullName.Substring($root.Length).TrimStart('\', '/') -replace '/', '\')
    }
}

# Deterministic GUID from a name: MD5(name) -> 16 bytes -> Guid. Stable across runs.
function Get-StableGuid([string]$name) {
  $md5 = [System.Security.Cryptography.MD5]::Create()
  try {
    $bytes = $md5.ComputeHash([System.Text.Encoding]::UTF8.GetBytes($name))
  } finally {
    $md5.Dispose()
  }
  return ([System.Guid]::new($bytes)).ToString('B').ToUpper()
}

# Full filter-node chain for a directory part under a prefix.
#   Get-FilterChain 'stage\section' 'Topics' -> Topics, Topics\stage, Topics\stage\section
#   Get-FilterChain ''              'Topics' -> Topics
function Get-FilterChain([string]$dirPart, [string]$prefix) {
  $chain = @($prefix)
  if ($dirPart) {
    $acc = $prefix
    foreach ($seg in ($dirPart -split '\\')) {
      $acc = "$acc\$seg"
      $chain += $acc
    }
  }
  return $chain
}

# Parent directory of a rel path ('.' -> ''), e.g. 'learn\topic_registry.cpp' -> 'learn'.
function Get-DirPart([string]$rel) {
  $parent = Split-Path $rel -Parent
  if ($parent -eq '.') { '' } else { $parent }
}

function Get-FilterName([string]$dirPart, [string]$prefix) {
  if ($dirPart) { "$prefix\$dirPart" } else { $prefix }
}

function Get-ObjectPath([string]$rel) {
  $withoutExt = [System.IO.Path]::ChangeExtension($rel, '.obj')
  return '$(IntDir)src\' + ($withoutExt -replace '/', '\')
}

# Escape XML attribute/text special characters.
function Escape-Xml([string]$s) {
  $s -replace '&', '&amp;' -replace '<', '&lt;' -replace '>', '&gt;' `
     -replace '"', '&quot;' -replace "'", '&apos;'
}

function Write-Utf8NoBomCrlf([string]$path, [string]$content) {
  $normalized = ($content -replace "`r`n", "`n") -replace "`r", "`n"
  $normalized = $normalized -replace "`n", "`r`n"
  $normalized = $normalized.TrimEnd("`r", "`n") + "`r`n"
  [System.IO.File]::WriteAllText($path, $normalized, [System.Text.UTF8Encoding]::new($false))
}

function Build-ProjectItemBlock([string[]]$cppFiles, [string[]]$hppFiles) {
  $sb = [System.Text.StringBuilder]::new()
  [void]$sb.AppendLine($beginMarker)
  [void]$sb.AppendLine('  <!-- Auto-generated explicit items. VS C++ Solution Explorer does not reliably bind wildcard items to .vcxproj.filters. -->')
  [void]$sb.AppendLine('  <ItemGroup Label="GeneratedClCompile">')
  foreach ($f in $cppFiles) {
    [void]$sb.AppendLine('    <ClCompile Include="..\src\' + (Escape-Xml $f) + '">')
    [void]$sb.AppendLine('      <ObjectFileName>' + (Escape-Xml (Get-ObjectPath $f)) + '</ObjectFileName>')
    [void]$sb.AppendLine('    </ClCompile>')
  }
  [void]$sb.AppendLine('  </ItemGroup>')
  [void]$sb.AppendLine('  <ItemGroup Label="GeneratedClInclude">')
  foreach ($f in $hppFiles) {
    [void]$sb.AppendLine('    <ClInclude Include="..\include\' + (Escape-Xml $f) + '" />')
  }
  [void]$sb.AppendLine('  </ItemGroup>')
  [void]$sb.AppendLine($endMarker)
  return $sb.ToString()
}

function Update-ProjectItems([string]$path, [string]$block) {
  $content = [System.IO.File]::ReadAllText($path)
  $pattern = '(?s)' + [System.Text.RegularExpressions.Regex]::Escape($beginMarker) + '.*?' +
    [System.Text.RegularExpressions.Regex]::Escape($endMarker)

  if (-not [System.Text.RegularExpressions.Regex]::IsMatch($content, $pattern)) {
    throw "Generated item markers not found in $path"
  }

  $replacement = $block.TrimEnd("`r", "`n")
  $updated = [System.Text.RegularExpressions.Regex]::Replace(
    $content,
    $pattern,
    [System.Text.RegularExpressions.MatchEvaluator]{ param($m) $replacement }
  )
  Write-Utf8NoBomCrlf $path $updated
}

function Build-FiltersFile([string[]]$cppFiles, [string[]]$hppFiles) {
  $filterNodes = [System.Collections.Generic.HashSet[string]]::new()
  foreach ($f in $cppFiles) {
    foreach ($n in (Get-FilterChain (Get-DirPart $f) 'Topics'))  { [void]$filterNodes.Add($n) }
  }
  foreach ($f in $hppFiles) {
    foreach ($n in (Get-FilterChain (Get-DirPart $f) 'Headers')) { [void]$filterNodes.Add($n) }
  }

  $sb = [System.Text.StringBuilder]::new()
  [void]$sb.AppendLine('<?xml version="1.0" encoding="utf-8"?>')
  [void]$sb.AppendLine('<!-- Auto-generated by scripts/generate-vs-filters.ps1; do not edit by hand.')
  [void]$sb.AppendLine('     Mirrors CMake source_group(TREE src PREFIX "Topics") and')
  [void]$sb.AppendLine('     source_group(TREE include PREFIX "Headers"). Consumed by the VS IDE for')
  [void]$sb.AppendLine('     Solution Explorer folders; MSBuild CLI ignores it. Re-run after add/remove.')
  [void]$sb.AppendLine('     File Include paths exactly match the explicit items generated in learn_cpp.vcxproj.')
  [void]$sb.AppendLine('     Build is unaffected by filters; they are VS-only metadata. -->')
  [void]$sb.AppendLine('<Project ToolsVersion="4.0" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">')

  [void]$sb.AppendLine('  <ItemGroup>')
  foreach ($n in ($filterNodes | Sort-Object)) {
    [void]$sb.AppendLine("    <Filter Include=`"$(Escape-Xml $n)`">")
    [void]$sb.AppendLine("      <UniqueIdentifier>$(Get-StableGuid $n)</UniqueIdentifier>")
    [void]$sb.AppendLine('    </Filter>')
  }
  [void]$sb.AppendLine('  </ItemGroup>')

  [void]$sb.AppendLine('  <ItemGroup>')
  foreach ($f in $cppFiles) {
    $filter = Get-FilterName (Get-DirPart $f) 'Topics'
    [void]$sb.AppendLine('    <ClCompile Include="..\src\' + (Escape-Xml $f) + '">')
    [void]$sb.AppendLine("      <Filter>$(Escape-Xml $filter)</Filter>")
    [void]$sb.AppendLine('    </ClCompile>')
  }
  [void]$sb.AppendLine('  </ItemGroup>')

  [void]$sb.AppendLine('  <ItemGroup>')
  foreach ($f in $hppFiles) {
    $filter = Get-FilterName (Get-DirPart $f) 'Headers'
    [void]$sb.AppendLine('    <ClInclude Include="..\include\' + (Escape-Xml $f) + '">')
    [void]$sb.AppendLine("      <Filter>$(Escape-Xml $filter)</Filter>")
    [void]$sb.AppendLine('    </ClInclude>')
  }
  [void]$sb.AppendLine('  </ItemGroup>')

  [void]$sb.AppendLine('</Project>')

  return @{
    Content = $sb.ToString()
    Count = $filterNodes.Count
  }
}

$cppFiles = @(Get-RelFiles $srcDir @('.cpp')       | Sort-Object)
$hppFiles = @(Get-RelFiles $incDir @('.hpp', '.h') | Sort-Object)

$projectBlock = Build-ProjectItemBlock $cppFiles $hppFiles
Update-ProjectItems $projectFile $projectBlock

$filters = Build-FiltersFile $cppFiles $hppFiles
Write-Utf8NoBomCrlf $filtersFile $filters.Content

Write-Host "Updated $projectFile"
Write-Host "Wrote $filtersFile"
Write-Host ("  Filter nodes: {0}   ClCompile: {1}   ClInclude: {2}" -f $filters.Count, $cppFiles.Count, $hppFiles.Count)
