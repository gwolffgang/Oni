#include <QApplication>
#include "oni.h"

oni *game;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    game = new oni;
    game->show();
    game->start();
    return app.exec();
}
