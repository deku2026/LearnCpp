#requires -Version 5.1

<#
.SYNOPSIS
Runs every registered LearnCpp topic in its own process.

.DESCRIPTION
The script asks a Release build of learn_cpp to list its registered topics,
validates the declared count and every topic id, and then runs the topics one
at a time. Each child process has an independent timeout, so a crashing or
hanging topic does not prevent the remaining topics from running.

Known-dangerous topic-name patterns are excluded by default. Use
-IncludeDangerous to disable the built-in exclusions, or -ExcludePattern to
add regular expressions of your own. Custom exclusions remain active when
-IncludeDangerous is present.

.PARAMETER Executable
Path to a Release or RelWithDebInfo learn_cpp executable. Its no-argument
behavior must be the registry listing, not the Debug in-process sweep.

.PARAMETER WorkingDirectory
Working directory assigned to every learn_cpp process. Defaults to the
repository root.

.PARAMETER TimeoutSeconds
Maximum runtime for the listing process and for each topic process.

.PARAMETER ExcludePattern
Additional regular expressions matched against complete topic ids.

.PARAMETER IncludeDangerous
Disables the built-in dangerous-topic exclusions. Additional patterns passed
through -ExcludePattern are still applied.

.PARAMETER ShowOutput
Prints captured standard output and standard error for successful topics too.
Failed and timed-out topics always print their captured output.
#>

[CmdletBinding()]
param(
    [string]$Executable,
    [string]$WorkingDirectory,
    [ValidateRange(1, 86400)]
    [int]$TimeoutSeconds = 15,
    [string[]]$ExcludePattern = @(),
    [switch]$IncludeDangerous,
    [switch]$ShowOutput
)

Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"
$expectedTopicCount = 874

if ([string]::IsNullOrWhiteSpace($Executable)) {
    $Executable = Join-Path $PSScriptRoot "..\build\windows-release\bin\learn_cpp.exe"
}
if ([string]::IsNullOrWhiteSpace($WorkingDirectory)) {
    $WorkingDirectory = Join-Path $PSScriptRoot ".."
}

function ConvertTo-ProcessArguments {
    param([string[]]$ArgumentList)

    foreach ($argument in $ArgumentList) {
        if ($argument -notmatch "^[A-Za-z0-9][A-Za-z0-9._/-]*$") {
            throw "Refusing to pass an unsafe process argument: '$argument'"
        }
    }

    return ($ArgumentList -join " ")
}

function Invoke-LearnCppProcess {
    param(
        [string]$FilePath,
        [string]$ProcessWorkingDirectory,
        [string[]]$ArgumentList,
        [int]$TimeoutMilliseconds
    )

    $startInfo = [System.Diagnostics.ProcessStartInfo]::new()
    $startInfo.FileName = $FilePath
    $startInfo.WorkingDirectory = $ProcessWorkingDirectory
    $startInfo.Arguments = ConvertTo-ProcessArguments -ArgumentList $ArgumentList
    $startInfo.UseShellExecute = $false
    $startInfo.RedirectStandardOutput = $true
    $startInfo.RedirectStandardError = $true
    $startInfo.CreateNoWindow = $true

    $process = [System.Diagnostics.Process]::new()
    $process.StartInfo = $startInfo
    $stopwatch = [System.Diagnostics.Stopwatch]::StartNew()
    $timedOut = $false
    $stdout = ""
    $stderr = ""
    $exitCode = $null

    try {
        if (-not $process.Start()) {
            throw "Failed to start '$FilePath'."
        }

        # Read both streams asynchronously so a verbose topic cannot deadlock
        # while the parent waits for the process to exit.
        $stdoutTask = $process.StandardOutput.ReadToEndAsync()
        $stderrTask = $process.StandardError.ReadToEndAsync()

        if (-not $process.WaitForExit($TimeoutMilliseconds)) {
            $timedOut = $true
            try {
                # Available on modern .NET and terminates any child processes.
                $process.Kill($true)
            } catch {
                try {
                    $process.Kill()
                } catch {
                    # The process may have exited between the timeout and Kill.
                }
            }
        }

        $process.WaitForExit()
        $stdout = $stdoutTask.GetAwaiter().GetResult()
        $stderr = $stderrTask.GetAwaiter().GetResult()
        if (-not $timedOut) {
            $exitCode = $process.ExitCode
        }
    } finally {
        $stopwatch.Stop()
        $process.Dispose()
    }

    return [pscustomobject]@{
        ExitCode = $exitCode
        TimedOut = $timedOut
        Elapsed = $stopwatch.Elapsed
        StdOut = $stdout
        StdErr = $stderr
    }
}

function Write-CapturedOutput {
    param([pscustomobject]$Result)

    if (-not [string]::IsNullOrWhiteSpace($Result.StdOut)) {
        Write-Host $Result.StdOut.TrimEnd()
    }
    if (-not [string]::IsNullOrWhiteSpace($Result.StdErr)) {
        Write-Host $Result.StdErr.TrimEnd()
    }
}

try {
    $resolvedExecutable = (Resolve-Path -LiteralPath $Executable -ErrorAction Stop).Path
    $resolvedWorkingDirectory = (Resolve-Path -LiteralPath $WorkingDirectory -ErrorAction Stop).Path
    if (-not (Test-Path -LiteralPath $resolvedExecutable -PathType Leaf)) {
        throw "Executable is not a file: $resolvedExecutable"
    }
    if (-not (Test-Path -LiteralPath $resolvedWorkingDirectory -PathType Container)) {
        throw "Working directory is not a directory: $resolvedWorkingDirectory"
    }

    $timeoutMilliseconds = $TimeoutSeconds * 1000
    $listResult = Invoke-LearnCppProcess -FilePath $resolvedExecutable `
        -ProcessWorkingDirectory $resolvedWorkingDirectory -ArgumentList @() `
        -TimeoutMilliseconds $timeoutMilliseconds

    if ($listResult.TimedOut) {
        throw "Timed out while asking learn_cpp to list topics. Use a Release or RelWithDebInfo executable."
    }
    if ($listResult.ExitCode -ne 0) {
        Write-CapturedOutput -Result $listResult
        throw "learn_cpp list command returned $($listResult.ExitCode)."
    }

    $lines = @($listResult.StdOut -split "\r?\n" | Where-Object { $_.Length -gt 0 })
    $headerLines = @($lines | Where-Object { $_ -match "^learn_cpp: [0-9]+ topics registered$" })
    if ($headerLines.Count -ne 1) {
        throw "Expected exactly one registry header in list output. Use a Release or RelWithDebInfo executable."
    }

    $null = $headerLines[0] -match "^learn_cpp: ([0-9]+) topics registered$"
    $declaredTopicCount = [int]$Matches[1]
    if ($declaredTopicCount -ne $expectedTopicCount) {
        throw "Registry declared $declaredTopicCount topics; expected exactly $expectedTopicCount."
    }
    $topicIds = [System.Collections.Generic.List[string]]::new()
    foreach ($line in $lines) {
        if ($line -eq $headerLines[0]) {
            continue
        }
        if ($line -notmatch "^  ([A-Za-z0-9][A-Za-z0-9._/-]*)$") {
            throw "Unexpected line in registry output: '$line'"
        }
        $topicIds.Add($Matches[1])
    }

    if ($topicIds.Count -ne $declaredTopicCount) {
        throw "Registry declared $declaredTopicCount topics but listed $($topicIds.Count)."
    }
    $uniqueTopicIds = @($topicIds | Sort-Object -Unique)
    if ($uniqueTopicIds.Count -ne $topicIds.Count) {
        throw "Registry output contains duplicate topic ids."
    }

    $defaultDangerousPatterns = @(
        "(?i)/(?:[^/]*(?:assume_unreachable|data_race|dangling|lifetime_violation|null_(?:pointer_)?dereference|out_of_bounds|signed_(?:integer_)?overflow|strict_aliasing|uninitialized_read|use_after_free)[^/]*)$",
        "(?i)/(?:[^/]*(?:deadlock|infinite_loop|abort|quick_exit|terminate)[^/]*)$"
    )
    $activeExcludePatterns = [System.Collections.Generic.List[string]]::new()
    if (-not $IncludeDangerous) {
        foreach ($pattern in $defaultDangerousPatterns) {
            $activeExcludePatterns.Add($pattern)
        }
    }
    foreach ($pattern in $ExcludePattern) {
        $activeExcludePatterns.Add($pattern)
    }
    foreach ($pattern in $activeExcludePatterns) {
        try {
            $null = [regex]::new($pattern)
        } catch {
            throw "Invalid exclusion regular expression '$pattern': $($_.Exception.Message)"
        }
    }

    Write-Host "learn_cpp: discovered $declaredTopicCount unique topics"
    Write-Host "run-all-topics: sequential execution, timeout ${TimeoutSeconds}s per topic"

    $passed = 0
    $failed = 0
    $timedOut = 0
    $excluded = 0

    foreach ($topicId in $topicIds) {
        $isExcluded = $false
        foreach ($pattern in $activeExcludePatterns) {
            if ([regex]::IsMatch($topicId, $pattern)) {
                $isExcluded = $true
                break
            }
        }

        if ($isExcluded) {
            ++$excluded
            Write-Host "[SKIP] $topicId"
            continue
        }

        $result = Invoke-LearnCppProcess -FilePath $resolvedExecutable `
            -ProcessWorkingDirectory $resolvedWorkingDirectory -ArgumentList @($topicId) `
            -TimeoutMilliseconds $timeoutMilliseconds
        $elapsedMilliseconds = [math]::Round($result.Elapsed.TotalMilliseconds)

        if ($result.TimedOut) {
            ++$timedOut
            Write-Host "[TIMEOUT] $topicId (${elapsedMilliseconds} ms)"
            Write-CapturedOutput -Result $result
        } elseif ($result.ExitCode -ne 0) {
            ++$failed
            Write-Host "[FAIL:$($result.ExitCode)] $topicId (${elapsedMilliseconds} ms)"
            Write-CapturedOutput -Result $result
        } else {
            ++$passed
            Write-Host "[PASS] $topicId (${elapsedMilliseconds} ms)"
            if ($ShowOutput) {
                Write-CapturedOutput -Result $result
            }
        }
    }

    Write-Host ""
    Write-Host "run-all-topics summary"
    Write-Host "  registered : $declaredTopicCount"
    Write-Host "  passed     : $passed"
    Write-Host "  failed     : $failed"
    Write-Host "  timed out  : $timedOut"
    Write-Host "  excluded   : $excluded"

    if ($failed -gt 0 -or $timedOut -gt 0) {
        exit 1
    }
    exit 0
} catch {
    Write-Host "run-all-topics: $($_.Exception.Message)"
    exit 2
}
