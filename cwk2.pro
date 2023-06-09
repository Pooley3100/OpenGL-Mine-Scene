QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

TEMPLATE = app
TARGET = cwk2
INCLUDEPATH += . /opt/local/include \
                 $$PWD/Objects

QT += widgets opengl gui

SOURCES += \
    main.cpp \
    minemain.cpp \
    minewidget.cpp \

HEADERS += \
    minemain.h \
    minewidget.h

LIBS += -lGLU                 # Switch library includes if running in linux
# LIBS     += opengl32.lib -lglu32

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
