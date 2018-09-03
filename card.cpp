#include <iostream>
#include "card.h"
#include "oni.h"

extern Oni *game;

    // randomizer for playing cards
int randomizer(const int max) {
    static bool seeded = false;
    if(!seeded) {
        srand(unsigned(time(nullptr)));
        seeded = true;
    }
    return(rand() % max +1);
}

Card::Card(QGraphicsItem *parent) : QGraphicsPixmapItem(parent), name(""), id(0) {
}

void Card::setCardValues(int cardID) {
    // determinate card
    id = cardID;
    if (id == 0)
        id = randomizer(16);
    // movement options
    switch (id) {
    case 1:
        name = "BOAR";
        choice[0][0] = 0;   choice[0][1] = 1;
        choice[1][0] = -1;  choice[1][1] = 0;
        choice[2][0] = 1;   choice[2][1] = 0;
        choice[3][0] = 0;   choice[3][1] = 0;
        break;
    case 2:
        name = "COBRA";
        choice[0][0] = 1;   choice[0][1] = 1;
        choice[1][0] = -1;  choice[1][1] = 0;
        choice[2][0] = 1;   choice[2][1] = -1;
        choice[3][0] = 0;   choice[3][1] = 0;
        break;
    case 3:
        name = "CRAB";
        choice[0][0] = 0;   choice[0][1] = 1;
        choice[1][0] = -2;  choice[1][1] = 0;
        choice[2][0] = 2;   choice[2][1] = 0;
        choice[3][0] = 0;   choice[3][1] = 0;
        break;
    case 4:
        name = "CRANE";
        choice[0][0] = 0;   choice[0][1] = 1;
        choice[1][0] = -1;  choice[1][1] = -1;
        choice[2][0] = 1;   choice[2][1] = -1;
        choice[3][0] = 0;   choice[3][1] = 0;
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
        choice[3][0] = 0;   choice[3][1] = 0;
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
        choice[3][0] = 0;   choice[3][1] = 0;
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
        choice[3][0] = 0;   choice[3][1] = 0;
        break;
    case 11:
        name = "MANTIS";
        choice[0][0] = -1;  choice[0][1] = 1;
        choice[1][0] = 1;   choice[1][1] = 1;
        choice[2][0] = 0;   choice[2][1] = -1;
        choice[3][0] = 0;   choice[3][1] = 0;
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
        choice[3][0] = 0;   choice[3][1] = 0;
        break;
    case 14:
        name = "RABBIT";
        choice[0][0] = 1;   choice[0][1] = 1;
        choice[1][0] = 2;   choice[1][1] = 0;
        choice[2][0] = -1;  choice[2][1] = -1;
        choice[3][0] = 0;   choice[3][1] = 0;
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
        choice[2][0] = 0;   choice[2][1] = 0;
        choice[3][0] = 0;   choice[3][1] = 0;
        break;
    case 17: // "Sensei's Path" Expansion
        name = "BEAR";
        choice[0][0] = -1;  choice[0][1] = 1;
        choice[1][0] = 0;   choice[1][1] = 1;
        choice[2][0] = 1;   choice[2][1] = -1;
        choice[3][0] = 0;   choice[3][1] = 0;
        break;
    case 18: // "Sensei's Path" Expansion
        name = "DOG";
        choice[0][0] = -1;  choice[0][1] = 1;
        choice[1][0] = -1;  choice[1][1] = 0;
        choice[2][0] = -1;  choice[2][1] = -1;
        choice[3][0] = 0;   choice[3][1] = 0;
        break;
    case 19: // "Sensei's Path" Expansion
        name = "FOX";
        choice[0][0] = 1;  choice[0][1] = 1;
        choice[1][0] = 1;   choice[1][1] = 0;
        choice[2][0] = 1;   choice[2][1] = -1;
        choice[3][0] = 0;   choice[3][1] = 0;
        break;
    case 20: // "Sensei's Path" Expansion
        name = "GIRAFFE";
        choice[0][0] = -2;  choice[0][1] = 1;
        choice[1][0] = 2;   choice[1][1] = 1;
        choice[2][0] = 0;   choice[2][1] = -1;
        choice[3][0] = 0;   choice[3][1] = 0;
        break;
    case 21: // "Sensei's Path" Expansion
        name = "IGUANA";
        choice[0][0] = -2;  choice[0][1] = 1;
        choice[1][0] = 0;   choice[1][1] = 1;
        choice[2][0] = 1;   choice[2][1] = -1;
        choice[3][0] = 0;   choice[3][1] = 0;
        break;
    case 22: // "Sensei's Path" Expansion
        name = "KIRIN";
        choice[0][0] = -1;  choice[0][1] = 2;
        choice[1][0] = 1;   choice[1][1] = 2;
        choice[2][0] = 0;   choice[2][1] = -2;
        choice[3][0] = 0;   choice[3][1] = 0;
        break;
    case 23: // "Sensei's Path" Expansion
        name = "MOUSE";
        choice[0][0] = 0;   choice[0][1] = 1;
        choice[1][0] = 1;   choice[1][1] = 0;
        choice[2][0] = -1;  choice[2][1] = -1;
        choice[3][0] = 0;   choice[3][1] = 0;
        break;
    case 24: // "Sensei's Path" Expansion
        name = "OTTER";
        choice[0][0] = -1;  choice[0][1] = 1;
        choice[1][0] = 2;   choice[1][1] = 0;
        choice[2][0] = 1;   choice[2][1] = -1;
        choice[3][0] = 0;   choice[3][1] = 0;
        break;
    case 25: // "Sensei's Path" Expansion
        name = "PANDA";
        choice[0][0] = 0;   choice[0][1] = 1;
        choice[1][0] = 1;   choice[1][1] = 1;
        choice[2][0] = -1;  choice[2][1] = -1;
        choice[3][0] = 0;   choice[3][1] = 0;
        break;
    case 26: // "Sensei's Path" Expansion
        name = "PHOENIX";
        choice[0][0] = -1;  choice[0][1] = 1;
        choice[1][0] = 1;   choice[1][1] = 1;
        choice[2][0] = -2;  choice[2][1] = 0;
        choice[3][0] = 2;   choice[3][1] = 0;
        break;
    case 27: // "Sensei's Path" Expansion
        name = "RAT";
        choice[0][0] = 0;   choice[0][1] = 1;
        choice[1][0] = -1;  choice[1][1] = 0;
        choice[2][0] = 1;   choice[2][1] = -1;
        choice[3][0] = 0;   choice[3][1] = 0;
        break;
    case 28: // "Sensei's Path" Expansion
        name = "SABLE";
        choice[0][0] = 1;   choice[0][1] = 1;
        choice[1][0] = -2;  choice[1][1] = 0;
        choice[2][0] = -1;  choice[2][1] = -1;
        choice[3][0] = 0;   choice[3][1] = 0;
        break;
    case 29: // "Sensei's Path" Expansion
        name = "SEA SNAKE";
        choice[0][0] = 0;   choice[0][1] = 1;
        choice[1][0] = 2;   choice[1][1] = 0;
        choice[2][0] = -1;  choice[2][1] = -1;
        choice[3][0] = 0;   choice[3][1] = 0;
        break;
    case 30: // "Sensei's Path" Expansion
        name = "TANUKI";
        choice[0][0] = 0;   choice[0][1] = 1;
        choice[1][0] = 2;   choice[1][1] = 1;
        choice[2][0] = -1;  choice[2][1] = -1;
        choice[3][0] = 0;   choice[3][1] = 0;
        break;
    case 31: // "Sensei's Path" Expansion
        name = "TURTLE";
        choice[0][0] = -2;  choice[0][1] = 0;
        choice[1][0] = 2;   choice[1][1] = 0;
        choice[2][0] = 1;   choice[2][1] = -1;
        choice[3][0] = -1;  choice[3][1] = -1;
        break;
    case 32: // "Sensei's Path" Expansion
        name = "VIPER";
        choice[0][0] = 0;   choice[0][1] = 1;
        choice[1][0] = -2;  choice[1][1] = 0;
        choice[2][0] = 1;   choice[2][1] = -1;
        choice[3][0] = 0;   choice[3][1] = 0;
        break;
    case 33: // "Goat and Sheep" Expansion
        name = "GOAT";
        choice[0][0] = 1;   choice[0][1] = 1;
        choice[1][0] = -1;  choice[1][1] = 0;
        choice[2][0] = 0;   choice[2][1] = -1;
        choice[3][0] = 0;   choice[3][1] = 0;
        break;
    case 34: // "Goat and Sheep" Expansion
        name = "SHEEP";
        choice[0][0] = -1;  choice[0][1] = 1;
        choice[1][0] = 1;   choice[1][1] = 0;
        choice[2][0] = 0;   choice[2][1] = -1;
        choice[3][0] = 0;   choice[3][1] = 0;
    }
}

void Card::drawCard(int player) {
    double borderX = game->getWindow()->getBorderX();
    double borderY = game->getWindow()->getBorderY();
    double slotHeight = game->getWindow()->getSlotHeight();
    double slotWidth = game->getWindow()->getSlotWidth();
    bool firstPlayersTurn = game->getFirstPlayersTurn();
    bool flippedBoard = game->getFlippedBoard();

    // drawing the card
    double width = slotWidth - 2*borderX -1;
    double height = (slotHeight - 2*borderY)*5/6 - 5/6;
    card = QPixmap(static_cast<int>(width+1), static_cast<int>(height*6/5 +1));
    setPos(borderX, borderY);
    QBrush lightGrayBrush = QBrush(Qt::lightGray, Qt::SolidPattern);
    QBrush grayBrush = QBrush(Qt::white, Qt::SolidPattern);
    QPen blackPen = QPen(Qt::black);
    QFont namesFont = QFont("Amburegul");
    namesFont.setPixelSize(static_cast<int>(height*0.1));
    QPainter painter(&card);
        painter.setFont(namesFont);
        painter.setBrush(lightGrayBrush);
        painter.setPen(blackPen);
        painter.drawRect(0, 0, static_cast<int>(width), static_cast<int>(height));
        painter.setBrush(Qt::NoBrush);
        painter.drawRect(static_cast<int>(0.2*width), 0, static_cast<int>(0.6*width), static_cast<int>(height));
        painter.drawRect(0, static_cast<int>(0.2*height), static_cast<int>(width), static_cast<int>(0.6*height));
        painter.drawRect(static_cast<int>(0.4*width), 0, static_cast<int>(0.2*width), static_cast<int>(height));
        painter.drawRect(0, static_cast<int>(0.4*height), static_cast<int>(width), static_cast<int>(0.2*height));
        painter.setBrush(grayBrush);
        painter.drawRect(0, static_cast<int>(height), static_cast<int>(width), static_cast<int>(0.2*height));
        painter.drawText(QRectF(0, 1.01*height, width, 0.2*height), Qt::AlignCenter, this->getName());
        drawYinYang(&painter, choice[0][0], choice[0][1]);
        drawYinYang(&painter, choice[1][0], choice[1][1]);
        drawYinYang(&painter, choice[2][0], choice[2][1]);
        drawYinYang(&painter, choice[3][0], choice[3][1]);
        drawYinYang(&painter, 0, 0);
    painter.end();
    setPixmap(card);

    // check if card has to be flipped
    setTransformOriginPoint(boundingRect().width() / 2, boundingRect().height() / 2); // middle of card
    if (!flippedBoard) {
        if ((firstPlayersTurn && player == 2) || (!firstPlayersTurn && (player == 0 || player == 2)))
            setRotation(180);
        else
            setRotation(0);
    } else {
        if ((!firstPlayersTurn && player == 1) || (firstPlayersTurn && (player == 0 || player == 1)))
            setRotation(180);
        else
            setRotation(0);
    }
}

void Card::drawYinYang(QPainter *painter, int x, int y) {
    double borderX = game->getWindow()->getBorderX();
    double borderY = game->getWindow()->getBorderY();
    double slotHeight = game->getWindow()->getSlotHeight();
    double slotWidth = game->getWindow()->getSlotWidth();
    double width = slotWidth - 2*borderX -1;
    double height = (slotHeight - 2*borderY)*5/6 - 5/6;
    QBrush colorBrush = QBrush(Qt::darkRed, Qt::SolidPattern);
    QPen colorPen = QPen(Qt::darkRed);
    if (x == 0 && y == 0) {
        colorBrush.setColor(Qt::black);
        colorPen.setColor(Qt::black);
    }
    QPen noPen = QPen(Qt::NoPen);
    QBrush whiteBrush = QBrush(Qt::white, Qt::SolidPattern);
    QPen whitePen = QPen(Qt::white);

    // drawing Yin Yang
    painter->setPen(noPen);
    painter->setBrush(whiteBrush);
    painter->drawEllipse(static_cast<int>(width*(x*0.2 + 0.4)+2), static_cast<int>(height*(-y*0.2 + 0.4)+2), static_cast<int>(0.2*width-4), static_cast<int>(0.2*height-4));
    painter->setBrush(colorBrush);
    painter->drawPie(QRectF(width*(x*0.2 + 0.4)+2, height*(-y*0.2 + 0.4)+2, 0.2*width-4, 0.2*height-4), 270*16, 180*16);
    painter->setPen(whitePen);
    painter->setBrush(whiteBrush);
    painter->drawPie(QRectF(width*(x*0.2 + 0.45), height*(-y*0.2 + 0.4)+2, 0.1*width, 0.1*height-4), 270*16, 180*16);
    painter->setPen(colorPen);
    painter->setBrush(colorBrush);
    painter->drawPie(QRectF(width*(x*0.2 + 0.45), height*(-y*0.2 + 0.5)-1, 0.1*width, 0.1*height-1), 90*16, 180*16);
    painter->drawEllipse(static_cast<int>(width*(x*0.2 + 0.48)), static_cast<int>(height*(-y*0.2 + 0.43)), static_cast<int>(0.04*width), static_cast<int>(0.04*height));
    painter->setPen(whitePen);
    painter->setBrush(whiteBrush);
    painter->drawEllipse(static_cast<int>(width*(x*0.2 + 0.48)), static_cast<int>(height*(-y*0.2 + 0.52)), static_cast<int>(0.04*width), static_cast<int>(0.04*height));
}
