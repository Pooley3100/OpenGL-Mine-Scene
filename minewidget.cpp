//#include "Windows.h" //Comment out if in linux
#include <GL/glu.h>
#include <QGLWidget>
#include "minewidget.h"
#include <QKeyEvent>
//#include <QDebug>
#include <QDir>
#include <QTextStream>

typedef struct materialStruct {
  GLfloat ambient[4];
  GLfloat diffuse[4];
  GLfloat specular[4];
  GLfloat shininess;
} materialStruct;


static materialStruct shinyMetal = {
  { 0.0, 0.0, 0.0, 1.0},
  { 0.7, 0.7, 0.0, 1.0},
  { 1.0, 1.0, 1.0, 1.0},
  120.0
};

static materialStruct doorColour = {
    {0.86, 0.58, 0.44, 1},
    {0.87,0.72,0.53, 1},
    {0.1, 0.1, 0.1, 1.0},
    5
};

static materialStruct wooden = {
    {0.2, 0.2, 0.2, 1},
    {0.5, 0.5, 0, 1},
    {1, 1, 1, 1},
    32
};

static materialStruct shinyRail = {
    {0.37, 0.37, 0.37, 1},
    {0.85, 0.85, 0.85, 1},
    {1, 1, 1, 1},
    100
};

static materialStruct cartGrey = {
    {0.20, 0.20, 0.20, 1},
    {0.37, 0.37, 0.37, 1},
    {0.9, 1, 1, 1},
    60
};

static materialStruct handleBar = {
    {0.33, 0.06, 0.07, 1},
    {0.34, 0.98, 0.09, 1},
    {1, 1, 1, 1},
    60
};

static materialStruct rainbow = {
    {0.70, 0.93, 0.23, 1},
    {0.75, 0.100, 0.24, 1},
    {1, 1, 1, 1},
    110
};

static materialStruct shinyYellow = {
    {0.70, 0.93, 0.23, 1},
    {0.70, 0.93, 0.23, 1},
    {1, 1, 1, 1},
    110
};


void MineWidget::moveGluLook(float x, float y, float z){
    gluX += x; gluY += y; gluZ += z;
    repaint();
    //qInfo() << "glu" << gluX << "GLuy" << gluY << "float y" << y;
}

MineWidget::MineWidget(QWidget *parent) : QGLWidget(parent)
{
    //Constructor
    cartAngle = 0;
    cartBeamInc = 5;
    cartBeamAngle = 0;
    crate = QVector<float>(100);
    rock = QVector<float>(100);

    gluX = 100; gluY = 0; gluZ = 500;
    gluAtX = -200; gluAtY = 0; gluAtZ = 0;
    heightOfPyramid = 5;

    //Load Images
    QImage imgTemp("../cwk2/Textures/markus.png");
    markusImg = QGLWidget::convertToGLFormat(imgTemp);
    QImage imgTemp1("../cwk2/Textures/Marc_Dekamps.png");
    markImg = QGLWidget::convertToGLFormat(imgTemp1);
    idolAngle = 0;
    QImage imgTemp2("../cwk2/Textures/Mercator-projection.png");
    mercatorImg = QGLWidget::convertToGLFormat(imgTemp2);
    QImage imgTemp3("../cwk2/Textures/dirt1.png");
    floorImg = QGLWidget::convertToGLFormat(imgTemp3);
    QImage imgTemp4("../cwk2/Textures/rockWall.jpg");
    wallImg = QGLWidget::convertToGLFormat(imgTemp4);
    //End of loading

    //Texture setup
    glEnable(GL_TEXTURE_2D);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                    GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_NEAREST);
    glDisable(GL_TEXTURE_2D);
    //End

    doorAngle = 0;
    doorAngleInc = -2;
    doorAnimate = false;

    light1 = true;
    light2 = true;

    cameraPosIndex = 0;
    cartAngleInc = 1;
    CartSpeed = 1;
    colourChangeIndex = 0;
    ambientEngaged = false;
}

void MineWidget::resizeGL(int w, int h){
    glViewport(0, 0, w, h);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //cart light
    GLfloat light_ambient[] = {1,1,1,1};
    GLfloat light_specular[] = {0.5, 0.5, 0, 1};

    glLightfv(GL_LIGHT1, GL_AMBIENT_AND_DIFFUSE, light_ambient);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);

    glShadeModel (GL_SMOOTH);

    //roof light
    GLfloat light_pos[] = {200, 300., 500., 1.};
    GLfloat light_direction [] = {0, 0, -1, 0.};
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glLightfv (GL_LIGHT0, GL_SPOT_DIRECTION,light_direction);


    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();
    glOrtho(-400.0, 400.0, -400.0, 400.0, -300, 1400.0);
}

void MineWidget::mineRectangle(float d, float w, float l, const materialStruct& material){

    float depth = d, width = w, length = l;

    glMaterialfv(GL_FRONT, GL_AMBIENT, material.ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, material.diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, material.specular);
    glMaterialf(GL_FRONT, GL_SHININESS, material.shininess);

    //Calculate Normal
    GLfloat normals[][3] = {{-1,0,0}};
    glNormal3fv(normals[0]);
    //Draw left plane (looking from above) plane now

    glColor3f(0.0,0.0,1.0);
    glBegin(GL_POLYGON);
    glVertex3f( -length, width,  depth);
    glVertex3f( -length, width, -depth);
    glVertex3f( -length,  -width, -depth);
    glVertex3f( -length,  -width,  depth);
    glEnd();

    //Calculate Normal
    normals[0][0] = 0;
    normals[0][1] = 1;
    normals[0][2] = 0;
    glNormal3fv(normals[0]);
    //Draw Top (looking from above) plane now

    glColor3f(0.0,1.0,1.0);
    glBegin(GL_POLYGON);

    glVertex3f( -length, width,  depth);
    glVertex3f( -length, width, -depth);
    glVertex3f( length,  width, -depth);
    glVertex3f( length,  width, depth);
    glEnd();

    //Calculate Normal
    normals[0][0] = 1;
    normals[0][1] = 0;
    normals[0][2] = 0;
    glNormal3fv(normals[0]);
    //Draw right (looking from above) plane now

    glColor3f(0.0,1.0,0.0);
    glBegin(GL_POLYGON);
    glVertex3f( length, width,  -depth);
    glVertex3f( length, width, depth);
    glVertex3f( length,  -width, depth);
    glVertex3f( length,  -width, -depth);
    glEnd();

    //Calculate Normal
    normals[0][0] = 0;
    normals[0][1] = -1;
    normals[0][2] = 0;
    glNormal3fv(normals[0]);
    //Draw bottom (looking from above) plane now

    glColor3f(1.0,0.0,0.0);
    glBegin(GL_POLYGON);
    glVertex3f( -length, -width,  depth);
    glVertex3f( -length, -width, -depth);
    glVertex3f( length,  -width, -depth);
    glVertex3f( length,  -width, depth);
    glEnd();

    //Calculate Normal
    normals[0][0] = 0;
    normals[0][1] = 0;
    normals[0][2] = 1;
    glNormal3fv(normals[0]);
    //Draw upper face (looking from above) plane now
    //This is the top of the box

    glColor3f(1.0,1.0,1.0);
    glBegin(GL_POLYGON);
    glNormal3f(0,0,1);
    glVertex3f( -length, width, depth);
    glVertex3f( -length,  -width, depth);
    glVertex3f( length,  -width, depth);
    glVertex3f( length, width,  depth);

    glEnd();
}

void MineWidget::minePole(float r, float h, const materialStruct& material){
    glMaterialfv(GL_FRONT, GL_AMBIENT, material.ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, material.diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, material.specular);
    glMaterialf(GL_FRONT, GL_SHININESS, material.shininess);

    //A pole of various height and radius and shader
    float radius = r;
    float height = h;
    float x0, y0, x1, y1;
    //const double pi = 3.14159265359;

    glColor3f(1.0,0.0,0.0);
    float sectorCount = 50;
    for(int i = 0;i<sectorCount;i++){
        x0 = radius*cos(2*i*pi/sectorCount);
        y0 = radius*sin(2*i*pi/sectorCount);
        x1 = radius*cos(2*(i+1)*pi/sectorCount);
        y1 = radius*sin(2*(i+1)*pi/sectorCount);
        glBegin(GL_POLYGON);

        glNormal3f(x0/radius, y0/radius, 0);
        glVertex3f(x0,y0,height);

        glNormal3f(x1/radius, y1/radius, 0);
        glVertex3f(x0,y0,0);

        glNormal3f(x0/radius, y0/radius, 0);
        glVertex3f(x1,y1,0);

        glNormal3f(x1/radius, y1/radius, 0);
        glVertex3f(x1,y1,height);

        glEnd();
    }
}

void MineWidget::mineTrack(float r){
    materialStruct material = shinyMetal;
    glMaterialfv(GL_FRONT, GL_AMBIENT, material.ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, material.diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, material.specular);
    glMaterialf(GL_FRONT, GL_SHININESS, material.shininess);

    //Same idea as cylinder but height 1 and each face is two rectangles
    float radius = r;
    float height = 1;
    float length = 0;  //This one changes
    float width = 30;
    float theta = 0;
    float x0, y0, x1, y1, xReal, yReal;
    const double pi = 3.14159265359;

    glColor3f(1.0,0.0,0.0);
    float sectorCount = 400;
    for(int j = 0; j<2;j++){
        length = (2*pi*radius) / sectorCount;
        for(int i = 0;i<sectorCount;i++){
            x0 = radius*cos(2*i*pi/sectorCount);
            y0 = radius*sin(2*i*pi/sectorCount);
            x1 = radius*cos(2*(i+1)*pi/sectorCount);
            y1 = radius*sin(2*(i+1)*pi/sectorCount);
            theta = (180/pi)*(2*i*pi/sectorCount);
            xReal = (x0 + x1)/2;
            yReal = (y1 + y0)/2;
            //Translate to middle of these two points and then move to middle of rectangle.
            //Then rotate by angle
            //Push and pop required
            glPushMatrix();
            glTranslatef(xReal, yReal, 0);
            glRotatef(theta,0,0,1);
            this->mineRectangle(height, width, length, shinyRail);
            glPopMatrix();
        }
        radius+=40;
    }

}

void MineWidget::wall(float width, float height){
    // create one face with brassMaterials
    materialStruct* material = &wooden;

    glMaterialfv(GL_FRONT, GL_AMBIENT,    material->ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,    material->diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,   material->specular);
    glMaterialf(GL_FRONT, GL_SHININESS,   material->shininess);


    glEnable(GL_TEXTURE_2D);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, wallImg.width(),
                 wallImg.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 wallImg.bits());


    //looking in +y direction when talking about walls and drawn from left of back wall
    //back wall
    glNormal3f(0,-1,0);
    //Drawn from bottom left position
    int texRepeat = 3;
    int tileInc = 5;
    glBegin(GL_QUADS);
    for (int x=0; x < width; x+=tileInc)
    {
        for (int y=0; y < height; y+=tileInc)
        {
            glTexCoord2f((x*texRepeat)/width, abs(y*texRepeat)/width);
            glVertex3f(x,0, y);
            glTexCoord2f(((x+tileInc)*texRepeat)/width, abs((y*texRepeat))/width);
            glVertex3f((x+tileInc), 0,  y);
            glTexCoord2f(((x+tileInc)*texRepeat)/width, abs((y+tileInc)*texRepeat)/width);
            glVertex3f((x+tileInc), 0, (y+tileInc));
            glTexCoord2f((x*texRepeat)/width, abs((y+tileInc)*texRepeat)/width);
            glVertex3f(x, 0,  (y+tileInc));


        }
    }
    glEnd();

    //left wall
    // y and z change, y inc first
    glNormal3f(1,0,0);
    texRepeat = 3;
    tileInc = 5;
    glBegin(GL_QUADS);
    for (int x=0; x > -width; x-=tileInc)
    {
        for (int y=0; y < height; y+=tileInc)
        {
            glTexCoord2f((x*texRepeat)/width, abs(y*texRepeat)/width);
            glVertex3f(0, x, y);
            glTexCoord2f(((x-tileInc)*texRepeat)/width, abs((y*texRepeat))/width);
            glVertex3f(0, (x-tileInc), y);
            glTexCoord2f(((x-tileInc)*texRepeat)/width, abs((y+tileInc)*texRepeat)/width);
            glVertex3f(0, (x-tileInc), (y+tileInc));
            glTexCoord2f((x*texRepeat)/width, abs((y+tileInc)*texRepeat)/width);
            glVertex3f(0, x, (y+tileInc));


        }
    }
    glEnd();

    //back wall
    // x and z change, x inc first
    glNormal3f(0,1,0);
    texRepeat = 3;
    tileInc = 5;
    glBegin(GL_QUADS);
    for (int x=0; x < width; x+=tileInc)
    {
        for (int y=0; y < height; y+=tileInc)
        {
            glTexCoord2f((x*texRepeat)/width, abs(y*texRepeat)/width);
            glVertex3f(x,-width, y);
            glTexCoord2f(((x+tileInc)*texRepeat)/width, abs((y*texRepeat))/width);
            glVertex3f((x+tileInc), -width,  y);
            glTexCoord2f(((x+tileInc)*texRepeat)/width, abs((y+tileInc)*texRepeat)/width);
            glVertex3f((x+tileInc), -width, (y+tileInc));
            glTexCoord2f((x*texRepeat)/width, abs((y+tileInc)*texRepeat)/width);
            glVertex3f(x, -width,  (y+tileInc));


        }
    }
    glEnd();



    //right wall
    glNormal3f(-1,0,0);
    texRepeat = 3;
    tileInc = 5;
    glBegin(GL_QUADS);
    for (int x=0; x > -width; x-=tileInc)
    {
        for (int y=0; y < height; y+=tileInc)
        {
            glTexCoord2f((x*texRepeat)/width, abs(y*texRepeat)/width);
            glVertex3f(width, x, y);
            glTexCoord2f(((x-tileInc)*texRepeat)/width, abs((y*texRepeat))/width);
            glVertex3f(width, (x-tileInc),  y);
            glTexCoord2f(((x-tileInc)*texRepeat)/width, abs((y+tileInc)*texRepeat)/width);
            glVertex3f(width, (x-tileInc),  (y+tileInc));
            glTexCoord2f((x*texRepeat)/width, abs((y+tileInc)*texRepeat)/width);
            glVertex3f(width, x,  (y+tileInc));


        }
    }
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void MineWidget::floor(float width){
    // create one face with brassMaterials

    //Material Setup
    materialStruct* material = &wooden;

    glMaterialfv(GL_FRONT, GL_AMBIENT,    material->ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,    material->diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,   material->specular);
    glMaterialf(GL_FRONT, GL_SHININESS,   material->shininess);

    glEnable(GL_TEXTURE_2D);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, floorImg.width(),
                 floorImg.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 floorImg.bits());



    glBegin(GL_QUADS);
    int texRepeat = 3;
    int tileInc = 5;
    glNormal3f(0, 0, 1);
    for (int x=0; x < width; x+=tileInc)
    {
        for (int y=0; y > -width; y-=tileInc)
        {

            glTexCoord2f((x*texRepeat)/width, abs(y*texRepeat)/width);
            glVertex3f(x, y, 0);
            glTexCoord2f((x*texRepeat)/width, abs((y-tileInc)*texRepeat)/width);
            glVertex3f(x,  (y-tileInc), 0);
            glTexCoord2f(((x+tileInc)*texRepeat)/width, abs((y-tileInc)*texRepeat)/width);
            glVertex3f((x+tileInc),  (y-tileInc), 0);
            glTexCoord2f(((x+tileInc)*texRepeat)/width, abs((y*texRepeat))/width);
            glVertex3f((x+tileInc),  y, 0);
        }
    }

    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void MineWidget::updateCart(){
    cartAngle += cartAngleInc;
    idolAngle += 10;
    if(cartAngle > 360){
        cartAngle = 0;
    }
    if(idolAngle > 360){
        idolAngle = 0;
    }
    mineCartX = 600*cos((pi/180)*cartAngle);
    mineCartY = 600*sin((pi/180)*cartAngle);
    mineCartZ = 0;

    //Temp set camera to follow
    setToFollow();
    //qInfo() << "gluX" << gluX << "gluY" << gluY << "gluZ" << gluZ;
    //qInfo() << "gluAtX" << gluAtX << "gluAtY" << gluAtY << "gluAtZ" << gluAtZ;
    repaint();
    //qInfo() << "Signal to update received";

    //Set angle for push beam on cart
    //bounce between -90 and 90 degrees; start at zero
    cartBeamAngle+=cartBeamInc;
    if(cartBeamAngle < -15){

            cartBeamInc = abs(cartBeamInc);


    } else if(cartBeamAngle > 15){

        cartBeamInc = cartBeamInc *-1;
    }

    //If door open engaged change door angle
    if(doorAnimate == true){
    doorAngle += doorAngleInc;
    if(doorAngle <= -90){
        doorAngleInc = 2;
    } else if(doorAngle >= 0){
        doorAngleInc = -2;
    }
    }
}

void MineWidget::setToFollow(){
    if(cameraPosIndex == 1){
    gluAtX = mineCartX;
    gluAtY = mineCartY;
    gluAtZ = mineCartZ;
    //go a bit behing cart about 100*
    //gluX = 600*cos((pi/180)*(cartAngle-0.5));
    //gluY = 600*sin((pi/180)*(cartAngle-0.5));
    gluZ = mineCartZ + 600;
    }
}


void MineWidget::mineCart(){
    //Draw the circle track first

    this->mineTrack(600);
    //glPopMatrix();

    //Hierarchical Mine Cart (begins above track)
    glPushMatrix();  //1

    //Rotate here according to updated cart <<
    glRotatef(cartAngle, 0, 0, 1);


    glTranslatef(615,0,30);

    //wheels front left
    glPushMatrix(); //2

    glTranslatef(-15, 10, 0);
    glPushMatrix(); // 3
    //glScalef(100,100,100);
    //glRotatef(cartAngle, 0,1,0);
    glRotatef(20, 0, 1 ,0);
    drawWheel(100);
    glPopMatrix();  //2

    //wheels back left
    glTranslatef(0, -85, 0);
    glPushMatrix(); //3
    //glScalef(100,100,100);
    glRotatef(20, 0, 1 ,0);
    drawWheel(100);
    glPopMatrix(); //2

    //Rotate and translate by 70 for right side wheels
    glPushMatrix(); //3
    glTranslatef(20,75,4);
    glRotatef(200, 0, 1, 0);

    //wheels front right
    glPushMatrix(); //4
    glTranslatef(-10, 10, 0);
    glPushMatrix(); //5
    //glScalef(100,100,100);
    drawWheel(100);
    glPopMatrix(); //4

    //wheels back right
    glTranslatef(0, -85, 0);
    glPushMatrix(); //5
    //glScalef(100,100,100);
    drawWheel(100);
    glPopMatrix(); //4

    glPopMatrix(); //3
    glPopMatrix(); //2
    glPopMatrix(); //1

    //cart
    //Select Colour
    if(colourChangeIndex == 2){
        this->mineRectangle(20,70,30, shinyYellow);
    } else if(colourChangeIndex == 3){
        this->mineRectangle(20,70,30, rainbow);
    }
    else{
        this->mineRectangle(20,70,30, cartGrey);
    }

    materialStruct choice;
    if(colourChangeIndex == 0){
        choice = cartGrey;
    } else if(colourChangeIndex == 1){
        choice = handleBar;
    } else if(colourChangeIndex == 2){
        choice = shinyYellow;
    } else{
        choice = rainbow;
    }

    glPushMatrix(); //2
    glTranslatef(0,0,30.0); //Middle of cart for pole
    this->minePole(10, 60, choice);
    glPopMatrix(); //1


    //Now draw side pole for pushing up and down
    //Get updated angle from timer and rotate
    glPushMatrix(); //2
    glRotatef(cartBeamAngle, 1, 0, 0);

    glPushMatrix(); //3
    glTranslatef(0,-90,90.0);
    glRotatef(270,1,0,0);
    this->minePole(10,180, choice);

    //handles back handle
    glPushMatrix(); //4
    glTranslatef(40,0,0);
    glRotatef(270,0,1,0);

    this->minePole(10,80, choice);
    glPopMatrix(); //3


    //top handle
    glPushMatrix(); //4
    glTranslatef(40,0,180);
    glRotatef(270,0,1,0);
    this->minePole(10,80, choice);
    glPopMatrix(); //3

    glPopMatrix(); //2

    //Get rid of support pole translation
    glPopMatrix(); //1

    //Get rid of cart rotation
    glPopMatrix(); //0
    //glPopMatrix();
}

void MineWidget::changeCartSpeed(int i){
    if(CartSpeed == 0){
        cartBeamInc = 4;
    }
    if(i>CartSpeed){
        cartAngleInc += 1;
        if(cartBeamInc>=0){
            cartBeamInc+=1;
        } else if(cartBeamInc<0){
            cartBeamInc-=1;
        }
    }else if(i == 0){
        cartAngleInc = 0;
        cartBeamInc = 0;
        cartBeamAngle = 0;
    } else if(i<CartSpeed){
        cartAngleInc -= 1;
        if(cartBeamInc>0){
            cartBeamInc-=1;
        } else if(cartBeamInc<=0){
            cartBeamInc+=1;
        }
    }
    CartSpeed = i;
}

void MineWidget::drawCrates(const materialStruct& material, int scale){
    glMaterialfv(GL_FRONT, GL_AMBIENT, material.ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, material.diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, material.specular);
    glMaterialf(GL_FRONT, GL_SHININESS, material.shininess);

    //qInfo() << QDir::currentPath();

    //Load in files on first read
    if(crate.length() < 400){
        //QString crateFile = "C:/Users/mpool/CompYear3Sem1LOCAL/Computer Graphics/Cwk2/cwk2/Objects/crate.obj";
        QString crateFile = "../cwk2/Objects/crate.obj";
        crate = loadObj(crateFile, 1);

        QImage crateTemp("../cwk2/Textures/crateTex.png");
        crateImg = QGLWidget::convertToGLFormat(crateTemp);
    }

    glEnable(GL_TEXTURE_2D);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, crateImg.width(),
                 crateImg.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 crateImg.bits());

    //Programmatically draw pyramid of crates
    float startX = -(heightOfPyramid/2)*(scale*2), startY = (heightOfPyramid/2)*(scale*2), startZ=0, tiles =heightOfPyramid;
    glPushMatrix();
    glTranslatef(startX, startY, startZ);
    startX = scale;
    startY = -scale;
    while(tiles>=2){
        for(int x = 0; x<((scale*2)*tiles);x+=(scale*2)){
            for(int y =0;y>-((scale*2)*tiles);y-=(scale*2)){
                glPushMatrix();
                            glTranslatef(x+startX, y+startY, startZ);
                            processObjLine(crate, scale, true);
                            glPopMatrix();
        }
        }
        startX += (scale);
        startY -= (scale);
        startZ += scale*2;
    tiles-=1;
    }
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
}

void MineWidget::drawWheel(int scale){
    if(wheel.length() < 400){
    QString crateFile = "../cwk2/Objects/wheel.obj";
    wheel = loadObj(crateFile, 2);
    }
    processObjLine(wheel, scale, false);
}

QVector<float> MineWidget::loadObj(QString name, int objNumber){
    QFile file(name);
    //qInfo() << file.exists();
    QVector<float> savedInput;
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return savedInput;

    int objCount = 0;
    QStringList objFile;
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList lineCheck = line.split(QLatin1Char(' '));
        if(lineCheck[0].compare("o")==0){
            objCount +=1;
        }
        if(lineCheck[0].compare("f")==0 && (objCount!=objNumber)){
            continue;
        }

        objFile.append(line);

    }

    //Now with whole file create a list of vertices and then go through faces and draw quads.
    QStringList verticesFile;
    QStringList facesFile;
    QStringList normalsFile;
    QStringList texturesFile;
    for(int i = 0; i<objFile.length();i++){
        QStringList list1 = objFile[i].split(QLatin1Char(' '));
        if(list1[0].compare("vn")==0){
            normalsFile.append(objFile[i]);
        }
        else if(list1[0].compare("v") == 0){
            verticesFile.append(objFile[i]);
        }
        else if(list1[0].compare("f") == 0){
            facesFile.append(objFile[i]);
        }
        else if(list1[0].compare("vt")==0){
            texturesFile.append(objFile[i]);
        }

    }

    //Now draw each face;
    for(int i = 0; i<facesFile.length();i++){
        //Split according to space then split again according to / and take first item of each
        //Then use this number to find the vertice indice and minus 1
        QStringList split1 = facesFile[i].split(QLatin1Char(' '));
        QStringList pList[4];
        pList[0] = split1[1].split(QLatin1Char('/'));
        pList[1] = split1[2].split(QLatin1Char('/'));
        pList[2] = split1[3].split(QLatin1Char('/'));

        if(split1.length()>4){
            pList[3] = split1[4].split(QLatin1Char('/'));

            savedInput.append(-999);
            glBegin(GL_POLYGON);
            for(int j = 0; j<4;j++){
                QString normalLine = normalsFile[pList[j][2].toInt()-1];
                QString verticeLine = verticesFile[pList[j][0].toInt()-1];
                QString texLine = texturesFile[pList[j][1].toInt()-1];
                QStringList verticeList = verticeLine.split(QLatin1Char(' '));
                QStringList normalList = normalLine.split(QLatin1Char(' '));
                QStringList texList = texLine.split(QLatin1Char(' '));

                glNormal3f(normalList[1].toFloat(), normalList[2].toFloat(), normalList[3].toFloat());
                glVertex3f(verticeList[1].toFloat(), verticeList[2].toFloat(), verticeList[3].toFloat());
                savedInput.append(normalList[1].toFloat());
                savedInput.append(normalList[2].toFloat());
                savedInput.append(normalList[3].toFloat());

                //Texture vt goes here
                savedInput.append(texList[1].toFloat());
                savedInput.append(texList[2].toFloat());

                savedInput.append(verticeList[1].toFloat());
                savedInput.append(verticeList[2].toFloat());
                savedInput.append(verticeList[3].toFloat());
            }

            glEnd();

         } else{
            savedInput.append(-998);
            glBegin(GL_POLYGON);
            for(int j = 0; j<3;j++){
                QString normalLine = normalsFile[pList[j][2].toInt()-1];
                QString texLine = texturesFile[pList[j][1].toInt()-1];
                QString verticeLine = verticesFile[pList[j][0].toInt()-1];

                QStringList verticeList = verticeLine.split(QLatin1Char(' '));
                QStringList normalList = normalLine.split(QLatin1Char(' '));
                QStringList texList = texLine.split(QLatin1Char(' '));

                glNormal3f(normalList[1].toFloat(), normalList[2].toFloat(), normalList[3].toFloat());
                glVertex3f(verticeList[1].toFloat(), verticeList[2].toFloat(), verticeList[3].toFloat());

                savedInput.append(normalList[1].toFloat());
                savedInput.append(normalList[2].toFloat());
                savedInput.append(normalList[3].toFloat());

                //Texture vt goes here
                savedInput.append(texList[1].toFloat());
                savedInput.append(texList[2].toFloat());

                savedInput.append(verticeList[1].toFloat());
                savedInput.append(verticeList[2].toFloat());
                savedInput.append(verticeList[3].toFloat());
            }

            glEnd();
        }


    }
    return savedInput;
}

void MineWidget::processObjLine(QVector<float> obj, int scale, bool tex){
    int count = 0;
    while(true){
        int loopCount = 0;
        if(obj[count] == -999){
            loopCount = 4;
            glBegin(GL_QUADS);
        } else{
            glBegin((GL_TRIANGLES));
            loopCount = 3;
        }
        count+=1;

        for(int i = 0;i<loopCount;i++){
            glNormal3f(obj[count], obj[count+1], obj[count+2]);
            if(tex){
            glTexCoord2f(obj[count+3],obj[count+4]);
            }
            glVertex3f(obj[count+5]*scale,obj[count+6]*scale,obj[count+7]*scale);

            count+=8;

        }
        glEnd();

        if(count>=obj.length()){
            break;
        }

    }  
}

void MineWidget::DrawRocks(const materialStruct& material, float scale){
    glMaterialfv(GL_FRONT, GL_AMBIENT, material.ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, material.diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, material.specular);
    glMaterialf(GL_FRONT, GL_SHININESS, material.shininess);

    //Edit this to store once read first time.
    if(rock.length() < 400){
        QString crateFile = "../cwk2/Objects/rock.obj";
        rock = loadObj(crateFile, 1);

        QImage crateTemp("../cwk2/Textures/rockTex.png");
        rockImg = QGLWidget::convertToGLFormat(crateTemp);
    }

    //Setup Texture
    static GLuint texName;

    glEnable(GL_TEXTURE_2D);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);


    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                    GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_NEAREST);

    glBindTexture(GL_TEXTURE_2D, texName);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, rockImg.width(),
                 rockImg.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 rockImg.bits());

    //Loop here to make pyramid

    glPushMatrix();
    glTranslatef(-650,850, 0);
    processObjLine(rock, scale, true);
    glTranslatef(1300,0, 0);
    processObjLine(rock, scale, true);
    glTranslatef(0,-1300, 0);
    processObjLine(rock, scale, true);
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
}

void MineWidget::drawIdolCube(float crateScale){
    //Find current z height
    float zHeight = crateScale*2*(heightOfPyramid)-crateScale;
    //Centre is 0, 0;
    //Box is 120 width and height;

    //Load first potrait (markus)
    glEnable(GL_TEXTURE_2D);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, markusImg.width(),
                 markusImg.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 markusImg.bits());


    crateScale = crateScale*2;
    glPushMatrix();
    //Top right corner of base of cube
    glTranslatef(-60, 60, zHeight);

    //back face
    glBegin(GL_POLYGON);
    glNormal3f(0,1,0);
    glTexCoord2f(0,0);
    glVertex3f(0,0,0);
    glTexCoord2f(0,1);
    glVertex3f(0,0,crateScale);
    glTexCoord2f(1,1);
    glVertex3f(crateScale,0,crateScale);
    glTexCoord2f(1,0);
    glVertex3f(crateScale,0,0);

    glEnd();

    //left face
    glBegin(GL_POLYGON);
    glNormal3f(-1,0,0);
    glTexCoord2f(0,0);
    glVertex3f(0,0,0);
    glTexCoord2f(1, 0);
    glVertex3f(0,-crateScale,0);
    glTexCoord2f(1, 1);
    glVertex3f(0,-crateScale,crateScale);
    glTexCoord2f(0, 1);
    glVertex3f(0,0,crateScale);

    glEnd();

    //right face
    glBegin(GL_POLYGON);
    glNormal3f(1,0,0);
    glTexCoord2f(0, 0);
    glVertex3f(crateScale,0,0);
    glTexCoord2f(1, 0);
    glVertex3f(crateScale,-crateScale,0);
    glTexCoord2f(1, 1);
    glVertex3f(crateScale,-crateScale,crateScale);
    glTexCoord2f(0, 1);
    glVertex3f(crateScale,0,crateScale);

    glEnd();

    //Changing texture to other potrait picture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, markImg.width(),
                 markImg.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 markImg.bits());

    //front face
    glBegin(GL_POLYGON);
    glNormal3f(0,-1,0);
    glTexCoord2f(0, 0);
    glVertex3f(0,-crateScale,0);
    glTexCoord2f(0, 1);
    glVertex3f(0,-crateScale,crateScale);
    glTexCoord2f(1, 1);
    glVertex3f(crateScale,-crateScale,crateScale);
    glTexCoord2f(1, 0);
    glVertex3f(crateScale,-crateScale,0);

    glEnd();

    //top face
    glBegin(GL_POLYGON);
    glNormal3f(0,0,1);
    glTexCoord2f(0, 0);
    glVertex3f(0,0,crateScale);
    glTexCoord2f(0, 1);
    glVertex3f(0,-crateScale,crateScale);
    glTexCoord2f(1, 1);
    glVertex3f(crateScale,-crateScale,crateScale);
    glTexCoord2f(1, 0);
    glVertex3f(crateScale,0,crateScale);

    glEnd();



    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void MineWidget::door(){
    //Setup

    //Material Setup
    materialStruct* material = &doorColour;

    glMaterialfv(GL_FRONT, GL_AMBIENT,    material->ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,    material->diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,   material->specular);
    glMaterialf(GL_FRONT, GL_SHININESS,   material->shininess);


    //Texture setup
    glEnable(GL_TEXTURE_2D);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mercatorImg.width(),
                 mercatorImg.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 mercatorImg.bits());

    //Load mercator projection image


    float length = 200;
    float pointLength = length/4;
    float height = 250;
    float widthDoor = 22;
    //This is a hexagonal opening door
    glPushMatrix();
    glTranslatef(-749, -700, 0);

    //Front of door
    glPushMatrix();
    glRotatef(doorAngle, 0, 0, 1);
    glBegin(GL_POLYGON);
    glNormal3f(1, 0, 0);
    glTexCoord2f(0.2, 0);
    glVertex3f(widthDoor, 0, 0);
    glTexCoord2f(0.8, 0);
    glVertex3f(widthDoor, length, 0);
    glTexCoord2f(1, 0.5);
    glVertex3f(widthDoor, length+pointLength, height/2);
    glTexCoord2f(0.8, 1);
    glVertex3f(widthDoor, length, height);
    glTexCoord2f(0.2, 1);
    glVertex3f(widthDoor, 0, height);
    glTexCoord2f(0, 0.5);
    glVertex3f(widthDoor, -pointLength, height/2);
    glEnd();

    //Back of door
    glBegin(GL_POLYGON);
    glNormal3f(-1, 0, 0);
    glVertex3f(2, 0, 0);
    glVertex3f(2, length, 0);
    glVertex3f(2, length+pointLength, height/2);
    glVertex3f(2, length, height);
    glVertex3f(2, 0, height);
    glVertex3f(2, -pointLength, height/2);
    glEnd();

    float startX = 2;

    //Edges around door starting with corner pointing in +y facing down
    glBegin(GL_POLYGON);
    glNormal3f(0, 1, -0.5);
    glVertex3f(startX, length, 0);
    glVertex3f(widthDoor, length, 0);
    glVertex3f(widthDoor, length+pointLength, height/2);
    glVertex3f(startX, length+pointLength, height/2);
    glEnd();

    glBegin(GL_POLYGON);
    glNormal3f(0, 1, 0.5);
    glVertex3f(startX, length+pointLength, height/2);
    glVertex3f(widthDoor, length+pointLength, height/2);
    glVertex3f(widthDoor, length, height);
    glVertex3f(startX, length, height);
    glEnd();

    glBegin(GL_POLYGON);
    glNormal3f(0, 0, 1);
    glVertex3f(widthDoor, length, height);
    glVertex3f(widthDoor, 0, height);
    glVertex3f(startX, 0, height);
    glVertex3f(startX, length, height);
    glEnd();

    glPopMatrix();

    glBegin(GL_POLYGON);
    glNormal3f(-1, 0, 0);
    glVertex3f(1, 0, 0);
    glVertex3f(1, length, 0);
    glVertex3f(1, length+pointLength, height/2);
    glVertex3f(1, length, height);
    glVertex3f(1, 0, height);
    glVertex3f(1, -pointLength, height/2);
    glEnd();


    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void MineWidget::paintScene(){
    //Draw crate Pyramid
    glPushMatrix();
    glTranslatef(0, 0, 120);
    drawCrates(wooden, 60);
    glPopMatrix();

    //Room is 1500 width, 1500 length and 400 height?
    //Draw the four walls and floor and ceiling
    glPushMatrix();
    glTranslatef(-750, 750, 0);
    this->floor(1500);
    this->wall(1500, 600);
    glPopMatrix();

    //Now the mine cart and track
    mineCart();

    //Instancing the platonic solid hexagon with normals (correct shaders and corner normals for the hexagon using normalize and glm) multiple scalings. this is rocks.
    DrawRocks(wooden, 100);

    //Something rotating spinning box at top of pyramid with faces???
    glPushMatrix();
    glRotatef(idolAngle, 0, 0, 1);
    drawIdolCube(60);
    glPopMatrix();

    //Hexagonal door with mercator projection
    door();

}

void MineWidget::paintGL(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);

    glEnable(GL_DEPTH_TEST);
    glLoadIdentity();
    gluLookAt(gluX,gluY,gluZ, gluAtX,gluAtY,gluAtZ, 0.0,0.0,1.0);



    paintScene();

    //Light switches
    if(light1){
        glEnable(GL_LIGHT0);
    } else{
        glDisable(GL_LIGHT0);
    }
    if(light2){
        glEnable(GL_LIGHT1);
    } else{
        glDisable(GL_LIGHT1);
    }

    //Update lighting. Light1 is cart light, light 0 is roof light
    float cameraX = 600*cos((pi/180)*(cartAngle+5));
    float cameraY = 600*sin((pi/180)*(cartAngle+5));
    GLfloat light_pos[] = {mineCartX, mineCartY, 100, 1.};
    float dirX = cameraX - mineCartX;
    float dirY = cameraY - mineCartY;
    GLfloat light_dir[] = {dirX, dirY, 10, 0.};
    glLightf (GL_LIGHT1, GL_SPOT_CUTOFF, 55);
    glLightfv(GL_LIGHT1, GL_POSITION, light_pos);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, light_dir);
    GLfloat exp(0.0);
    glLightfv(GL_LIGHT1, GL_SPOT_EXPONENT, &exp);
    GLfloat light_ambient[] = {0.8,0.8,0,1};
    glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);

    //Option to change ambience of world
    if(ambientEngaged){
        GLfloat global_light_ambient[] = {ambientLevel,ambientLevel,ambientLevel,1};
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_light_ambient); //Ambience of the whole scene
    }

    glLoadIdentity();

    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();
    if(cameraPosIndex == 1){
        glOrtho(-400.0, 400.0, -400.0, 400.0, -300, 1400.0);
    } else if(cameraPosIndex == 0){
        glOrtho(-750, 750, -750, 750, -550, 1050);
    }
    //glOrtho (-w, w, -h, h, -100.0, 1000.0);

    glFlush();
}

void MineWidget::initializeGL(){
 glClearColor(0.3, 0.3, 0.3, 0.0);
}

//All UI from here

void MineWidget::moveGluLookAt(float x, float y, float z){
    gluAtX += x; gluAtY += y; gluAtZ += z;
    //qInfo() << "AT X" << gluAtX << "AT Y" << gluAtY << "AT Z " << gluAtZ;
    repaint();
}

void MineWidget::keyPressEvent(QKeyEvent* event){
    switch ( event->key() ) {
        case Qt::Key_W:
            //qInfo() << "HI";
            break;
    }
}

void MineWidget::changeGluX(double d){
    gluX = d;
}
void MineWidget::changeGluY(double d){
    gluY = d;
}
void MineWidget::changeGluZ(double d){
    gluZ = d;
}
void MineWidget::changeGluATX(double d){
    gluAtX = d;
}
void MineWidget::changeGluATY(double d){
    gluAtY = d;
}
void MineWidget::changeGluATZ(double d){
    gluAtZ = d;
}
void MineWidget::setPyramidHeight(int d){
    heightOfPyramid = d;
}
void MineWidget::setDoorAnimate(int in){
    if(in){
    doorAnimate = true;
    } else{
        doorAnimate = false;
    }
}
void MineWidget::switchLight1(int in){
    if(in){
        light1 = true;
    } else{
        light1 = false;
    }
}
void MineWidget::switchLight2(int in){
    if(in){
        light2 = true;
    } else{
        light2 =false;
    }
}
void MineWidget::changeCameraPos(int index){
    //0 is top down, 1 is follow cart
    cameraPosIndex = index;
    if(index == 1){
        cameraX = gluX;
        cameraY = gluY;
        cameraZ = gluZ;
        cameraAtX = gluAtX;
        cameraAtY = gluAtY;
        cameraAtZ = gluAtZ;
    }
    else if(index == 0){
        gluX = cameraX;
        gluY = cameraY;
        gluZ = cameraZ;
        gluAtX = cameraAtX;
        gluAtY = cameraAtY;
        gluAtZ = cameraAtZ;
    }
}
void MineWidget::changeCartColour(int index){
    colourChangeIndex = index;
}
void MineWidget::changeAmbience(int value){
    //value set between 1 and 100
    ambientEngaged = true;
    ambientLevel = value/100;
}


