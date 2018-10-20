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
    window(new MainWindow), windowDatabase(nullptr), piecesSet("ComicStyle"), currentDisplayedMove(0),
    flippedBoard(false), cardChoiceActive(false), match(new Match) {

    // create AppData folder and needed files, if not done yet
    setupFoldersAndFiles();
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
                match->setOpenDatabaseGameNumber(gameData["openDatabaseGameNumber"].toInt());
            if (gameData.contains("playerNameBlue") && gameData["playerNameBlue"].isString())
                match->setPlayerNameBlue(gameData["playerNameBlue"].toString());
            if (gameData.contains("playerNameRed") && gameData["playerNameRed"].isString())
                match->setPlayerNameRed(gameData["playerNameRed"].toString());
            if (gameData.contains("result") && gameData["result"].isDouble())
                match->setResult(gameData["result"].toInt());
            if (gameData.contains("event") && gameData["event"].isString())
                match->setEvent(gameData["event"].toString());
            if (gameData.contains("city") && gameData["city"].isString())
                match->setCity(gameData["city"].toString());
            if (gameData.contains("date") && gameData["date"].isString()) {
                QString readDateString = gameData["date"].toString();
                if (readDateString != "") {
                    QDate datum;
                    match->setDate(datum.fromString(readDateString, "dd.MM.yyyy"));
                }
            }
            if (gameData.contains("round") && gameData["round"].isDouble())
                match->setRound(gameData["round"].toDouble());
            if (gameData.contains("turns") && gameData["turns"].isArray()) {
                    QJsonArray turnsArray = gameData["turns"].toArray();
                    match->getTurns()->clear();
                    for (int k = 0; k < turnsArray.size(); k++) match->getTurns()->append(turnsArray[k].toString());
                }
        }
    tempFile.close();
    return true;
}

void Oni::setupFoldersAndFiles() {
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
        if (databaseFile.size() == 0) {
            databaseFile.remove();
            backupFile.copy(databaseFileName);
        }
        backupFile.remove();
    }
    databaseFile.copy(backupFileName);
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
            tempGame["openDatabaseGameNumber"] = match->getOpenDatabaseGameNumber();
            tempGame["playerNameBlue"] = match->getPlayerNameBlue();
            tempGame["playerNameRed"] = match->getPlayerNameRed();
            tempGame["result"] = match->getResult();
            tempGame["event"] = match->getEvent();
            tempGame["city"] = match->getCity();
            tempGame["round"] = match->getRound();
            tempGame["date"] = match->getDate().toString();
            QJsonArray turnsArray;
                for (auto & turn : *match->getTurns())
                    turnsArray.append(turn);
            tempGame["turns"] = turnsArray;
        QJsonObject newTempFileData;
        if (copyGames.size() > 0)
            newTempFileData["copyGames"] = copyGames;
        newTempFileData["tempGame"] = tempGame;
        QJsonDocument newTempFileDoc(newTempFileData);
        tempFile.write(newTempFileDoc.toJson());
    tempFile.close();
    return true;
}
