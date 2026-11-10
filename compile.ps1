$bin = "C:\Users\ANGULAKSHMI SUBBURAJ\AppData\Local\Microsoft\WinGet\Packages\BrechtSanders.WinLibs.POSIX.UCRT_Microsoft.Winget.Source_8wekyb3d8bbwe\mingw64\bin"
if ($env:Path -notlike "*$bin*") { $env:Path = "$bin;$env:Path" }
$specsPath = "$PSScriptRoot\specs"
& "$bin\g++.exe" -specs="$specsPath" $args
