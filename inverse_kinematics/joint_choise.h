#ifndef JOINT_CHOISE_H
#define JOINT_CHOISE_H

#include <QDialog>
#include <kinematic_chain.h>
#include <scene.h>

namespace Ui {
class joint_choise;
}

class joint_choise : public QDialog
{
    Q_OBJECT
    
public:
    explicit joint_choise(QWidget *parent = 0);
    ~joint_choise();
    void set_kc(kinematic_chain* nkc,int ni);
    void set_scene(Scene *sc);
signals:
    void set_max_link();
private slots:
    void on_ball_clicked();

    void on_hinge1_clicked();

    void on_hinge2_clicked();

private:
    Ui::joint_choise *ui;
    int i;
    kinematic_chain *kc;
    Scene *scene;
};

#endif // JOINT_CHOISE_H
