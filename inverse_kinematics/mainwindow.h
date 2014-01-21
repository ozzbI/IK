#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "glwidget.h"
#include <joint_choise.h>
#include <QFile>
#include <QFileDialog>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    double move_sens;
    GLWidget* glw;
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:

    Ui::MainWindow *ui;
    joint_choise* jc;
    QDoubleValidator* start_param_validator;
    bool ctrl_pressed;
    QMenu *mnFile;
    QMenu *mnSceneObjects;
    QMenu *mnGraphic;
    QAction *ssaoAct;
    QAction *shadowsAct;

signals:
    void rotate_link(int id,double yaw,double pitch,double roll);
    void set_link_rotation(int id,double yaw,double pitch,double roll);

private slots:
    void on_STOP_START_clicked();

    void on_RESTART_clicked();
    void set_move_sens(int new_move_sens);

    void max_links(int ml);

    void on_link_valueChanged(int arg1);
    void on_yaw_sl_valueChanged(int value);
    void on_pitch_sl_valueChanged(int value);
    void on_roll_sl_valueChanged(int value);
    void on_free_cam_but_clicked();
    void on_object_cam_but_clicked();
    void on_pushButton_2_clicked();
    void on_stop_but_clicked();
    void on_vel_slider_valueChanged(int value);
    void on_del_link_but_clicked();
    void on_add_link_but_clicked();
    void on_red_link_but_clicked();
    void on_save_to_file_but_clicked();
    void on_opne_file_but_clicked();
    void set_max_link();
    void on_vel_slider_actionTriggered(int action);
    void on_Target_Activated_checkbox_clicked(bool checked);
    void on_elastic_joints_checkbox_clicked(bool checked);
    void on_Target_Activated_checkbox_stateChanged(int arg1);
    void on_elastic_joints_checkbox_stateChanged(int arg1);
    void on_ssao_changed(bool state);
    void on_shadows_changed(bool state);

    //menu
    void createActions();
    void createMenus();
};

#endif // MAINWINDOW_H
