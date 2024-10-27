# Build and Run GEngine
# $batFilePath = Join-Path -Path $PSScriptRoot -ChildPath "compile.ps1"
# $batDirectory = Split-Path -Path $batFilePath
# Set-Location -Path $batDirectory
# & "..\shaders\compile.ps1"
Set-Location ..\\build\\
cmake --build .
# cd .\\Debug\\
.\\GEngine.exe
Read-Host Press enter to close...
Set-Location ..\\scripts\\