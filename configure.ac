#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.63])
AC_INIT([glplot], [0.1])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_PROG_INSTALL

# Checks for programs.
AC_PROG_CXX
AM_PROG_LEX
AC_PROG_YACC
AC_PROG_LIBTOOL

# Checks for libraries.
PKG_CHECK_MODULES([GL], [gl >= 7.7.0]) 
PKG_CHECK_MODULES([GLU], [glu >= 7.7.0])
PKG_CHECK_MODULES([ILUT], [ILUT >= 1.7.0]) 
PKG_CHECK_MODULES([ILU], [ILU >= 1.7.0]) 
PKG_CHECK_MODULES([IL], [IL >= 1.7.0]) 
PKG_CHECK_MODULES([QTGUI], [QtGui >= 4.7.0]) 
PKG_CHECK_MODULES([QTOPENGL], [QtOpenGL >= 4.7.0]) 
AC_CHECK_LIB(glut, glutMainLoop)

# Scalable fonts in files:
# 	FreeType
#	Bitstream Speedo rasterizer
#
AC_ARG_ENABLE(freetype, [ --disable-freetype],[GLPLOT_FREETYPE=$enableval],[GLPLOT_FREETYPE=yes])
AM_CONDITIONAL(GLPLOT_FREETYPE, [test x$GLPLOT_FREETYPE = xyes])
if test x$GLPLOT_FREETYPE = xyes; then
	AC_DEFINE(GLPLOT_FREETYPE,1,[Support FreeType rasterizer for nearly all font file formats])
	AC_ARG_WITH(freetype-config, [  --with-freetype-config=PROG   Use FreeType configuration program PROG], freetype_config=$withval, freetype_config=auto)
	
	if test "$freetype_config" = "auto" ; then
		PKG_CHECK_MODULES(FREETYPE, freetype2, 
			freetype_config=no, freetype_config=yes)
	fi

	if test "$freetype_config" = "yes"; then 
		AC_PATH_PROG(ft_config,freetype-config,no)
		if test "$ft_config" = "no"; then
			AC_MSG_ERROR([You must have freetype installed; see http://www.freetype.org/])
		fi
	else
		ft_config="$freetype_config"
	fi

	if test "$freetype_config" != "no"; then 	
		FREETYPE_CFLAGS="`$ft_config --cflags`"
		FREETYPE_LIBS="`$ft_config --libs`"
	fi
	FREETYPE_REQUIRES="freetype2"
else
	FREETYPE_CFLAGS=""
	FREETYPE_LIBS=""
	FREETYPE_REQUIRES=""
fi
AC_SUBST(FREETYPE_LIBS)
AC_SUBST(FREETYPE_CFLAGS)
AC_SUBST(FREETYPE_REQUIRES)

# Checks for header files.
AC_CHECK_HEADERS([stdint.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_TYPE_UINT32_T

# Checks for library functions.
CFLAGS="-Werror -std=gnu99"
CXXFLAGS="-Werror -std=gnu++0x"

AC_MSG_CHECKING(whether to enable debugging)
debug_default="yes"

AC_ARG_ENABLE(debug, [  --enable-debug=[no/yes] turn on debugging
                       [default=$debug_default]],, enable_debug=$debug_default)
if test "x$enable_debug" = "xyes"; then
         CFLAGS="$CFLAGS -g -DOSMGL_DEBUG"
	 CXXFLAGS="$CXXFLAGS -g -DOSMGL_DEBUG"
AC_MSG_RESULT(yes)
else
AC_MSG_RESULT(no)
fi




AC_CONFIG_FILES([Makefile src/Makefile src/qt/Makefile test/Makefile include/Makefile pkgconfig/libglplot.pc])
AC_OUTPUT
