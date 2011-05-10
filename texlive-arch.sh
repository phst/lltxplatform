#!/bin/bash

set -e

system="$(uname -s)"
machine="$(uname -m)"

case "$system" in
    MINGW*)
        arch=win32 ;;
    Darwin)
        [[ "$(sysctl -n hw.optional.x86_64)" == 1 ]] && arch=x86_64-darwin || arch=universal-darwin ;;
    Linux)
        case "$machine" in
            x86_64|i386) arch="$machine-linux" ;;
        esac
        ;;
esac

if [[ -n "$arch" ]]
then
    echo "$arch"
else
    exit 1
fi
