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

int myrandom(int i) { srand(unsigned(time(nullptr))); return std::rand()%i; }

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow),
    colorHovered(Qt::gray), colorSelected(Qt::darkCyan), colorChooseableCard1(Qt::yellow),
    colorChooseableCard2(Qt::blue), colorChooseableBoth(Qt::green), screen(QGuiApplication::primaryScreen()), scene(nullptr),
    dialogAbout(new DialogAbout(this)), windowDatabase(nullptr), axisLabel(new QList<QGraphicsTextItem*>),
    windowPosX(0), windowPosY(0), windowHeight(0), windowWidth(0), borderX(0), borderY(0),
    fieldSize(0), slotHeight(0.0), slotWidth(0.0), sideBarSize(0), axisLabelSize(0), MSWindowsCorrection(0) {

    ui->setupUi(this);
    updateLayout();

    // connects
    connect(ui->notation, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(showMove(QListWidgetItem*)));
}

MainWindow::~MainWindow() {
    delete ui;
}

bool MainWindow::getFlipEveryMove() {
    return ui->actionFlipEveryMove->isChecked();
}

bool MainWindow::analyseSetupString(QString string) {
    string = string.remove(" ");
    QList<Card*> *cards = game->getCards();
    QList<Piece*> *pieces = game->getPieces();

    // seperate pieces and cards
    QStringList part = string.split("|");

    // get pieces
    if (part.size() == 0) return false;
    int count_M = 0, count_m = 0, count_S = 0, count_s = 0;
    QStringList elem = part.at(0).split(",");
    for (int i = 0; i < elem.size(); i++) {
        Piece *piece = new Piece;
        piece->setType(static_cast<char>(elem[i].at(0).unicode()));
        char c = static_cast<char>(tolower(elem[i].at(1).unicode()));
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
        for (int k = 0; k < pieces->size(); k++)
            if (piece->getCol() == pieces->at(k)->getCol() && piece->getRow() == pieces->at(k)->getRow()) {
                nope = true;
                continue;
            }
        if (!nope) {
            if (piece->getType() == 'S' && count_S < 4) {
                count_S++;
                pieces->append(piece);
            } else if (piece->getType() == 's' && count_s < 4) {
                count_s++;
                pieces->append(piece);
            } else if (piece->getType() == 'M' && count_M < 1) {
                count_M++;
                pieces->append(piece);
            } else if (piece->getType() == 'm' && count_m < 1) {
                count_m++;
                pieces->append(piece);
            }
        }
    }
    // check number of masters
    if (!(count_M == 1 && count_m == 1)) {
        if (count_M == 1 && count_m == 0)
            game->getMatch()->setResult(1);
        else {
            if (count_M == 0 && count_m == 1)
                game->getMatch()->setResult(-1);
            else
                return false;
        }
    }

    // take care of missing pieces
    game->getCapturedRed()->clear();
    for (int i = count_S+1; i < 5; i++) {
        Piece *victim = new Piece;
        victim->setRow(-1);
        victim->setCol(-1);
        victim->setType('S');
        game->getCapturedRed()->append(victim);
        victim->drawPiece();
    }
    game->getCapturedBlue()->clear();
    for (int i = count_s+1; i < 5; i++) {
        Piece *victim = new Piece;
        victim->setRow(-1);
        victim->setCol(-1);
        victim->setType('s');
        game->getCapturedBlue()->append(victim);
        victim->drawPiece();
    }

    // get cards
    cards->clear();
    if (part.size() > 1) {
        elem = part.at(1).split(",");
        bool nope = false;
        for (int i = 0; i < elem.size(); i++) {
            Card *card = new Card;
            card->setCardValues(elem.at(i).toInt());
            nope = false;
            for (int k = 0; k < cards->size(); k++)
                if (card->getID() == cards->at(k)->getID()) {
                    nope = true;
                    continue;
                }
            if (!nope) cards->append(card);
        }
    }
        if (cards->size() != 5) {
            cards->clear();
            QList<Card*> availableCards;
            if (ui->actionBasisGame->isChecked()) {
                for (int k = 1; k <= 16; k++) {
                    Card *card = new Card;
                    card->setCardValues(k);
                    availableCards.append(card);
                }
            }
            if (ui->actionSenseisPath->isChecked()) {
                for (int k = 17; k <= 32; k++) {
                    Card *card = new Card;
                    card->setCardValues(k);
                    availableCards.append(card);
                }
            }
            if (ui->actionGoatSheep->isChecked()) {
                for (int k = 33; k <= 34; k++) {
                    Card *card = new Card;
                    card->setCardValues(k);
                    availableCards.append(card);
                }
            }
            for (int i = 0; i < 5; i++) cards->append(availableCards.takeAt(rand() % availableCards.size()));
        }
    return true;
}

void MainWindow::drawAxisLabeling() {
    if (ui->actionAxisLabeling->isChecked()) {
        bool flippedBoard = game->getFlippedBoard();
        for (int row = 0; row < 5; row++) {
            QGraphicsTextItem *label = new QGraphicsTextItem;
            if (flippedBoard) label->setPlainText(QString::number(row+1));
            else label->setPlainText(QString::number(5-row)); 
            if (ui->actionTinyWindow->isChecked() == false)
                label->setPos(borderX, borderY + fieldSize/2 + fieldSize*row - label->boundingRect().height()/2);
            else
                label->setPos(-2*borderX, borderY + fieldSize/2 + fieldSize*row - label->boundingRect().height()/2);
            axisLabel->append(label);
            scene->addItem(label);
        }
        for (int col = 0; col < 5; col++) {
            QGraphicsTextItem *label = new QGraphicsTextItem;
            if (flippedBoard) label->setPlainText(QString('e'-col));
            else label->setPlainText(QString('a'+col));
            label->setPos(borderX + axisLabelSize + fieldSize/2 + fieldSize*col - label->boundingRect().width()/2, 2*borderY + 5*fieldSize);
            axisLabel->append(label);
            scene->addItem(label);
        }
    }
}

void MainWindow::drawBackGroundPicture() {
    double size = fieldSize * 5;
    QGraphicsPixmapItem *back = new QGraphicsPixmapItem(QPixmap(":/pics/back.png"));
        back->setPixmap(back->pixmap().scaled(static_cast<int>(size), static_cast<int>(size)));
        back->setPos(borderX + axisLabelSize, borderY);
        scene->addItem(back);
    }

void MainWindow::drawBoard() {
    // drawing the board
    if (game->getBoard()->size() == 0) {
        QList<Field*> fieldsRow;
        for (int row = 0; row < 5; row++) {
            fieldsRow.clear();
            for (int col = 0; col < 5; col++) {
                // create field
                Field *field = new Field;
                field->setCol(col);
                field->setRow(row);
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
        for (int row = 0; row < 5; row++) {
            for (int col = 0; col < 5; col++) {
                Field *field = game->getBoard()->at(row).at(col);
                if (!game->getFlippedBoard()) field->setPos(borderX + axisLabelSize + fieldSize * col, borderY + fieldSize * (4-row));
                else field->setPos(borderX + axisLabelSize + fieldSize * (4-col), borderY + fieldSize * row);
                double size = game->getWindow()->getFieldSize();
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
        int maxSlots = 2;
        if (player == 0) maxSlots = 1;
        // draw cardslots
        for (int number = 0; number < maxSlots; number++) {
            if (game->getSlotsGrid()->size() < 3) {
                slot = new CardSlot;
                slot->setOwner(player);
                slot->assignCard(player, number);
            } else {
                if (game->getSlotsGrid()->at(2).size() < 2) {
                    slot = new CardSlot;
                    slot->setOwner(player);
                    slot->assignCard(player, number);
                } else slot = game->getSlotsGrid()->at(player).at(number);
            }
            slot->setRect(0, 0, slotWidth, slotHeight);
            double posX = scene->height() - MSWindowsCorrection + number * slotWidth + (number + 1) * borderX;
            double posY;
            if (game->getFlippedBoard()) posY = ((1.5 * player - 2.5) * player + 1) * slotHeight + ((1.5 * player - 2.5) * player + 2) * borderY;
            else posY = ((-1.5 * player + 2.5) * player + 1) * slotHeight + ((-1.5 * player + 2.5) * player + 2) * borderY;

            if (game->getCardChoiceActive()) {
                if ((game->getFirstPlayersTurn() && player == 1) || (!game->getFirstPlayersTurn() && player == 2)) {
                    posX = 0.1*windowWidth + 1.1*number*slotWidth;
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
        victim->drawPiece();
        double victimHeight = (windowHeight -MSWindowsCorrection -7*borderY -sideBarSize -turnRed->boundingRect().height() -turnBlue->boundingRect().height() -flipButton->boundingRect().height()) / 8;
        double victimWidth = victimHeight / victim->pixmap().height() * victim->pixmap().width();
        if (victim->getType() == 's') {
            victim->setPixmap(victim->pixmap().scaled(static_cast<int>(victimHeight), static_cast<int>(victimWidth)));
            victim->setPos(posXright - 1.5 * borderX, windowHeight - MSWindowsCorrection - 2*borderY - turnRed->boundingRect().height() - sideBarSize - axisLabelSize - victim->pixmap().height()*(i+1));
            delItem = false;
            for (int k = 0; k < scene->items().size(); k++)
                if (scene->items().at(k) == victim) delItem = true;
            if (delItem) scene->removeItem(victim);
            scene->addItem(victim);
        }
    }
    for (int i = 0; i < game->getCapturedRed()->count(); i++) {
        Piece *victim = game->getCapturedRed()->at(i);
        victim->drawPiece();
        double victimHeight = (windowHeight -MSWindowsCorrection -7*borderY -sideBarSize -turnRed->boundingRect().height() -turnBlue->boundingRect().height() -flipButton->boundingRect().height()) / 8;
        double victimWidth = victimHeight / victim->pixmap().height() * victim->pixmap().width();
        if (victim->getType() == 'S') {
            victim->setPixmap(victim->pixmap().scaled(static_cast<int>(victimHeight), static_cast<int>(victimWidth)));
            victim->setPos(posXright - 1.5 * borderX, 2*borderY + turnRed->boundingRect().height() + victim->pixmap().height()*i);
            delItem = false;
            for (int k = 0; k < scene->items().size(); k++)
                if (scene->items().at(k) == victim) delItem = true;
            if (delItem) scene->removeItem(victim);
            scene->addItem(victim);
        }
    }
}

QString MainWindow::generateNotationString(QString lastTurn, QString thisTurn) {
    lastTurn = lastTurn.remove(" ");
    thisTurn = thisTurn.remove(" ");
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
        if (usedPieces.at(2).at(0) == usedPieces.at(0).at(0)) notationString += usedPieces.at(0) + "x" + usedPieces.at(1).at(1) + usedPieces.at(1).at(2);
        else notationString += usedPieces.at(1) + "x" + usedPieces.at(0).at(1) + usedPieces.at(0).at(2);
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
        if (saveString != "") saveString += ", ";
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
    saveString += "| ";
    // set cards
    if (game->getSlotsGrid()->size() > 0) {
        for (int k = 0; k < 3; k++) {
            for (int l = 0; l < game->getSlotsGrid()->at(k).size(); l++) {
                if (k != 0) saveString += ", ";
                saveString += QString::number(game->getSlotsGrid()->at(k).at(l)->getCard()->getID());
            }
        }
    } else
        for (int k = 0; k < 5; k++) {
            if (k != 0) saveString += ", ";
            saveString += QString::number(game->getCards()->at(k)->getID());
        }
    return saveString;
}

void MainWindow::newGame(QString setupString) {
    game->setCardChoiceActive(false);
    if (setupString == "") {
        // reset settings
        game->getMatch()->setResult(0);
        game->getMatch()->setPlayerNameRed("Red");
        game->getMatch()->setPlayerNameBlue("Blue");
        game->getMatch()->setEvent("");
        game->getMatch()->setCity("");
        game->getMatch()->setRound(0);
        game->getMatch()->setDate(QDate::currentDate());
        setWindowTitle("Oni - Red vs. Blue - unsaved game");
        game->getMatch()->setOpenDatabaseGameNumber(-1);
        game->setCurrentDisplayedMove(0);
        if (game->getMatch()->getTurns()) game->getMatch()->getTurns()->clear();
    }

    // reset lists
    resetLists();

    // setup string
    const QString defaultString = "Sa1, Sb1, Mc1, Sd1, Se1, sa5, sb5, mc5, sd5, se5|";
    if (setupString == "" || setupString == "1-0" || setupString == "0-1") setupString = defaultString;
    if (!analyseSetupString(setupString)) {
        game->getMatch()->setResult(0);
        setupString = defaultString;
        resetLists();
        analyseSetupString(setupString);
    }
    if (game->getMatch()->getTurns()->size() == 0) game->getMatch()->getTurns()->append(generateSetupString());

    updateLayout();
}

void MainWindow::notateVictory(QString result) {
    QList<QString> *turns = game->getMatch()->getTurns();
    if ( "1-0" == turns->last() || "0-1" == turns->last()) {
        turns->removeLast();
        turns->append(result);
    }
    if ("1-0" == result) game->getMatch()->setResult(1);
    else if ("0-1" == result) game->getMatch()->setResult(-1);
    else game->getMatch()->setResult(0);
    game->getWindow()->prepareGame();
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
    setupNotation();
    game->writeConfig();
}

void MainWindow::readWindowConfig(QJsonObject &json) {
    if (json.contains("actionFlipEveryMove") && json["actionFlipEveryMove"].isBool())
        ui->actionFlipEveryMove->setChecked(json["actionFlipEveryMove"].toBool());
    if (json.contains("actionBasisGame") && json["actionBasisGame"].isBool())
        ui->actionBasisGame->setChecked(json["actionBasisGame"].toBool());
    if (json.contains("actionSenseisPath") && json["actionSenseisPath"].isBool())
        ui->actionSenseisPath->setChecked(json["actionSenseisPath"].toBool());
    if (json.contains("actionGoatSheep") && json["actionGoatSheep"].isBool())
        ui->actionGoatSheep->setChecked(json["actionGoatSheep"].toBool());
    if (json.contains("actionHideNotation") && json["actionHideNotation"].isBool())
        ui->actionHideNotation->setChecked(json["actionHideNotation"].toBool());
    if (json.contains("actionAxisLabeling") && json["actionAxisLabeling"].isBool())
        ui->actionAxisLabeling->setChecked(json["actionAxisLabeling"].toBool());
    if (json.contains("actionPiecesComicStyle") && json["actionPiecesComicStyle"].isBool())
        ui->actionPiecesComicStyle->setChecked(json["actionPiecesComicStyle"].toBool());
    if (json.contains("actionPiecesHanzi") && json["actionPiecesHanzi"].isBool())
        ui->actionPiecesHanzi->setChecked(json["actionPiecesHanzi"].toBool());
    if (json.contains("actionTinyWindow") && json["actionTinyWindow"].isBool())
        ui->actionTinyWindow->setChecked(json["actionTinyWindow"].toBool());
    if (json.contains("actionSmallWindow") && json["actionSmallWindow"].isBool())
        ui->actionSmallWindow->setChecked(json["actionSmallWindow"].toBool());
    if (json.contains("actionNormalWindow") && json["actionNormalWindow"].isBool())
        ui->actionNormalWindow->setChecked(json["actionNormalWindow"].toBool());
    if (json.contains("actionLargeWindow") && json["actionLargeWindow"].isBool())
        ui->actionLargeWindow->setChecked(json["actionLargeWindow"].toBool());
    if (json.contains("windowTitle") && json["windowTitle"].isString())
        setWindowTitle(json["windowTitle"].toString());
}

void MainWindow::resetLists() {
    ui->notation->clear();
    if (game->getBoard()) game->getBoard()->clear();
    if (game->getCards()) game->getCards()->clear();
    if (game->getPieces()) game->getPieces()->clear();
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

void MainWindow::saveTurnInNotation() {
    // refreshing of the notation if jumped back
    QString lastMove, thisMove;
    QList<QString> *turns = game->getMatch()->getTurns();
    int currentDisplayedMove = game->getCurrentDisplayedMove();
    if (turns->size() > (currentDisplayedMove+1))
         for(int i = turns->size(); i > currentDisplayedMove+1; i--) turns->removeLast();
    thisMove = generateSetupString();
    lastMove = turns->last();
    turns->append(thisMove);
    game->setCurrentDisplayedMove(currentDisplayedMove+1);
    ui->notation->scrollToBottom();
    setupNotation();
}

void MainWindow::saveWindowConfig(QJsonObject &json) const {
    json["windowTitle"] = this->windowTitle();
    json["actionFlipEveryMove"] = ui->actionFlipEveryMove->isChecked();
    json["actionBasisGame"] = ui->actionBasisGame->isChecked();
    json["actionSenseisPath"] = ui->actionSenseisPath->isChecked();
    json["actionGoatSheep"] = ui->actionGoatSheep->isChecked();
    json["actionHideNotation"] = ui->actionHideNotation->isChecked();
    json["actionAxisLabeling"] = ui->actionAxisLabeling->isChecked();
    json["actionPiecesComicStyle"] = ui->actionPiecesComicStyle->isChecked();
    json["actionPiecesHanzi"] = ui->actionPiecesHanzi->isChecked();
    json["actionTinyWindow"] = ui->actionTinyWindow->isChecked();
    json["actionSmallWindow"] = ui->actionSmallWindow->isChecked();
    json["actionNormalWindow"] = ui->actionNormalWindow->isChecked();
    json["actionLargeWindow"] = ui->actionLargeWindow->isChecked();
}

void MainWindow::setupNotation() {
    QList<QString> *turns = game->getMatch()->getTurns();
    QString lastTurn = turns->last();
    double posX = scene->height() - MSWindowsCorrection + slotWidth + 2*borderX + 2.5;
    double posY = slotHeight + 2*borderY + 2.5;
    ui->notation->setGeometry(static_cast<int>(posX), static_cast<int>(posY), static_cast<int>(slotWidth), static_cast<int>(slotHeight));
    ui->notation->clear();
    int maxLines = turns->size();
    if (turns->size() > 1 && (lastTurn == "1-0" || lastTurn == "0-1"))
        maxLines--;
    for (int i = 1; i < maxLines; i++) {
        QListWidgetItem *item;
        if (i%2 == 1) {
            item = new QListWidgetItem(QString::number(static_cast<int>(ui->notation->count()/2+1)) + ". "
                                       + generateNotationString(turns->at(i-1), turns->at(i)));
            item->setBackground(QBrush(QColor(200,55,55), Qt::Dense4Pattern));
            if (ui->actionTinyWindow->isChecked()) item->setFont(QFont("Arial", 6));
            else if (ui->actionSmallWindow->isChecked()) item->setFont(QFont("Arial", 9));
                 else if (ui->actionNormalWindow->isChecked()) item->setFont(QFont("Arial", 12));
                      else item->setFont(QFont("Arial", 15));
        }
        else {
            item = new QListWidgetItem(generateNotationString(turns->at(i-1), turns->at(i)));
            item->setBackground(QBrush(Qt::blue, Qt::Dense4Pattern));
            item->setTextAlignment(Qt::AlignRight);
            if (ui->actionTinyWindow->isChecked()) item->setFont(QFont("Arial", 6));
            else if (ui->actionSmallWindow->isChecked()) item->setFont(QFont("Arial", 9));
                 else if (ui->actionNormalWindow->isChecked()) item->setFont(QFont("Arial", 12));
                      else item->setFont(QFont("Arial", 15));
        }
        ui->notation->addItem(item);
    }
    if (lastTurn == "1-0" || lastTurn == "0-1")
        ui->notation->addItem(lastTurn);
    if (ui->actionHideNotation->isChecked())
        ui->notation->setVisible(false);
    else
        ui->notation->setVisible(true);
}

void MainWindow::updateLayout() {
    if (!scene || game->getMatch()->getOpenDatabaseGameNumber() == -1)
        setWindowTitle("Oni - Red vs. Blue - unsaved game");
    else
        setWindowTitle("Oni - " + game->getMatch()->getPlayerNameRed() + " vs. " + game->getMatch()->getPlayerNameBlue());

    // Change menu checkings
    double factor = 0.0;
    if (ui->actionFullScreen->isChecked())
        factor = 1.0;
    else
        if (ui->actionTinyWindow->isChecked())
            factor = 0.3;
        else
            if (ui->actionSmallWindow->isChecked())
                factor = 0.5;
            else
                if (ui->actionNormalWindow->isChecked())
                    factor = 0.7;
                else
                    factor = 0.9;

    // measure and fill available screen
    QRect desktop = screen->availableGeometry();
    if (scene && ui->actionAxisLabeling->isChecked())
        axisLabelSize = 30 * factor;
    else
        axisLabelSize = 0;
    borderX = 10 * factor;
    borderY = 10 * factor;
    sideBarSize = 40 * factor;
    // setting window size in dependency of available screen
    windowHeight = factor * desktop.height() +1;
    do {
        windowHeight--;
        slotWidth = (windowHeight - 4*borderY - MSWindowsCorrection)/3 * 5/6;
        windowWidth = windowHeight - MSWindowsCorrection + 3*borderX + 2*slotWidth;
    }
    while (windowWidth > desktop.width()-4 || windowHeight > desktop.height()-4);
    setGeometry(static_cast<int>(windowPosX), static_cast<int>(windowPosY), static_cast<int>(windowWidth)+5, static_cast<int>(windowHeight)+5);
    setFixedSize(static_cast<int>(windowWidth)+5, static_cast<int>(windowHeight)+5);
    slotHeight = slotWidth * 6/5;
    fieldSize = (windowHeight - MSWindowsCorrection - 2*borderY - sideBarSize - axisLabelSize) / 5;
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
    ui->view->setFixedSize(static_cast<int>(windowWidth)+5, static_cast<int>(windowHeight)+5);
    scene->setSceneRect(0, 0, windowWidth, windowHeight);
    if (sceneWasSetUp)
        prepareGame();
}

void MainWindow::moveEvent(QMoveEvent *event) {
   QMainWindow::moveEvent(event);
   windowPosX = geometry().x();
   windowPosY = geometry().y();
}

void MainWindow::showMove(QListWidgetItem *item) {
    if (item->text() != "1-0" && item->text() != "0-1") {
        for (int elem = 0; elem < ui->notation->count(); elem++) {
            if (ui->notation->item(elem) == item) {
                newGame(game->getMatch()->getTurns()->at(elem+1));
                game->setCurrentDisplayedMove(elem+1);
                game->getWindow()->updateLayout();
                ui->notation->scrollToItem(ui->notation->item(elem));
                return;
            }
        }
    }
}

void MainWindow::on_actionNew_triggered() {
    QMessageBox::StandardButton reply = QMessageBox::question(nullptr, "New game", "Do you want to start a new game?<br>All unsaved changes to the current game will be lost.");
    if (reply == QMessageBox::Yes)
        newGame();
}

void MainWindow::on_actionSetupPosition_triggered() {

}

void MainWindow::on_actionStartingPosition_triggered() {
    game->setCurrentDisplayedMove(0);
    ui->notation->scrollToTop();
    newGame(game->getMatch()->getTurns()->at(game->getCurrentDisplayedMove()));
}

void MainWindow::on_actionPreviousMove_triggered() {
    int displayedMove = game->getCurrentDisplayedMove();
    ui->notation->scrollToItem(ui->notation->item(displayedMove));
    if (game->getMatch()->getTurns()->size() > 1 && displayedMove > 0) {
        game->setCurrentDisplayedMove(displayedMove-1);
        newGame(game->getMatch()->getTurns()->at(game->getCurrentDisplayedMove()));
    }
}

void MainWindow::on_actionNextMove_triggered() {
    int displayedMove = game->getCurrentDisplayedMove();
    ui->notation->scrollToItem(ui->notation->item(displayedMove));
    if (game->getMatch()->getTurns()->size() > 1 && displayedMove < game->getMatch()->getTurns()->size()-1) {
        game->setCurrentDisplayedMove(displayedMove+1);
        newGame(game->getMatch()->getTurns()->at(game->getCurrentDisplayedMove()));
    }
}

void MainWindow::on_actionLastMove_triggered() {
    game->setCurrentDisplayedMove(game->getMatch()->getTurns()->size()-1);
    ui->notation->scrollToBottom();
    newGame(game->getMatch()->getTurns()->at(game->getCurrentDisplayedMove()));
}

void MainWindow::on_actionResign_triggered() {
    if (game->getMatch()->getResult() == 0 && !game->getCardChoiceActive()) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Resign match", "Do you really want to resign?", QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            if (game->getFirstPlayersTurn()) {
                game->getMatch()->setResult(-1);
                game->getWindow()->notateVictory("0-1");
            }
            else {
                game->getMatch()->setResult(1);
                game->getWindow()->notateVictory("1-0");
            }
            prepareGame();
        }
    }
}

void MainWindow::on_actionRedEasy_triggered() {}

void MainWindow::on_actionRedMedium_triggered() {}

void MainWindow::on_actionRedHard_triggered() {}

void MainWindow::on_actionBlueEasy_triggered() {}

void MainWindow::on_actionBlueMedium_triggered() {}

void MainWindow::on_actionBlueHard_triggered() {}

void MainWindow::on_actionBasisGame_triggered() {
    if (!ui->actionSenseisPath->isChecked())
        ui->actionBasisGame->setChecked(true);
    game->writeConfig();
}

void MainWindow::on_actionSenseisPath_triggered() {
    if (!ui->actionBasisGame->isChecked())
        ui->actionSenseisPath->setChecked(true);
    game->writeConfig();
}

void MainWindow::on_actionGoatSheep_triggered() {
    game->writeConfig();
}

void MainWindow::on_actionDatabase_triggered() {
    QFile databaseFile(game->databaseFileName);
    QFile backupFile(game->backupFileName);
    backupFile.remove();
    databaseFile.copy(game->backupFileName);
    openDatabase();
}

void MainWindow::on_actionFlipOnce_triggered() {
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
    game->writeConfig();
}

void MainWindow::on_actionAxisLabeling_triggered() {
    updateLayout();
}

void MainWindow::on_actionPiecesComicStyle_triggered() {
    if (ui->actionPiecesHanzi->isChecked())
        ui->actionPiecesHanzi->setChecked(false);
    game->setPiecesSet("ComicStyle");
    prepareGame();
}

void MainWindow::on_actionPiecesHanzi_triggered() {
    if (ui->actionPiecesComicStyle->isChecked())
        ui->actionPiecesComicStyle->setChecked(false);
    game->setPiecesSet("Hanzi");
    prepareGame();
}

void MainWindow::on_actionTinyWindow_triggered() {
    QMainWindow::showNormal();
    ui->actionFullScreen->setChecked(false);
    ui->actionTinyWindow->setChecked(true);
    ui->actionSmallWindow->setChecked(false);
    ui->actionNormalWindow->setChecked(false);
    ui->actionLargeWindow->setChecked(false);
    updateLayout();
}

void MainWindow::on_actionSmallWindow_triggered() {
    QMainWindow::showNormal();
    ui->actionFullScreen->setChecked(false);
    ui->actionTinyWindow->setChecked(false);
    ui->actionSmallWindow->setChecked(true);
    ui->actionNormalWindow->setChecked(false);
    ui->actionLargeWindow->setChecked(false);
    updateLayout();
}

void MainWindow::on_actionNormalWindow_triggered() {
    QMainWindow::showNormal();
    ui->actionFullScreen->setChecked(false);
    ui->actionTinyWindow->setChecked(false);
    ui->actionSmallWindow->setChecked(false);
    ui->actionNormalWindow->setChecked(true);
    ui->actionLargeWindow->setChecked(false);
    updateLayout();
}

void MainWindow::on_actionLargeWindow_triggered() {
    QMainWindow::showNormal();
    ui->actionFullScreen->setChecked(false);
    ui->actionTinyWindow->setChecked(false);
    ui->actionSmallWindow->setChecked(false);
    ui->actionNormalWindow->setChecked(false);
    ui->actionLargeWindow->setChecked(true);
    updateLayout();
}

void MainWindow::on_actionFullScreen_triggered(){
    if (ui->actionFullScreen->isChecked())
        QMainWindow::showFullScreen();
    else
        QMainWindow::showNormal();
    updateLayout();
}

void MainWindow::on_actionAboutQt_triggered() {
    QApplication::aboutQt();
}
