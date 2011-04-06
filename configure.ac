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
AC_CHECK_HEADERS([stddef.h stdlib.h limits.h string.h])
AC_FUNC_MALLOC
AC_CHECK_FUNCS([strdup])
AC_CHECK_HEADER([lua.h])
case "$host_os" in
     mingw32*|cygwin*) os_type=windows ;;
     darwin*)          os_type=osx     ;;
     linux*)           os_type=linux   ;;
     *)                os_type=unknown ;;
esac
if test windows = "$os_type"
then
AC_CHECK_LIB([lua51], [lua_newstate], [], [AC_MSG_ERROR([lua library not found])])
fi
AM_CONDITIONAL(PLAT_WINDOWS, [test windows = "$os_type"])
AM_CONDITIONAL(PLAT_OSX, [test osx = "$os_type"])
AM_CONDITIONAL(PLAT_LINUX, [test linux = "$os_type"])
AC_OUTPUT
