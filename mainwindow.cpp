#include <QDialogButtonBox>
#include <QMessageBox>
#include <QTimer>
#include <iostream>
#include <time.h>
#include <QDate>

#include "mainwindow.h"
#include "oni.h"
#include "ui_mainwindow.h"

extern Oni *game;

int myrandom(int i) { srand(unsigned(time(NULL))); return std::rand()%i; }

bool fileExists(QString path) {
    QFileInfo check_file(path);
    // check if file exists and if yes: Is it really a file and no directory?
    return check_file.exists() && check_file.isFile();
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), colorHovered(Qt::gray), colorSelected(Qt::darkCyan),
    colorChooseableCard1(Qt::yellow), colorChooseableCard2(Qt::blue), colorChooseableBoth(Qt::green),
    screen(QGuiApplication::primaryScreen()), scene(NULL), dialogAbout(new DialogAbout(this)), dialogSaveAs(new DialogSave(this)),
    windowTitle("Oni - new unsaved game"), axisLabel(new QList<QGraphicsTextItem*>),
    windowPosX(0), windowPosY(0), windowHeight(0), windowWidth(0), borderX(0), borderY(0),
    fieldSize(0), slotSize(0), sideBarSize(0), axisLabelSize(0), MSWindowsCorrection(0) {

    // set up the UI
    ui->setupUi(this);

    // presettings
    changeLayout();
    setWindowTitle(windowTitle);

    // connects
    connect(ui->notation, SIGNAL(itemClicked(QListWidgetItem*)),
                this, SLOT(showMove(QListWidgetItem*)));
}

MainWindow::~MainWindow() {
    delete ui;
}

bool MainWindow::getAxisLabeling() {
    return ui->actionAxisLabeling->isChecked();
}

bool MainWindow::getFlipEveryMove() {
    return ui->actionFlipEveryMove->isChecked();
}

bool MainWindow::analyseSetupString(QString string) {
    // seperate pieces and cards
    QStringList part = string.split("|");

    // get pieces
    if (part.size() == 0) return false;
    int count_M = 0, count_m = 0, count_S = 0, count_s = 0;
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
        bool nope = false;
        for (int k = 0; k < game->getPieces()->size(); k++)
            if (piece->getCol() == game->getPieces()->at(k)->getCol() && piece->getRow() == game->getPieces()->at(k)->getRow()) {
                nope = true;
                continue;
            }
        if (!nope) {
            if (piece->getType() == 'S' && count_S < 4) {
                count_S++;
                game->getPieces()->append(piece);
            } else if (piece->getType() == 's' && count_s < 4) {
                count_s++;
                game->getPieces()->append(piece);
            } else if (piece->getType() == 'M' && count_M < 1) {
                count_M++;
                game->getPieces()->append(piece);
            } else if (piece->getType() == 'm' && count_m < 1) {
                count_m++;
                game->getPieces()->append(piece);
            }
        }
    }
    // check numbers of masters
    if (count_M != 1 || count_m != 1) return false;

    // take care of missing pieces
    for (int i = count_S+1; i < 5; i++) {
        Piece *victim = new Piece;
        victim->setRow(-1);
        victim->setCol(-1);
        victim->setType('S');
        game->getCapturedRed()->append(victim);
        victim->drawPiece();
    }
    for (int i = count_s+1; i < 5; i++) {
        Piece *victim = new Piece;
        victim->setRow(-1);
        victim->setCol(-1);
        victim->setType('s');
        game->getCapturedBlue()->append(victim);
        victim->drawPiece();
    }

    // get cards
    // clear cards list
    game->getCards()->clear();
    if (part.size() > 1) {
        elem = part.at(1).split(",");
        bool nope = false;
        for (int i = 0; i < elem.size(); i++) {
            Card *card = new Card;
            card->setCardValues(elem.at(i).toInt());
            nope = false;
            for (int k = 0; k < game->getCards()->size(); k++)
                if (card->getID() == game->getCards()->at(k)->getID()) {
                    nope = true;
                    continue;
                }
            if (!nope) game->getCards()->append(card);
        }
    }
        if (game->getCards()->size() != 5) {
            // clear cards list
            game->getCards()->clear();
            // determine random, unique cards
            int intArray[34] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, // Basis Game
                                17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, // "Sensei's Path" Expansion
                                33, 34}; // "Goat & Sheep" Expansion
            srand(unsigned((NULL)));
            std::random_shuffle(&intArray[0], &intArray[34], myrandom);
            for (int i = 0; i < 5; i++) {
                Card *card = new Card;
                card->setCardValues(intArray[i]);
                game->getCards()->append(card);
            }
        }
    // get game settings
    if (part.size() >2) {
        elem = part.at(2).split(",");
        if (elem.at(0) == "true") game->setFirstPlayersTurn(true);
        else game->setFirstPlayersTurn(false);
        if (elem.at(1) == "true") game->setFlippedBoard(true);
        else game->setFlippedBoard(false);
        if (elem.at(2) == "true") ui->actionFlipEveryMove->setChecked(true);
        else ui->actionFlipEveryMove->setChecked(false);
    }
    return true;
}

void MainWindow::changeLayout(double factor) {
    // Change menu checkings
    if (!ui->actionFullScreen->isChecked()) {
        if (factor <= 0.3) {
            ui->actionTinyLayout->setChecked(true);
            ui->actionSmallLayout->setChecked(false);
            ui->actionNormalLayout->setChecked(false);
            ui->actionLargeLayout->setChecked(false);
        } else if (factor <= 0.5) {
            ui->actionTinyLayout->setChecked(false);
            ui->actionSmallLayout->setChecked(true);
            ui->actionNormalLayout->setChecked(false);
            ui->actionLargeLayout->setChecked(false);
        } else if (factor <= 0.7) {
            ui->actionTinyLayout->setChecked(false);
            ui->actionSmallLayout->setChecked(false);
            ui->actionNormalLayout->setChecked(true);
            ui->actionLargeLayout->setChecked(false);
        } else if (factor <= 0.9) {
            ui->actionTinyLayout->setChecked(false);
            ui->actionSmallLayout->setChecked(false);
            ui->actionNormalLayout->setChecked(false);
            ui->actionLargeLayout->setChecked(true);
        }
    }
    // measure and fill available screen
    QRect desktop = screen->availableGeometry();
    // default factor: 0.70
    if (getAxisLabeling() == true) axisLabelSize = 30 * factor;
    else axisLabelSize = 0;
    borderX = 10 * factor;
    borderY = 10 * factor;
    sideBarSize = 40 * factor;
    // setting window size in dependency of available screen
    windowHeight = desktop.height() * factor;
    windowWidth = windowHeight - MSWindowsCorrection + 3*borderX + 2*((windowHeight - 4*borderY - MSWindowsCorrection) / 3);
    while (windowWidth > desktop.width()-4) {
        windowHeight--;
        windowWidth = windowHeight + 3*borderX + 2*((windowHeight - 4*borderY) / 3);
    }
    setGeometry(windowPosX, windowPosY, windowWidth+5, windowHeight+5);
    setFixedSize(windowWidth+5, windowHeight+5);
    bool sceneWasSetUp = true;
    if (!scene) {
        sceneWasSetUp = false;
        // scene setup
        scene = new QGraphicsScene(this);
        scene->setBackgroundBrush(QBrush(QImage(":/pics/paper.png")));
        ui->view->setScene(scene);
        ui->view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        ui->view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        ui->notation->setSelectionMode(QAbstractItemView::SingleSelection);
    }
    ui->view->setFixedSize(windowWidth+5, windowHeight+5);
    scene->setSceneRect(0, 0, windowWidth, windowHeight);
    fieldSize = (scene->sceneRect().height() - MSWindowsCorrection - 2*borderY - sideBarSize - axisLabelSize) / 5;
    slotSize = (scene->sceneRect().height() - MSWindowsCorrection - 4*borderY ) / 3;
    if (sceneWasSetUp) prepareGame();
}

void MainWindow::drawAxisLabeling() {
    if (getAxisLabeling()) {
        for (int row = 0; row < 5; row++) {
            QGraphicsTextItem *label = new QGraphicsTextItem;
            if (game->getFlippedBoard()) label->setPlainText(QString::number(row+1));
            else label->setPlainText(QString::number(5-row)); 
            if (ui->actionTinyLayout->isChecked() == false)
                label->setPos(borderX, borderY + fieldSize/2 + fieldSize*row - label->boundingRect().height()/2);
            else
                label->setPos(-2*borderX, borderY + fieldSize/2 + fieldSize*row - label->boundingRect().height()/2);
            axisLabel->append(label);
            scene->addItem(label);
        }
        for (int col = 0; col < 5; col++) {
            QGraphicsTextItem *label = new QGraphicsTextItem;
            if (game->getFlippedBoard()) label->setPlainText(QString('e'-col));
            else label->setPlainText(QString('a'+col));
            label->setPos(borderX + axisLabelSize + fieldSize/2 + fieldSize*col - label->boundingRect().width()/2, 2*borderY + 5*fieldSize);
            axisLabel->append(label);
            scene->addItem(label);
        }
    }
}

void MainWindow::drawBackGroundPicture() {
    QGraphicsPixmapItem *back = new QGraphicsPixmapItem(QPixmap(":/pics/back.png"));
        back->setPixmap(back->pixmap().scaled(fieldSize*5, fieldSize*5));
        back->setPos(borderX + axisLabelSize, borderY);
        scene->addItem(back);
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
                if (!game->getFlippedBoard()) field->setPos(borderX + axisLabelSize + fieldSize * col, borderY + fieldSize * (4-row));
                else field->setPos(borderX + axisLabelSize + fieldSize * (4-col), borderY + fieldSize * row);
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
    } else {
        for (int row = 0; row < game->getRows(); row++) {
            for (int col = 0; col < game->getCols(); col++) {
                Field *field = game->getBoard()->at(row).at(col);
                if (!game->getFlippedBoard()) field->setPos(borderX + axisLabelSize + fieldSize * col, borderY + fieldSize * (4-row));
                else field->setPos(borderX + axisLabelSize + fieldSize * (4-col), borderY + fieldSize * row);
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
}

void MainWindow::drawCardSlots() {
    // drawing the slotsGrid
    QList<CardSlot*> slotsRow;
    CardSlot *slot;
    for (int player = 0; player < 3; player++) {
        slotsRow.clear();
        // determite number of needed cardslots
        int maxSlots = game->getCardsPerPlayer();
        if (player == 0) maxSlots = game->getNeutralCardsPerGame();
        // draw cardslots
        for (int number = 0; number < maxSlots; number++) {
            if (game->getSlotsGrid()->size() < 3) {
                slot = new CardSlot(slotSize);
                slot->setOwner(player);
                slot->assignCard(player, number);
            } else {
                if (game->getSlotsGrid()->at(2).size() < 2) {
                    slot = new CardSlot(slotSize);
                    slot->setOwner(player);
                    slot->assignCard(player, number);
                } else slot = game->getSlotsGrid()->at(player).at(number);
            }
            slot->setRect(0, 0, slotSize, slotSize);
            double posX = scene->height() - MSWindowsCorrection + number * slotSize + (number + 1) * borderX;
            double posY;
            if (game->getFlippedBoard()) posY = ((1.5 * player - 2.5) * player + 1) * slotSize + ((1.5 * player - 2.5) * player + 2) * borderY;
            else posY = ((-1.5 * player + 2.5) * player + 1) * slotSize + ((-1.5 * player + 2.5) * player + 2) * borderY;

            if (game->getCardChoiceActive()) {
                if ((game->getFirstPlayersTurn() && player == 1) || (!game->getFirstPlayersTurn() && player == 2)) {
                    posX = 0.1*windowWidth + 1.1*number*slotSize;
                    posY = 0.2*windowHeight;
                }
            }

            slot->setPos(posX, posY);
            slot->getCard()->setParentItem(slot);
            slot->getCard()->drawCard(player);
            scene->addItem(slot);
            slot->colorizePlayersSlots(player, number);
            if (game->getSlotsGrid()->size() < 3) slotsRow.append(slot);
            else if(game->getSlotsGrid()->at(2).size() < 2)
                slotsRow.append(slot);
        }
        if (game->getSlotsGrid()->size() < 3) game->getSlotsGrid()->append(slotsRow);
        else if(game->getSlotsGrid()->at(2).size() < 2) game->getSlotsGrid()->append(slotsRow);
    }
}

void MainWindow::drawSideBar() {
    double posXright = 0, posYbuttom = 0, posY = 0;
    // drawing in-game sidebar "right"
    posXright = this->height() - MSWindowsCorrection - sideBarSize - borderX;
    posYbuttom = this->height() - MSWindowsCorrection - sideBarSize - axisLabelSize - borderY;

    // FlipButton
    flipButton = new Button;
    flipButton->drawButton("flipButton", "right");
    flipButton->setPos(posXright, (this->height() - MSWindowsCorrection - sideBarSize - axisLabelSize - flipButton->pixmap().height() ) /2);
    if (game->getFlippedBoard()) {
        flipButton->setTransformOriginPoint(flipButton->pixmap().width() / 2, flipButton->pixmap().height() / 2);
        flipButton->setRotation(180);
    }
    scene->addItem(flipButton);

    // TurnMarker
    turnRed = new Button;
    turnRed->drawButton("turnRed", "right");
    if (game->getFlippedBoard()) posY = borderY;
    else posY = (this->height() - MSWindowsCorrection - sideBarSize - axisLabelSize - borderY - turnRed->pixmap().height() );
    turnRed->setPos(posXright, posY);

    turnBlue = new Button;
    turnBlue->drawButton("turnBlue", "right");
    if (game->getFlippedBoard()) posY = (this->height() - MSWindowsCorrection - sideBarSize - axisLabelSize - borderY - turnBlue->pixmap().height() );
    else posY = borderY;
    turnBlue->setPos(posXright, posY);

    if (game->getFirstPlayersTurn()) scene->addItem(turnRed);
    else scene->addItem(turnBlue);

    // Captured pieces
    bool delItem = false;
    for (int i = 0; i < game->getCapturedBlue()->count(); i++) {
        Piece *victim = game->getCapturedBlue()->at(i);
        victim->setPixmap(QPixmap(":/pics/pieces/scolar_blue.png"));
        double victimHeight = (windowHeight -MSWindowsCorrection -7*borderY -sideBarSize -turnRed->boundingRect().height() -turnBlue->boundingRect().height() -flipButton->boundingRect().height()) / 8;
        double victimWidth = victimHeight / victim->pixmap().height() * victim->pixmap().width();
        if (victim->getType() == 's') {
            victim->setPixmap(victim->pixmap().scaled(victimHeight, victimWidth));
            victim->setPos(posXright - 2*borderX, windowHeight - MSWindowsCorrection - 2*borderY - turnRed->boundingRect().height() - sideBarSize - axisLabelSize - victim->pixmap().height()*(i+1));
            delItem = false;
            for (int k = 0; k < scene->items().size(); k++)
                if (scene->items().at(k) == victim) delItem = true;
            if (delItem) scene->removeItem(victim);
            scene->addItem(victim);
        }
    }
    for (int i = 0; i < game->getCapturedRed()->count(); i++) {
        Piece *victim = game->getCapturedRed()->at(i);
        victim->setPixmap(QPixmap(":/pics/pieces/scolar_red.png"));
        double victimHeight = (windowHeight -MSWindowsCorrection -7*borderY -sideBarSize -turnRed->boundingRect().height() -turnBlue->boundingRect().height() -flipButton->boundingRect().height()) / 8;
        double victimWidth = victimHeight / victim->pixmap().height() * victim->pixmap().width();
        if (victim->getType() == 'S') {
            victim->setPixmap(victim->pixmap().scaled(victimHeight, victimWidth));
            victim->setPos(posXright - 2*borderX, 2*borderY + turnRed->boundingRect().height() + victim->pixmap().height()*i);
            delItem = false;
            for (int k = 0; k < scene->items().size(); k++)
                if (scene->items().at(k) == victim) delItem = true;
            if (delItem) scene->removeItem(victim);
            scene->addItem(victim);
        }
    }
}

QString MainWindow::generateNotationString(QString lastTurn, QString thisTurn) {
    QString notationString = "";
    QStringList part = lastTurn.split("|");
    QStringList lastPieces = part.at(0).split(",");
    part = thisTurn.split("|");
    QStringList newPieces = part.at(0).split(",");
    QStringList newCards = part.at(1).split(",");
    QStringList usedPieces;
    foreach (QString pieceLast, lastPieces) {
        bool found = false;
        foreach (QString pieceNew, newPieces)
            if (pieceLast == pieceNew) found = true;
        if (found == false) usedPieces.append(pieceLast);
    }
    foreach (QString pieceNew, newPieces) {
        bool found = false;
        foreach (QString pieceLast, lastPieces)
            if (pieceLast == pieceNew) found = true;
        if (found == false) usedPieces.append(pieceNew);
    }
    if (usedPieces.size() == 2) notationString += usedPieces.at(0) + "-" + usedPieces.at(1).at(1) + usedPieces.at(1).at(2);
    else {
        if (usedPieces.at(2).at(0) == usedPieces.at(0).at(0)) notationString += usedPieces.at(0) + "x" + usedPieces.at(1);
        else notationString += usedPieces.at(1) + "x" + usedPieces.at(0);
    }
    // add used card (neutral card of actual cards)
    Card temp;
    temp.setCardValues(newCards.at(0).toInt());
    notationString += " (" + QString(temp.getName()) + ")";

    return notationString;
}

QString MainWindow::generateSetupString() {
    // set fields
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
    // set cards
    if (game->getSlotsGrid()->size() > 0) {
        for (int k = 0; k < 3; k++) {
            for (int l = 0; l < game->getSlotsGrid()->at(k).size(); l++) {
                if (k != 0) saveString += ",";
                saveString += QString::number(game->getSlotsGrid()->at(k).at(l)->getCard()->getID());
            }
        }
    } else
        for (int k = 0; k < 5; k++) {
            if (k != 0) saveString += ",";
            saveString += QString::number(game->getCards()->at(k)->getID());
        }
    saveString += "|";
    // set game settings
    saveString += QString(game->getFirstPlayersTurn() ? "true" : "false") + ",";
    saveString += QString(game->getFlippedBoard() ? "true" : "false") + ",";
    saveString += QString(ui->actionFlipEveryMove->isChecked() ? "true" : "false");
    return saveString;
}

void MainWindow::loadGame(QString fileName) {
    // load previously saved game
    if (fileName == "") fileName = QFileDialog::getOpenFileName(this, tr("Open savegame"), QDir::currentPath()+"/saves", "Oni Savegames (*.oni)");
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
            QString line = "";
            game->getTurns()->clear();
            while (!in.atEnd()) {
                line = in.readLine();
                game->getTurns()->append(line);
            }
        #ifndef QT_NO_CURSOR
            QApplication::restoreOverrideCursor();
        #endif

        game->setOpenGameFileName(fileName);
        if (game->getTurns()->last() == "1-0" || game->getTurns()->last() == "0-1")
            newGame(game->getTurns()->at( game->getTurns()->size()-2) );
        else newGame(game->getTurns()->last());

        // set window title
        QStringList elem = fileName.split("/");
        QStringList name = elem.last().split(".");
        windowTitle = "Oni - " + name.first();
        setWindowTitle(windowTitle);
    }
}

void MainWindow::newGame(QString setupString) {
    // reset settings
    game->setGameResult(0);
    game->setFirstPlayersTurn(true);
    game->setFlippedBoard(false);
    game->getCapturedBlue()->clear();
    game->getCapturedRed()->clear();
    game->setCardChoiceActive(false);
    windowTitle = "Oni - new unsaved game";
    setWindowTitle(windowTitle);
    game->setOpenGameFileName("");

    // reset lists
    if (setupString == "") game->getTurns()->clear();
    resetLists();

    // setup string
    QString defaultString = "Sa1,Sb1,Mc1,Sd1,Se1,sa5,sb5,mc5,sd5,se5|";
    if (setupString == "" || setupString == "1-0" || setupString == "0-1") setupString = defaultString;
    bool success = analyseSetupString(setupString);
    if (!success) {
        setupString = defaultString;
        resetLists();
        success = analyseSetupString(setupString);
    }
    if (game->getTurns()->size() == 0) game->getTurns()->append(generateSetupString());

    // fill notation window
    refreshNotation();

    // prepare the game
    prepareGame();
}

void MainWindow::notateVictory(QString result) {
    ui->notation->addItem(result);
    ui->notation->scrollToBottom();
    game->getTurns()->append(result);
    if (result == "1-0") game->setGameResult(1);
    else game->setGameResult(-1);
}

void MainWindow::positionNotation() {
    double posX = scene->height() - MSWindowsCorrection + slotSize + 2*borderX + 2.5;
    double posY = slotSize + 2*borderY + 2.5;
    ui->notation->scrollToBottom();
    ui->notation->setGeometry(posX, posY, slotSize, slotSize);
}

void MainWindow::prepareGame() {
    // Remove items from scene
    QList<QGraphicsItem*> items = scene->items();
    foreach(QGraphicsItem *item, items) scene->removeItem(item);

    // Generate and draw Items to the scene
    drawBackGroundPicture();
    drawBoard();
    drawSideBar();
    drawAxisLabeling();
    drawCardSlots();
    positionNotation();
}

void MainWindow::saveGame(const QString &fileName) {
    QString newFileName = fileName;
    if (newFileName == "") {
        if (game->getWindow()->getDialogSaveAs()->exec() == QDialog::Accepted) {
            QList<QString> names = game->getWindow()->getDialogSaveAs()->getValues();
            game->setPlayerNames(names.at(0),names.at(1));

            int i = 0;
            QString number = "";
            do {
                if (++i > 1) number = " " + QString::number(i);
                newFileName = "saves/" + names.at(0) + "-" + names.at(1) + " (" + QDate::currentDate().toString("yyyy-MM-dd") + ")"+ number +".oni";
            } while (fileExists(newFileName));

            // set window title
            QStringList elem = newFileName.split("/");
            QStringList name = elem.last().split(".");
            windowTitle = "Oni - " + name.first();
            setWindowTitle(windowTitle);
            game->setOpenGameFileName(newFileName);
        }
    }
    if (newFileName != "") {
        // (over)write current savegame
        QFile file(newFileName);
        if (!file.open(QFile::WriteOnly | QFile::Text))
            QMessageBox::warning(this, tr("Application"), tr("Cannot write file %1:\n%2.").arg(QDir::toNativeSeparators(fileName),file.errorString()));
        QTextStream out(&file);
        #ifndef QT_NO_CURSOR
            QApplication::setOverrideCursor(Qt::WaitCursor);
        #endif
        for (int i = 0; i < game->getTurns()->size(); i++)
            out << game->getTurns()->at(i) << endl;
        #ifndef QT_NO_CURSOR
            QApplication::restoreOverrideCursor();
        #endif
    }
}

void MainWindow::saveTurnInNotation() {
    // refreshing of the notation if jumped back
    QString lastMove, thisMove;
    if (game->getTurns()->size() > (game->getActuallyDisplayedMove()+1)) {
         for(int i = game->getTurns()->size(); i > game->getActuallyDisplayedMove()+1; i--)
             game->getTurns()->removeLast();
         game->getWindow()->refreshNotation();
    }

    thisMove = generateSetupString();
    lastMove = game->getTurns()->last();
    game->getTurns()->append(thisMove);
    game->setActuallyDisplayedMove(game->getActuallyDisplayedMove()+1);

    // building notation entry
    QListWidgetItem *item;
    if (!game->getFirstPlayersTurn()) {
        item = new QListWidgetItem(QString::number((int)ui->notation->count()/2+1) + ". " + generateNotationString(lastMove, thisMove));
        item->setBackground(QBrush(QColor(200,55,55), Qt::Dense4Pattern));
    } else {
        item = new QListWidgetItem(generateNotationString(lastMove, thisMove));
        item->setBackground(QBrush(Qt::blue, Qt::Dense4Pattern));
        item->setTextAlignment(Qt::AlignRight);
    }
    ui->notation->addItem(item);
}

void MainWindow::moveEvent(QMoveEvent *event) {
   QMainWindow::moveEvent(event);
   QRect geo = geometry();
   windowPosX = geo.x();
   windowPosY = geo.y();
}

void MainWindow::refreshNotation() {
    ui->notation->clear();
    int maxLines = game->getTurns()->size();
    if (game->getTurns()->size() > 1 && (game->getTurns()->last() == "1-0" || game->getTurns()->last() == "0-1")) maxLines--;
    for (int i = 1; i < maxLines; i++) {
        QListWidgetItem *item;
        if (i%2 == 1) {
            item = new QListWidgetItem(QString::number((int)ui->notation->count()/2+1) + ". "
                                       + generateNotationString(game->getTurns()->at(i-1), game->getTurns()->at(i)));
            item->setBackground(QBrush(QColor(200,55,55), Qt::Dense4Pattern));
        } else {
            item = new QListWidgetItem(generateNotationString(game->getTurns()->at(i-1), game->getTurns()->at(i)));
            item->setBackground(QBrush(Qt::blue, Qt::Dense4Pattern));
            item->setTextAlignment(Qt::AlignRight);
        }
        ui->notation->addItem(item);
    }
    if (game->getTurns()->last() == "1-0" || game->getTurns()->last() == "0-1") ui->notation->addItem(game->getTurns()->last());
}

void MainWindow::resetLists() {
    ui->notation->clear();
    if (game->getPieces()) game->getPieces()->clear();
    if (game->getCards()) game->getCards()->clear();
    if (game->getBoard()) game->getBoard()->clear();
    if (game->getSlotsGrid()) game->getSlotsGrid()->clear();
    for (int k = 0; k < game->getCapturedBlue()->size(); k++) {
        Piece *victim = game->getCapturedBlue()->at(k);
        bool delItem = false;
        for (int k = 0; k < scene->items().size(); k++)
            if (scene->items().at(k) == victim) delItem = true;
        if (delItem) scene->removeItem(victim);
    }
    for (int k = 0; k < game->getCapturedRed()->size(); k++) {
        Piece *victim = game->getCapturedRed()->at(k);
        bool delItem = false;
        for (int k = 0; k < scene->items().size(); k++)
            if (scene->items().at(k) == victim) delItem = true;
        if (delItem) scene->removeItem(victim);
    }
}

void MainWindow::showMove(QListWidgetItem *item) {
    if (item->text() != "1-0" && item->text() != "0-1") {
        for (int i = 0; i < ui->notation->count(); i++) {
            if (ui->notation->item(i) == item) {
                newGame(game->getTurns()->at(i+1));
                game->setActuallyDisplayedMove(i+1);
                return;
            }
        }
    }
}

void MainWindow::on_actionNew_triggered() {
    QMessageBox::StandardButton reply = QMessageBox::question(NULL, "New game", "Do you want to start a new game?<br>All unsaved changes to the actual game will be lost.");
    if (reply == QMessageBox::Yes) newGame();
}

void MainWindow::on_actionSetupPosition_triggered() {

}

void MainWindow::on_actionLoad_triggered() {
    QMessageBox::StandardButton reply = QMessageBox::question(NULL, "Load game", "Do you want to load another game?<br>All unsaved changes to the actual game will be lost.");
    if (reply == QMessageBox::Yes) loadGame();
}

void MainWindow::on_actionSave_triggered() {
    saveGame(game->getOpenGameFileName());
}

void MainWindow::on_actionStartingPosition_triggered() {
    game->setActuallyDisplayedMove(0);
    newGame(game->getTurns()->at(game->getActuallyDisplayedMove()));
}

void MainWindow::on_actionPreviousMove_triggered() {
    if (game->getTurns()->size() > 1 && game->getActuallyDisplayedMove() > 0) {
        game->setActuallyDisplayedMove(game->getActuallyDisplayedMove()-1);
        newGame(game->getTurns()->at(game->getActuallyDisplayedMove()));
    }
}

void MainWindow::on_actionNextMove_triggered() {
    if (game->getTurns()->size() > 1 && game->getActuallyDisplayedMove() < game->getTurns()->size()-1) {
        game->setActuallyDisplayedMove(game->getActuallyDisplayedMove()+1);
        newGame(game->getTurns()->at(game->getActuallyDisplayedMove()));
    }
}

void MainWindow::on_actionLastMove_triggered() {
    game->setActuallyDisplayedMove(game->getTurns()->size()-1);
    newGame(game->getTurns()->at(game->getActuallyDisplayedMove()));
}

void MainWindow::on_actionResign_triggered() {
    if (game->getGameResult() == 0 && !game->getCardChoiceActive()) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Resign match", "Do you really want to resign?", QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            if (game->getFirstPlayersTurn()) {
                game->setGameResult(-1);
                game->getWindow()->notateVictory("0-1");
            } else {
                game->setGameResult(1);
                game->getWindow()->notateVictory("1-0");
            }
        }
    }
}

void MainWindow::on_actionRedEasy_triggered() {

}

void MainWindow::on_actionRedMedium_triggered() {}

void MainWindow::on_actionRedHard_triggered() {

}

void MainWindow::on_actionBlueEasy_triggered() {

}

void MainWindow::on_actionBlueMedium_triggered() {

}

void MainWindow::on_actionBlueHard_triggered() {

}

void MainWindow::on_actionFlipOnce_triggered() {
    // menubar option
    game->setFlippedBoard(!game->getFlippedBoard());
    prepareGame();
}

void MainWindow::on_actionHideNotation_triggered() {
    if (ui->notation->isVisible()) {
        ui->notation->setVisible(false);
        ui->actionHideNotation->setChecked(true);
    }
    else {
        ui->notation->setVisible(true);
        ui->actionHideNotation->setChecked(false);
    }
}

void MainWindow::on_actionAxisLabeling_triggered() {
    // menubar option
    if (ui->actionTinyLayout->isChecked()) changeLayout(0.30);
    if (ui->actionSmallLayout->isChecked()) changeLayout(0.50);
    if (ui->actionNormalLayout->isChecked()) changeLayout(0.70);
    if (ui->actionLargeLayout->isChecked()) changeLayout(0.90);
}

void MainWindow::on_actionTinyLayout_triggered() {
    QMainWindow::showNormal();
    ui->actionFullScreen->setChecked(false);
    changeLayout(0.3);
}

void MainWindow::on_actionSmallLayout_triggered() {
    QMainWindow::showNormal();
    ui->actionFullScreen->setChecked(false);
    changeLayout(0.5);
}

void MainWindow::on_actionNormalLayout_triggered() {
    QMainWindow::showNormal();
    ui->actionFullScreen->setChecked(false);
    changeLayout(0.7);
}

void MainWindow::on_actionLargeLayout_triggered() {
    QMainWindow::showNormal();
    ui->actionFullScreen->setChecked(false);
    changeLayout(0.9);
}

void MainWindow::on_actionFullScreen_triggered(){
    // menubar option
    if (ui->actionFullScreen->isChecked()) {
        ui->actionFullScreen->setChecked(true);
        changeLayout(1.0);
        QMainWindow::showFullScreen();
    } else {
        ui->actionFullScreen->setChecked(false);
        QMainWindow::showNormal();
        if (ui->actionTinyLayout->isChecked()) changeLayout(0.30);
        if (ui->actionSmallLayout->isChecked()) changeLayout(0.50);
        if (ui->actionNormalLayout->isChecked()) changeLayout(0.70);
        if (ui->actionLargeLayout->isChecked()) changeLayout(0.90);
    }
}

void MainWindow::on_actionAboutQt_triggered() {
    // menubar option
    QApplication::aboutQt();
}
