param(
    [Parameter(Mandatory = $true)]
    [string]$XexPath,

    [string]$OutputPath = "",
    [string]$XenonAnalysePath = "D:\Xbox360\IntentoRecompilacion\XenonRecomp\build\XenonAnalyse\XenonAnalyse.exe"
)

$ErrorActionPreference = "Stop"

if ([string]::IsNullOrWhiteSpace($OutputPath)) {
    $OutputPath = Join-Path $PSScriptRoot "..\config\ufc_switch_tables.toml"
}

if (-not (Test-Path $XexPath)) {
    throw "XEX not found: $XexPath"
}

if (-not (Test-Path $XenonAnalysePath)) {
    throw "XenonAnalyse not found: $XenonAnalysePath"
}

$outDir = Split-Path $OutputPath
if (-not (Test-Path $outDir)) {
    New-Item -ItemType Directory -Path $outDir | Out-Null
}

Write-Host "Analyzing XEX:"
Write-Host "  input : $XexPath"
Write-Host "  output: $OutputPath"

& $XenonAnalysePath $XexPath $OutputPath

if ($LASTEXITCODE -ne 0) {
    throw "XenonAnalyse failed with exit code $LASTEXITCODE"
}

Write-Host "Done."
