#requires -Version 7.0

<#
.SYNOPSIS
  Performs read-only validation of the LearnCpp topic catalog.

.DESCRIPTION
  Checks catalog counts, metadata, paths, registrations, document mappings,
  unique topic ids, and the repository's no-modules rule. Strict mode also
  rejects placeholder markers, TODOs, and empty run() bodies.

.PARAMETER CppStudyRoot
  Directory containing the external Chinese CppStudy markdown files.

.PARAMETER AllowPlaceholders
  Transitional mode. Placeholder markers, TODOs, and empty run() bodies are
  permitted; every structural, mapping, import, and module check still runs.
#>
[CmdletBinding()]
param(
  [string]$CppStudyRoot = 'C:\MyFile\ArcForges\ArchitectureDesign\CppStudy',
  [switch]$AllowPlaceholders
)

Set-StrictMode -Version 3.0
$ErrorActionPreference = 'Stop'

$expectedTopicCount = 874
$expectedTranslationUnitCount = 876
$branchEStage = 'part6_branch_e_templates_compile_time_system'
$branchEAnchor = '支线 E · 模板与编译期系统(深水 TMP)'

$stageData = @'
part2_stage01_syntax_and_translation_model|第2部分-阶段1-语法基础与翻译模型.md|part2/stage01|19
part2_stage02_type_system_value_category_deduction|第2部分-阶段2-类型系统-值类别与类型推导.md|part2/stage02|53
part2_stage03_functions_overloading_lambdas|第2部分-阶段3-函数-重载-lambda.md|part2/stage03|35
part2_stage04_classes_and_object_model_basics|第2部分-阶段4-类与对象模型基础.md|part2/stage04|47
part2_stage05_copy_move_smart_pointers|第2部分-阶段5-拷贝与移动语义-智能指针.md|part2/stage05|32
part2_stage06_templates_and_generics|第2部分-阶段6-模板与泛型编程.md|part2/stage06|40
part2_stage07_compile_time_programming|第2部分-阶段7-编译期编程.md|part2/stage07|21
part2_stage08_control_flow_and_modern_syntax|第2部分-阶段8-控制流与现代语法增强.md|part2/stage08|20
part2_stage09_exceptions_error_handling_ub|第2部分-阶段9-异常-错误处理与UB纪律.md|part2/stage09|45
part2_stage10_stl_deep_dive|第2部分-阶段10-STL深潜.md|part2/stage10|132
part2_stage11_concurrency_and_threading|第2部分-阶段11-并发与多线程.md|part2/stage11|35
part2_stage12_coroutines|第2部分-阶段12-协程.md|part2/stage12|32
part2_stage14_keywords_and_features_index|第2部分-阶段14-特性与关键字全表.md|part2/stage14|18
part2_stage15_classic_idioms|第2部分-阶段15-C++经典惯用法.md|part2/stage15|22
part3_standard_library_systematized|第3部分-标准库系统化.md|part3|86
part4_engineering_system|第4部分-工程系统.md|part4|52
part6_branch_a_object_model|第6部分-支线A-对象模型.md|part6/a|16
part6_branch_b_lifetime_and_ownership|第6部分-支线B-生命周期与所有权.md|part6/b|29
part6_branch_c_memory_management|第6部分-支线C-内存管理.md|part6/c|29
part6_branch_d_name_lookup_overload_resolution|第6部分-支线D-名字查找ADL与重载决议.md|part6/d|23
part6_branch_e_templates_compile_time_system|Cpp-Modern-完整学习路线图-C++23.md|part6/e|26
part6_branch_f_memory_model_and_concurrency|第6部分-支线F-内存模型与并发.md|part6/f|21
part6_branch_g_abi_compile_link_ub_performance|第6部分-支线G-ABI编译链接UB性能.md|part6/g|41
'@

$stageMap = @{}
foreach ($line in ($stageData -split '\r?\n')) {
  if ([string]::IsNullOrWhiteSpace($line)) {
    continue
  }
  $parts = $line -split '\|', 4
  $stageMap[$parts[0]] = [pscustomobject]@{
    Doc = $parts[1]
    TopicPrefix = $parts[2]
    TopicCount = [int]$parts[3]
  }
}

$errors = [System.Collections.Generic.List[string]]::new()
$topicIds = [System.Collections.Generic.List[string]]::new()
$placeholderFiles = [System.Collections.Generic.List[string]]::new()
$todoFiles = [System.Collections.Generic.List[string]]::new()
$emptyRunFiles = [System.Collections.Generic.List[string]]::new()
$forbiddenSyntaxFiles = [System.Collections.Generic.List[string]]::new()

function Add-ValidationError([string]$Message) {
  $script:errors.Add($Message)
}

function Get-SingleMetadataValue(
  [string]$Content,
  [string]$Label,
  [string]$RelativePath
) {
  $pattern = '(?m)^//\s*' + [regex]::Escape($Label) + '\s*:\s*(.*?)\s*$'
  $matches = [regex]::Matches($Content, $pattern)
  if ($matches.Count -ne 1) {
    Add-ValidationError "$($RelativePath): expected exactly one '$Label' field; found $($matches.Count)."
    return $null
  }
  return $matches[0].Groups[1].Value.Trim()
}

function Remove-CppComments([string]$Content) {
  $withoutBlocks = [regex]::Replace($Content, '(?s)/\*.*?\*/', '')
  return [regex]::Replace($withoutBlocks, '(?m)//.*$', '')
}

function Add-AggregateFailure(
  [string]$Label,
  [System.Collections.Generic.List[string]]$Files
) {
  if ($Files.Count -eq 0) {
    return
  }

  $sample = @($Files | Select-Object -First 8) -join ', '
  if ($Files.Count -gt 8) {
    $sample += ', ...'
  }
  Add-ValidationError "$Label ($($Files.Count)): $sample"
}

$repoRoot = (Resolve-Path (Join-Path $PSScriptRoot '..')).Path
$sourceRoot = Join-Path $repoRoot 'src'
if (-not (Test-Path -LiteralPath $sourceRoot -PathType Container)) {
  Write-Error "src directory not found: $sourceRoot"
  exit 1
}

$sourceRoot = (Resolve-Path $sourceRoot).Path.TrimEnd('\', '/')
$allCpp = @(Get-ChildItem -LiteralPath $sourceRoot -Recurse -File -Filter '*.cpp' |
  Sort-Object FullName)
if ($allCpp.Count -ne $expectedTranslationUnitCount) {
  Add-ValidationError "translation units: expected $expectedTranslationUnitCount .cpp files; found $($allCpp.Count)."
}

$topicFiles = @($allCpp | Where-Object {
  $relative = $_.FullName.Substring($sourceRoot.Length).TrimStart('\', '/') -replace '\\', '/'
  $relative -ne 'main.cpp' -and $relative -ne 'learn/topic_registry.cpp'
})
if ($topicFiles.Count -ne $expectedTopicCount) {
  Add-ValidationError "topics: expected $expectedTopicCount files; found $($topicFiles.Count)."
}

$actualStageCounts = @{}
foreach ($file in $topicFiles) {
  $stageDirectory = $file.Directory.Parent.Name
  if (-not $actualStageCounts.ContainsKey($stageDirectory)) {
    $actualStageCounts[$stageDirectory] = 0
  }
  ++$actualStageCounts[$stageDirectory]
}
foreach ($entry in $stageMap.GetEnumerator()) {
  $actualCount = if ($actualStageCounts.ContainsKey($entry.Key)) {
    $actualStageCounts[$entry.Key]
  } else {
    0
  }
  if ($actualCount -ne $entry.Value.TopicCount) {
    Add-ValidationError "stage $($entry.Key): expected $($entry.Value.TopicCount) topics; found $actualCount."
  }
}
foreach ($stageDirectory in $actualStageCounts.Keys) {
  if (-not $stageMap.ContainsKey($stageDirectory)) {
    Add-ValidationError "unexpected stage directory: $stageDirectory"
  }
}

$docRootExists = Test-Path -LiteralPath $CppStudyRoot -PathType Container
if (-not $docRootExists) {
  Add-ValidationError "CppStudy root does not exist: $CppStudyRoot"
} else {
  foreach ($entry in $stageMap.GetEnumerator()) {
    $docPath = Join-Path $CppStudyRoot $entry.Value.Doc
    if (-not (Test-Path -LiteralPath $docPath -PathType Leaf)) {
      Add-ValidationError "mapped document missing for $($entry.Key): $docPath"
    }
  }
}

$includeRoot = Join-Path $repoRoot 'include'
$codeRoots = @($sourceRoot)
if (Test-Path -LiteralPath $includeRoot -PathType Container) {
  $codeRoots += (Resolve-Path $includeRoot).Path.TrimEnd('\', '/')
}

$moduleExtensions = @('.ixx', '.cppm', '.mpp', '.mxx', '.cxxm')
$moduleUnits = @(
  foreach ($codeRoot in $codeRoots) {
    Get-ChildItem -LiteralPath $codeRoot -Recurse -File |
      Where-Object { $moduleExtensions -contains $_.Extension.ToLowerInvariant() }
  }
)
if ($moduleUnits.Count -gt 0) {
  $relativeUnits = @($moduleUnits | ForEach-Object {
    [System.IO.Path]::GetRelativePath($repoRoot, $_.FullName) -replace '\\', '/'
  }) -join ', '
  Add-ValidationError "module units are forbidden ($($moduleUnits.Count)): $relativeUnits"
}

$emptyRunPattern = '(?s)\bint\s+run\s*\(\s*int\s+\w+\s*,\s*char\s*\*\*\s*\w+\s*\)\s*\{\s*(?:\(void\)\s*\w+\s*;\s*)*(?:return\s+0\s*;\s*)?\}'
$importDeclarationPattern = '(?im)^[\t ]*(?:export[\t ]+)?import(?=[\t :<"])(?:[\t ]+)?[^;\r\n]+;'
$moduleDeclarationPattern = '(?im)^[\t ]*(?:export[\t ]+)?module(?=[\t :;])(?:[\t ]+)?[^;\r\n]*;'
$languageExtensions = @('.c', '.cc', '.cpp', '.cxx', '.h', '.hh', '.hpp', '.hxx', '.inc', '.ipp', '.tpp')
$languageFiles = @(
  foreach ($codeRoot in $codeRoots) {
    Get-ChildItem -LiteralPath $codeRoot -Recurse -File |
      Where-Object { $languageExtensions -contains $_.Extension.ToLowerInvariant() }
  }
)
foreach ($file in $languageFiles) {
  $relativePath = [System.IO.Path]::GetRelativePath($repoRoot, $file.FullName) -replace '\\', '/'
  $codeWithoutComments = Remove-CppComments ([System.IO.File]::ReadAllText($file.FullName))
  if ($codeWithoutComments -match $importDeclarationPattern) {
    $forbiddenSyntaxFiles.Add("$relativePath (import declaration)")
  }
  if ($codeWithoutComments -match $moduleDeclarationPattern) {
    $forbiddenSyntaxFiles.Add("$relativePath (module declaration)")
  }
}

foreach ($file in $topicFiles) {
  $relativePath = $file.FullName.Substring($sourceRoot.Length).TrimStart('\', '/')
  $content = [System.IO.File]::ReadAllText($file.FullName)

  $doc = Get-SingleMetadataValue $content 'Doc' $relativePath
  $stage = Get-SingleMetadataValue $content 'Stage' $relativePath
  $section = Get-SingleMetadataValue $content 'Section' $relativePath
  $item = Get-SingleMetadataValue $content 'Item' $relativePath
  $topicId = Get-SingleMetadataValue $content 'Topic id' $relativePath

  if ($null -eq $doc -or $null -eq $stage -or $null -eq $section -or
      $null -eq $item -or $null -eq $topicId) {
    continue
  }

  $topicIds.Add($topicId)

  $stageDirectory = $file.Directory.Parent.Name
  $sectionDirectory = $file.Directory.Name
  if ($file.BaseName -cne $item) {
    Add-ValidationError "$($relativePath): filename '$($file.BaseName)' does not match Item '$item'."
  }
  if ($sectionDirectory -cne $section) {
    Add-ValidationError "$($relativePath): directory '$sectionDirectory' does not match Section '$section'."
  }
  if ($stageDirectory -cne $stage) {
    Add-ValidationError "$($relativePath): directory '$stageDirectory' does not match Stage '$stage'."
  }

  if (-not $stageMap.ContainsKey($stage)) {
    Add-ValidationError "$($relativePath): unknown Stage '$stage'."
  } else {
    $mapping = $stageMap[$stage]
    if ($doc -cne $mapping.Doc) {
      Add-ValidationError "$($relativePath): Doc '$doc' does not match '$($mapping.Doc)'."
    }

    $sectionMatch = [regex]::Match($section, '^section\d+')
    if (-not $sectionMatch.Success) {
      Add-ValidationError "$($relativePath): Section '$section' has no sectionNN prefix."
    } else {
      $expectedTopicId = "$($mapping.TopicPrefix)/$($sectionMatch.Value)/$item"
      if ($topicId -cne $expectedTopicId) {
        Add-ValidationError "$($relativePath): Topic id '$topicId' should be '$expectedTopicId'."
      }
    }
  }

  $registrations = [regex]::Matches($content, '::learn::topic<\s*"([^"]+)"')
  if ($registrations.Count -ne 1) {
    Add-ValidationError "$($relativePath): expected one topic registration; found $($registrations.Count)."
  } elseif ($registrations[0].Groups[1].Value -cne $topicId) {
    Add-ValidationError "$($relativePath): registered id does not match Topic id '$topicId'."
  }

  if ($stage -ceq $branchEStage) {
    $anchors = [regex]::Matches($content, '(?m)^//\s*Doc anchor\s*:\s*(.*?)\s*$')
    if ($anchors.Count -ne 1 -or
        $anchors[0].Groups[1].Value.Trim() -cne $branchEAnchor) {
      Add-ValidationError "$($relativePath): missing or incorrect branch E Doc anchor."
    }
  }

  $codeWithoutComments = Remove-CppComments $content
  if (-not $AllowPlaceholders) {
    if ($content -match '(?m)^//\s*LearnCpp placeholder\s*$') {
      $placeholderFiles.Add($relativePath)
    }
    if ($content -match '\bTODO\b') {
      $todoFiles.Add($relativePath)
    }
    if ($codeWithoutComments -match $emptyRunPattern) {
      $emptyRunFiles.Add($relativePath)
    }
  }
}

$duplicateTopicGroups = @($topicIds | Group-Object |
  Where-Object { $_.Count -gt 1 })
foreach ($group in $duplicateTopicGroups) {
  Add-ValidationError "duplicate Topic id '$($group.Name)' appears $($group.Count) times."
}

Add-AggregateFailure 'forbidden import/module syntax' $forbiddenSyntaxFiles
if (-not $AllowPlaceholders) {
  Add-AggregateFailure 'placeholder markers remain' $placeholderFiles
  Add-AggregateFailure 'TODO markers remain' $todoFiles
  Add-AggregateFailure 'empty run() bodies remain' $emptyRunFiles
}

$mode = if ($AllowPlaceholders) {
  'transition (-AllowPlaceholders)'
} else {
  'strict'
}

if ($errors.Count -gt 0) {
  Write-Host "[FAIL] LearnCpp topic catalog validation ($mode)" -ForegroundColor Red
  foreach ($validationError in $errors) {
    Write-Host "  - $validationError" -ForegroundColor Red
  }
  exit 1
}

Write-Host "[PASS] LearnCpp topic catalog validation ($mode)" -ForegroundColor Green
Write-Host "  Translation units: $($allCpp.Count)"
Write-Host "  Topics:            $($topicFiles.Count)"
Write-Host "  Stages:            $($actualStageCounts.Count)"
Write-Host "  CppStudy root:     $CppStudyRoot"
exit 0
