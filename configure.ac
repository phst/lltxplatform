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

AC_ARG_VAR([RST2HTML], [command for converting reStructuredText to HTML])
AC_CHECK_PROGS([RST2HTML], [rst2html.py rst2html], [NOTFOUND])
AM_CONDITIONAL([HAVE_RST2HTML], [test NOTFOUND != "$RST2HTML"])

if test NOTFOUND = "$RST2HTML"
then
  AC_MSG_NOTICE([rst2html not found; HTML version of manual will not be built])
fi

[
case "$host_os" in
  mingw*|cygwin*) os_type=windows ;;
  darwin1[01].*)  os_type=osx106  ;;
  darwin9.*)      os_type=osx105  ;;
  linux*)         os_type=linux   ;;
  *)              os_type=unknown ;;
esac
]

case "$os_type" in
  windows)
    AC_CHECK_LIB([lua51], [lua_newstate], [], [AC_MSG_ERROR([lua library not found])])
    ;;
  linux)
    AC_CHECK_HEADER([fontconfig/fontconfig.h], [], [AC_MSG_ERROR([fontconfig header not found])])
    AC_CHECK_LIB([fontconfig], [FcFontList], [], [AC_MSG_ERROR([fontconfig library not found])])
    ;;
esac

AM_CONDITIONAL(PLAT_WINDOWS, [test windows = "$os_type"])
AM_CONDITIONAL(PLAT_OSX_10_6, [test osx106 = "$os_type"])
AM_CONDITIONAL(PLAT_OSX_10_5, [test osx105 = "$os_type"])
AM_CONDITIONAL(PLAT_LINUX, [test linux = "$os_type"])

AC_OUTPUT
