#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.71])
AC_ARG_WITH([x], [AS_HELP_STRING([--with-x=yes], [Enable feature X])])
AC_INIT([OpenSwitcher],[0.9.0],[gald.rus@yandex.ru])
AM_INIT_AUTOMAKE([-Wall -Werror foreign subdir-objects])
AC_CONFIG_HEADERS([include/config.h])

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_GREP
AC_PROG_LN_S
AC_PROG_RANLIB


# Checks for libraries.
PKG_CHECK_MODULES(DEPS, x11 xkbcommon icu-uc icu-io)

# Checks for header files.
AC_PATH_X
AC_INCLUDES_DEFAULT
AC_CHECK_HEADERS(fcntl.h unistd.h)

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

AC_CONFIG_FILES([scripts/header.sh], [chmod +x scripts/header.sh])
AC_CONFIG_FILES([scripts/install.sh], [chmod +x scripts/install.sh])
AC_CONFIG_FILES([scripts/installpkgs.sh], [chmod +x scripts/installpkgs.sh])
AC_CONFIG_FILES([scripts/installfiles.sh], [chmod +x scripts/installfiles.sh])
AC_CONFIG_FILES([scripts/uninstall.sh], [chmod +x scripts/uninstall.sh])
AC_CONFIG_FILES([scripts/openswitcher.sh], [chmod +x scripts/openswitcher.sh])
AC_CONFIG_FILES([scripts/input-device-info.sh], [chmod +x scripts/input-device-info.sh])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT