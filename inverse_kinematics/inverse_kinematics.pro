#-------------------------------------------------
#
# Project created by QtCreator 2012-04-23T22:07:19
#
#-------------------------------------------------

QT       += core gui
QT       += opengl

TARGET = inverse_kinematics
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    box.cpp \
    glwidget.cpp \
    camera.cpp \
    kinematic_chain.cpp \
    kchain_link.cpp \
    joint_choise.cpp \
    redact_joint.cpp

HEADERS  += mainwindow.h \
    box.h \
    glwidget.h \
    camera.h \
    coord_axis.h \
    sphere.h \
    kinematic_chain.h \
    kchain_link.h \
    prism.h \
    cylinder.h \
    ball_joint.h \
    hinge_joint.h \
    abstract_joint.h \
    hinge_ra.h \
    joint_choise.h \
    redact_joint.h

FORMS    += mainwindow.ui \
    joint_choise.ui \
    redact_joint.ui

OTHER_FILES += \
    frag_shader.frag \
    vert_shader.vert

RESOURCES += \
    resources.qrc


