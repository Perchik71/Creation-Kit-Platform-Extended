[int]$buildverint = Get-Content -Path ".\build_version.txt"
$buildverint+1 | out-file -FilePath ".\build_version.txt" -Force

$verfile = Get-Content -Path ".\version.tmp"
$verfile = $verfile -Replace "<BUILD>", $buildverint
$verfile | out-file -FilePath ".\version.h" -Force