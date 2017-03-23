#include <QMessageBox>
#include <QTimer>

#include "mainwindow.h"
#include "oni.h"
#include "ui_mainwindow.h"

extern Oni *game;

int myrandom(int i) { return std::rand()%i; }

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    scene = NULL;

    // presettings
    changeLayout(0.75);
    windowTitle = "Oni - new unsaved game";
    setWindowTitle(windowTitle);

    // setup brushcolors
    colorHovered         = Qt::gray;
    colorSelected        = Qt::darkCyan;
    colorChooseableCard1 = Qt::yellow;
    colorChooseableCard2 = Qt::blue;
    colorChooseableBoth  = Qt::green;

    // scene setup
    scene = new QGraphicsScene(this);
    scene->setBackgroundBrush(QBrush(QImage(":/pics/paper.png")));
    scene->setSceneRect(0, 0, windowWidth, windowHeight);
    fieldSize = (scene->sceneRect().height() - 2*borderY - sideBarSize) / 5;
    slotSize = (scene->height() - 4*borderY) / 3;
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
    // seperate pieces and cards
    QStringList part = string.split("|");

    // get pieces
    QStringList elem = part.at(0).split(",");
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
        if (!game->getPieces()->contains(piece)) game->getPieces()->append(piece);
    }
    // get cards
    elem = part.at(1).split(",");
    for (int i = 0; i < elem.size(); i++) {
        Card *card = new Card;
        card->setCardValues(elem.at(i).toInt());
        if (!game->getCards()->contains(card)) game->getCards()->append(card);
    }
    if (game->getCards()->size() != 5) {
        // clear cards list
        game->getCards()->clear();
        // determine random, unique cards
        int intArray[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
        srand(unsigned(time(NULL)));
        std::random_shuffle(&intArray[0], &intArray[16], myrandom);
        for (int i = 0; i < 5; i++) {
            Card *card = new Card;
            card->setCardValues(intArray[i]);
            game->getCards()->append(card);
        }
    }
    // set owner
    game->getCards()->at(0)->setOwner(0);
    game->getCards()->at(1)->setOwner(1);
    game->getCards()->at(2)->setOwner(1);
    game->getCards()->at(3)->setOwner(2);
    game->getCards()->at(4)->setOwner(2);
}

void MainWindow::changeLayout(double factor) {
    // Change menu checkings
    if (!ui->actionFullScreen->isChecked()) {
        if (factor == 0.25) {
            ui->actionTinyLayout->setChecked(true);
            ui->actionSmallLayout->setChecked(false);
            ui->actionNormalLayout->setChecked(false);
            ui->actionLargeLayout->setChecked(false);
        } else if (factor == 0.50) {
            ui->actionTinyLayout->setChecked(false);
            ui->actionSmallLayout->setChecked(true);
            ui->actionNormalLayout->setChecked(false);
            ui->actionLargeLayout->setChecked(false);
        } else if (factor == 0.75) {
            ui->actionTinyLayout->setChecked(false);
            ui->actionSmallLayout->setChecked(false);
            ui->actionNormalLayout->setChecked(true);
            ui->actionLargeLayout->setChecked(false);
        } else if (factor == 1.00) {
            ui->actionTinyLayout->setChecked(false);
            ui->actionSmallLayout->setChecked(false);
            ui->actionNormalLayout->setChecked(false);
            ui->actionLargeLayout->setChecked(true);
        }
    }
    screen = QGuiApplication::primaryScreen();
    QRect desktop = screen->availableGeometry();
    borderX = 10 * factor;
    borderY = 10 * factor;
    sideBarSize = 40 * factor;
    windowHeight = desktop.height() * factor;
    windowWidth = windowHeight + 3*borderX + 2*((windowHeight - 4*borderY) / 3);
    while (windowWidth > desktop.width()-4) {
        windowHeight--;
        windowWidth = 3*borderX + windowHeight + 2*((windowHeight - 4*borderY) / 3);
    }
    setGeometry(0,0,windowWidth+4,windowHeight+4);
    setFixedSize(windowWidth+4, windowHeight+4);
    if (scene != NULL) {
        scene->setSceneRect(0, 0, windowWidth, windowHeight);
        setViewSize(windowWidth+4, windowHeight+4);
        fieldSize = ((scene->sceneRect().height() - 2*borderY - sideBarSize) / 5);
        slotSize = (scene->height() - 4*borderY) / 3;
        prepareGame();
    }
}

void MainWindow::drawBoard() {
    // drawing the board
    if (game->getBoard()->size() == 0) {
        QList<Field*> fieldsRow;
        for (int row = 0; row < game->getRows(); row++) {
            fieldsRow.clear();
            for (int col = 0; col < game->getCols(); col++) {
                // create field
                Field *field = new Field;
                field->setRow(row);
                field->setCol(col);
                field->setPieceType(' ');
                field->setPos(borderX + fieldSize * col, borderY + fieldSize * (4-row));

                // add piece to field
                int pieceNumber = field->identifyPiece();
                if (pieceNumber != -1) {
                    field->linkPiece(game->getPieces()->at(pieceNumber));
                    field->getPiece()->drawPiece();
                }
                // add field to row
                fieldsRow.append(field);
                scene->addItem(field);
            }
            // add row to board
            game->getBoard()->append(fieldsRow);
        }
    }
    else {
        for (int row = 0; row < game->getRows(); row++)
            for (int col = 0; col < game->getCols(); col++) {
                Field *field = game->getBoard()->at(row).at(col);
                field->setPos(borderX + fieldSize * col, borderY + fieldSize * (4-row));
                float size = game->getWindow()->getFieldSize();
                field->setRect(0, 0, size, size);

                // add piece to field
                int pieceNumber = field->identifyPiece();
                if (pieceNumber != -1) {
                    field->linkPiece(game->getPieces()->at(pieceNumber));
                    field->getPiece()->drawPiece();
                }
                scene->addItem(field);
            }
    }
}

void MainWindow::drawCardSlots() {
    // drawing the slotsGrid
    QList<CardSlot*> slotsRow;
    for (int player = 0; player < 3; player++) {
        slotsRow.clear();
        int maxSlots = game->getCardsPerPlayer();
        if (player == 0) maxSlots = game->getNeutralCardsPerGame();
        for (int number = 0; number < maxSlots; number++) {
            CardSlot *slot = new CardSlot(slotSize);
            slot->setOwner(player);
            double posX = scene->height() + number * slotSize + (number + 1) * borderX;
            double posY = ((-1.5 * player + 2.5) * player + 1) * slotSize + ((-1.5 * player + 2.5) * player + 2) * borderY;
            slot->setPos(posX, posY);
            slot->drawCard(player, number);
            scene->addItem(slot);
            slotsRow.append(slot);
        }
        game->getSlotsGrid()->append(slotsRow);
    }
}

void MainWindow::drawSideBar() {
    flipButton = new Button;
    flipButton->drawButton("flipButton", "right");
    double posX = this->height() - sideBarSize - borderX;
    double posY = (this->height() - sideBarSize - flipButton->pixmap().height() ) /2;
    flipButton->setPos(posX, posY);
    if (game->getFlipBoard()) {
        flipButton->setTransformOriginPoint(flipButton->pixmap().width() / 2, flipButton->pixmap().height() / 2);
        flipButton->setRotation(180);
    }
    scene->addItem(flipButton);
}

QString MainWindow::generateSetupString() {
    QString saveString = "";
    for (int i = 0; i < game->getPieces()->size(); i++) {
        Piece *piece = game->getPieces()->at(i);
        if (saveString != "") saveString += ",";
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
    saveString += "|";
    for (int k = 0; k < 3; k++) {
        for (int i = 0; i < game->getCards()->size(); i++)
            if (game->getCards()->at(i)->getOwner() == k) {
                if (k != 0) saveString += ",";
                saveString += QString::number(game->getCards()->at(i)->getID());
            }
    }

    return saveString;
}

void MainWindow::loadGame() {
    //if (maybeSave()) {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open savegame"), "/current/saves", "Oni Savegames (*.oni)");
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (!file.open(QFile::ReadOnly | QFile::Text)) {
            QMessageBox::warning(this, tr("Application"), tr("Cannot read file %1:\n%2.")
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
    ui->notation->clear();

    // reset lists
    if (game->getPieces()) game->getPieces()->clear();
    if (game->getCards()) game->getCards()->clear();

    // setup string
    if (setupString == "") setupString = "Sa1,Sb1,Mc1,Sd1,Se1,sa5,sb5,mc5,sd5,se5|";
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
    // Remove items from scene
    QList<QGraphicsItem*> items = scene->items();
    foreach(QGraphicsItem *item, items) scene->removeItem(item);

    // Generate and draw Items to the scene
    drawBoard();
    drawSideBar();
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

void MainWindow::unparentPieces() {
    for (int i = 0; i < game->getPieces()->size(); i++)
        game->getPieces()->at(i)->setParentItem(NULL);
}

void MainWindow::setViewSize(double width, double height) {
    ui->view->setFixedSize(width, height);
}

void MainWindow::on_actionSave_triggered() {
    bool success = false;
    if (game->getOpenGameFileName() != "") success = saveGame(game->getOpenGameFileName());
    else success = saveGameAs();
}

void MainWindow::on_actionFlipOnce_triggered() {
    game->flipBoard();
    prepareGame();
}

void MainWindow::on_actionAboutQt_triggered() {
    QApplication::aboutQt();
}

void MainWindow::on_actionFullScreen_triggered()
{
    if (ui->actionFullScreen->isChecked()) {
        ui->actionFullScreen->setChecked(true);
        QMainWindow::showFullScreen();
        changeLayout(1.0);
    } else {
        ui->actionFullScreen->setChecked(false);
        QMainWindow::showNormal();
        if (ui->actionTinyLayout->isChecked()) changeLayout(0.25);
        if (ui->actionSmallLayout->isChecked()) changeLayout(0.50);
        if (ui->actionNormalLayout->isChecked()) changeLayout(0.75);
        if (ui->actionLargeLayout->isChecked()) changeLayout(1.00);
    }
}
