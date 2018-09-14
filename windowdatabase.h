#ifndef WINDOWDATABASE_H
#define WINDOWDATABASE_H

#include <QtCore>
#include <QtGui>
#include "dialogsave.h"

enum column { // for gamesTable
    colRed, colBlue, colResult, colMoves, colCards, colEvent, colCity, colRound, colDate
};

namespace Ui {
    class WindowDatabase;
}

class WindowDatabase : public QWidget {
    Q_OBJECT

public:
    // constructor
    explicit WindowDatabase(QWidget *parent = nullptr);

    // destructor
    ~WindowDatabase() override;

    // methods
    void closeDatabase();
    bool copyGames();
    bool editGame(QModelIndex index);
    bool exportGames();
    QString generateMovesString(QJsonArray turnsList = {});
    bool loadGames();
    void openGame(QModelIndex index);
    bool pasteGames();
    bool saveChanges() const;
    bool saveGame(bool newSave = false);
    void setCell(int row, column col, const QVariant &value, Qt::AlignmentFlag alignment = Qt::AlignCenter);
    void updateLayout();

protected:
    // events
    void moveEvent(QMoveEvent *event) override;

private slots:
    inline void on_close_clicked() { closeDatabase(); }
    inline void on_copyGame_clicked() { copyGames(); }
    void on_deleteGame_clicked();
    void on_editGame_clicked();
    inline void on_exportGames_clicked() { exportGames(); }
    void on_filterGames_clicked();
    void on_importGames_clicked();
    void on_GamesTable_doubleClicked(const QModelIndex &index);
    void on_openGame_clicked();
    inline void on_pasteGame_clicked() { pasteGames(); }
    void on_undoChanges_clicked();

private:
    // variables
    Ui::WindowDatabase *ui;
    DialogSave *dialogSave;
    int windowPosX, windowPosY, modelCols, modelRows;
    QStandardItemModel *model;
    QJsonArray gamesArray;
};

#endif // WINDOWDATABASE_H
