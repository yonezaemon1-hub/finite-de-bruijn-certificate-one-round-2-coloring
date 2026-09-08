$ErrorActionPreference = "Stop"

function Assert-LastExitCode([string]$Step) {
    if ($LASTEXITCODE -ne 0) { throw "$Step failed with exit code $LASTEXITCODE" }
}

$cxx = if ($env:CXX) { $env:CXX } else { "g++" }

& $cxx -O2 -std=c++17 reproduce_n497_search.cpp -o reproduce_n497_search.exe
Assert-LastExitCode "compile reproduce_n497_search.cpp"
& $cxx -O2 -std=c++17 check_n497_embedded_certificate.cpp -o check_n497.exe
Assert-LastExitCode "compile check_n497_embedded_certificate.cpp"
& $cxx -O2 -std=c++17 check_n281_base.cpp -o check_n281.exe
Assert-LastExitCode "compile check_n281_base.cpp"

$searchOut = & .\reproduce_n497_search.exe n497_reproduced_certificate.json
Assert-LastExitCode "n497 search reproduction"
$searchOut | ForEach-Object { Write-Host $_ }
$searchOut | Set-Content -Encoding ASCII n497_search_rerun_log.txt
if ((Get-FileHash n497_reproduced_certificate.json -Algorithm SHA256).Hash -ne (Get-FileHash n497_certificate.json -Algorithm SHA256).Hash) {
    throw "Generated n497 certificate does not match canonical certificate"
}
Write-Host "certificate_byte_match=PASS"

$n497Out = & .\check_n497.exe
Assert-LastExitCode "n497 exact checker"
$n497Out | ForEach-Object { Write-Host $_ }
$n497Out | Set-Content -Encoding ASCII n497_checker_rerun_log.txt
$expected497 = Get-Content n497_checker_log.txt
if (Compare-Object $expected497 $n497Out) { throw "n497 checker rerun log mismatch" }
Write-Host "n497_checker_log_match=PASS"

$n281Out = & .\check_n281.exe
Assert-LastExitCode "n281 exact checker"
$n281Out | ForEach-Object { Write-Host $_ }
$n281Out | Set-Content -Encoding ASCII n281_checker_rerun_log.txt
$expected281 = Get-Content n281_checker_log.txt
if (Compare-Object $expected281 $n281Out) { throw "n281 checker rerun log mismatch" }
Write-Host "n281_checker_log_match=PASS"
Write-Host "FULL_REPRODUCTION=PASS"
