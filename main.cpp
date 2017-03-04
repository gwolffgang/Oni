#include <QApplication>
#include "oni.h"

Oni *game;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    game = new Oni;
    game->show();
    game->new_game();
    return app.exec();
}
