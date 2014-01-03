#-------------------------------------------------
#
# Project created by QtCreator 2012-04-23T22:07:19
#
#-------------------------------------------------

QT       += core gui
QT       += opengl

TARGET = inverse_kinematics
TEMPLATE = app

#QMAKE_CXXFLAGS_DEBUG += -pg
#QMAKE_LFLAGS_DEBUG += -pg

SOURCES += main.cpp\
        mainwindow.cpp \
    box.cpp \
    glwidget.cpp \
    camera.cpp \
    kinematic_chain.cpp \
    kchain_link.cpp \
    joint_choise.cpp \
    redact_joint.cpp \
    scene.cpp \
    figure.cpp \
    octree.cpp

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
    redact_joint.h \
    figure.h \
    polygon.h \
    scene.h \
    octree.h

FORMS    += mainwindow.ui \
    joint_choise.ui \
    redact_joint.ui

OTHER_FILES += \
    frag_shader.frag \
    vert_shader.vert \
    ssao_build_buffer.vert \
    ssao_build_buffer.frag \
    ssao_build_shaded_texture.vert \
    ssao_build_shaded_texture.frag \
    ssao_blur.frag

RESOURCES += \
    resources.qrc
