#include "minemain.h"
#include <QDebug>
#include <QKeyEvent>
#include <QPushButton>
#include <QHBoxLayout>
#include <QDialogButtonBox>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QLabel>
#include <QCheckBox>
#include <QComboBox>
#include <QSlider>


MineMain::MineMain(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    QVBoxLayout* sideOptions = new QVBoxLayout();

    //Main drawing widget
    mineWidget = new MineWidget(NULL);
    mainLayout->addWidget(mineWidget, 90);

    //Add glu Pos options horizonals
    QLabel* gluPoslabel = new QLabel("Glu Position");
    sideOptions->addWidget(gluPoslabel);

    //glu x
    QHBoxLayout* gluPosX = new QHBoxLayout;
    QLabel* gluXl = new QLabel("GluX");
    QDoubleSpinBox* gluX = new QDoubleSpinBox;
    gluX->setMaximum(1500);
    gluX->setMinimum(-1500);
    gluX->setValue(100);
    gluPosX->addWidget(gluXl);
    gluPosX->addWidget(gluX);
    connect(gluX, SIGNAL(valueChanged(double)), mineWidget, SLOT(changeGluX(double)));

    sideOptions->addLayout(gluPosX);

    //glu y
    QHBoxLayout* gluPosY = new QHBoxLayout;
    QLabel* gluYl = new QLabel("GluY");
    QDoubleSpinBox* gluY = new QDoubleSpinBox;
    gluY->setMaximum(1500);
    gluY->setMinimum(-1500);
    gluPosY->addWidget(gluYl);
    gluPosY->addWidget(gluY);
    connect(gluY, SIGNAL(valueChanged(double)), mineWidget, SLOT(changeGluY(double)));

    sideOptions->addLayout(gluPosY);

    //glu z
    QHBoxLayout* gluPosZ = new QHBoxLayout;
    QLabel* gluZl = new QLabel("GluZ");
    QDoubleSpinBox* gluZ = new QDoubleSpinBox;
    gluZ->setMaximum(1500);
    gluZ->setMinimum(-1500);
    gluZ->setValue(500);
    gluPosZ->addWidget(gluZl);
    gluPosZ->addWidget(gluZ);
    connect(gluZ, SIGNAL(valueChanged(double)), mineWidget, SLOT(changeGluZ(double)));

    sideOptions->addLayout(gluPosZ);
    sideOptions->addSpacing(50);

    //Add glu Pos options horizonals
    QLabel* gluAtlabel = new QLabel("Glu Point At");
    sideOptions->addWidget(gluAtlabel);

    //glu At x
    QHBoxLayout* gluAtX = new QHBoxLayout;
    QLabel* gluAtXl = new QLabel("GluAtX");
    QDoubleSpinBox* gluAX = new QDoubleSpinBox;
    gluAX->setMaximum(1500);
    gluAX->setMinimum(-1500);
    gluAX->setValue(-200);
    gluAtX->addWidget(gluAtXl);
    gluAtX->addWidget(gluAX);
    connect(gluAX, SIGNAL(valueChanged(double)), mineWidget, SLOT(changeGluATX(double)));

    sideOptions->addLayout(gluAtX);

    //glu At y
    QHBoxLayout* gluAtY = new QHBoxLayout;
    QLabel* gluAtYl = new QLabel("GluAtY");
    QDoubleSpinBox* gluAY = new QDoubleSpinBox;
    gluAY->setMaximum(1500);
    gluAY->setMinimum(-1500);
    gluAY->setValue(0);
    gluAtY->addWidget(gluAtYl);
    gluAtY->addWidget(gluAY);
    connect(gluAY, SIGNAL(valueChanged(double)), mineWidget, SLOT(changeGluATY(double)));

    sideOptions->addLayout(gluAtY);

    //glu At z
    QHBoxLayout* gluAtZ = new QHBoxLayout;
    QLabel* gluAtZl = new QLabel("GluAtZ");
    QDoubleSpinBox* gluAZ = new QDoubleSpinBox;
    gluAZ->setMaximum(1500);
    gluAZ->setMinimum(-1500);
    gluAZ->setValue(0);
    gluAtZ->addWidget(gluAtZl);
    gluAtZ->addWidget(gluAZ);
    connect(gluAZ, SIGNAL(valueChanged(double)), mineWidget, SLOT(changeGluATZ(double)));

    sideOptions->addLayout(gluAtZ);
    sideOptions->addSpacing(30);

    //Pyramid Height
    QLabel* pyHl = new QLabel("Pyramid Height");
    QSpinBox* pyHBox = new QSpinBox;
    connect(pyHBox, SIGNAL(valueChanged(int)), mineWidget, SLOT(setPyramidHeight(int)));
    pyHBox->setValue(5);
    pyHBox->setMaximum(7);
    pyHBox->setMinimum(1);
    sideOptions->addSpacing(20);
    sideOptions->addWidget(pyHl);
    sideOptions->addWidget(pyHBox);


    //Cart Speed
    QLabel* speedl = new QLabel("Speed of Cart");
    QSpinBox* speed = new QSpinBox;
    speed->setValue(1);
    speed->setMaximum(10);
    speed->setMinimum(-2);
    connect(speed, SIGNAL(valueChanged(int)), this, SLOT(setTimer(int)));

    sideOptions->addSpacing(20);
    sideOptions->addWidget(speedl);
    sideOptions->addWidget(speed);

    //Door Animate
    QLabel* doorl = new QLabel("Open Door");
    QCheckBox* door = new QCheckBox;
    connect(door, SIGNAL(stateChanged(int)), mineWidget, SLOT(setDoorAnimate(int)));

    sideOptions->addSpacing(20);
    sideOptions->addWidget(doorl);
    sideOptions->addWidget(door);

    //Light switches
    QLabel* lightl1 = new QLabel("Roof Light");
    QLabel* lightl2 = new QLabel("Cart Light");
    QCheckBox* light1 = new QCheckBox;
    QCheckBox* light2 = new QCheckBox;
    QHBoxLayout* lightlabel = new QHBoxLayout;
    QHBoxLayout* lightbox = new QHBoxLayout;

    connect(light1, SIGNAL(stateChanged(int)), mineWidget, SLOT(switchLight1(int)));
    connect(light2, SIGNAL(stateChanged(int)), mineWidget, SLOT(switchLight2(int)));

    lightlabel->addWidget(lightl1);
    lightlabel->addWidget(lightl2);
    lightbox->addWidget(light1);
    lightbox->addWidget(light2);

    light1->setCheckState(Qt::Checked);
    light2->setCheckState(Qt::Checked);

    sideOptions->addSpacing(20);
    sideOptions->addLayout(lightlabel);
    sideOptions->addLayout(lightbox);

    //Camera Position drop down box
    QLabel *cameraOptionsl = new QLabel("Camera Position");
    QComboBox * cameraOptions = new QComboBox();
    cameraOptions->addItem("Top Down (Glu pos can be changed)");
    cameraOptions->addItem("Follow Cart (Glu pos can't be changed)");

    connect(cameraOptions, SIGNAL(currentIndexChanged(int)), mineWidget, SLOT(changeCameraPos(int)));

    sideOptions->addSpacing(20);
    sideOptions->addWidget(cameraOptionsl);
    sideOptions->addWidget(cameraOptions);

    //Select Cart Material
    QLabel *cartOptionsl = new QLabel("Cart Material");
    QComboBox * cartOptions = new QComboBox();

    cartOptions->addItem("Grey");
    cartOptions->addItem("Just pushdown bar green");
    cartOptions->addItem("Whole cart shiny yellow");
    cartOptions->addItem("Whole carte rainbow");

    connect(cartOptions, SIGNAL(currentIndexChanged(int)), mineWidget, SLOT(changeCartColour(int)));

    sideOptions->addSpacing(30);
    sideOptions->addWidget(cartOptionsl);
    sideOptions->addWidget(cartOptions);

    //Change Colour Ambience of World
    QLabel *ambiencel = new QLabel("World Ambience");
    QSlider *ambience = new QSlider(Qt::Horizontal);

    ambience->setRange(1, 100);
    ambience->setSingleStep(1);
    connect(ambience, SIGNAL(valueChanged(int)), mineWidget, SLOT(changeAmbience(int)));

    sideOptions->addSpacing(30);
    sideOptions->addWidget(ambiencel);
    sideOptions->addWidget(ambience);

    sideOptions->addStretch();
    mainLayout->addLayout(sideOptions, 10);


   //Animation go through here.
   animateTimer = new QTimer(this);
   animateTimer->start(15);
   connect(animateTimer, SIGNAL(timeout()), mineWidget, SLOT(updateCart()));
}

MineMain::~MineMain()
{

}

void MineMain::setTimer(int i){
    mineWidget->changeCartSpeed(i);
}

void MineMain::keyPressEvent(QKeyEvent* event){
    switch ( event->key() ) {
    case Qt::Key_W:
      //qInfo() << "HI";
        mineWidget->moveGluLook(0, 10, 0);
        break;
    case Qt::Key_S:
        mineWidget->moveGluLook(0,-10,0);
        break;
    case Qt::Key_A:
        mineWidget->moveGluLook(-10,0,0);
        break;
    case Qt::Key_D:
        mineWidget->moveGluLook(10,0,0);
        break;
    case Qt::Key_Q:
        mineWidget->moveGluLook(0,0,10);
        break;
    case Qt::Key_E:
        mineWidget->moveGluLook(0,0,-10);
        break;
    case Qt::Key_Up:
        mineWidget->moveGluLookAt(0,10,0);
        break;
    case Qt::Key_Down:
        mineWidget->moveGluLookAt(0,-10,0);
        break;
    case Qt::Key_Left:
        mineWidget->moveGluLookAt(-10,0,0);
        break;
    case Qt::Key_Right:
        mineWidget->moveGluLookAt(10,0,0);
        break;
    case Qt::Key_K:
        mineWidget->moveGluLookAt(0,0,10);
        break;
    case Qt::Key_J:
        mineWidget->moveGluLookAt(0,0,-10);
        break;


    }
}

