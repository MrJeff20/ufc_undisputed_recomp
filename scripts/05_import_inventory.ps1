$ErrorActionPreference = "Stop"

$sharedHeader = Join-Path $PSScriptRoot "..\ppc_output\ppc_recomp_shared.h"
$nativeRoot = Join-Path $PSScriptRoot "..\src\native"
$pattern = 'PPC_(?:EXTERN_)?FUNC\((__imp__[^)]+)\)'

$declared = Select-String -Path $sharedHeader -Pattern $pattern |
    ForEach-Object { $_.Matches[0].Groups[1].Value } |
    Sort-Object -Unique

$nativeSources = Get-ChildItem $nativeRoot -Recurse -Filter *.cpp
$implemented = $nativeSources |
    Where-Object { $_.Name -ne "generated_stubs.cpp" } |
    Select-String -Pattern $pattern |
    ForEach-Object { $_.Matches[0].Groups[1].Value } |
    Sort-Object -Unique
$fallbacks = $nativeSources |
    Where-Object { $_.Name -eq "generated_stubs.cpp" } |
    Select-String -Pattern $pattern |
    ForEach-Object { $_.Matches[0].Groups[1].Value } |
    Sort-Object -Unique
$pending = $declared | Where-Object { $_ -notin $implemented -and $_ -notin $fallbacks }

Write-Host "Native import inventory"
Write-Host "  Declared:    $($declared.Count)"
Write-Host "  Implemented: $($implemented.Count)"
Write-Host "  Fallbacks:   $($fallbacks.Count)"
Write-Host "  Unresolved:  $($pending.Count)"

if ($pending.Count -gt 0) {
    Write-Host ""
    $pending
}