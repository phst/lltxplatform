AM_CFLAGS = -std=c99 \
            -Werror -pedantic -Wall -Wextra -Winit-self -Wformat=2 -Wunused -Wuninitialized -Wundef \
            -Wstrict-aliasing=1 -Wstrict-overflow=5 -Wfloat-equal -Wdeclaration-after-statement \
            -Wshadow -Wunsafe-loop-optimizations -Wbad-function-cast -Wc++-compat -Wcast-qual -Wcast-align \
            -Wwrite-strings -Wconversion -Waggregate-return -Wstrict-prototypes -Wold-style-definition \
            -Wmissing-prototypes -Wmissing-declarations -Wmissing-noreturn -Wmissing-format-attribute -Wpacked \
            -Wredundant-decls -Wnested-externs -Winline -Wdisabled-optimization -Wstack-protector \
            -g2 -O3

if PLAT_WINDOWS
AM_CPPFLAGS = -DNOMINMAX -DSTRICT -DUNICODE -D_UNICODE -DWIN32_LEAN_AND_MEAN \
              -DNTDDI_VERSION=0x05010000 -D_WIN32_WINNT=0x0501 -DWINVER=0x0501 -D_WIN32_IE=0x0600
else
if PLAT_OSX
AM_CPPFLAGS = -DCF_OPEN_SOURCE -DCF_EXCLUDE_CSTD_HEADERS
AM_CFLAGS += -fno-common
else
AM_CPPFLAGS = -D_XOPEN_SOURCE=500
endif
endif

pkglib_LTLIBRARIES = lltxplatform.la
lltxplatform_la_SOURCES = main.c fonts.h fonts.c fonts_impl.h
lltxplatform_la_LDFLAGS = -module

if PLAT_WINDOWS
lltxplatform_la_SOURCES += fonts_windows.c
lltxplatform_la_LIBADD = -ladvapi32 -lshell32 -lshlwapi -llua51
## lltxplatform_la_LDFLAGS += /WX /EXPORT:luaopen_lltxplatform
else
if PLAT_OSX
lltxplatform_la_SOURCES += fonts_osx.c
lltxplatform_la_LDFLAGS += -framework CoreFoundation -framework ApplicationServices \
                           -undefined dynamic_lookup
else
lltxplatform_la_SOURCES += fonts_fontconfig.c
lltxplatform_la_LIBADD = -lfontconfig
endif
endif
