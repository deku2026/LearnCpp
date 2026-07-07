<#
.SYNOPSIS
  Generates vs/learn_cpp.vcxproj.filters so VS2026's Solution Explorer shows the
  src/ and include/ folder hierarchy, mirroring CMake's source_group(TREE ...).

.DESCRIPTION
  The LearnCpp.slnx + vs/learn_cpp.vcxproj build is independent of CMake: the vcxproj
  compiles every TU via the wildcard <ClCompile Include="..\src\**\*.cpp" />. That
  wildcard is RELATIVE on purpose: MSBuild expands a relative wildcard to RELATIVE
  item identities (..\src\...\file.cpp), and the filters entries below use the same
  relative strings, so VS matches each filters entry to its project item and folders
  the file. An absolute $(RepoRoot) wildcard would yield absolute items that relative
  filters cannot match, leaving the folders empty.

  Visual Studio's C++ project system does not natively show wildcard items in
  Solution Explorer, so a filters file alone is not enough. The vcxproj sets
  <ReplaceWildcardsInProjectItems>true</ReplaceWildcardsInProjectItems> to tell MSBuild
  to expand the wildcards into explicit project items (matching the relative paths
  written here), and <ReadOnlyProject>true</ReadOnlyProject> so Visual Studio does not
  save those expanded items back into the vcxproj file.

  Without these two properties plus a .vcxproj.filters sidecar, VS2026 lists all 876
  translation units flat in Solution Explorer (no folder hierarchy). This script scans
  src/ and include/ and emits a .filters file whose Topics\... / Headers\... tree matches
  what CMake produces through

      source_group(TREE "${CMAKE_SOURCE_DIR}/src"     PREFIX "Topics"  FILES ...)
      source_group(TREE "${CMAKE_SOURCE_DIR}/include"  PREFIX "Headers" FILES ...)

  i.e. each src/<stage>/<section>/<item>.cpp lives under Topics\<stage>\<section>,
  src/<dir>/<file>.cpp under Topics\<dir>, src/<file>.cpp under Topics, and every
  include/ header under Headers\<...>.

  The .filters file is consumed ONLY by the VS IDE project system; MSBuild command-
  line builds (and the CI slnx-msbuild job) ignore it entirely, so adding it cannot
  change what gets compiled. Re-run this script after you add or remove source
  files to refresh the Solution Explorer tree (the build itself needs no change --
  the vcxproj wildcard already picks up new files).

  Filter <UniqueIdentifier>s are derived deterministically from each filter name
  (MD5 -> Guid) so the file is byte-stable across regenerations and produces clean
  diffs. Entries are sorted for the same reason.

.PARAMETER RepoRoot
  Repository root. Defaults to the parent of this script's directory (scripts/ -> repo).

.EXAMPLE
  pwsh scripts/generate-vs-filters.ps1
#>
param(
  [string]$RepoRoot = (Split-Path -Parent $PSScriptRoot)
)

$ErrorActionPreference = 'Stop'

$srcDir  = Join-Path $RepoRoot 'src'
$incDir  = Join-Path $RepoRoot 'include'
$outFile = Join-Path $RepoRoot 'vs\learn_cpp.vcxproj.filters'

if (-not (Test-Path -LiteralPath $srcDir)) { throw "src/ not found at $srcDir" }
if (-not (Test-Path -LiteralPath $incDir)) { throw "include/ not found at $incDir" }

# Return repo-relative (backslash) paths of files under $root whose extension is in
# $extensions. $root is expected to be the src/ or include/ directory itself, so the
# returned strings are relative to that directory.
function Get-RelFiles([string]$root, [string[]]$extensions) {
  Get-ChildItem -LiteralPath $root -Recurse -File |
    Where-Object { $extensions -contains $_.Extension.ToLower() } |
    ForEach-Object {
      ($_.FullName.Substring($root.Length).TrimStart('\', '/') -replace '/', '\')
    }
}

# Deterministic GUID from a name: MD5(name) -> 16 bytes -> Guid. Stable across runs
# so regeneration yields identical bytes and clean diffs.
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
#   Get-FilterChain ''            'Topics' -> Topics
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

# Escape XML attribute/text special characters (filenames here are plain, but be safe).
function Escape-Xml([string]$s) {
  $s -replace '&', '&amp;' -replace '<', '&lt;' -replace '>', '&gt;' `
     -replace '"', '&quot;' -replace "'", '&apos;'
}

$cppFiles = Get-RelFiles $srcDir @('.cpp')       | Sort-Object
$hppFiles = Get-RelFiles $incDir @('.hpp', '.h') | Sort-Object

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
[void]$sb.AppendLine('     Filter Include paths are relative (..\src\...) and match the vcxproj''s')
[void]$sb.AppendLine('     ..\src\**\*.cpp wildcard items exactly: MSBuild expands a relative wildcard to')
[void]$sb.AppendLine('     relative item identities, so each filters entry is the same string as the project')
[void]$sb.AppendLine('     item it folders. Build is unaffected (filters are VS-only; MSBuild CLI ignores them). -->')
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

# Write UTF-8 without BOM and CRLF line endings, matching .gitattributes
# (*.filters text eol=crlf) and the existing vs/learn_cpp.vcxproj convention. git
# stores LF in the blob (text normalization); the working copy stays CRLF. The
# .pre-commit-config.yaml mixed-line-ending (--fix=lf) hook excludes .filters, as
# it does .vcxproj, so CRLF is fine; the CI build job runs `pre-commit run --all-files`.
# [IO.File]::WriteAllText does not translate newlines, so normalize to CRLF first.
$content = ($sb.ToString() -replace "`r`n", "`n") -replace "`r", "`n"
$content = $content -replace "`n", "`r`n"
$content = $content.TrimEnd("`r", "`n") + "`r`n"
[System.IO.File]::WriteAllText($outFile, $content, [System.Text.UTF8Encoding]::new($false))

Write-Host "Wrote $outFile"
Write-Host ("  Filter nodes: {0}   ClCompile: {1}   ClInclude: {2}" -f $filterNodes.Count, $cppFiles.Count, $hppFiles.Count)
