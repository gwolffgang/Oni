#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QDesktopServices>

#include "aboutwindow.h"
#include "button.h"
#include "cardslot.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    QScreen *screen;
    QGraphicsScene *scene;
    AboutWindow *about;
    QString windowTitle;
    Button *flipButton, *turnRed, *turnBlue;
    QList<QGraphicsTextItem*> *axisLabel;
    double windowHeight, windowWidth, borderX, borderY, fieldSize, slotSize, sideBarSize, axisLabelSize;

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
    inline double getFieldSize() { return fieldSize; }
    bool getFlipEveryMove();
    inline QGraphicsScene *getScene() { return scene; }
    inline double getSlotSize() { return slotSize; }
    inline double getSideBarSize() { return sideBarSize; }
    inline Button *getTurnBlue() { return turnBlue; }
    inline Button *getTurnRed() { return turnRed; }
    inline double getWindowHeight() { return windowHeight; }
    inline double getWindowWidth() { return windowWidth; }

    // setter
    inline void setBorderX(int newX) { borderX = newX; }
    inline void setBorderY(int newY) { borderY = newY; }
    inline void setFieldSize(double newFieldSize) { fieldSize = newFieldSize; }
    inline void setSlotSize(double newSlotSize) { slotSize = newSlotSize; }

    // functions
    inline void aboutOni() { about->show(); }
    void analyseSetupString(QString string);
    void changeLayout(double factor);
    void createCardslots();
    void drawAxisLabeling();
    void drawBoard();
    void drawCardSlots();
    void drawSideBar();
    QString generateNotationString(QString lastTurn, QString thisTurn);
    QString generateSetupString();
    void loadGame();
    void newGame(QString setupString = "");
    void notateVictory(QString result);
    void positionNotation();
    void prepareGame();
    bool saveGame(const QString &fileName);
    bool saveGameAs();
    void saveTurnInNotation();

private slots:
    // Refresh
    void refreshWindow() { prepareGame(); }
    // Game
    void on_actionNew_triggered() { newGame(); }
    void on_actionSetupPosition_triggered();
    void on_actionLoad_triggered() { loadGame(); }
    void on_actionSave_triggered();
    void on_actionSaveAs_triggered() { saveGameAs(); }
    // Match
    void on_actionResign_triggered();
    // Settings
    void on_actionFlipOnce_triggered();
    void on_actionHideNotation_triggered();
    // Layout
    void on_actionTinyLayout_triggered() { changeLayout(0.3); }
    void on_actionSmallLayout_triggered() { changeLayout(0.5); }
    void on_actionNormalLayout_triggered() { changeLayout(0.7); }
    void on_actionLargeLayout_triggered() { changeLayout(0.9); }
    void on_actionFullScreen_triggered();
    // About
    void on_actionAboutOni_triggered() { aboutOni(); }
    void on_actionAboutQt_triggered();
    void on_actionAxisLabeling_triggered();
};

#endif // MAINWINDOW_H
