#ifndef MINEWIDGET_H
#define MINEWIDGET_H

#include <QWidget>
#include <QGLWidget>
#include <QString>
#include <math.h>


struct materialStruct;

class MineWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit MineWidget(QWidget *parent = nullptr);
    void keyPressEvent(QKeyEvent* event);
    void moveGluLook(float x, float y, float z);
    void moveGluLookAt(float x, float y, float z);

    float cartAngle;
    float cartBeamInc;
    float cartBeamAngle;
    void changeCartSpeed(int i);

public slots:
    void updateCart();
    void changeGluX(double d);
    void changeGluY(double d);
    void changeGluZ(double d);
    void changeGluATX(double d);
    void changeGluATY(double d);
    void changeGluATZ(double d);
    void setPyramidHeight(int d);
    void setDoorAnimate(int in);
    void switchLight1(int in);
    void switchLight2(int in);
    void changeCameraPos(int index);
    void changeCartColour(int index);
    void changeAmbience(int value);


protected:
// called when OpenGL context is set up
void initializeGL();
// called every time the widget is resized
void resizeGL(int w, int h);
// called every time the widget needs painting
void paintGL();



private:
float gluX, gluY, gluZ, gluAtX, gluAtY, gluAtZ;
float OrthoX, OrthoY, OrthoZ;
void mineRectangle(float d, float w, float l, const materialStruct& material);
void mineCart();
void minePole(float r, float h, const materialStruct& material);
void mineTrack(float r);
void paintScene();
void wall(float width, float height);
void drawCrates(const materialStruct& material, int scale);
QVector<float> loadObj(QString name, int objNumber);
void processObjLine(QVector<float> obj, int scale, bool tex);
void drawWheel(int scale);
QVector<float> crate;
QVector<float> wheel;
void floor(float width);
void DrawRocks(const materialStruct& material, float scale);
QImage crateImg;
QImage rockImg;
QImage wallImg;
QImage floorImg;
QImage markusImg;
QImage markImg;
QImage mercatorImg;
QVector<float> rock;
float mineCartX, mineCartY, mineCartZ;
float idolAngle;
float doorAngle;
float doorAngleInc;
bool light1, light2;
void setToFollow();
const double pi = 3.14159265359;
float heightOfPyramid;
void drawIdolCube(float crateScale);
void door();
bool doorAnimate;
float cameraX, cameraY, cameraZ;
float cameraAtX, cameraAtY, cameraAtZ;
float cameraPosIndex;
GLuint wallBind;
float cartAngleInc;
int CartSpeed;
int colourChangeIndex;
bool ambientEngaged;
float ambientLevel;

signals:

};

#endif // MINEWIDGET_H
