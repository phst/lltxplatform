#!/bin/bash

set -e

package=lualatex-platform
libdir="lib/lualatex/lua/$package"

if [[ "$(uname -s)" == MINGW* ]]
then
    libext=dll
else
    libext=so
fi

libfile="lltxplatform.$libext"

make

install -v -d "$libdir"
install -v -c -m 755 "$libfile" "$libdir"

zip -v lualatex-platform-tlcontrib.zip "$libdir/$libfile"
