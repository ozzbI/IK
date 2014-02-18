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
    octree.cpp \
    cubemapfbo.cpp \
    addboxdialog.cpp \
    utils.cpp

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
    octree.h \
    cubemapfbo.h \
    addboxdialog.h \
    utils.h

FORMS    += mainwindow.ui \
    joint_choise.ui \
    redact_joint.ui \
    addboxdialog.ui

OTHER_FILES += \
    ssao_build_shaded_texture.vert \
    ssao_build_shaded_texture.frag \
    ssao_build_normal_buffer.frag \
    ssao_build_pos_buffer.frag \
    fullscreen_texture.vert \
    fullscreen_texture.frag \
    ssao_build_normal_buffer.vert \
    ssao_build_pos_buffer.vert \
    fullscreen_texture_2D.frag \
    combine_textures.frag \
    combine_textures.vert \
    ssao_blur.frag \
    ssao_blur.vert \
    main_lightning_frag_shader.frag \
    main_lightning_frag_shader.vert \
    upscale_interpolation.frag \
    upscale_interpolation.vert \
    only_depth.frag \
    build_shadows.frag \
    build_shadows.vert \
    build_global_pos_buffer.vert \
    line.frag \
    line.vert

RESOURCES += \
    resources.qrc
