#ifndef RPGCREATOR_H
#define RPGCREATOR_H

#include <QMainWindow>
#include "../common.h"

namespace Ui {
class RpgCreator;
}

class RpgCreator : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit RpgCreator(QWidget *parent = 0);
    ~RpgCreator();
    
private:
    Ui::RpgCreator *ui;
};

#endif // RPGCREATOR_H
