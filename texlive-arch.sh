#!/bin/bash

set -e

case "$MACHTYPE" in
    i[36]86-*-msys*)
        echo win32 ;;
    x86_64-*-darwin10.*)
        echo x86_64-darwin ;;
    *-*-darwin*)
        echo universal-darwin ;;
    x86_64-*-linux*)
        echo x86_64-linux ;;
    i[36]86-*-linux*)
        echo i386-linux ;;
    *)
        echo "Could not determine native TeX Live architecture from system type $system and machine type $machine" >&2
        exit 1
esac
