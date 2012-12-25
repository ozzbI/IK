#ifndef REDACT_JOINT_H
#define REDACT_JOINT_H

#include <QDialog>
#include <kinematic_chain.h>
namespace Ui {
class redact_joint;
}

class redact_joint : public QDialog
{
    Q_OBJECT
    
public:
    explicit redact_joint(QWidget *parent = 0);
    ~redact_joint();
    void set_kc(kinematic_chain* nkc,int ni);
    
private slots:
    void on_Ok_but_clicked();

    void on_Canc_but_clicked();

private:
    Ui::redact_joint *ui;
    int i;
    kinematic_chain* kc;
};

#endif // REDACT_JOINT_H
