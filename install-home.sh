#!/bin/bash

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

install -v -d "$pkgdir"
install -v -c -m 644 lualatex-platform.lua "$pkgdir"

install -v -d "$libdir"
install -v -c -m 755 "lltxplatform.$libext" "$libdir"

install -v -d "$docdir"
install -v -c -m 644 lualatex-platform.html "$docdir"
