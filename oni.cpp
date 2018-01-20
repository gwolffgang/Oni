#include <QMessageBox>
#include <QTimer>
#include <QFontDatabase>
#include "oni.h"

extern Oni *game;

Oni::Oni() : window(new MainWindow), board(new QList<QList<Field*>>),
    pieces(new QList<Piece*>), capturedBlue(new QList<Piece*>), capturedRed(new QList<Piece*>),
    slotsGrid(new QList<QList<CardSlot*>>), cards(new QList<Card*>),
    openGameFileName(""), piecesSet("ComicStyle"), pickedUpPiece(NULL), fieldOfOrigin(NULL),
    actuallyDisplayedMove(0), firstPlayersTurn(true), flippedBoard(false), cardChoiceActive(false) {

    match.playerNameBlue = "Blue";
    match.playerNameRed = "Red";
    match.turns = new QList<QString>;
    match.gameResult = 0;

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

bool Oni::readConfig() {
    QFile configFile(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + QStringLiteral("/oni_cfg.json"));
    if (!configFile.open(QIODevice::ReadOnly)) { qWarning("Couldn't open config file (yet)."); return false; }
    QJsonDocument configDoc(QJsonDocument::fromJson(configFile.readAll()));
    QJsonObject configData = configDoc.object();
    if (configData.contains("flippedBoard") && configData["flippedBoard"].isBool())
        flippedBoard = configData["flippedBoard"].toBool();
    if (configData.contains("piecesSet") && configData["piecesSet"].isString())
        piecesSet = configData["piecesSet"].toString();
    window->readWindowConfig(configData);

    QFile databaseFile(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + QStringLiteral("/oni_save.json"));
    if (!databaseFile.open(QIODevice::ReadOnly)) { qWarning("Couldn't open database file (yet)."); return false; }
    QJsonDocument databaseDoc(QJsonDocument::fromJson(databaseFile.readAll()));
    QJsonObject databaseData = databaseDoc.object();
    QJsonObject gameData;
    if (databaseData.contains("tempGame") && databaseData["tempGame"].isObject()) {
        gameData = databaseData["tempGame"].toObject();
        if (gameData.contains("actuallyDisplayedMove") && gameData["actuallyDisplayedMove"].isDouble())
            actuallyDisplayedMove = gameData["actuallyDisplayedMove"].toInt();
        if (gameData.contains("firstPlayersTurn") && gameData["firstPlayersTurn"].isBool())
            firstPlayersTurn = gameData["firstPlayersTurn"].toBool();
        if (gameData.contains("openGameFileName") && gameData["openGameFileName"].isString())
            openGameFileName = gameData["openGameFileName"].toString();
        if (gameData.contains("playerNameBlue") && gameData["playerNameBlue"].isString())
            match.playerNameBlue = gameData["playerNameBlue"].toString();
        if (gameData.contains("playerNameRed") && gameData["playerNameRed"].isString())
            match.playerNameRed = gameData["playerNameRed"].toString();
        if (gameData.contains("turns") && gameData["turns"].isArray()) {
                QJsonArray turnsArray = gameData["turns"].toArray();
                match.turns->clear();
                for (int k = 0; k < turnsArray.size(); k++) match.turns->append(turnsArray[k].toString());
            }
    }
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
    match.gameResult = winner;
    QString victor;
    if (winner == 1) victor = game->getPlayerNameRed();
    else victor = game->getPlayerNameBlue();
    QMessageBox::StandardButton reply = QMessageBox::information(NULL, "VICTORY!", victor + " has won the game. Congratulations!", QMessageBox::Ok, QMessageBox::Save);
    if (reply == QMessageBox::Save) QTimer::singleShot( 1, game->getWindow(), SLOT(on_actionSave_triggered()) );
}

bool Oni::writeConfig() const {
    QFile configFile(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + QStringLiteral("/oni_cfg.json"));
    if (!configFile.open(QIODevice::WriteOnly)) { qWarning("Couldn't open config file."); return false; }
    QJsonObject jsonConfig;
        jsonConfig["flippedBoard"] = flippedBoard;
        jsonConfig["piecesSet"] = piecesSet;
        window->saveWindowConfig(jsonConfig);
    QJsonDocument configDoc(jsonConfig);
    configFile.write(configDoc.toJson());

    QFile databaseFile(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + QStringLiteral("/oni_save.json"));
    if (!databaseFile.open(QIODevice::ReadOnly)) { qWarning("Couldn't open config file."); return false; }
    QJsonDocument databaseDoc(QJsonDocument::fromJson(databaseFile.readAll()));
    QJsonObject databaseData = databaseDoc.object();
    QJsonArray gamesDatabase;
        if (databaseData.contains("Games") && databaseData["Games"].isArray())
            gamesDatabase = databaseData["Games"].toArray();
    databaseFile.close();
    if (!databaseFile.open(QIODevice::WriteOnly)) { qWarning("Couldn't open config file."); return false; }
    QJsonObject tempGame;
        tempGame["actuallyDisplayedMove"] = actuallyDisplayedMove;
        tempGame["firstPlayersTurn"] = firstPlayersTurn;
        tempGame["openGameFileName"] = openGameFileName;
        tempGame["playerNameBlue"] = match.playerNameBlue;
        tempGame["playerNameRed"] = match.playerNameRed;
        QJsonArray turnsArray;
            for (auto & turn : *match.turns) turnsArray.append(turn);
        tempGame["turns"] = turnsArray;
    QJsonObject newDatabaseData;
    newDatabaseData["Games"] = gamesDatabase;
    newDatabaseData["tempGame"] = tempGame;
    QJsonDocument newDatabaseDoc(newDatabaseData);
    databaseFile.write(newDatabaseDoc.toJson());

    return true;
}
