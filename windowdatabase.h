#ifndef WINDOWDATABASE_H
#define WINDOWDATABASE_H

#include <QWidget>
#include <QtCore>
#include <QtGui>

enum column {
    red, blue, result, moves, cards, source, city, date
};

namespace Ui {
    class WindowDatabase;
}

class WindowDatabase : public QWidget {
    Q_OBJECT

public:
    explicit WindowDatabase(QWidget *parent = 0);
    ~WindowDatabase();

    // methods
    bool loadGames();
    void openGame();
    bool saveChanges() const;
    bool saveGameInDatabase();
    void setCell(int row, column col, const QVariant &value, Qt::AlignmentFlag alignment = Qt::AlignCenter);
    void updateLayout();

protected:
    // events
    void moveEvent(QMoveEvent *event) override;


private slots:
    inline void on_closeOnly_clicked() { close(); }
    void on_deleteGame_clicked();
    void on_GamesTable_clicked(const QModelIndex &index);
    void on_GamesTable_doubleClicked(const QModelIndex &index);
    void on_openGame_clicked();
    inline void on_saveAndClose_clicked() { saveChanges(); close(); }
    inline void on_saveChanges_clicked() { saveChanges(); }
    inline void on_undoChanges_clicked() { gamesArray = {}; loadGames(); updateLayout(); }

private:
    // variables
    Ui::WindowDatabase *ui;
    int windowPosX, windowPosY, modelCols, modelRows, selectedRow;
    QStandardItemModel *model;
    QJsonArray gamesArray;
    QJsonObject tempGame;
};

#endif // WINDOWDATABASE_H
