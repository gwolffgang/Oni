#include "oni.h"

Oni *game;

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setOrganizationName("Wolffgang");
    app.setApplicationName("Oni");
    app.setApplicationDisplayName("Oni");
    app.setApplicationVersion("1.0");
    app.setWindowIcon(QIcon(":/pics/Oni.icns"));

    game = new Oni;
    if (game->getMatch()->getTurns()->size() > 0)
        game->getWindow()->newGame(game->getMatch()->getTurns()->last());
    else
        game->getWindow()->newGame();
    return app.exec();
}
