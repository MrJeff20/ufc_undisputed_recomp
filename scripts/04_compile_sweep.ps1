param(
    [string]$Root = (Resolve-Path (Join-Path $PSScriptRoot '..')).Path,
    [string]$Compiler = 'C:\Program Files\Microsoft Visual Studio\18\Community\VC\Tools\Llvm\x64\bin\clang++.exe',
    [string]$OutDir = (Join-Path $Root 'logs\compile_sweep'),
    [int]$StartIndex = 0,
    [int]$StopIndex = [int]::MaxValue,
    [int]$Limit = 0,
    [switch]$IncludeMapping,
    [switch]$EmitObjects
)

$ErrorActionPreference = 'Stop'

$ppcDir = Join-Path $Root 'ppc_output'
$simdeDir = Join-Path $Root 'tools\XenonRecomp-ufc\thirdparty\simde'
$objDir = Join-Path $OutDir 'obj'

if (-not (Test-Path $Compiler)) {
    throw "Clang compiler was not found: $Compiler"
}

if (-not (Test-Path (Join-Path $ppcDir 'ppc_recomp_shared.h'))) {
    throw "Generated PPC output was not found: $ppcDir"
}

if (-not (Test-Path (Join-Path $simdeDir 'x86\avx.h'))) {
    throw "SIMDe headers were not found: $simdeDir"
}

New-Item -ItemType Directory -Force -Path $OutDir | Out-Null
if ($EmitObjects) {
    New-Item -ItemType Directory -Force -Path $objDir | Out-Null
}

$generatedSources = Get-ChildItem -Path $ppcDir -Filter 'ppc_recomp.*.cpp' | Sort-Object {
    if ($_.BaseName -match 'ppc_recomp\.(\d+)$') { [int]$Matches[1] } else { [int]::MaxValue }
}

$sources = New-Object System.Collections.Generic.List[System.IO.FileInfo]
if ($IncludeMapping) {
    $sources.Add((Get-Item (Join-Path $ppcDir 'ppc_func_mapping.cpp')))
}
foreach ($source in $generatedSources) {
    if ($source.BaseName -notmatch 'ppc_recomp\.(\d+)$') { continue }
    $index = [int]$Matches[1]
    if ($index -lt $StartIndex -or $index -gt $StopIndex) { continue }
    $sources.Add($source)
}

if ($Limit -gt 0 -and $sources.Count -gt $Limit) {
    $sources = [System.Collections.Generic.List[System.IO.FileInfo]]($sources | Select-Object -First $Limit)
}

$commonArgs = @(
    '-std=c++20',
    '-Wno-c++11-narrowing',
    '-Wno-unused-variable',
    '-Wno-unused-but-set-variable',
    '-Wno-unused-function',
    '-fno-strict-aliasing',
    '-I', $ppcDir,
    '-I', $simdeDir
)

$summaryRows = New-Object System.Collections.Generic.List[object]
$startedAt = Get-Date
$summaryPath = Join-Path $OutDir 'summary.csv'
$reportPath = Join-Path $OutDir 'summary.txt'

Write-Host "Compile sweep started: $($startedAt.ToString('s'))"
Write-Host "Sources: $($sources.Count)"
Write-Host "Mode: $(if ($EmitObjects) { 'object compile' } else { 'syntax-only' })"
Write-Host "Logs: $OutDir"

$ordinal = 0
foreach ($source in $sources) {
    $ordinal++
    $name = $source.Name
    $stem = [System.IO.Path]::GetFileNameWithoutExtension($name)
    $logPath = Join-Path $OutDir "$stem.log"
    $sw = [System.Diagnostics.Stopwatch]::StartNew()

    if ($EmitObjects) {
        $objectPath = Join-Path $objDir "$stem.obj"
        $args = $commonArgs + @('-c', $source.FullName, '-o', $objectPath)
    } else {
        $args = $commonArgs + @('-fsyntax-only', $source.FullName)
    }

    Write-Host ("[{0}/{1}] {2}" -f $ordinal, $sources.Count, $name)
    $compilerOutput = & $Compiler @args 2>&1 | Out-String
    $exitCode = $LASTEXITCODE
    $sw.Stop()

    Set-Content -Path $logPath -Value $compilerOutput -NoNewline

    $firstError = ''
    if ($exitCode -ne 0) {
        $firstError = ($compilerOutput -split "`r?`n" | Where-Object { $_ -match 'error:' } | Select-Object -First 1)
        if ($null -eq $firstError) { $firstError = ($compilerOutput -split "`r?`n" | Where-Object { $_.Trim().Length -gt 0 } | Select-Object -First 1) }
    }

    $summaryRows.Add([pscustomobject]@{
        Source = $name
        ExitCode = $exitCode
        Seconds = [math]::Round($sw.Elapsed.TotalSeconds, 3)
        Log = $logPath
        FirstError = $firstError
    })
}

$failed = @($summaryRows | Where-Object { $_.ExitCode -ne 0 })
$passed = $summaryRows.Count - $failed.Count
$finishedAt = Get-Date

$summaryRows | Export-Csv -Path $summaryPath -NoTypeInformation

$lines = New-Object System.Collections.Generic.List[string]
$lines.Add("UFC generated C++ compile sweep")
$lines.Add("Started:  $($startedAt.ToString('s'))")
$lines.Add("Finished: $($finishedAt.ToString('s'))")
$lines.Add("Mode:     $(if ($EmitObjects) { 'object compile' } else { 'syntax-only' })")
$lines.Add("Sources:  $($summaryRows.Count)")
$lines.Add("Passed:   $passed")
$lines.Add("Failed:   $($failed.Count)")
$lines.Add("")
if ($failed.Count -gt 0) {
    $lines.Add("Failures:")
    foreach ($row in $failed) {
        $lines.Add("- $($row.Source): $($row.FirstError)")
        $lines.Add("  log: $($row.Log)")
    }
} else {
    $lines.Add("No compile failures found.")
}
Set-Content -Path $reportPath -Value $lines

Write-Host ""
Write-Host "Sweep complete: $passed passed, $($failed.Count) failed."
Write-Host "Summary: $summaryPath"
Write-Host "Report:  $reportPath"

if ($failed.Count -gt 0) { exit 1 }
exit 0