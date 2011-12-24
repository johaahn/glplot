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

#endif // MYWINDOW_H
