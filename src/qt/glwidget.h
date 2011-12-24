/*
 * glwidget.h
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
#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QtOpenGL>
#include <QGLWidget>

namespace glplot {
class glwidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit glwidget(int framesPerSecond = 0, QWidget *parent = 0, char *name = 0);
    virtual void initializeGL() = 0;
    virtual void resizeGL(int width, int height) = 0;
    virtual void paintGL() = 0;
    virtual void keyPressEvent( QKeyEvent *keyEvent );

public slots:
    virtual void timeOutSlot();

private:
    QTimer *t_Timer;

};
}


#endif // GLWIDGET_H
