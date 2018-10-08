#ifndef ONI_H
#define ONI_H

#include <QApplication>
#include <QFileDialog>
#include <QMessageBox>

#include "mainwindow.h"
#include "match.h"
#include "windowdatabase.h"

class Oni : public QObject {
    Q_OBJECT

public:
    // variables
    QString backupFileName, configFileName, databaseFileName, tempDataFileName;

    // constructor
    Oni();

    // getter
    inline int getCurrentDisplayedMove() { return currentDisplayedMove; }
    inline bool getCardChoiceActive() { return cardChoiceActive; }
    inline bool getFlippedBoard() { return flippedBoard; }
    inline Match *getMatch() { return match; }
    inline QString getPieceSet() { return piecesSet; }
    inline MainWindow *getWindow() { return window; }
    inline WindowDatabase *getWindowDatabase() { return windowDatabase; }

    // setter
    inline void setCardChoiceActive(bool state) { cardChoiceActive = state; }
    inline void setCurrentDisplayedMove(int newMove) { currentDisplayedMove = newMove; }
    inline void setFlippedBoard(bool state) { flippedBoard = state; }
    inline void setPiecesSet(QString newString) { piecesSet = newString; }

    // methods
    inline bool getFirstPlayersTurn() { if (currentDisplayedMove%2 == 0) return true; else return false; }
    bool readConfig();
    void setupFoldersAndFiles();
    void winGame(int winner);
    bool writeConfig();

private:
    // variables
    MainWindow *window;
    WindowDatabase *windowDatabase;
    QString piecesSet;
    int currentDisplayedMove;
    bool flippedBoard, cardChoiceActive;
    Match *match;
};

#endif // ONI_H
