lib_LTLIBRARIES = libglplot.la
libglplotdir=$(includedir)/glplot
libglplot_la_SOURCES = misc.cc core.cc plot2d.cc imagesc.cc font.cc core2d.cc curve.cc
libglplot_la_CPPFLAGS =  -I@top_srcdir@/include ${GL_CFLAGS} ${FREETYPE_CFLAGS}
libglplot_la_LIBS = ${GL_LIBS} ${FREETYPE_LIBS} ${GLU_LIBS}
SUBDIRS = qt
