[CmdletBinding()]
param (
    [string] $archiveName, [string] $targetName
)
# External environment variables include:
# archiveName: ${{ matrix.qt_ver }}-${{ matrix.qt_arch }}
# winSdkDir: ${{ steps.build.outputs.winSdkDir }}
# winSdkVer: ${{ steps.build.outputs.winSdkVer }}
# vcToolsInstallDir: ${{ steps.build.outputs.vcToolsInstallDir }}
# vcToolsRedistDir: ${{ steps.build.outputs.vcToolsRedistDir }}
# msvcArch: ${{ matrix.msvc_arch }}


# winSdkDir: C:\Program Files (x86)\Windows Kits\10\ 
# winSdkVer: 10.0.19041.0\ 
# vcToolsInstallDir: C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\VC\Tools\MSVC\14.28.29333\ 
# vcToolsRedistDir: C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\VC\Redist\MSVC\14.28.29325\ 
# archiveName: 5.9.9-win32_msvc2015
# msvcArch: x86

$scriptDir = $PSScriptRoot
$currentDir = Get-Location
Write-Host "currentDir" $currentDir
Write-Host "scriptDir" $scriptDir

function Main() {

    New-Item -ItemType Directory $archiveName
    # Copy exe
    Copy-Item bin\release\$targetName $archiveName\
    
    # Copying additional files: primarily for network access.
    Copy-Item ExtBin\*.dll $archiveName\
    Copy-Item ExtBin\*.exe $archiveName\
    Copy-Item ExtBin\OCAT.bat $archiveName\OCAT.bat
    # libcrypto-1_1-x64.dll
    # cp ExtBin/libssl-1_1-x64.dll libssl-1_1-x64.dll
    # cp ExtBin/msvcr100.dll msvcr100.dll  win7 64-bit use
    
    #$Database=Database -f
    Copy-Item Database $archiveName\Database -recurse
    
    #$devDatabase=devDatabase -f
    #Copy-Item devDatabase $archiveName\devDatabase -recurse
    
    # Copy dependency
    windeployqt --qmldir . --plugindir $archiveName\plugins --no-translations --compiler-runtime $archiveName\$targetName

    # Newly added row - Remove here vc_redist.x64.exe
    Remove-Item -Path (Join-Path $archiveName "vc_redist.x64.exe") -ErrorAction SilentlyContinue

    # Delete unnecessary files
    $excludeList = @("*.qmlc", "*.ilk", "*.exp", "*.lib", "*.pdb")
    Remove-Item -Path $archiveName -Include $excludeList -Recurse -Force
    # Copy vcRedist dll
    $redistDll="{0}{1}\*.CRT\*.dll" -f $env:vcToolsRedistDir.Trim(),$env:msvcArch
    Copy-Item $redistDll $archiveName\
    # Copy WinSDK dll
    $sdkDll="{0}Redist\{1}ucrt\DLLs\{2}\*.dll" -f $env:winSdkDir.Trim(),$env:winSdkVer.Trim(),$env:msvcArch
    Copy-Item $sdkDll $archiveName\
    # Package into zip
    Compress-Archive -Path $archiveName $archiveName'.zip'
}

if ($null -eq $archiveName || $null -eq $targetName) {
    Write-Host "args missing, archiveName is" $archiveName ", targetName is" $targetName
    return
}
Main


