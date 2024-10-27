# Create a new build useing cmake
Set-Location ..
if(Test-Path -Path .\\build\\){
    Remove-Item .\\build\\ -Recurse -Force
    Write-Host Deleting build folder
}
mkdir build
Set-Location .\\build\\
cmake ..
Set-Location ..
Set-Location .\\scripts\\
& .\\bNr.ps1