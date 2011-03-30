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

import sys

plat_info = {"win32": ("windows", "dll"),
             "darwin": ("osx", "so"),
             "linux": ("fontconfig", "so")}

comp_flags = {"cl": ["/O2", "/MD", "/W4", "/Wall", "/WX", "/wd4001", "/wd4255", "/wd4668", "/wd4711", "/wd4820"],
              "gcc": ["-std=c99",
                      "-Werror", "-pedantic", "-Wall", "-Wextra", "-Winit-self", "-Wformat=2", "-Wunused", "-Wuninitialized",
                      "-Wstrict-aliasing=1", "-Wstrict-overflow=5", "-Wfloat-equal", "-Wdeclaration-after-statement", "-Wundef",
                      "-Wshadow", "-Wunsafe-loop-optimizations", "-Wbad-function-cast", "-Wc++-compat", "-Wcast-qual", "-Wcast-align",
                      "-Wwrite-strings", "-Wconversion", "-Waggregate-return", "-Wstrict-prototypes", "-Wold-style-definition",
                      "-Wmissing-prototypes", "-Wmissing-declarations", "-Wmissing-noreturn", "-Wmissing-format-attribute", "-Wpacked",
                      "-Wredundant-decls", "-Wnested-externs", "-Winline", "-Wdisabled-optimization", "-Wstack-protector",
                      "-g2", "-O3"]}

vars = Variables()
vars.Add(PathVariable("luainc", "Directory where Lua headers are located", ".", PathVariable.PathIsDir))
vars.Add(PathVariable("lualib", "Directory where the Lua library is located", ".", PathVariable.PathIsDir))
vars.Add(PathVariable("fcinc", "Directory where FontConfig headers are located", ".", PathVariable.PathIsDir))
vars.Add(PathVariable("fclib", "Directory where FontConfig libraries are located", ".", PathVariable.PathIsDir))

env = Environment(variables=vars, CPPPATH=["$luainc", "$fcinc"], LIBPATH=["$lualib", "$fclib"])
env.Append(CCFLAGS=comp_flags[env["CC"]])

plat_name, plat_ext = plat_info[sys.platform]

if sys.platform == "darwin":
    env.Append(CCFLAGS=["-fno-common"], LINKFLAGS=["-undefined", "dynamic_lookup"])
    env.Append(FRAMEWORKS=["CoreFoundation", "ApplicationServices"])
elif sys.platform == "win32":
    env.Append(CPPDEFINES=["NOMINMAX", "STRICT", "UNICODE", "_UNICODE", "WIN32_LEAN_AND_MEAN",
                           ("NTDDI_VERSION", "0x06000000"), ("_WIN32_WINNT", "0x0600"), ("WINVER", "0x0600")],
               LINKFLAGS=["/WX", "/EXPORT:luaopen_lltxplatform"],
               LIBS=["advapi32", "ole32", "shell32", "shlwapi", "lua51"])

if plat_name == "fontconfig":
    env.Append(LIBS=["fontconfig"])

common_files = ["main.c", "fonts.c"]
plat_files = env.Glob("*_%s.c" % plat_name)
library_file = File("lltxplatform.%s" % plat_ext)

env.SharedLibrary(library_file, common_files + plat_files)
