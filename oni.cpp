#include <QMessageBox>
#include <QTimer>
#include <QFontDatabase>
#include "oni.h"

extern Oni *game;

Oni::Oni() : window(new MainWindow), board(new QList<QList<Field*>>),
    pieces(new QList<Piece*>), capturedBlue(new QList<Piece*>), capturedRed(new QList<Piece*>),
    slotsGrid(new QList<QList<CardSlot*>>), cards(new QList<Card*>), turns(new QList<QString>),
    playerNameRed("Red"), playerNameBlue("Blue"), openGameFileName(""), pickedUpPiece(NULL), fieldOfOrigin(NULL), rows(5), cols(5),
    cardsPerPlayer(2), neutralCardsPerGame(1), actuallyDisplayedMove(0), gameResult(0),
    firstPlayersTurn(true), flippedBoard(false), cardChoiceActive(false), piecesSet("ComicStyle") {

    readConfig();

    // create AppData folder, if not done yet
    QDir appDataFolder = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    if (!appDataFolder.exists()) appDataFolder.mkpath("./saves");

    // seed up randomizer
    srand(unsigned(time(NULL)));

    QFontDatabase::addApplicationFont(":/fonts/Amburegul.ttf");
    QFontDatabase::addApplicationFont(":/fonts/wts11.ttf");

    window->show();
}

void Oni::setPlayerNames(QString nameRed, QString nameBlue) {
    if (nameBlue != "") playerNameBlue = nameBlue;
    if (nameRed != "") playerNameRed = nameRed;
}

bool Oni::readConfig() {
    QFile configFile(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + QStringLiteral("/oni_cfg.json"));
    if (!configFile.open(QIODevice::ReadOnly)) { qWarning("Couldn't open config file."); return false; }
    QByteArray saveData = configFile.readAll();
    QJsonDocument jsonDoc(QJsonDocument::fromJson(saveData));
    QJsonObject json = jsonDoc.object();
    if (json.contains("actuallyDisplayedMove") && json["actuallyDisplayedMove"].isDouble())
        actuallyDisplayedMove = json["actuallyDisplayedMove"].toInt();
    if (json.contains("flippedBoard") && json["flippedBoard"].isBool())
        flippedBoard = json["flippedBoard"].toBool();
    if (json.contains("firstPlayersTurn") && json["firstPlayersTurn"].isBool())
        firstPlayersTurn = json["firstPlayersTurn"].toBool();
    if (json.contains("openGameFileName") && json["openGameFileName"].isString())
        openGameFileName = json["openGameFileName"].toString();
    if (json.contains("piecesSet") && json["piecesSet"].isString())
        playerNameBlue = json["piecesSet"].toString();
    if (json.contains("playerNameBlue") && json["playerNameBlue"].isString())
        playerNameBlue = json["playerNameBlue"].toString();
    if (json.contains("playerNameRed") && json["playerNameRed"].isString())
        playerNameRed = json["playerNameRed"].toString();
    if (json.contains("turns") && json["turns"].isArray()) {
            QJsonArray turnsArray = json["turns"].toArray();
            turns->clear();
            for (int k = 0; k < turnsArray.size(); k++) turns->append(turnsArray[k].toString());
        }
    window->readWindowConfig(json);
    return true;
}

QList<Card*> Oni::identifyCards(int owner) {
    QList<Card*> list;
    int slotsSize = slotsGrid->at(owner).size();
    for (int k = 0; k < slotsSize; k++) {
        list.append(slotsGrid->at(owner).at(k)->getCard());
    }
    return list;
}

void Oni::switchCards(CardSlot *usedCardSlot) {
    Card *temporary = NULL;
    // switch cards
    temporary = usedCardSlot->getCard();
    usedCardSlot->setCard(game->getSlotsGrid()->at(0).at(0)->getCard());
    game->getSlotsGrid()->at(0).at(0)->setCard(temporary);
}

void Oni::winGame(int winner) {
    gameResult = winner;
    QString victor;
    if (winner == 1) victor = game->getPlayerNameRed();
    else victor = game->getPlayerNameBlue();
    QMessageBox::StandardButton reply = QMessageBox::information(NULL, "VICTORY!", victor + " has won the game. Congratulations!", QMessageBox::Ok, QMessageBox::Save);
    if (reply == QMessageBox::Save) QTimer::singleShot( 1, game->getWindow(), SLOT(on_actionSave_triggered()) );
}

bool Oni::writeConfig() const {
    QFile configFile(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + QStringLiteral("/oni_cfg.json"));
    if (!configFile.open(QIODevice::WriteOnly)) { qWarning("Couldn't open config file."); return false; }
    QJsonObject json;
        json["actuallyDisplayedMove"] = actuallyDisplayedMove;
        json["flippedBoard"] = flippedBoard;
        json["firstPlayersTurn"] = firstPlayersTurn;
        json["openGameFileName"] = openGameFileName;
        json["piecesSet"] = piecesSet;
        json["playerNameBlue"] = playerNameBlue;
        json["playerNameRed"] = playerNameRed;
        QJsonArray turnsArray;
        for (auto & turn : *turns) turnsArray.append(turn);
        json["turns"] = turnsArray;
        window->saveWindowConfig(json);
    QJsonDocument configDoc(json);
    configFile.write(configDoc.toJson());
    return true;
}
