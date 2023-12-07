[int]$buildverint = Get-Content -Path ".\Version\build_version.txt"
$buildverint+1 | out-file -FilePath ".\Version\build_version.txt" -Force

$verfile = Get-Content -Path ".\Version\resource_version2.tmp"
$verfile = $verfile -Replace "<BUILD>", $buildverint
$verfile | out-file -FilePath ".\Version\resource_version2.h" -Force