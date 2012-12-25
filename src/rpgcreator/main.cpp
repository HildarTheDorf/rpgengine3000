#include <QApplication>
#include "rpgcreator.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    RpgCreator window;
    window.show();

    return app.exec();
}
