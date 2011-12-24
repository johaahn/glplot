/*
 * figure.cpp
 *
 *  Created on: 22 déc. 2011
 *
 * Copyright (C) 2010 <Johann Baudy> johann.baudy@gnu-log.net
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3.0 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 */

#include <glplot/figure.hh>
#include <QApplication>
#include "window.h"

using namespace glplot;

void * f_figure_routine (void * in_pv_arg) {
	figure * pc_figure = reinterpret_cast<figure*>(in_pv_arg);

	return pc_figure->f_main_loop();
}

figure::figure(core & in_c_core) {
	_pc_core = &in_c_core;

	pthread_create(&_s_thread, NULL, f_figure_routine, this);
}
int
figure::f_wait(void) {
	void * pc_ec;
	pthread_join(_s_thread, &pc_ec);
	return (int)pc_ec;
}

void *
figure::f_main_loop(void) {
	int i_argc = 1;

	char * ppc_argv[2] = { (char*)"toto", (char*)"ttut" };

	QApplication app(i_argc, ppc_argv);
	#if 1
		QGLFormat glf = QGLFormat::defaultFormat();
		glf.setSampleBuffers(true);
		glf.setSamples(4);
		QGLFormat::setDefaultFormat(glf);
	#endif

	glplot::window myWin(_pc_core); //Ajout de notre classe myWindow

	myWin.show(); //Exécution de notre fenêtre de rendu OpenGL
	return (void*) app.exec();
}
