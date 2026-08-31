$ErrorActionPreference = "Stop"

$root = Split-Path $PSScriptRoot -Parent
$header = Join-Path $root "ppc_output\ppc_recomp_shared.h"
$output = Join-Path $root "src\native\imports\generated_stubs.cpp"
$pattern = 'PPC_EXTERN_FUNC\((__imp__[^)]+)\)'

$declared = Select-String $header -Pattern $pattern |
    ForEach-Object { $_.Matches[0].Groups[1].Value } |
    Sort-Object -Unique
$implemented = Get-ChildItem (Join-Path $root "src\native\imports") -Filter *.cpp |
    Where-Object { $_.FullName -ne $output } |
    Select-String -Pattern 'PPC_FUNC\((__imp__[^)]+)\)' |
    ForEach-Object { $_.Matches[0].Groups[1].Value } |
    Sort-Object -Unique
$pending = $declared | Where-Object { $_ -notin $implemented }

$lines = [Collections.Generic.List[string]]::new()
$lines.Add('#include "ppc_context.h"')
$lines.Add('')
$lines.Add('namespace')
$lines.Add('{')
$lines.Add('constexpr uint64_t kNotImplemented = 0xC0000002;')
$lines.Add('}')
foreach ($name in $pending) {
    $lines.Add('')
    $lines.Add("PPC_FUNC($name)")
    $lines.Add('{')
    $lines.Add('    ctx.r3.u64 = kNotImplemented;')
    $lines.Add('}')
}
[IO.File]::WriteAllLines($output, $lines)
Write-Host "Generated $($pending.Count) fallback imports."