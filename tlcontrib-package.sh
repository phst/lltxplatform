#!/bin/bash

set -e
shopt -u nullglob
shopt -s failglob

package=lualatex-platform
library=lltxplatform
root=texmf-dist
rundir="$root/tex/lualatex/$package"
docdir="$root/doc/lualatex/$package"
srcdir="$root/source/lualatex/$package"
arch="$(bash texlive-arch.sh)"
stage="stage/$arch"

if [[ "$arch" == win32 ]]
then
    libext=dll
else
    libext=so
fi

libfile="$library.$libext"

bash build.sh

base_pkg() {
    install -v -d "$rundir"
    install -v -c -m 644 "$package".{lua,sty} "$rundir"
    install -v -d "$docdir"
    install -v -c -m 644 "$package.html" "$docdir"
    install -v -d "$srcdir"
    install -v -c -m 644 README *.m4 *.ac *.am *.in "$srcdir"
    install -v -d "$srcdir/src"
    install -v -c -m 644 src/*.am src/*.in src/*.c src/*.h "$srcdir/src"
    zip -v -r tlcontrib.zip "$rundir" "$docdir" "$srcdir"
}

binary_pkg() {
    local arch="$1"
    local libdir="bin/$arch/lib/lualatex/lua/$package"
    install -v -d "$libdir"
    install -v -c -m 755 "stage/$arch/lib/$libfile" "$libdir"
    zip -v -r "tlcontrib-$arch.zip" "$libdir/$libfile"
}

base_pkg
binary_pkg "$arch"
[[ "$arch" == x86_64-darwin ]] && binary_pkg universal-darwin
