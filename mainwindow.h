#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDesktopServices>
#include <QGraphicsScene>
#include <QListWidget>
#include <QMainWindow>
#include <QMoveEvent>

#include "dialogabout.h"
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
    bool getAxisLabeling();
    inline int getBorderX() { return borderX; }
    inline int getBorderY() { return borderY; }
    inline DialogSave *getDialogSaveAs() { return dialogSaveAs; }
    inline double getFieldSize() { return fieldSize; }
    bool getFlipEveryMove();
    inline QGraphicsScene *getScene() { return scene; }
    inline double getSlotHeight() { return slotHeight; }
    inline double getSlotWidth() {return slotWidth;}
    inline double getSideBarSize() { return sideBarSize; }
    inline Button *getTurnBlue() { return turnBlue; }
    inline Button *getTurnRed() { return turnRed; }
    inline double getWindowHeight() { return windowHeight; }
    inline double getWindowWidth() { return windowWidth; }

    // setter
    inline void setBorderX(int newX) { borderX = newX; }
    inline void setBorderY(int newY) { borderY = newY; }
    inline void setFieldSize(double newFieldSize) { fieldSize = newFieldSize; }

    // methods
    inline void aboutOni() { dialogAbout->show(); }
    bool analyseSetupString(QString string);
    void changeLayout(double factor = 0.7);
    void drawAxisLabeling();
    void drawBackGroundPicture();
    void drawBoard();
    void drawCardSlots();
    void drawSideBar();
    QString generateNotationString(QString lastTurn, QString thisTurn);
    QString generateSetupString();
    void loadGame(QString fileName = "");
    void newGame(QString setupString = "");
    void notateVictory(QString result);
    void positionNotation();
    void prepareGame();
    void refreshNotation();
    void resetLists();
    void saveGame(const QString &fileName = "");
    void saveTurnInNotation();

private:
    QScreen *screen;
    QGraphicsScene *scene;
    DialogAbout *dialogAbout;
    DialogSave *dialogSaveAs;
    QString windowTitle;
    Button *flipButton, *turnRed, *turnBlue;
    QList<QGraphicsTextItem*> *axisLabel;
    double windowPosX, windowPosY, windowHeight, windowWidth, borderX, borderY, fieldSize, slotHeight, slotWidth, sideBarSize, axisLabelSize, MSWindowsCorrection;

protected:
    // events
    void moveEvent(QMoveEvent *event) override;

private slots:
    inline void refreshWindow() { prepareGame(); }
    void showMove(QListWidgetItem *item = NULL);
    // Game
    void on_actionNew_triggered();
    void on_actionSetupPosition_triggered();
    void on_actionLoad_triggered();
    void on_actionSave_triggered();
    inline void on_actionSaveAs_triggered() { saveGame(); }
    // Match
    void on_actionStartingPosition_triggered();
    void on_actionPreviousMove_triggered();
    void on_actionNextMove_triggered();
    void on_actionLastMove_triggered();
    void on_actionResign_triggered();
    // Engine
    void on_actionRedEasy_triggered();
    void on_actionRedMedium_triggered();
    void on_actionRedHard_triggered();
    void on_actionBlueEasy_triggered();
    void on_actionBlueMedium_triggered();
    void on_actionBlueHard_triggered();
    // Cards
    void on_actionBasisGame_triggered();
    void on_actionSenseisPath_triggered();
    void on_actionGoatSheep_triggered();
    // Settings
    void on_actionFlipOnce_triggered();
    void on_actionHideNotation_triggered();
    void on_actionAxisLabeling_triggered();
    // Layout
    void on_actionTinyLayout_triggered();
    void on_actionSmallLayout_triggered();
    void on_actionNormalLayout_triggered();
    void on_actionLargeLayout_triggered();
    void on_actionFullScreen_triggered();
    // About
    inline void on_actionAboutOni_triggered() { aboutOni(); }
    void on_actionAboutQt_triggered();
};

#endif // MAINWINDOW_H
