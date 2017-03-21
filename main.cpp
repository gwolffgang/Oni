#include "oni.h"

Oni *game;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    game = new Oni;
    game->getWindow()->newGame();
    return app.exec();
}
