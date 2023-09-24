#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.71])
AC_INIT([OpenSwitcher],[0.1.1],[gald.rus@yandex.ru])
AC_CONFIG_SRCDIR([src/main.c])  
#AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_GREP
AC_PROG_LN_S
AC_PROG_RANLIB


# Checks for libraries.
# AC_CHECK_LIBS([x11], [XOpenDisplay])
# AC_CHECK_LIBS([xkbcommon], [xkb_keysym_to_utf32])
# AC_CHECK_LIBS([icu-uc], [u_charDigitValue])
# AC_CHECK_LIBS([icu-io], [u_fopen])

# Checks for header files.
AC_PATH_X
AC_INCLUDES_DEFAULT
AC_CHECK_HEADERS(fcntl.h unistd.h)
AC_PROG_EGREP


# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_INT32_T
AC_TYPE_SIZE_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS(read strlen open write)

# AC_CONFIG_FILES([autoconf], [chmod +x autoconf])
# AC_CONFIG_FILES([Makefile src/Makefile nls/Makefile man/Makefile tests/Makefile])
AC_OUTPUT
