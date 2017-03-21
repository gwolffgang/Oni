#include <QMessageBox>

#include "mainwindow.h"
#include "oni.h"
#include "ui_mainwindow.h"

extern Oni *game;

int myrandom(int i) { return std::rand()%i; }

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    scene = NULL;

    // presettings
    changeLayout(0.9);
    windowTitle = "Oni - new unsaved game";
    setWindowTitle(windowTitle);

    // scene setup
    scene = new QGraphicsScene(this);
    scene->setBackgroundBrush(QBrush(QImage(":/pics/paper.png")));
    scene->setSceneRect(0, 0, windowWidth, windowHeight);
    fieldHeight = ((scene->sceneRect().height() - 2*borderY) / 5);
    ui->view->setScene(scene);
    ui->view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setViewSize(windowWidth+5, windowHeight+5);

    // About dialog
    about = new AboutWindow(this);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::analyseSetupString(QString string) {
    QStringList elem = string.split(" ");
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
}

void MainWindow::changeLayout(double factor) {
    screen = QGuiApplication::primaryScreen();
    QRect desktop = screen->availableGeometry();
    borderX = 10 * factor;
    borderY = 10 * factor;
    windowHeight = desktop.height() * factor;
    windowWidth = 3*borderX + windowHeight + 2*((windowHeight - 4*borderY) / 3);
    while (windowWidth > desktop.width()-4) {
        windowHeight--;
        windowWidth = 3*borderX + windowHeight + 2*((windowHeight - 4*borderY) / 3);
    }
    setFixedSize(windowWidth, windowHeight);
    if (scene != NULL) {
        scene->setSceneRect(0, 0, windowWidth, windowHeight);
        setViewSize(windowWidth+4, windowHeight+4);
        fieldHeight = ((scene->sceneRect().height() - 2*borderY) / 5);
        prepareGame();
    }
}

void MainWindow::createBoard() {
    // creating the board
    game->getBoard()->clear();
    QList<Field*> fieldsRow;
    for (int row = 0; row < game->getRows(); row++) {
        fieldsRow.clear();
        for (int col = 0; col < game->getCols(); col++) {
            // create field
            Field *field = new Field;
            field->setRow(row);
            field->setCol(col);
            field->setPieceType(' ');
            field->setPos(borderX + fieldHeight * col, borderY + fieldHeight * (4-row));

            // add field to row
            fieldsRow.append(field);
        }
        // add row to board
        game->getBoard()->append(fieldsRow);
    }
}

void MainWindow::createCardSlots() {

}

void MainWindow::drawBoard() {
    // drawing the board
    Oni *test = game;
    for (int row = 0; row < game->getBoard()->size(); row++) {
        for (int col = 0; col < game->getBoard()->at(row).size(); col++) {
            Field *field = game->getBoard()->at(row).at(col);
            // add piece to field
            int pieceNumber = field->identifyPiece();
            if (pieceNumber != -1) {
                field->linkPiece(game->getPieces()->at(pieceNumber));
                field->getPiece()->drawPiece();
            }
            // add field to scene
            scene->addItem(field);
        }
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

QString MainWindow::generateSetupString() {
    QString saveString = "";
    for (int i = 0; i < game->getPieces()->size(); i++) {
        Piece *piece = game->getPieces()->at(i);
        if (saveString != "") saveString += " ";
        saveString += piece->getType();
        switch (game->getPieces()->at(i)->getCol()) {
        case 0:
            saveString += "a";
            break;
        case 1:
            saveString += "b";
            break;
        case 2:
            saveString += "c";
            break;
        case 3:
            saveString += "d";
            break;
        case 4:
            saveString += "e";
        }
        saveString += QString::number(game->getPieces()->at(i)->getRow()+1);
    }
    return saveString;
}

void MainWindow::loadGame() {
    //if (maybeSave()) {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open savegame"), "/current/saves", "Oni Savegames (*.oni)");
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(QDir::toNativeSeparators(fileName), file.errorString()));
                return;
            }
        QTextStream in(&file);
        #ifndef QT_NO_CURSOR
            QApplication::setOverrideCursor(Qt::WaitCursor);
        #endif
        QString newString = in.readAll();
        #ifndef QT_NO_CURSOR
            QApplication::restoreOverrideCursor();
        #endif

        game->setOpenGameFileName(fileName);
        QMessageBox::information(this, "Text", newString);
        newGame(newString);

        // set window title
        QStringList elem = fileName.split("/");
        QStringList name = elem.last().split(".");
        windowTitle = "Oni - " + name.first();
        setWindowTitle(windowTitle);
    }
}

void MainWindow::newGame(QString setupString) {
    // reset settings
    if (!game->getFirstPlayersTurn()) game->changePlayersTurn();
    if (game->getFlipBoard()) game->flipBoard();

    // reset lists
    if (game->getPieces()) game->getPieces()->clear();
    if (game->getCards()) game->getCards()->clear();

    // setup string
    if (setupString == "") setupString = "Sa1 Sb1 Mc1 Sd1 Se1 sa5 sb5 mc5 sd5 se5";
    analyseSetupString(setupString);

    // prepare the game
    prepareGame();
}

void MainWindow::positionNotation() {
    double size = (scene->height() - 4*borderY) / 3;
    double posX = scene->height() + size + 2*borderX;
    double posY = size + 2*borderY;
    ui->notation->setGeometry(posX, posY, size, size);
}

void MainWindow::prepareGame() {
   // if (game->getPieces()) unparentPieces();
    scene->clear();
    createBoard();
    createCardSlots();
    drawBoard();
    drawCardSlots();
    positionNotation();
}

bool MainWindow::saveGame(const QString &fileName) {
    QFile file(fileName);
        if (!file.open(QFile::WriteOnly | QFile::Text)) {
            QMessageBox::warning(this, tr("Application"),
                                 tr("Cannot write file %1:\n%2.")
                                 .arg(QDir::toNativeSeparators(fileName),
                                      file.errorString()));
            return false;
        }

        QTextStream out(&file);
    #ifndef QT_NO_CURSOR
        QApplication::setOverrideCursor(Qt::WaitCursor);
    #endif
        out << generateSetupString();
    #ifndef QT_NO_CURSOR
        QApplication::restoreOverrideCursor();
    #endif

    game->setOpenGameFileName(fileName);

    // set window title
    QStringList elem = fileName.split("/");
    QStringList name = elem.last().split(".");
    windowTitle = "Oni - " + name.first();
    setWindowTitle(windowTitle);
    return true;
}

bool MainWindow::saveGameAs() {
    QFileDialog dialog(this);
        dialog.setWindowModality(Qt::WindowModal);
        dialog.setAcceptMode(QFileDialog::AcceptSave);
        if (dialog.exec() != QDialog::Accepted)
            return false;
        return saveGame(dialog.selectedFiles().first());
}

void MainWindow::saveTurnInNotation() {
    ui->notation->addItem(generateSetupString());
}

/* void MainWindow::showPosition(QListWidgetItem *) {

} */

void MainWindow::unparentPieces() {
    for (int i = 0; i < game->getPieces()->size(); i++)
        game->getPieces()->at(i)->setParentItem(NULL);
}

void MainWindow::setViewSize(double width, double height) {
    ui->view->setFixedSize(width, height);
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    // picked up piece follows the cursor
    if (game->getPickedUpPiece() != NULL)
        game->getPickedUpPiece()->setPos(event->pos()
            - QPointF(game->getPickedUpPiece()->pixmap().height() / 2,
                      game->getPickedUpPiece()->pixmap().height() / 2));
}

void MainWindow::on_actionAboutQt_triggered() {
    QApplication::aboutQt();
}

void MainWindow::on_actionSave_triggered() {
    bool success = false;
    if (game->getOpenGameFileName() != "") success = saveGame(game->getOpenGameFileName());
    else success = saveGameAs();
}
