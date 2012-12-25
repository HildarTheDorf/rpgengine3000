#include "rpgcreator.h"
#include "ui_rpgcreator.h"

RpgCreator::RpgCreator(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RpgCreator)
{
    ui->setupUi(this);
}

RpgCreator::~RpgCreator()
{
    delete ui;
}
