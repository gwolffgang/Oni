#include "mainwindow.h"
#include "oni.h"
#include "ui_mainwindow.h"

extern Oni *game;

int myrandom(int i) { return std::rand()%i; }

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    // presettings
    borderX = 10;
    borderY = 10;
    changeLayout(1.0);
    setWindowTitle("Oni");

    // scene setup
    scene = new QGraphicsScene(this);
    scene->setBackgroundBrush(QBrush(QImage(":/pics/wood.svg")));
    scene->setSceneRect(0, 0, windowWidth, windowHeight);
    fieldHeight = ((scene->sceneRect().height() - 2*borderY) / 5);
    ui->view->setScene(scene);
    ui->view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setViewSize(windowWidth+5, windowHeight+5);

    connect(ui->actionNew, SIGNAL(triggered()), this, SLOT(game->window->newGame();));
    connect(ui->actionTinyLayout,SIGNAL(triggered),this, SLOT(game->window->changeLayout(0.25, true);));
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::changeLayout(double factor, bool resize) {
    screen = QGuiApplication::primaryScreen();
    QRect desktop = screen->availableGeometry();
    windowHeight = desktop.height() * factor;
    windowWidth = 3*borderX + windowHeight + 2*((windowHeight - 4*borderY) / 3);
    while (windowWidth > desktop.width()-4) {
        windowHeight += -1;
        windowWidth = 3*borderX + windowHeight + 2*((windowHeight - 4*borderY) / 3);
    }
    setFixedSize(windowWidth+5, windowHeight+5);
    if (resize) resizeWindow();
}

void MainWindow::drawBoard(bool redraw) {
    // drawing the board
    QList<Field*> fieldsRow;
    for (int row = 0; row < game->getRows(); row++) {
        fieldsRow.clear();
        for (int col = 0; col < game->getCols(); col++) {
            // create field
            Field *field = new Field(fieldHeight);
            field->setRow(row);
            field->setCol(col);
            field->setPos(borderX + fieldHeight * col, borderY + fieldHeight * (4-row));
            // add piece to field
            if (game->getPieces()) {
                int pieceNumber = field->identifyPiece();
                if (pieceNumber != -1) {
                    if (!redraw) field->linkPiece(game->getPieces()->at(pieceNumber));
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
    double sizeY = (scene->height() - 4*borderY) / 3;
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
            double posX = scene->height() + number * sizeX + (number + 1) * borderX;
            double posY = ((-1.5 * player + 2.5) * player + 1) * sizeY + ((-1.5 * player + 2.5) * player + 2) * borderY;
            slot->setPos(posX, posY);
            slot->addCard(intArray[elem++], slot->rect().width(), slot->rect().height(), player);
            scene->addItem(slot);
            slotsRow.append(slot);
        }
        game->getSlotsGrid()->append(slotsRow);
    }
}

void MainWindow::redrawWindow() {
    //scene->clear();
    //drawBoard(true);
    //drawCardSlots();
}

void MainWindow::resizeWindow()
{
    setFixedSize(windowWidth+5, windowHeight+5);
    scene->setSceneRect(0, 0, windowWidth, windowHeight);
    setViewSize(windowWidth+5, windowHeight+5);
}

void MainWindow::setViewSize(double width, double height) {
    ui->view->setFixedSize(width, height);
}

void MainWindow::start()
{
    drawBoard();
    drawCardSlots();
}

void MainWindow::loadGame() {

}

void MainWindow::newGame() {
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
        game->getPieces()->append(piece);
    }
    // start the game
    game->getWindow()->start();
}

void MainWindow::saveGame() {

}

void MainWindow::saveGameAs() {

}
