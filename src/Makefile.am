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
            -Werror \
            -Wstack-protector

lib_LTLIBRARIES = lltxplatform.la
lltxplatform_la_SOURCES = main.c malloc.c fonts.h fonts.c fonts_impl.h
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
AM_CFLAGS += -fno-common \
             -Wglobal-constructors -Wshorten-64-to-32
lltxplatform_la_SOURCES += fonts_coretext.c
lltxplatform_la_LDFLAGS += -framework CoreFoundation -framework ApplicationServices \
                           -bundle -undefined dynamic_lookup
else
if PLAT_OSX_10_5
AM_CPPFLAGS = -DCF_EXCLUDE_CSTD_HEADERS
AM_CFLAGS += -fno-common -arch i386 -arch ppc -force_cpusubtype_ALL \
             -Wglobal-constructors -Wshorten-64-to-32
lltxplatform_la_SOURCES += fonts_ats.c
lltxplatform_la_LDFLAGS += -framework CoreFoundation -framework ApplicationServices \
                           -bundle -undefined dynamic_lookup
else
AM_CPPFLAGS = -D_XOPEN_SOURCE=500
lltxplatform_la_SOURCES += fonts_fontconfig.c
endif
endif
endif

if HAVE_CROSS_X86
AM_CFLAGS += -$(CROSS_X86)
lltxplatform_la_LDFLAGS += -$(CROSS_X86()
endif
