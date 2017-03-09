#include "mainwindow.h"
#include "oni.h"
#include "ui_mainwindow.h"

extern Oni *game;

int myrandom(int i) { return std::rand()%i; }

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    // QMainWindow::showFullScreen();       // FullScreen

    // scene setup
    scene = new QGraphicsScene(this);
    ui->view->setScene(scene);
    ui->view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::setViewSize(double width, double height)
{
    ui->view->setFixedSize(width, height);
}

void MainWindow::drawBoard() {
    // drawing the board
    QList<Field*> fieldsRow;
    for (int row = 0; row < game->getRows(); row++) {
        fieldsRow.clear();
        for (int col = 0; col < game->getCols(); col++) {
            // create field
            Field *field = new Field(game->getFieldHeight());
            field->setRow(row);
            field->setCol(col);
            field->setPos(game->getBorderX() + game->getFieldHeight() * col, game->getBorderY() + game->getFieldHeight() * (4-row));
            // add piece to field
            if (game->getPieces()) {
                int pieceNumber = field->identifyPiece();
                if (pieceNumber != -1) {
                    field->linkPiece(game->getPieces()->at(pieceNumber));
                    field->getPiece()->drawPiece();
                }
            }
            // add field to scene
            scene->addItem(field);
            // add field to board
            fieldsRow.append(field);
        }
        game->getBoard()->append(fieldsRow);
    }
}

void MainWindow::drawCardSlots() {
    // scaling the cardslots to windowsize
    double sizeY = (scene->height() - 4*game->getBorderY()) / 3;
    double sizeX = sizeY;

    // determine the cards
    int intArray[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    srand(unsigned(time(NULL)));
    std::random_shuffle(&intArray[0], &intArray[16], myrandom);

    // drawing the slotsGrid
    QList<CardSlot*> slotsRow;
    int elem = 0;
    for (int player = 0; player < 3; player++) {
        slotsRow.clear();
        int maxSlots = game->getCardsPerPlayer();
        if (player == 0) maxSlots = game->getNeutralCardsPerPlayer();
        for (int number = 0; number < maxSlots; number++) {
            CardSlot *slot = new CardSlot(sizeY);
            slot->setOwner(player);
            double posX = scene->height() + number * sizeX + (number + 1) * game->getBorderX();
            double posY = ((-1.5 * player + 2.5) * player + 1) * sizeY + ((-1.5 * player + 2.5) * player + 2) * game->getBorderY();
            slot->setPos(posX, posY);
            slot->addCard(intArray[elem++], slot->rect().width(), slot->rect().height(), player);
            scene->addItem(slot);
            slotsRow.append(slot);
        }
        game->getSlotsGrid()->append(slotsRow);
    }
}
