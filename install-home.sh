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

texmf="$(kpsewhich --var-value=TEXMFHOME)"
bundle=phst
branch="lualatex/$bundle"
pkgdir="$texmf/tex/$branch"
libdir="$texmf/lib/lua/$branch"
docdir="$texmf/doc/$branch"

if [[ "$(uname -s)" == MINGW* ]]
then
    libext=dll
else
    libext=so
fi

make

install -v -d "$pkgdir"
install -v -c -m 644 lualatex-platform.lua "$pkgdir"

install -v -d "$libdir"
install -v -c -m 755 "lltxplatform.$libext" "$libdir"

install -v -d "$docdir"
install -v -c -m 644 lualatex-platform.html "$docdir"
