#include "oni.h"
//#include "table.h"

extern Oni *game;

Oni::Oni() {
    // window settings
    borderX = 10;
    borderY = 10;
    windowHeight = 700;
    windowWidth = 3*borderX + windowHeight + 2*((windowHeight - 4*borderY) / 3);
    window = new MainWindow;
    window->show();
    window->setWindowTitle("Oni");
    window->setFixedSize(windowWidth+5, windowHeight+5);
    window->getScene()->setSceneRect(0, 0, windowWidth, windowHeight);
    window->getScene()->setBackgroundBrush(QBrush(QImage(":/pics/wood.svg")));
    window->setViewSize(windowWidth+5, windowHeight+5);

    // board settings
    board = new QList<QList<Field*>>;
    rows = 5;
    cols = 5;
    flipBoard = false;
    // scaling the board to windowsize
    fieldHeight = ((window->getScene()->sceneRect().height() - 2*borderY) / rows);

    // card settings
    slotsGrid = new QList<QList<CardSlot*>>;
    cardsPerPlayer = 2;
    neutralCardsPerPlayer = 1;

    // game settings
    gameResult = 0;
    firstPlayersTurn = true;
    pickedUpPiece = NULL;
    pieceToReposition = NULL;

    // initiate pieces lists
    pieces = new QList<Piece*>;
    capturedPieces = new QList<Piece*>;
}

void Oni::mouseMoveEvent(QMouseEvent *event) {
    // picked up piece follows the mousecursor
    if (pickedUpPiece != NULL)
        pickedUpPiece->setPos(event->pos() - QPointF(pickedUpPiece->pixmap().height()/2, pickedUpPiece->pixmap().height()/2));

    //QGraphicsView::mouseMoveEvent(event);
}

void Oni::setPickedUpPiece(Piece *newPiece) {
    if (newPiece == NULL)
        pickedUpPiece = NULL;
    else {
        pickedUpPiece = new Piece;
        pickedUpPiece->setCol(newPiece->getCol());
        pickedUpPiece->setRow(newPiece->getRow());
        pickedUpPiece->setType(newPiece->getType());
        pickedUpPiece->drawPiece();
        QPointF piecePos = game->getBoard()->at(newPiece->getRow()).at(newPiece->getCol())->pos() ;
        pickedUpPiece->setPos(piecePos + QPointF(borderX, borderY));
        window->getScene()->addItem(pickedUpPiece);
    }
}

void Oni::start() {
    window->drawBoard();
    window->drawCardSlots();
}

void Oni::loadGame() {

}

void Oni::newGame() {
    // name pieces string
    QString setUpString = "Sa1 Sb1 Mc1 Sd1 Se1 sa5 sb5 mc5 sd5 se5";

    // seperate and analyse pieces string
    QStringList elem = setUpString.split(" ");
    for (int i = 0; i < elem.size(); i++) {
        Piece *piece = new Piece;
        piece->setType(elem[i].at(0).unicode());
        char c = tolower(elem[i].at(1).unicode());
        switch (c) {
        case 'a':
            piece->setCol(0);
            break;
        case 'b':
            piece->setCol(1);
            break;
        case 'c':
            piece->setCol(2);
            break;
        case 'd':
            piece->setCol(3);
            break;
        case 'e':
            piece->setCol(4);
        }
        piece->setRow(elem[i].at(2).digitValue() - 1);

        // add piece to pieces list
        pieces->append(piece);
    }
    // start the game
    start();
}

void Oni::saveGame() {

}

void Oni::saveGameAs() {

}
