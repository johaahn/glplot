/*
 * window.h
 *
 *  Created on: 4 sept. 2011
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
#ifndef MYWINDOW_H
#define MYWINDOW_H

#include "glwidget.h"
#include <glplot/core.hh>


namespace glplot {
class window: public glwidget {
Q_OBJECT

	core * _pc_core;

public:
	explicit window(core * in_pc_core, QWidget *parent = 0);

public:
	void customEvent(QEvent* e);

signals:
	void clicked();

protected:
	void initializeGL();
	void resizeGL(int width, int height);
	void paintGL();

	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
};
}

static const int UPDATEGL_CUSTOM_EVENT=1002;
static const int NEW_FIGURE_CUSTOM_EVENT=1001;

#endif // MYWINDOW_H
