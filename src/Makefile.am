AM_CFLAGS = -std=c99 \
            -Werror -pedantic -Wall -Wextra -Winit-self -Wformat=2 -Wunused -Wuninitialized -Wundef \
            -Wstrict-aliasing=1 -Wstrict-overflow=5 -Wfloat-equal -Wdeclaration-after-statement \
            -Wshadow -Wunsafe-loop-optimizations -Wbad-function-cast -Wc++-compat -Wcast-qual -Wcast-align \
            -Wwrite-strings -Wconversion -Waggregate-return -Wstrict-prototypes -Wold-style-definition \
            -Wmissing-prototypes -Wmissing-declarations -Wmissing-noreturn -Wmissing-format-attribute -Wpacked \
            -Wredundant-decls -Wnested-externs -Winline -Wdisabled-optimization -Wstack-protector \
            -g2 -O3

lib_LTLIBRARIES = lltxplatform.la
lltxplatform_la_SOURCES = main.c fonts.h fonts.c fonts_impl.h
lltxplatform_la_LDFLAGS = -module

if PLAT_WINDOWS
AM_CPPFLAGS = -DNOMINMAX -DSTRICT -DUNICODE -D_UNICODE -DWIN32_LEAN_AND_MEAN \
              -DNTDDI_VERSION=0x05010000 -D_WIN32_WINNT=0x0501 -DWINVER=0x0501 -D_WIN32_IE=0x0600
lltxplatform_la_SOURCES += fonts_windows.c
lltxplatform_la_LDFLAGS += -ladvapi32 -lshell32 -lshlwapi \
                           -no-undefined -export-symbols-regex luaopen_lltxplatform
else
if PLAT_OSX_10_6
AM_CPPFLAGS = -DCF_EXCLUDE_CSTD_HEADERS
AM_CFLAGS += -fno-common
lltxplatform_la_SOURCES += fonts_coretext.c
lltxplatform_la_LDFLAGS += -framework CoreFoundation -framework ApplicationServices \
                           -bundle -undefined dynamic_lookup
else
if PLAT_OSX_10_5
AM_CPPFLAGS = -DCF_EXCLUDE_CSTD_HEADERS
AM_CFLAGS += -fno-common -arch i386 -arch ppc -force_cpusubtype_ALL
lltxplatform_la_SOURCES += fonts_ats.c
lltxplatform_la_LDFLAGS += -framework CoreFoundation -framework ApplicationServices \
                           -bundle -undefined dynamic_lookup
else
AM_CPPFLAGS = -D_XOPEN_SOURCE=500
lltxplatform_la_SOURCES += fonts_fontconfig.c
endif
endif
endif
