#ifndef MINEMAIN_H
#define MINEMAIN_H

#include <QMainWindow>
#include <QGLWidget>
#include "minewidget.h"
#include <QBoxLayout>
#include <QMenuBar>
#include <QTimer>



class MineMain : public QWidget
{
Q_OBJECT
public slots:
    void setTimer(int i);

public:
    MineWidget* mineWidget;
    MineMain(QWidget *parent = nullptr);
    void keyPressEvent(QKeyEvent* event);
    ~MineMain();
    QTimer *animateTimer;

private:

};
#endif // MINEMAIN_H
