#include <QApplication>
#include "oni.h"

Oni *game;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    game = new Oni;
    game->newGame();
    return app.exec();
}
