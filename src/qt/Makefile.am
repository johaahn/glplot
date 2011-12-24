lib_LTLIBRARIES = libglplotqt.la

libglplotqt_la_SOURCES = figure.cpp window.cpp glwidget.cpp window.h glwidget.h
libglplotqt_la_CXXFLAGS = -I@top_srcdir@/include  ${GL_CFLAGS} ${GLU_CFLAGS} ${QTGUI_CFLAGS} ${QTOPENGL_CFLAGS}
libglplotqt_la_LIBADD = ${GL_LIBS} ${GLU_LIBS} ${QTGUI_LIBS} ${QTOPENGL_LIBS}
#nobase_include_HEADERS = @top_srcdir@/include/glplot/*.hh
$(libglplotqt_la_OBJECTS) : \
	 window.moc.hpp \
	 glwidget.moc.hpp

%.moc.hpp : %.h
	moc -i -o "$@" "$<"

clean-moc-extra:
	rm -vf *.moc.hpp

clean-local: clean-moc-extra
