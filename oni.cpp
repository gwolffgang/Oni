#include <QMessageBox>
#include <QTimer>
#include <QFontDatabase>
#include "oni.h"

extern Oni *game;

bool fileExists(QString filename) {
    QFileInfo check_file(filename);
    return check_file.exists() && check_file.isFile();
}

Oni::Oni() :
    backupFileName(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + QStringLiteral("/oni_backup.json")),
    configFileName(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + QStringLiteral("/oni_cfg.json")),
    databaseFileName(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + QStringLiteral("/oni_save.json")),
    tempDataFileName(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + QStringLiteral("/oni_temp.json")),
    window(new MainWindow), windowDatabase(nullptr), board(new QList<QList<Field*>>),
    pieces(new QList<Piece*>), capturedBlue(new QList<Piece*>), capturedRed(new QList<Piece*>),
    slotsGrid(new QList<QList<CardSlot*>>), cards(new QList<Card*>),
    piecesSet("ComicStyle"), pickedUpPiece(nullptr), fieldOfOrigin(nullptr), currentDisplayedMove(0),
    flippedBoard(false), cardChoiceActive(false) {

    match.openDatabaseGameNumber = -1;
    match.playerNameBlue = "Blue";
    match.playerNameRed = "Red";
    match.turns = new QList<QString>;
    match.gameResult = 0;
    match.event = "";
    match.city = "";
    match.date = QDate::currentDate();
    match.round = 0;

    // create AppData folder and needed files, if not done yet
    QDir appDataFolder = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QFile backupFile(backupFileName);
    QFile databaseFile(databaseFileName);
    QFile tempFile(tempDataFileName);
    if (!appDataFolder.exists())
        appDataFolder.mkpath(".");
    if (!fileExists(databaseFileName))
        databaseFile.open(QIODevice::WriteOnly);
    if (!fileExists(tempDataFileName))
        tempFile.open(QIODevice::WriteOnly);
    if (backupFile.exists()) {
        if(databaseFile.size() == 0) {
            databaseFile.remove();
            backupFile.copy(databaseFileName);
        }
        backupFile.remove();
    }
    databaseFile.copy(backupFileName);
    if (fileExists(configFileName))
        readConfig();

    // seed up randomizer
    srand(unsigned(time(nullptr)));

    QFontDatabase::addApplicationFont(":/fonts/Amburegul.ttf");
    QFontDatabase::addApplicationFont(":/fonts/wts11.ttf");

    window->show();
}

bool Oni::readConfig() {
    QFile configFile(configFileName);
    if (!configFile.open(QIODevice::ReadOnly)) { qWarning("readConfig: Couldn't open config file for reading."); return false; }
        QJsonDocument configDoc(QJsonDocument::fromJson(configFile.readAll()));
        QJsonObject configData = configDoc.object();
        if (configData.contains("flippedBoard") && configData["flippedBoard"].isBool())
            flippedBoard = configData["flippedBoard"].toBool();
        if (configData.contains("piecesSet") && configData["piecesSet"].isString())
            piecesSet = configData["piecesSet"].toString();
        window->readWindowConfig(configData);
    configFile.close();

    QFile tempFile(tempDataFileName);
    if (!tempFile.open(QIODevice::ReadOnly)) { qWarning("readConfig: Couldn't open tempFile file for reading."); return false; }
        QJsonDocument tempFileDoc(QJsonDocument::fromJson(tempFile.readAll()));
        QJsonObject tempFileData = tempFileDoc.object();
        QJsonObject gameData;
        if (tempFileData.contains("tempGame") && tempFileData["tempGame"].isObject()) {
            gameData = tempFileData["tempGame"].toObject();
            if (gameData.contains("currentDisplayedMove") && gameData["currentDisplayedMove"].isDouble())
                currentDisplayedMove = gameData["currentDisplayedMove"].toInt();
            if (gameData.contains("openDatabaseGameNumber") && gameData["openDatabaseGameNumber"].isDouble())
                match.openDatabaseGameNumber = gameData["openDatabaseGameNumber"].toInt();
            if (gameData.contains("playerNameBlue") && gameData["playerNameBlue"].isString())
                match.playerNameBlue = gameData["playerNameBlue"].toString();
            if (gameData.contains("playerNameRed") && gameData["playerNameRed"].isString())
                match.playerNameRed = gameData["playerNameRed"].toString();
            if (gameData.contains("gameResult") && gameData["gameResult"].isDouble())
                match.gameResult = gameData["gameResult"].toInt();
            if (gameData.contains("event") && gameData["event"].isString())
                match.event = gameData["event"].toString();
            if (gameData.contains("city") && gameData["city"].isString())
                match.city = gameData["city"].toString();
            if (gameData.contains("date") && gameData["date"].isString())
                match.date.fromString(gameData["date"].toString(),"YY.MM.dd");
            if (gameData.contains("round") && gameData["round"].isDouble())
                match.round = gameData["round"].toDouble();
            if (gameData.contains("turns") && gameData["turns"].isArray()) {
                    QJsonArray turnsArray = gameData["turns"].toArray();
                    match.turns->clear();
                    for (int k = 0; k < turnsArray.size(); k++) match.turns->append(turnsArray[k].toString());
                }
        }
    tempFile.close();
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
    Card *temporary = nullptr;
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
    QMessageBox::StandardButton reply = QMessageBox::information(nullptr, "VICTORY!", victor + " has won the game. Congratulations!", QMessageBox::Ok, QMessageBox::Save);
    if (reply == QMessageBox::Save) QTimer::singleShot( 1, game->getWindow(), SLOT(on_actionSave_triggered()) );
}

bool Oni::writeConfig() {
    QFile configFile(configFileName);
    if (!configFile.open(QIODevice::WriteOnly)) { qWarning("writeConfig: Couldn't open config file for writing."); return false; }
        QJsonObject jsonConfig;
            jsonConfig["flippedBoard"] = flippedBoard;
            jsonConfig["piecesSet"] = piecesSet;
            window->saveWindowConfig(jsonConfig);
        QJsonDocument configDoc(jsonConfig);
        configFile.write(configDoc.toJson());
    configFile.close();

    QFile tempFile(tempDataFileName);
    if (!tempFile.open(QIODevice::ReadOnly)) { qWarning("writeConfig: Couldn't open tempFile file for reading."); return false; }
        QJsonDocument tempFileDoc(QJsonDocument::fromJson(tempFile.readAll()));
        QJsonObject tempFileData = tempFileDoc.object();
        QJsonArray copyGames;
            if (tempFileData.contains("copyGames") && tempFileData["copyGames"].isArray())
                copyGames = tempFileData["copyGames"].toArray();
    tempFile.close();
    if (!tempFile.open(QIODevice::WriteOnly)) { qWarning("writeConfig: Couldn't open tempFile file for writing."); return false; }
        QJsonObject tempGame;
            tempGame["currentDisplayedMove"] = currentDisplayedMove;
            tempGame["openDatabaseGameNumber"] = match.openDatabaseGameNumber;
            tempGame["playerNameBlue"] = match.playerNameBlue;
            tempGame["playerNameRed"] = match.playerNameRed;
            tempGame["gameResult"] = match.gameResult;
            tempGame["event"] = match.event;
            tempGame["city"] = match.city;
            tempGame["round"] = match.round;
            tempGame["date"] = match.date.toString();
            QJsonArray turnsArray;
                for (auto & turn : *match.turns) turnsArray.append(turn);
            tempGame["turns"] = turnsArray;
        QJsonObject newTempFileData;
        if (copyGames.size() > 0) newTempFileData["copyGames"] = copyGames;
        newTempFileData["tempGame"] = tempGame;
        QJsonDocument newTempFileDoc(newTempFileData);
        tempFile.write(newTempFileDoc.toJson());
    tempFile.close();
    return true;
}
