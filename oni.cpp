#include "oni.h"
#include "table.h"
#include <QBrush>
#include <QImage>

Oni::Oni(QWidget *parent) {
    // presettings
    rows = 5;
    cols = 5;
    borderX = borderY = 10;
    gameResult = 0;
    studentsLeft[0] = studentsLeft[1] = 4;
    firstPlayersTurn = true;
    int height = 700, width = 1200;

    // set up the screen
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFixedSize(width,height);

    // set up the scene
    scene = new QGraphicsScene;
    scene->setSceneRect(0, 0, width, height);
    scene->setBackgroundBrush(QBrush(QImage(":/pics/wood.svg")));
    setScene(scene);
}

void Oni::mouseMoveEvent(QMouseEvent *event) {
    // picked up piece follows the mousecursor
    if (pickedUpPiece)
        pickedUpPiece->setPos(event->pos()-QPointF(pickedUpPiece->pixmap().height()/2, pickedUpPiece->pixmap().height()/2));

    QGraphicsView::mouseMoveEvent(event);
}

int Oni::getStudentsLeft(int player) {
    if (player == 1) return studentsLeft[0];
    if (player == 2) return studentsLeft[1];
    return -1;
}

void Oni::start() {
    // set up table
    table = new Table;
    table->drawBoard();
    table->drawCardSlots(borderX, borderY);
}

void Oni::load_game() {

}

void Oni::new_game() {
    // name pieces string
    QString setUpString = "Sa1 Sb1 Mc1 Sd1 Se1 sa5 sb5 mc5 sd5 se5";

    // initiate pieces list
    pieces = new QList<Piece*>;

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

void Oni::save_game() {

}
