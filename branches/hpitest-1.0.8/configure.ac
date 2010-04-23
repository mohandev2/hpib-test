#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.50])
AC_INIT([hpitest], [1.0.5])
AC_CONFIG_SRCDIR([src/HpiTest.cc])
AC_CONFIG_HEADER([config.h])
AM_INIT_AUTOMAKE

# Checks for programs.
if test -n "${CXXFLAGS}"; then
    user_set_cxxflags=yes
fi
AC_PROG_CXX
if test X$user_set_cxxflags != Xyes; then
    CXXFLAGS="-O2"
fi

AC_ARG_WITH(threads, [build with support for pthreads],
    [if test $withval == 'yes'; then 
		ACX_PTHREAD([AC_DEFINE([USE_THREADS],[1],[pthread support])
		             LIBS="$PTHREAD_LIBS $LIBS"
		             CXXFLAGS="$CXXFLAGS $PTHREAD_CXXFLAGS"
		             CXX="$PTHREAD_CXX"])
    fi],
    [ACX_PTHREAD([AC_DEFINE([USE_THREADS],[1],[pthread support]) 
	              LIBS="$PTHREAD_LIBS $LIBS"
	              CXXFLAGS="$CXXFLAGS $PTHREAD_CXXFLAGS"
	              CXX="$PTHREAD_CXX"])])

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([sys/time.h termios.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_HEADER_TIME

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE
AC_HEADER_STDC
AC_FUNC_MEMCMP
AC_FUNC_SELECT_ARGTYPES
AC_TYPE_SIGNAL
AC_FUNC_VPRINTF
AC_CHECK_FUNCS([alarm gettimeofday select strcasecmp])

AC_CONFIG_FILES([Makefile])

AC_OUTPUT
