#include "oni.h"
#include "windowdatabase.h"
#include "ui_windowdatabase.h"

extern Oni *game;

WindowDatabase::WindowDatabase(QWidget *parent) : QWidget(parent), ui(new Ui::WindowDatabase),
    windowPosX(50), windowPosY(50), modelCols(8), modelRows(0), selectedRow(-1),
    model(new QStandardItemModel(modelRows, modelCols, this)), gamesArray({}), tempGame({}) {
    ui->setupUi(this);

    setWindowTitle("Oni - Games Database");
    loadGames();
    updateLayout();
    show();
}

WindowDatabase::~WindowDatabase() {
    delete ui;
}

bool WindowDatabase::loadGames() {
    if (gamesArray.size() == 0) {
        QFile databaseFile(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + QStringLiteral("/oni_save.json"));
        if (!databaseFile.open(QIODevice::ReadOnly)) { qWarning("Couldn't open database file for reading."); return false; }
        QJsonDocument databaseDoc(QJsonDocument::fromJson(databaseFile.readAll()));
        QJsonObject databaseData = databaseDoc.object();
        if (databaseData.contains("tempGame") && databaseData["tempGame"].isObject())
            tempGame = databaseData["tempGame"].toObject();
        if (databaseData.contains("games") && databaseData["games"].isArray()) {
            gamesArray = databaseData["games"].toArray();
        }
    }
    model->setRowCount(gamesArray.size());
    for (int row = 0; row < gamesArray.size(); row++) {
        QJsonObject game = gamesArray.at(row).toObject();
        if (game.contains("playerNameRed") && game["playerNameRed"].isString())
            setCell(row, red, game["playerNameRed"].toString());
        if (game.contains("playerNameBlue") && game["playerNameBlue"].isString())
            setCell(row, blue, game["playerNameBlue"].toString());
        if (game.contains("gameResult") && game["gameResult"].isDouble()) {
            switch (game["gameResult"].toInt()) {
            case -1: setCell(row, result, "0-1"); break;
            case 0: setCell(row, result, "*"); break;
            case 1: setCell(row, result, "1-0");
            }
        }
        if (game.contains("turns") && game["turns"].isArray())
            setCell(row, moves, game["turns"].toArray().size() / 2);
    }
    return true;
}

void WindowDatabase::openGame() {
    QJsonObject gameData = gamesArray.at(selectedRow).toObject();
    if (gameData.contains("firstPlayersTurn") && gameData["firstPlayersTurn"].isBool())
        game->setFirstPlayersTurn(gameData["firstPlayersTurn"].toBool());
    if (gameData.contains("playerNameBlue") && gameData["playerNameBlue"].isString() &&
        gameData.contains("playerNameRed") && gameData["playerNameRed"].isString())
        game->setPlayerNames(gameData["playerNameRed"].toString(), gameData["playerNameBlue"].toString());
    if (gameData.contains("gameResult") && gameData["gameResult"].isDouble())
        game->setGameResult(gameData["gamesResult"].toInt());
    if (gameData.contains("turns") && gameData["turns"].isArray()) {
        QJsonArray turnsArray = gameData["turns"].toArray();
        game->getTurns()->clear();
        for (int k = 0; k < turnsArray.size(); k++) game->getTurns()->append(turnsArray[k].toString());
    }

    if (game->getTurns()->last() == "1-0" || game->getTurns()->last() == "0-1")
        game->getWindow()->newGame(game->getTurns()->at( game->getTurns()->size()-2) );
    else game->getWindow()->newGame(game->getTurns()->last());
    game->setActuallyDisplayedMove(game->getTurns()->size());
}

bool WindowDatabase::saveChanges() const {
    QFile databaseFile(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + QStringLiteral("/oni_save.json"));
    if (!databaseFile.open(QIODevice::WriteOnly)) { qWarning("Couldn't open database file for writing."); return false; }
        QJsonObject newDatabaseData;
        newDatabaseData["games"] = gamesArray;
        newDatabaseData["tempGame"] = tempGame;
        QJsonDocument newDatabaseDoc(newDatabaseData);
        databaseFile.write(newDatabaseDoc.toJson());
    return true;
}

bool WindowDatabase::saveGameInDatabase() {
    QFile databaseFile(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + QStringLiteral("/oni_save.json"));

    if (!databaseFile.open(QIODevice::ReadOnly)) { qWarning("Couldn't open database file for reading."); return false; }
        QJsonDocument databaseDoc(QJsonDocument::fromJson(databaseFile.readAll()));
        QJsonObject databaseData = databaseDoc.object();
        QJsonArray gamesArray;
            if (databaseData.contains("games") && databaseData["games"].isArray())
                gamesArray = databaseData["games"].toArray();
        QJsonObject tempGame;
            if (databaseData.contains("tempGame") && databaseData["tempGame"].isObject())
                tempGame = databaseData["tempGame"].toObject();
    databaseFile.close();

    if (!databaseFile.open(QIODevice::WriteOnly)) { qWarning("Couldn't open database file for writing."); return false; }
        QJsonObject match;
            match["playerNameRed"] = game->getPlayerNameRed();
            match["playerNameBlue"] = game->getPlayerNameBlue();
            match["firstPlayersTurn"] = game->getFirstPlayersTurn();
            match["gameResult"] = game->getGameResult();
            QJsonArray turnsArray;
                for (auto & turn : *game->getTurns()) turnsArray.append(turn);
            match["turns"] = turnsArray;
        gamesArray.append(match);
        game->setOpenDatabaseGameNumber(gamesArray.size()-1);

        QJsonObject newDatabaseData;
        newDatabaseData["games"] = gamesArray;
        newDatabaseData["tempGame"] = tempGame;

        QJsonDocument newDatabaseDoc(newDatabaseData);
        databaseFile.write(newDatabaseDoc.toJson());
    this->updateLayout();
    return true;
}

void WindowDatabase::setCell(int row, column col, const QVariant &value, Qt::AlignmentFlag alignment) {
    QModelIndex index = model->index(row, col, QModelIndex());
    model->setData(index, alignment, Qt::TextAlignmentRole);
    model->setData(index, value);
}

void WindowDatabase::updateLayout() {
    // name table headers
    model->setHeaderData(red, Qt::Horizontal, "Red");
    model->setHeaderData(blue, Qt::Horizontal, "Blue");
    model->setHeaderData(result, Qt::Horizontal, "Result");
    model->setHeaderData(moves, Qt::Horizontal, "Moves");
    model->setHeaderData(cards, Qt::Horizontal, "Cards");
    model->setHeaderData(source, Qt::Horizontal, "Source");
    model->setHeaderData(city, Qt::Horizontal, "City");
    model->setHeaderData(date, Qt::Horizontal, "Date");

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
    double windowHeight = tableHeight + 2 * buttonsHeight;
    table->setGeometry(0, 0, tableWidth, tableHeight);
    table->setFixedSize(tableWidth, tableHeight);

    // position buttons
    ui->filterGames->setGeometry(0, tableHeight, buttonsWidth, buttonsHeight);
    ui->importGames->setGeometry(buttonsWidth, tableHeight, buttonsWidth, buttonsHeight);
    ui->exportGames->setGeometry(2 * buttonsWidth, tableHeight, buttonsWidth, buttonsHeight);
    ui->openGame->setGeometry(tableWidth, 0, buttonsWidth, buttonsHeight);
    ui->editGame->setGeometry(tableWidth, buttonsHeight, buttonsWidth, buttonsHeight);
    ui->deleteGame->setGeometry(tableWidth, 1.8 * buttonsHeight, buttonsWidth, buttonsHeight);
    ui->copyGame->setGeometry(tableWidth, 3 * buttonsHeight, buttonsWidth, buttonsHeight);
    ui->pasteGame->setGeometry(tableWidth, 3.8 * buttonsHeight, buttonsWidth, buttonsHeight);
    ui->saveChanges->setGeometry(0, tableHeight + buttonsHeight, buttonsWidth, buttonsHeight);
    ui->undoChanges->setGeometry(buttonsWidth, tableHeight + buttonsHeight, buttonsWidth, buttonsHeight);
    ui->saveAndClose->setGeometry(2* buttonsWidth, tableHeight + buttonsHeight, buttonsWidth, buttonsHeight);
    ui->closeOnly->setGeometry(tableWidth, tableHeight + buttonsHeight, buttonsWidth, buttonsHeight);

    setGeometry(windowPosX, windowPosY, windowWidth, windowHeight);
    setFixedSize(windowWidth, windowHeight);
}

void WindowDatabase::moveEvent(QMoveEvent *event) {
    QWidget::moveEvent(event);
    windowPosX = geometry().x();
    windowPosY = geometry().y();
}

void WindowDatabase::on_deleteGame_clicked() {
    gamesArray.removeAt(selectedRow);
    loadGames();
    updateLayout();
}

void WindowDatabase::on_GamesTable_clicked(const QModelIndex &index) {
    ui->GamesTable->selectRow(index.row());
    selectedRow = index.row();
}

void WindowDatabase::on_GamesTable_doubleClicked(const QModelIndex &index) {
    QMessageBox::StandardButton reply = QMessageBox::question(NULL, "Load game", "Do you want to load another game?<br>All unsaved changes to the actual game will be lost.");
    if (reply == QMessageBox::Yes) {
        selectedRow = index.row();
        openGame();
        close();
    }
}

void WindowDatabase::on_openGame_clicked() {
    QMessageBox::StandardButton reply = QMessageBox::question(NULL, "Load game", "Do you want to load another game?<br>All unsaved changes to the actual game will be lost.");
    if (reply == QMessageBox::Yes) {
        openGame();
        close();
    }
}
