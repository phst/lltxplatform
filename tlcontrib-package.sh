#!/bin/bash
#
# Copyright (c) 2011, Philipp Stephani <st_philipp@yahoo.de>
#
# Permission is hereby granted, free of charge, to any person obtaining
# a copy of this software and associated documentation files (the
# "Software"), to deal in the Software without restriction, including
# without limitation the rights to use, copy, modify, merge, publish,
# distribute, sublicense, and/or sell copies of the Software, and to
# permit persons to whom the Software is furnished to do so, subject to
# the following conditions:
#
# The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
# IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
# CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
# TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
# SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

set -e
shopt -u nullglob
shopt -s failglob

package=lualatex-platform
library=lltxplatform
version=0
root=texmf-dist
rundir="$root/tex/lualatex/$package"
docdir="$root/doc/lualatex/$package"
srcdir="$root/source/lualatex/$package"
arch="$(bash texlive-arch.sh)"
stage="stage/$arch"

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
    if [[ "$arch" == win32 ]]
    then
        local libext=dll
        local suffix="-$version"
    else
        local libext=so
        local suffix=''
    fi
    local libfile="$library.$libext"
    local libsource="$library$suffix.$libext"
    install -v -d "$libdir"
    install -v -c -m 755 "stage/$arch/lib/$libsource" "$libdir/$libfile"
    zip -v -r "tlcontrib-$arch.zip" "$libdir/$libfile"
}

base_pkg

while read -r arch
do
    binary_pkg "$arch"
done < built-archs.lst
