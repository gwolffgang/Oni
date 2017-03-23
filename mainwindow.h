#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include "aboutwindow.h"
#include "button.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    Ui::MainWindow *ui;
    QColor colorNone, colorHovered, colorSelected, colorChooseableCard1, colorChooseableCard2, colorChooseableBoth;

    // constructors
    explicit MainWindow(QWidget *parent = 0);

    // destructors
    ~MainWindow();

    // getter
    inline int getBorderX() { return borderX; }
    inline int getBorderY() { return borderY; }
    inline double getFieldSize() { return fieldSize; }
    inline QGraphicsScene *getScene() { return scene; }
    inline double getSlotSize() { return slotSize; }
    inline double getSideBarSize() { return sideBarSize; }
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
    void drawBoard();
    void drawCardSlots();
    void drawSideBar();
    QString generateSetupString();
    void loadGame();
    void newGame(QString setupString = "");
    void positionNotation();
    void prepareGame();
    bool saveGame(const QString &fileName);
    bool saveGameAs();
    void saveTurnInNotation();
    void unparentPieces();
    void setViewSize(double width, double height);

private slots:
    // Refresh
    void refreshWindow() { prepareGame(); }
    // Game
    void on_actionNew_triggered() { newGame(); }
    void on_actionLoad_triggered() { loadGame(); }
    void on_actionSave_triggered();
    void on_actionSaveAs_triggered() { saveGameAs(); }
    // Settings
    void on_actionFlipOnce_triggered();
    // Layout
    void on_actionTinyLayout_triggered() { changeLayout(0.25); }
    void on_actionSmallLayout_triggered() { changeLayout(0.5); }
    void on_actionNormalLayout_triggered() { changeLayout(0.75); }
    void on_actionLargeLayout_triggered() { changeLayout(1.0); }
    void on_actionFullScreen_triggered();
    // Help
    void on_actionAboutOni_triggered() { aboutOni(); }
    void on_actionAboutQt_triggered();

private:
    QScreen *screen;
    QGraphicsScene *scene;
    AboutWindow *about;
    QString windowTitle;
    Button *flipButton;
    double windowHeight, windowWidth, borderX, borderY, fieldSize, slotSize, sideBarSize;
};

#endif // MAINWINDOW_H
