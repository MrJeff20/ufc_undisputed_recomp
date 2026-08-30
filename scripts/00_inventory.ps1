param(
    [string]$XboxRoot = "D:\Xbox360",
    [string]$PreviousRoot = "D:\Xbox360\IntentoRecompilacion"
)

$ErrorActionPreference = "Stop"

Write-Host "== UFC Undisputed 3 Recomp Inventory =="
Write-Host ""

$paths = [ordered]@{
    "UFC ISO" = Join-Path $XboxRoot "UFC Undisputed 3 (USA, Europe) (En,Fr,De,Es,It,Pt).iso"
    "NCAA ISO" = Join-Path $XboxRoot "NCAA Football 14 (USA).iso"
    "Previous recomp root" = $PreviousRoot
    "Ghidra" = Join-Path $PreviousRoot "ghidra_12.0_PUBLIC\ghidraRun.bat"
    "Xenia Canary" = Join-Path $PreviousRoot "xenia_pruebas\xenia_canary.exe"
    "XenonAnalyse" = Join-Path $PreviousRoot "XenonRecomp\build\XenonAnalyse\XenonAnalyse.exe"
    "XenonRecomp" = Join-Path $PreviousRoot "XenonRecomp\build\XenonRecomp\XenonRecomp.exe"
}

foreach ($item in $paths.GetEnumerator()) {
    $exists = Test-Path $item.Value
    $mark = if ($exists) { "OK " } else { "MISS" }
    Write-Host ("[{0}] {1}: {2}" -f $mark, $item.Key, $item.Value)
}

Write-Host ""
Write-Host "Commands in PATH:"
"cmake","git","python","py","java","7z","xextool","quickbms","xenia" | ForEach-Object {
    $cmd = Get-Command $_ -ErrorAction SilentlyContinue
    if ($cmd) {
        Write-Host ("[OK ] {0}: {1}" -f $_, $cmd.Source)
    } else {
        Write-Host ("[MISS] {0}" -f $_)
    }
}

Write-Host ""
Write-Host "Possible UFC extracted folders:"
Get-ChildItem -Path $XboxRoot -Directory -ErrorAction SilentlyContinue |
    Where-Object { $_.Name -match "ufc|undisputed" } |
    Select-Object FullName, LastWriteTime |
    Format-Table -AutoSize
