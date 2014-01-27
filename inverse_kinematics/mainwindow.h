#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "glwidget.h"
#include "joint_choise.h"
#include "addboxdialog.h"
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
    AddBoxDialog* addBoxDialog;
    QDoubleValidator* start_param_validator;
    bool ctrl_pressed;

    QMenu *mnFile;
    QMenu *mnSceneObjects;
    QMenu *mnGraphic;
    QMenu *mnObstaclesAvoiding;
    QMenu *mnPathFinding;
    QMenu *mnChainOptions;

    QAction *ssaoAct;
    QAction *shadowsAct;
    QAction *edgesAct;

    QAction *openChainAct;
    QAction *saveChainAct;
    QAction *openSceneAct;
    QAction *saveSceneAct;
    QAction *exitAct;

    QAction *addBoxAct;
    QAction *deleteBoxAct;
    QAction *redactSceneAct;
    QAction *selectMoveAct;

    QAction *geometryRepulsion;
    QAction *targetShift;

    QAction *findPathToTarget;
    QAction *showPathToTarget;
    QAction *moveEffectorToTarget;

    QAction *forceNormAct;
    QAction *moveMaxForce;

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
    void on_open_file_but_clicked();
    void on_save_scene_to_file_but_clicked();
    void on_open_scene_file_but_clicked();
    void set_max_link();
    void on_vel_slider_actionTriggered(int action);
    void on_Target_Activated_checkbox_clicked(bool checked);
    void on_elastic_joints_checkbox_clicked(bool checked);
    void on_Target_Activated_checkbox_stateChanged(int arg1);
    void on_elastic_joints_checkbox_stateChanged(int arg1);
    void on_ssao_changed(bool state);
    void on_shadows_changed(bool state);    
    void on_draw_edges_changed(bool state);
    void create_add_box_dialog();
    void on_redact_scene();
    void on_select_move(bool state);
    void delete_box();
    void set_force_normalizing(bool state);
    void set_max_force_rotation(bool state);

    //menu
    void createActions();
    void createMenus();
};

#endif // MAINWINDOW_H
