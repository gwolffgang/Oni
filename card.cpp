#include "card.h"
#include "oni.h"

extern Oni *game;

int randomizer(const int max) {
    static bool seeded = false;
    if(!seeded) {
        srand(unsigned(time(NULL)));
        seeded = true;
    }
    return(rand() % max +1);
}

Card::Card(QGraphicsItem *parent) : QGraphicsPixmapItem(parent) {}

void Card::setCardValues(int cardID, double slotWidth, double slotHeight) {
    // determinate card
    id = cardID;
    if (id == 0) id = randomizer(16);
    switch (id) {
    case 1:
        name = "BOAR";
        choice[0][0] = 0;   choice[0][1] = 1;
            choice[1][0] = -1;  choice[1][1] = 0;
            choice[2][0] = 1;   choice[2][1] = 0;
            break;
        case 2:
            name = "COBRA";
            choice[0][0] = 1;   choice[0][1] = 1;
            choice[1][0] = -1;  choice[1][1] = 0;
            choice[2][0] = 1;   choice[2][1] = -1;
            break;
        case 3:
            name = "CRAB";
            choice[0][0] = 0;   choice[0][1] = 1;
            choice[1][0] = -2;  choice[1][1] = 0;
            choice[2][0] = 2;   choice[2][1] = 0;
            break;
        case 4:
            name = "CRANE";
            choice[0][0] = 0;   choice[0][1] = 1;
            choice[1][0] = -1;  choice[1][1] = -1;
            choice[2][0] = 1;   choice[2][1] = -1;
            break;
        case 5:
            name = "DRAGON";
            choice[0][0] = -2;  choice[0][1] = 1;
            choice[1][0] = 2;   choice[1][1] = 1;
            choice[2][0] = -1;  choice[2][1] = -1;
            choice[3][0] = 1;   choice[3][1] = -1;
            break;
        case 6:
            name = "EEL";
            choice[0][0] = -1;  choice[0][1] = 1;
            choice[1][0] = 1;   choice[1][1] = 0;
            choice[2][0] = -1;  choice[2][1] = -1;
            break;
        case 7:
            name = "ELEPHANT";
            choice[0][0] = -1;  choice[0][1] = 1;
            choice[1][0] = 1;   choice[1][1] = 1;
            choice[2][0] = -1;  choice[2][1] = 0;
            choice[3][0] = 1;   choice[3][1] = 0;
            break;
        case 8:
            name = "FROG";
            choice[0][0] = -1;  choice[0][1] = 1;
            choice[1][0] = -2;  choice[1][1] = 0;
            choice[2][0] = 1;   choice[2][1] = -1;
            break;
        case 9:
            name = "GOOSE";
            choice[0][0] = -1;  choice[0][1] = 1;
            choice[1][0] = -1;  choice[1][1] = 0;
            choice[2][0] = 1;   choice[2][1] = 0;
            choice[3][0] = 1;   choice[3][1] = -1;
            break;
        case 10:
            name = "HORSE";
            choice[0][0] = 0;   choice[0][1] = 1;
            choice[1][0] = -1;  choice[1][1] = 0;
            choice[2][0] = 0;   choice[2][1] = -1;
            break;
        case 11:
            name = "MANTIS";
            choice[0][0] = -1;  choice[0][1] = 1;
            choice[1][0] = 1;   choice[1][1] = 1;
            choice[2][0] = 0;   choice[2][1] = -1;
            break;
        case 12:
            name = "MONKEY";
            choice[0][0] = -1;  choice[0][1] = 1;
            choice[1][0] = 1;   choice[1][1] = 1;
            choice[2][0] = -1;  choice[2][1] = -1;
            choice[3][0] = 1;   choice[3][1] = -1;
            break;
        case 13:
            name = "OX";
            choice[0][0] = 0;   choice[0][1] = 1;
            choice[1][0] = 1;   choice[1][1] = 0;
            choice[2][0] = 0;   choice[2][1] = -1;
            break;
        case 14:
            name = "RABBIT";
            choice[0][0] = 1;   choice[0][1] = 1;
            choice[1][0] = 2;   choice[1][1] = 0;
            choice[2][0] = -1;  choice[2][1] = -1;
            break;
        case 15:
            name = "ROOSTER";
            choice[0][0] = 1;   choice[0][1] = 1;
            choice[1][0] = -1;  choice[1][1] = 0;
            choice[2][0] = 1;   choice[2][1] = 0;
            choice[3][0] = -1;  choice[3][1] = -1;
            break;
        case 16:
            name = "TIGER";
            choice[0][0] = 0;   choice[0][1] = 2;
            choice[1][0] = 0;   choice[1][1] = -1;
    }
    // drawing the card
    QPixmap img(":/pics/cards/" + name + ".png");
    setPos(game->border_x, game->border_y);
    img = img.scaled(slotWidth - 2*game->border_x, slotHeight - 2*game->border_y);
    setPixmap(img);
}
