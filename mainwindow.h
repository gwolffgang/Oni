#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDesktopServices>
#include <QGraphicsScene>
#include <QListWidget>
#include <QMainWindow>
#include <QMoveEvent>

#include "dialogabout.h"
#include "windowdatabase.h"
#include "dialogsave.h"
#include "button.h"
#include "cardslot.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    Ui::MainWindow *ui;
    QColor colorHovered, colorSelected, colorChooseableCard1, colorChooseableCard2, colorChooseableBoth;

    // constructors
    explicit MainWindow(QWidget *parent = 0);

    // destructors
    ~MainWindow();

    // getter
    inline double getBorderX() { return borderX; }
    inline double getBorderY() { return borderY; }
    inline double getFieldSize() { return fieldSize; }
    bool getFlipEveryMove();
    inline QGraphicsScene *getScene() { return scene; }
    inline double getSlotHeight() { return slotHeight; }
    inline double getSlotWidth() {return slotWidth;}
    inline double getSideBarSize() { return sideBarSize; }
    inline Button *getTurnBlue() { return turnBlue; }
    inline Button *getTurnRed() { return turnRed; }
    inline WindowDatabase *getWindowDatabase() { return windowDatabase; }
    inline double getWindowHeight() { return windowHeight; }
    inline double getWindowPosX() { return windowPosX; }
    inline double getWindowPosY() { return windowPosY; }
    inline double getWindowWidth() { return windowWidth; }

    // setter
    inline void setBorderX(int newPosX) { borderX = newPosX; }
    inline void setBorderY(int newPosY) { borderY = newPosY; }
    inline void setFieldSize(double newFieldSize) { fieldSize = newFieldSize; }

    // methods
    inline void aboutOni() { dialogAbout->show(); }
    bool analyseSetupString(QString string);
    void drawAxisLabeling();
    void drawBackGroundPicture();
    void drawBoard();
    void drawCardSlots();
    void drawSideBar();
    QString generateNotationString(QString lastTurn, QString thisTurn);
    QString generateSetupString();
    void newGame(QString setupString = "");
    void notateVictory(QString result);
    void prepareGame();
    void readWindowConfig(QJsonObject &json);
    void resetLists();
    void saveGame(QString fileName = "");
    void saveTurnInNotation();
    void saveWindowConfig(QJsonObject &json) const;
    void setupNotation();
    void updateLayout();

private:
    QScreen *screen;
    QGraphicsScene *scene;
    DialogAbout *dialogAbout;
    WindowDatabase *windowDatabase;
    DialogSave *dialogSave;
    Button *flipButton, *turnRed, *turnBlue;
    QList<QGraphicsTextItem*> *axisLabel;
    double windowPosX, windowPosY, windowHeight, windowWidth, borderX, borderY, fieldSize, slotHeight, slotWidth, sideBarSize, axisLabelSize, MSWindowsCorrection;

protected:
    // events
    void moveEvent(QMoveEvent *event) override;

private slots:
    inline void openDatabase() { windowDatabase = new WindowDatabase; windowDatabase->show(); }
    inline void refreshWindow() { prepareGame(); }
    void showMove(QListWidgetItem *item = NULL);
    // Game - Cards
    void on_actionBasisGame_triggered();
    void on_actionSenseisPath_triggered();
    void on_actionGoatSheep_triggered();
    // Game - Database
    void on_actionDatabase_triggered();
    // Game - Layout - PieceSet
    void on_actionPiecesComicStyle_triggered();
    void on_actionPiecesHanzi_triggered();
    // Game - Layout - Window
    void on_actionTinyWindow_triggered();
    void on_actionSmallWindow_triggered();
    void on_actionNormalWindow_triggered();
    void on_actionLargeWindow_triggered();
    void on_actionFullScreen_triggered();
    // Game - Layout
    void on_actionFlipOnce_triggered();
    void on_actionAxisLabeling_triggered();
    void on_actionHideNotation_triggered();
    // Match - GoTo
    void on_actionStartingPosition_triggered();
    void on_actionPreviousMove_triggered();
    void on_actionNextMove_triggered();
    void on_actionLastMove_triggered();
    // Match
    void on_actionNew_triggered();
    void on_actionSetupPosition_triggered();
    inline void on_actionSave_triggered() { windowDatabase->saveGameInDatabase(); }
    inline void on_actionSaveAs_triggered() { windowDatabase->saveGameInDatabase(); }
    void on_actionResign_triggered();
    // Sensei
    void on_actionRedEasy_triggered();
    void on_actionRedMedium_triggered();
    void on_actionRedHard_triggered();
    void on_actionBlueEasy_triggered();
    void on_actionBlueMedium_triggered();
    void on_actionBlueHard_triggered();
    // About
    inline void on_actionAboutOni_triggered() { aboutOni(); }
    void on_actionAboutQt_triggered();
};

#endif // MAINWINDOW_H
