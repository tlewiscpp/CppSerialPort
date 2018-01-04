
#!/bin/bash

##########################################
# uninstall.sh
#
# Intended to be a placeholder until 
# I learn how to add a sudo make 
# install/uninstall to CMake
#
##########################################

baseName="CppSerialPort"
programLongName="CppSerialPort"
programName="CppSerialPort"

absolutePath="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)/$(basename "${BASH_SOURCE[0]}")"
fileName="${absolutePath##*/}"
filePath="$(dirname "$absolutePath")/"
iconPath="$resourceDir/$iconName"
globalBinDir="/usr/bin/"
globalLibDir="/usr/lib/"
globalIncludeDir="/usr/include/"

function bailout() {
    rm -rf "$buildDir"
}

function displayHelp() {
    echo "Usage: install-$programName.sh [--install/--uninstall] [build-dir]"
}

function cleanUp() {
    echo "All cleaned up"
}

function showSuccess() {
    echo "success"
}

function showFailure() {
    echo "failure"
    cleanUp
}

function removeFile() {
    echo -n "Removing \"$1\"..."
    rm -f "$1"
    if [[ "$?" -ne "0" ]]; then
        showFailure
        return 1
    else 
        showSuccess
        return 0
    fi
}
function suRemoveFile() {
    echo -n "Removing \"$1\"..."
    $SUDO rm -f "$1"
    if [[ "$?" -ne "0" ]]; then
        showFailure
        return 1
    else 
        showSuccess
        return 0
    fi
}

function removeDirectory() {
    echo -n "Removing directory \"$1\"..."
    $SUDO rm -rf "$1"
    if [[ "$?" -ne "0" ]]; then
        showFailure
        return 1
    else
        showSuccess
        return 0
    fi
}

function suRemoveDirectory() {
    echo -n "Removing directory \"$1\"..."
    $SUDO rm -rf "$1"
    if [[ "$?" -ne "0" ]]; then
        showFailure
        return 1
    else
        showSuccess
        return 0
    fi
}
if [[ "$EUID" -eq "0" ]]; then
    SUDO=
else
    SUDO=sudo
fi

trap bailout INT QUIT TERM

suRemoveDirectory $globalLibDir/$programName || { echo "Unable to remove library directory \"$globalLibDir/$programName\", exiting"; exit 1; }
suRemoveDirectory $globalIncludeDir/$programName || { echo "Unable to remove library directory \"$globalLibDir/$programName\", exiting"; exit 1; }

installMessage="$programLongName Uninstalled Successfully!"
totalLength=${#installMessage} 

echo
for (( i=0; i<totalLength+4; i++ )); do  
   echo -n "*"
done
echo
echo "**$installMessage**"
for (( i=0; i<totalLength+4; i++ )); do  
   echo -n "*"
done
echo
echo
exit 0
