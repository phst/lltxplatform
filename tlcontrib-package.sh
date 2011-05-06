#!/bin/bash

set -e
shopt -u nullglob
shopt -s failglob

package=lualatex-platform
root=texmf-dist
rundir="$root/tex/lualatex/$package"
libdir="$root/lib/lualatex/lua/$package"
docdir="$root/doc/lualatex/$package"
srcdir="$root/source/lualatex/$package"
arch="$(bash texlive-arch.sh)"

if [[ "$arch" == win32 ]]
then
    libext=dll
else
    libext=so
fi

libfile="lltxplatform.$libext"

make

install -v -d "$rundir"
install -v -c -m 644 "$package".{lua,sty} "$rundir"

install -v -d "$libdir"
install -v -c -m 755 "$libfile" "$libdir"

install -v -d "$docdir"
install -v -c -m 644 "$package.html" "$docdir"

install -v -d "$srcdir"
install -v -c -m 644 README *.m4 *.ac *.am *.in "$srcdir"

install -v -d "$srcdir/src"
install -v -c -m 644 src/*.am src/*.in src/*.c src/*.h "$srcdir/src"

zip -v -r tlcontrib.zip "$rundir" "$docdir" "$srcdir"
zip -v -r "tlcontrib-$arch.zip" "$libdir"
