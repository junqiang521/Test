#ifndef PCIE_SET_SCRIPT_H
#define PCIE_SET_SCRIPT_H

#include <QDialog>

namespace Ui {
class pcie_set_script;
}

class pcie_set_script : public QDialog
{
    Q_OBJECT

public:
    explicit pcie_set_script(QWidget *parent = 0);
    ~pcie_set_script();

private:
    Ui::pcie_set_script *ui;
};

#endif // PCIE_SET_SCRIPT_H
