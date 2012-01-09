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
#include <QThread>
#include "window.h"
#include <iostream>

using namespace glplot;
using namespace std;

//TODO TREE : FIGURE -> APP -> DOCK -> SUBPLOT -> PLOT

/* Allocate static thread for all figure on this app */
pthread_t figure::_s_thread;
/* First figure must create first main window */
//bool figure::_b_thread_created = false;
//void * figure::_pv_thread = NULL;
void * figure::_pv_app = NULL;

class figure_event: public QEvent {

public:
	core * _pc_core;
public:
	static const QEvent::Type myType =
			static_cast<QEvent::Type> (NEW_FIGURE_CUSTOM_EVENT);
	figure_event(core * in_pc_core);
};

figure_event::figure_event(core * in_pc_core) :
	QEvent(figure_event::myType) {
	_pc_core = in_pc_core;
}

class figure_app: public QApplication {
public:
	figure_app(int & argc, char ** argv);
	void f_new_window(glplot::core * in_pc_core);
	void customEvent(QEvent* e);
};

figure_app::figure_app(int & argc, char ** argv) :
	QApplication(argc, argv) {

}
void f_figure_update_cb(void* in_pv_arg) {
	glplot::window * pc_win = reinterpret_cast<glplot::window*> (in_pv_arg);
	//cout << "request update GL" << endl;
	QApplication::postEvent(reinterpret_cast<QObject*> (pc_win), new QEvent(
			(QEvent::Type) UPDATEGL_CUSTOM_EVENT));
}

void figure_app::f_new_window(glplot::core * in_pc_core) {
	glplot::window *_pc_win = new glplot::window(in_pc_core);

	in_pc_core->f_redraw_set_cb(f_figure_update_cb, (void*) _pc_win);

	_pc_win->show();
}

void figure_app::customEvent(QEvent* e) {
	if (e->type() == (QEvent::Type) NEW_FIGURE_CUSTOM_EVENT) {
		glplot::core * _pc_core = dynamic_cast<figure_event*> (e)->_pc_core;
		f_new_window(_pc_core);
	}

}
;

void * f_figure_routine(void * in_pv_arg) {
	figure * pc_figure = reinterpret_cast<figure*> (in_pv_arg);

	return pc_figure->f_main_loop();
}

figure::figure(core & in_c_core) {
	_pc_core = &in_c_core;

	if (_pv_app == NULL) {
		/* Creating thread */
		pthread_create(&_s_thread, NULL, f_figure_routine, this);
		/* Waiting app creation */
		while (!_pv_app)
			;
	} else {

		reinterpret_cast<QApplication*> (_pv_app)->postEvent(
				reinterpret_cast<QObject*> (_pv_app), new figure_event(
						&in_c_core));
	}

	cout << "sending event !" << endl;

	//reinterpret_cast<QApplication*>(_pv_app)->postEvent(reinterpret_cast<QObject*>(_pv_app),new figure_event(&in_c_core));
}
int figure::f_wait(void) {
	void * pc_ec;
	pthread_join(_s_thread, &pc_ec);
	return (int) pc_ec;
}

void *
figure::f_main_loop(void) {

	QApplication * _pc_app;
	int i_argc = 1;
	char * ppc_argv[2] = { (char*) "toto", (char*) "ttut" };
	cout << "Creating app " << pthread_self() << endl;
	figure_app c_app(i_argc, ppc_argv);

#if 1
	QGLFormat glf = QGLFormat::defaultFormat();
	glf.setSampleBuffers(true);
	glf.setSamples(4);
	QGLFormat::setDefaultFormat(glf);
#endif

	c_app.f_new_window(_pc_core);
	//cout << "core: " <<_pc_core << endl;
	//glplot::window *_pc_win = new glplot::window(_pc_core);

	/* Affichage de notre premiere fenêtre de rendu OpenGL */
	//_pc_win->show();

	/* Registring application pointer */
	_pv_app = &c_app;
	/* Running QT Main LOOP */
	c_app.exec();
}
