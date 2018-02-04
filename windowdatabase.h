#ifndef WINDOWDATABASE_H
#define WINDOWDATABASE_H

#include <QWidget>
#include <QtCore>
#include <QtGui>
#include "dialogsave.h"

enum column { // for gamesTable
    colRed, colBlue, colResult, colMoves, colCards, colEvent, colCity, colDate
};

namespace Ui {
    class WindowDatabase;
}

class WindowDatabase : public QWidget {
    Q_OBJECT

public:
    // constructor
    explicit WindowDatabase(QWidget *parent = 0);

    // destructor
    ~WindowDatabase();

    // methods
    void closeDatabase();
    bool editGame(QModelIndex index);
    bool loadGames();
    void openGame(QModelIndex index);
    bool saveChanges() const;
    bool saveGameInDatabase(bool newSave = false);
    void setCell(int row, column col, const QVariant &value, Qt::AlignmentFlag alignment = Qt::AlignCenter);
    void updateLayout();

protected:
    // events
    void moveEvent(QMoveEvent *event) override;


private slots:
    void on_close_clicked();
    void on_deleteGame_clicked();
    void on_editGame_clicked();
    void on_GamesTable_doubleClicked(const QModelIndex &index);
    void on_openGame_clicked();
    void on_undoChanges_clicked();

private:
    // variables
    Ui::WindowDatabase *ui;
    DialogSave *dialogSave;
    int windowPosX, windowPosY, modelCols, modelRows;
    QStandardItemModel *model;
    QJsonArray gamesArray;
    QJsonObject tempGame;
};

#endif // WINDOWDATABASE_H
