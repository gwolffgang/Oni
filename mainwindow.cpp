#include <QMessageBox>
#include <QTimer>
#include <iostream>
#include <time.h>

#include "mainwindow.h"
#include "oni.h"
#include "ui_mainwindow.h"

extern Oni *game;

int myrandom(int i) { srand(unsigned(time(NULL))); return std::rand()%i; }

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    // set up the UI
    ui->setupUi(this);
    scene = NULL;

    // presettings
    changeLayout(0.70);
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
    fieldSize = (scene->sceneRect().height() - 2*borderY - sideBarSize - axisLabelSize) / 5;
    slotSize = (scene->height() - 4*borderY -32) / 3;
    ui->view->setScene(scene);
    ui->view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->view->setFixedSize(windowWidth+5, windowHeight+5);
    ui->notation->setSelectionMode(QAbstractItemView::SingleSelection);
    axisLabel = new QList<QGraphicsTextItem*>;

    // dialog windows
    about = new AboutWindow(this);

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
        if (game->getCards()->size() != 5) {
            // clear cards list
            game->getCards()->clear();
            // determine random, unique cards
            int intArray[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
            srand(unsigned((NULL)));
            std::random_shuffle(&intArray[0], &intArray[16], myrandom);
            for (int i = 0; i < 5; i++) {
                Card *card = new Card;
                card->setCardValues(intArray[i]);
                game->getCards()->append(card);
            }
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
    windowWidth = windowHeight + 3*borderX + 2*((windowHeight - 4*borderY) / 3);
    while (windowWidth > desktop.width()-4) {
        windowHeight--;
        windowWidth = windowHeight + 3*borderX + 2*((windowHeight - 4*borderY) / 3);
    }
    setGeometry(0,0,windowWidth+5,windowHeight+5);
    setFixedSize(windowWidth+5, windowHeight+5);

    if (scene != NULL) {
        scene->setSceneRect(0, 0, windowWidth, windowHeight);
        ui->view->setFixedSize(windowWidth+5, windowHeight+5);
        fieldSize = ((scene->sceneRect().height() - 2*borderY - sideBarSize - axisLabelSize) / 5);
        slotSize = (scene->height() - 4*borderY -32) / 3;
        prepareGame();
    }
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
        for (int row = 0; row < game->getRows(); row++)
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
            double posX = scene->height() + number * slotSize + (number + 1) * borderX;
            double posY;
            if (game->getFlippedBoard())
                //  1->0, 0->1, 2->2
                posY = ((1.5 * player - 2.5) * player + 1) * slotSize + ((1.5 * player - 2.5) * player + 2) * borderY;
            else
                //   2->0, 0->1, 1->2
                posY = ((-1.5 * player + 2.5) * player + 1) * slotSize + ((-1.5 * player + 2.5) * player + 2) * borderY;

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
    double posXright = 0, posYbuttom = 0, posX = 0, posY = 0;
    // drawing in-game sidebar "right"
    posXright = this->height() - sideBarSize - borderX;
    posYbuttom = this->height() - sideBarSize - axisLabelSize - borderY;

    // FlipButton
    flipButton = new Button;
    flipButton->drawButton("flipButton", "right");
    flipButton->setPos(posXright, (this->height() - sideBarSize - axisLabelSize - flipButton->pixmap().height() ) /2);
    if (game->getFlippedBoard()) {
        flipButton->setTransformOriginPoint(flipButton->pixmap().width() / 2, flipButton->pixmap().height() / 2);
        flipButton->setRotation(180);
    }
    scene->addItem(flipButton);

    // TurnMarker
    turnRed = new Button;
    turnRed->drawButton("turnRed", "right");
    if (game->getFlippedBoard()) posY = borderY;
    else posY = (this->height() - sideBarSize - axisLabelSize - borderY - turnRed->pixmap().height() );
    turnRed->setPos(posXright, posY);

    turnBlue = new Button;
    turnBlue->drawButton("turnBlue", "right");
    if (game->getFlippedBoard()) posY = (this->height() - sideBarSize - axisLabelSize - borderY - turnBlue->pixmap().height() );
    else posY = borderY;
    turnBlue->setPos(posXright, posY);

    if (game->getFirstPlayersTurn()) scene->addItem(turnRed);
    else scene->addItem(turnBlue);

    // Captured pieces
    bool delItem = false;
    for (int i = 0; i < game->getCapturedBlue()->count(); i++) {
        Piece *victim = game->getCapturedBlue()->at(i);
        if (victim->getType() == 's') {
            victim->setPixmap(victim->pixmap().scaled(sideBarSize*2, sideBarSize*2 / victim->pixmap().width() * victim->pixmap().height()));
            victim->setPos(posXright - 2*borderX, windowHeight - 2*borderY - turnRed->boundingRect().height() - sideBarSize - axisLabelSize - victim->pixmap().height()*(i+1));
            delItem = false;
            for (int k = 0; k < scene->items().size(); k++)
                if (scene->items().at(k) == victim) delItem = true;
            if (delItem) scene->removeItem(victim);
            scene->addItem(victim);
        }
    }
    for (int i = 0; i < game->getCapturedRed()->count(); i++) {
        Piece *victim = game->getCapturedRed()->at(i);
        if (victim->getType() == 'S') {
            victim->setPixmap(victim->pixmap().scaled(game->getWindow()->getSideBarSize()*2, game->getWindow()->getSideBarSize()*2 / victim->pixmap().width() * victim->pixmap().height()));
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

void MainWindow::loadGame() {
    // load previously saved game
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

    // setup string
    if (setupString == "" || setupString == "1-0" || setupString == "0-1") setupString = "Sa1,Sb1,Mc1,Sd1,Se1,sa5,sb5,mc5,sd5,se5|";
    analyseSetupString(setupString);
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
    double size = (scene->height() - 32 - 4*borderY) / 3;
    double posX = scene->height() + size + 2*borderX;
    double posY = size + 2*borderY;
    ui->notation->scrollToBottom();
    ui->notation->setGeometry(posX, posY, size, size);
}

void MainWindow::prepareGame() {
    // Remove items from scene
    QList<QGraphicsItem*> items = scene->items();
    foreach(QGraphicsItem *item, items) scene->removeItem(item);

    // Generate and draw Items to the scene
    drawBoard();
    drawSideBar();
    drawAxisLabeling();
    drawCardSlots();
    positionNotation();
}

bool MainWindow::saveGame(const QString &fileName) {
    // overwrite current save game
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
        for (int i = 0; i < game->getTurns()->size(); i++)
            out << game->getTurns()->at(i) << endl;
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
    // save current game
    QFileDialog dialog(this, qApp->applicationDirPath().append("/saves"));
        dialog.setWindowModality(Qt::WindowModal);
        dialog.setAcceptMode(QFileDialog::AcceptSave);
        if (dialog.exec() != QDialog::Accepted)
            return false;
        return saveGame(dialog.selectedFiles().first());
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

void MainWindow::refreshNotation() {
    ui->notation->clear();
    int maxLines = game->getTurns()->size();
    if (game->getTurns()->size() > 0 && (game->getTurns()->last() == "1-0" || game->getTurns()->last() == "0-1")) maxLines--;
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
    if (game->getTurns()->last() == "1-0" || game->getTurns()->last() == "0-1") notateVictory(game->getTurns()->last());
}

void MainWindow::showMove(QListWidgetItem *item) {
    for (int i = 0; i < ui->notation->count(); i++) {
        if (ui->notation->item(i) == item) {
            newGame(game->getTurns()->at(i+1));
            game->setActuallyDisplayedMove(i+1);
        }
    }
}

void MainWindow::on_actionNew_triggered() {
    QMessageBox::StandardButton reply = QMessageBox::question(NULL, "New game", "Do you want to play a new game?<br>All unsaved changes to the actual game will be lost.");
    if (reply == QMessageBox::Yes) newGame();
}

void MainWindow::on_actionSetupPosition_triggered() {

}

void MainWindow::on_actionLoad_triggered() {
    QMessageBox::StandardButton reply = QMessageBox::question(NULL, "New game", "Do you want to load another game?<br>All unsaved changes to the actual game will be lost.");
    if (reply == QMessageBox::Yes) loadGame();
}

void MainWindow::on_actionSave_triggered() {
    // menubar option
    bool success = false;
    if (game->getOpenGameFileName() != "") success = saveGame(game->getOpenGameFileName());
    else success = saveGameAs();
}

void MainWindow::on_actionLastMove_triggered() {
    if (ui->notation->count() > 1 && ui->notation->currentRow() > 1) showMove(ui->notation->item(ui->notation->currentRow()-1));
}

void MainWindow::on_actionNextMove_triggered() {
    if (ui->notation->count() > 1 && ui->notation->currentRow() < ui->notation->count()) showMove(ui->notation->item(ui->notation->currentRow()+1));
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

void MainWindow::on_actionRedMedium_triggered() {

}

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
    if (ui->actionTinyLayout->isChecked()) changeLayout(0.30);
    if (ui->actionSmallLayout->isChecked()) changeLayout(0.50);
    if (ui->actionNormalLayout->isChecked()) changeLayout(0.70);
    if (ui->actionLargeLayout->isChecked()) changeLayout(0.90);
}

void MainWindow::on_actionFullScreen_triggered(){
    // menubar option
    if (ui->actionFullScreen->isChecked()) {
        ui->actionFullScreen->setChecked(true);
        QMainWindow::showFullScreen();
        changeLayout(1.0);
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
