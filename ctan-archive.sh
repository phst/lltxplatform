#!/bin/bash

set -e
shopt -u nullglob
shopt -s failglob

bash build.sh

zip -v ctan.zip README *.m4 *.ac *.am *.in src/*.am src/*.in src/*.c src/*.h lualatex-platform.{lua,sty,html}
