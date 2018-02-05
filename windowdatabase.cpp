#include "oni.h"
#include "windowdatabase.h"
#include "ui_windowdatabase.h"

extern Oni *game;

WindowDatabase::WindowDatabase(QWidget *parent) : QWidget(parent), ui(new Ui::WindowDatabase),
    dialogSave(NULL), windowPosX(50), windowPosY(50), modelCols(8), modelRows(0),
    model(new QStandardItemModel(modelRows, modelCols, this)), gamesArray({}), tempGame({}) {
    ui->setupUi(this);
    ui->GamesTable->setSelectionBehavior(QAbstractItemView::SelectRows);

    setWindowTitle("Oni - Games Database");
    loadGames();
    updateLayout();
    show();
}

WindowDatabase::~WindowDatabase() {
    delete ui;
}

void WindowDatabase::closeDatabase() {
    QFile databaseFile(game->databaseFileName);
    QFile backupFile(game->backupFileName);
    backupFile.remove();
    databaseFile.copy(game->backupFileName);
    close();
}

bool WindowDatabase::editGame(QModelIndex index) {
    QFile databaseFile(game->databaseFileName);
    QJsonObject match;
    if (!databaseFile.open(QIODevice::ReadOnly)) { qWarning("editGame: Couldn't open database file for reading."); return false; }
        QJsonDocument databaseDoc(QJsonDocument::fromJson(databaseFile.readAll()));
        QJsonObject databaseData = databaseDoc.object();
        QJsonArray gamesArray;
            if (databaseData.contains("games") && databaseData["games"].isArray())
                gamesArray = databaseData["games"].toArray();
        QJsonObject tempGame;
            if (databaseData.contains("tempGame") && databaseData["tempGame"].isObject())
                tempGame = databaseData["tempGame"].toObject();
    databaseFile.close();
    dialogSave = new DialogSave(this);
    dialogSave->setData(gamesArray.at(index.row()).toObject());
    if (dialogSave->exec() == QDialog::Accepted) {
        QList<QString> data = dialogSave->getValues();
        match["playerNameRed"] = data.at(0);
        match["playerNameBlue"] = data.at(1);
        match["event"] = data.at(2);
        match["city"] = data.at(3);
        match["round"] = data.at(4).toDouble();
        match["date"] = data.at(5);
        match["gameResult"] = data.at(6).toInt();
        if (data.size() > 7) {
            if (data.last() == "1-0" || data.last() == "0-1") data.removeLast();
            QJsonArray turnsArray = {};
            for (int k = 7; k < data.size(); k++) turnsArray.append(data.at(k));
            if (data.at(6) != "*") data.append(data.at(6));
            match["turns"] = turnsArray;
        }
    } else return false;
    if (!databaseFile.open(QIODevice::WriteOnly)) { qWarning("editGame: Couldn't open database file for writing."); return false; }
        gamesArray.removeAt(index.row());
        gamesArray.insert(index.row(), match);
        QJsonObject newDatabaseData;
        newDatabaseData["games"] = gamesArray;
        newDatabaseData["tempGame"] = tempGame;
        QJsonDocument newDatabaseDoc(newDatabaseData);
        databaseFile.write(newDatabaseDoc.toJson());
    databaseFile.close();
    this->updateLayout();
    return true;
}

bool WindowDatabase::loadGames() {
    if (gamesArray.size() == 0) {
        QFile databaseFile(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + QStringLiteral("/oni_save.json"));
            if (!databaseFile.open(QIODevice::ReadOnly)) { qWarning("loadGames: Couldn't open database file for reading."); return false; }
            QJsonDocument databaseDoc(QJsonDocument::fromJson(databaseFile.readAll()));
            QJsonObject databaseData = databaseDoc.object();
            if (databaseData.contains("tempGame") && databaseData["tempGame"].isObject())
                tempGame = databaseData["tempGame"].toObject();
            if (databaseData.contains("games") && databaseData["games"].isArray())
                gamesArray = databaseData["games"].toArray();
        databaseFile.close();
    }
    model->setRowCount(gamesArray.size());
    for (int row = 0; row < gamesArray.size(); row++) {
        QJsonObject game = gamesArray.at(row).toObject();
        if (game.contains("playerNameRed") && game["playerNameRed"].isString())
            setCell(row, colRed, game["playerNameRed"].toString());
        if (game.contains("playerNameBlue") && game["playerNameBlue"].isString())
            setCell(row, colBlue, game["playerNameBlue"].toString());
        if (game.contains("gameResult") && game["gameResult"].isDouble()) {
            switch (game["gameResult"].toInt()) {
            case -1: setCell(row, colResult, "0-1"); break;
            case 0: setCell(row, colResult, "*"); break;
            case 1: setCell(row, colResult, "1-0");
            }
        }
        if (game.contains("turns") && game["turns"].isArray())
            setCell(row, colMoves, game["turns"].toArray().size() / 2);
        if (game.contains("event") && game["event"].isString())
            setCell(row, colEvent, game["event"].toString());
        if (game.contains("city") && game["city"].isString())
            setCell(row, colCity, game["city"].toString());
        if (game.contains("date") && game["date"].isString())
            setCell(row, colDate, game["date"].toString());
    }
    return true;
}

void WindowDatabase::openGame(QModelIndex index) {
    int selectedRow = index.row();
    QJsonObject gameData = gamesArray.at(selectedRow).toObject();
    if (gameData.contains("playerNameBlue") && gameData["playerNameBlue"].isString() &&
        gameData.contains("playerNameRed") && gameData["playerNameRed"].isString())
        game->setPlayerNames(gameData["playerNameRed"].toString(), gameData["playerNameBlue"].toString());
    if (gameData.contains("gameResult") && gameData["gameResult"].isDouble())
        game->setGameResult(gameData["gameResult"].toInt());
    if (gameData.contains("turns") && gameData["turns"].isArray()) {
        QJsonArray turnsArray = gameData["turns"].toArray();
        game->getTurns()->clear();
        for (int k = 0; k < turnsArray.size(); k++) game->getTurns()->append(turnsArray[k].toString());
    }

    if (game->getTurns()->last() == "1-0" || game->getTurns()->last() == "0-1")
        game->getWindow()->newGame(game->getTurns()->at( game->getTurns()->size()-2) );
    else game->getWindow()->newGame(game->getTurns()->last());
    game->setActuallyDisplayedMove(game->getTurns()->size());
    game->setOpenDatabaseGameNumber(selectedRow);
    game->getWindow()->prepareGame();
}

bool WindowDatabase::saveChanges() const {
    QFile databaseFile(game->databaseFileName);
    if (!databaseFile.open(QIODevice::WriteOnly)) { qWarning("saveChanges: Couldn't open database file for writing."); return false; }
        QJsonObject newDatabaseData;
        newDatabaseData["games"] = gamesArray;
        newDatabaseData["tempGame"] = tempGame;
        QJsonDocument newDatabaseDoc(newDatabaseData);
        databaseFile.write(newDatabaseDoc.toJson());
    return true;
}

bool WindowDatabase::saveGame(bool newSave) {
    QFile databaseFile(game->databaseFileName);
    if (!databaseFile.open(QIODevice::ReadOnly)) { qWarning("saveGameInDatabase: Couldn't open database file for reading."); return false; }
        QJsonDocument databaseDoc(QJsonDocument::fromJson(databaseFile.readAll()));
        QJsonObject databaseData = databaseDoc.object();
        QJsonArray gamesArray;
            if (databaseData.contains("games") && databaseData["games"].isArray())
                gamesArray = databaseData["games"].toArray();
        QJsonObject tempGame;
            if (databaseData.contains("tempGame") && databaseData["tempGame"].isObject())
                tempGame = databaseData["tempGame"].toObject();
    databaseFile.close();
    if (newSave == true || game->getOpenDatabaseGameNumber() == -1) {
        dialogSave = new DialogSave(this);
        if (dialogSave->exec() == QDialog::Accepted) {
            QList<QString> data = dialogSave->getValues();
            game->setPlayerNames(data.at(0), data.at(1));
            game->setEvent(data.at(2));
            game->setCity(data.at(3));
            game->setRound(data.at(4).toDouble());
            game->setDate(data.at(5));
            game->setGameResult(data.at(6));
            game->getWindow()->notateVictory(data.at(6));
            game->setOpenDatabaseGameNumber(gamesArray.size());
            game->getWindow()->prepareGame();
        } else return false;
    }
    if (!databaseFile.open(QIODevice::WriteOnly)) { qWarning("saveGameInDatabase: Couldn't open database file for writing."); return false; }
        QJsonObject match;
            match["playerNameRed"] = game->getPlayerNameRed();
            match["playerNameBlue"] = game->getPlayerNameBlue();
            match["event"] = game->getEvent();
            match["city"] = game->getCity();
            match["round"] = game->getRound();
            match["date"] = game->getDate().toString();
            match["gameResult"] = game->getGameResult();
            QJsonArray turnsArray;
            for (auto & turn : *game->getTurns()) turnsArray.append(turn);
            match["turns"] = turnsArray;
        if (game->getOpenDatabaseGameNumber() == gamesArray.size()) gamesArray.append(match);
        else {
            gamesArray.removeAt(game->getOpenDatabaseGameNumber());
            gamesArray.insert(game->getOpenDatabaseGameNumber(), match);
        }

        QJsonObject newDatabaseData;
        newDatabaseData["games"] = gamesArray;
        newDatabaseData["tempGame"] = tempGame;

        QJsonDocument newDatabaseDoc(newDatabaseData);
        databaseFile.write(newDatabaseDoc.toJson());
    databaseFile.close();
    this->updateLayout();
    game->getWindow()->updateLayout();
    return true;
}

void WindowDatabase::setCell(int row, column col, const QVariant &value, Qt::AlignmentFlag alignment) {
    QModelIndex index = model->index(row, col, QModelIndex());
    model->setData(index, alignment, Qt::TextAlignmentRole);
    model->setData(index, value);
}

void WindowDatabase::updateLayout() {
    // name table headers
    model->setHeaderData(colRed, Qt::Horizontal, "Red");
    model->setHeaderData(colBlue, Qt::Horizontal, "Blue");
    model->setHeaderData(colResult, Qt::Horizontal, "Result");
    model->setHeaderData(colMoves, Qt::Horizontal, "Moves");
    model->setHeaderData(colCards, Qt::Horizontal, "Cards");
    model->setHeaderData(colEvent, Qt::Horizontal, "Event");
    model->setHeaderData(colCity, Qt::Horizontal, "City");
    model->setHeaderData(colDate, Qt::Horizontal, "Date");

    QTableView *table = ui->GamesTable;
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setModel(model);
    table->resizeColumnsToContents();
    table->resizeRowsToContents();

    QRect desktop = QGuiApplication::primaryScreen()->availableGeometry();
    int windowMaxHeight = 0.8 * desktop.height();
    int windowMaxWidth = 0.8 * desktop.width();
    int buttonsWidth = ui->openGame->width();
    int buttonsHeight = ui->openGame->height();
    double tableWidth = std::min(std::max(table->horizontalHeader()->length() + table->verticalHeader()->width(), 3 * buttonsWidth), windowMaxWidth);
    double tableHeight = std::min(std::max(table->verticalHeader()->length() + table->horizontalHeader()->height(), 5 * buttonsHeight), windowMaxHeight);
    double windowWidth = tableWidth + buttonsWidth;
    double windowHeight = tableHeight + 1.8 * buttonsHeight;
    table->setGeometry(0, 0, tableWidth, tableHeight);
    table->setFixedSize(tableWidth+2, tableHeight+2);

    // position buttons
    ui->filterGames->setGeometry(0, tableHeight, buttonsWidth, buttonsHeight);
    ui->importGames->setGeometry(buttonsWidth, tableHeight, buttonsWidth, buttonsHeight);
    ui->exportGames->setGeometry(2 * buttonsWidth, tableHeight, buttonsWidth, buttonsHeight);
    ui->openGame->setGeometry(tableWidth, 0, buttonsWidth, buttonsHeight);
    ui->editGame->setGeometry(tableWidth, buttonsHeight, buttonsWidth, buttonsHeight);
    ui->deleteGame->setGeometry(tableWidth, 1.8 * buttonsHeight, buttonsWidth, buttonsHeight);
    ui->copyGame->setGeometry(tableWidth, 3 * buttonsHeight, buttonsWidth, buttonsHeight);
    ui->pasteGame->setGeometry(tableWidth, 3.8 * buttonsHeight, buttonsWidth, buttonsHeight);
    ui->undoChanges->setGeometry(tableWidth, tableHeight, buttonsWidth, buttonsHeight);
    ui->close->setGeometry(tableWidth, tableHeight + 0.8 * buttonsHeight, buttonsWidth, buttonsHeight);

    setGeometry(windowPosX, windowPosY, windowWidth, windowHeight);
    setFixedSize(windowWidth, windowHeight);
}

void WindowDatabase::moveEvent(QMoveEvent *event) {
    QWidget::moveEvent(event);
    windowPosX = geometry().x();
    windowPosY = geometry().y();
}

void WindowDatabase::on_close_clicked() {
    closeDatabase();
}

void WindowDatabase::on_deleteGame_clicked() {
    QModelIndexList selection = ui->GamesTable->selectionModel()->selectedRows();
    int openGame = game->getOpenDatabaseGameNumber();
    for (int row = 0; row < selection.count(); row++) {
        QModelIndex index = selection.at(row);
        if (index.row() < openGame) game->setOpenDatabaseGameNumber(openGame-1);
        else if (index.row() == openGame) game->setOpenDatabaseGameNumber(-1);
        gamesArray.removeAt(index.row());
    }
    saveChanges();
    loadGames();
    updateLayout();
    ui->GamesTable->clearSelection();
}

void WindowDatabase::on_editGame_clicked() {
    QModelIndexList selection = ui->GamesTable->selectionModel()->selectedRows();
    editGame(selection.first());
    gamesArray = {};
    loadGames();
    updateLayout();
}

void WindowDatabase::on_GamesTable_doubleClicked(const QModelIndex &index) {
    QMessageBox::StandardButton reply = QMessageBox::question(NULL, "Load game", "Do you want to open this game?<br>All unsaved changes to the actual game will be lost.");
    if (reply == QMessageBox::Yes) {
        openGame(index);
        closeDatabase();
    }
}

void WindowDatabase::on_openGame_clicked() {
    QMessageBox::StandardButton reply = QMessageBox::question(NULL, "Load game", "Do you want to open this game?<br>All unsaved changes to the actual game will be lost.");
    if (reply == QMessageBox::Yes) {
        QModelIndexList selection = ui->GamesTable->selectionModel()->selectedRows();
        openGame(selection.first());
        closeDatabase();
    }
}

void WindowDatabase::on_undoChanges_clicked() {
    QFile databaseFile(game->databaseFileName);
    QFile backupFile(game->backupFileName);
    databaseFile.remove();
    backupFile.copy(game->databaseFileName);
    gamesArray = {};
    loadGames();
    updateLayout();
    updateLayout();
}
