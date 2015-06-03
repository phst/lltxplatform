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

AM_CPPFLAGS = -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=2

AM_CFLAGS = -std=c89 \
            -pedantic \
            -Wformat=2 -Winit-self -Werror-implicit-function-declaration -Wmissing-include-dirs \
            -Wswitch-default -Wswitch-enum -Wunused-parameter -Wunused -Wuninitialized -Wstrict-aliasing=2 -Wstrict-overflow=5 \
            -Wall -Wextra \
            -Wfloat-equal -Wdeclaration-after-statement -Wundef -Wshadow -Wunsafe-loop-optimizations -Wpointer-arith \
            -Wbad-function-cast -Wc++-compat -Wcast-qual -Wcast-align -Wwrite-strings -Wconversion \
            -Waggregate-return -Wstrict-prototypes -Wold-style-definition -Wmissing-prototypes -Wmissing-declarations \
            -Wmissing-noreturn -Wmissing-format-attribute -Wpacked -Wpadded -Wredundant-decls -Wnested-externs -Wunreachable-code \
            -Winline -Wvolatile-register-var -Wdisabled-optimization \
            -Wstack-protector -fstack-protector-strong $(LUA52_CFLAGS)

lib_LTLIBRARIES = lltxplatform.la
lltxplatform_la_SOURCES = main.c malloc.c fonts.h fonts.c fonts_impl.h
lltxplatform_la_LDFLAGS = -module -Wl,-z,relro -Wl,-z,now -fPIE -pie $(LUA52_LIBS)

if PLAT_WINDOWS
AM_CPPFLAGS += -DNOMINMAX -DSTRICT -DUNICODE -D_UNICODE -DWIN32_LEAN_AND_MEAN \
              -DNTDDI_VERSION=0x05010000 -D_WIN32_WINNT=0x0501 -DWINVER=0x0501 -D_WIN32_IE=0x0600
lltxplatform_la_SOURCES += fonts_windows.c
lltxplatform_la_LDFLAGS += -ladvapi32 -lshell32 -lshlwapi \
                           -no-undefined -export-symbols-regex luaopen_lltxplatform
else
if PLAT_OSX_10_6
AM_CPPFLAGS += -DCF_EXCLUDE_CSTD_HEADERS
AM_CFLAGS += -fno-common \
             -Wglobal-constructors -Wshorten-64-to-32
lltxplatform_la_SOURCES += fonts_coretext.c
lltxplatform_la_LDFLAGS += -framework CoreFoundation -framework ApplicationServices \
                           -bundle -undefined dynamic_lookup
else
if PLAT_OSX_10_5
AM_CPPFLAGS += -DCF_EXCLUDE_CSTD_HEADERS
AM_CFLAGS += -fno-common -arch i386 -arch ppc -force_cpusubtype_ALL \
             -Wglobal-constructors -Wshorten-64-to-32
lltxplatform_la_SOURCES += fonts_ats.c
lltxplatform_la_LDFLAGS += -framework CoreFoundation -framework ApplicationServices \
                           -bundle -undefined dynamic_lookup
else
AM_CPPFLAGS += -D_XOPEN_SOURCE=500
AM_CFLAGS +=  $(FONTCONFIG_CFLAGS)
lltxplatform_la_SOURCES += fonts_fontconfig.c
lltxplatform_la_LDFLAGS += $(FONTCONFIG_LIBS)
endif
endif
endif

if HAVE_CROSS_X86
AM_CFLAGS += -$(CROSS_X86)
lltxplatform_la_LDFLAGS += -$(CROSS_X86()
endif
