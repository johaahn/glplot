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
