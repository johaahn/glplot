/*
 * window.cpp
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

#include "window.h"
#include "window.moc.hpp"
#include <stdio.h>
#include <iostream>
using namespace std;
using namespace glplot;


window::window(core * in_pc_core, QWidget *parent)
    : glwidget(60, parent, (char*)"Figure GLPLOT")
{
	_pc_core = in_pc_core;
}

void
window::customEvent(QEvent* e)
{
	if(e->type() == (QEvent::Type)UPDATEGL_CUSTOM_EVENT)
	{
		cout << "UPDATING GL" << endl;
		updateGL();
	}
};

void window::initializeGL()
{
	//_c_plot.f_init();
	_pc_core->f_gl_init();
}

void window::resizeGL(int width, int height)
{
	cout << "Resizing("<< width << "," << height << ")"<< endl;
	//_c_plot.f_resize(width, height);
	_pc_core->f_gl_resize(width, height);
}

void window::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    _pc_core->f_gl_render();
}

void window::mousePressEvent(QMouseEvent *event)
{
	cout << "PressEvent " << event->buttons() << endl;
	if (event->buttons() & Qt::LeftButton) {
		_pc_core->f_zoom_pos_start(event->x(),event->y());
	}

	if (event->buttons() & Qt::RightButton) {
		_pc_core->f_zoom_reset();
	}
}

void window::mouseMoveEvent(QMouseEvent *event)
{

    cout << "Event" << endl;
    if (event->buttons() & Qt::LeftButton) {
    	_pc_core->f_zoom_pos_update(event->x(),event->y());
    }
    updateGL();
}

void window::mouseReleaseEvent(QMouseEvent * event )
{
	cout << "ReleaseEvent "<< event->buttons() << endl;
	if (!(event->buttons() & Qt::LeftButton)) {
		_pc_core->f_zoom_pos_end(event->x(),event->y());
	}
	updateGL();
}

