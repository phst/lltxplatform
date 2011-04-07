AC_PREREQ(2.61)
AC_INIT([lltxplatform], [0.1])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_CONFIG_HEADER([config.h])
AC_CONFIG_FILES([Makefile src/Makefile])
AC_CANONICAL_HOST
AC_PROG_CC
AC_PROG_CC_STDC
AC_LIBTOOL_WIN32_DLL
AC_PROG_LIBTOOL
AC_C_CONST
AC_TYPE_SIZE_T
AC_HEADER_STDC
AC_CHECK_HEADERS([stddef.h stdlib.h limits.h string.h], [], [AC_MSG_ERROR([required C headers not found])])
AC_FUNC_MALLOC
AC_CHECK_FUNCS([strdup], [], [AC_MSG_ERROR([strdup function not found])])
AC_CHECK_HEADERS([lua.h], [], [AC_MSG_ERROR([lua header not found])])
case "$host_os" in
     mingw32*|cygwin*) os_type=windows ;;
     darwin*)          os_type=osx     ;;
     linux*)           os_type=linux   ;;
     *)                os_type=unknown ;;
esac
case "$os_type" in
     windows)
        AC_CHECK_LIB([advapi32], [RegOpenKeyExW], [], [AC_MSG_ERROR([advapi32 library not found])])
        AC_CHECK_LIB([shell32], [SHGetFolderPathW], [], [AC_MSG_ERROR([shell32 library not found])])
        AC_CHECK_LIB([shlwapi], [PathAppendW], [], [AC_MSG_ERROR([shlwapi library not found])])
        AC_CHECK_LIB([lua51], [lua_newstate], [], [AC_MSG_ERROR([lua library not found])])
        ;;
     linux)
        AC_CHECK_HEADER([fontconfig/fontconfig.h], [], [AC_MSG_ERROR([fontconfig header not found])])
        AC_CHECK_LIB([fontconfig], [FcFontList], [], [AC_MSG_ERROR([fontconfig library not found])])
        ;;
esac
AM_CONDITIONAL(PLAT_WINDOWS, [test windows = "$os_type"])
AM_CONDITIONAL(PLAT_OSX, [test osx = "$os_type"])
AM_CONDITIONAL(PLAT_LINUX, [test linux = "$os_type"])
AC_OUTPUT
