#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    Ui::MainWindow *ui;

    // constructors
    explicit MainWindow(QWidget *parent = 0);

    // destructors
    ~MainWindow();

    //getter
    inline int getBorderX() { return borderX; }
    inline int getBorderY() { return borderY; }
    inline double getFieldHeight() { return fieldHeight; }
    inline QGraphicsScene *getScene() { return scene; }
    inline double getWindowHeight() { return windowHeight; }
    inline double getWindowWidth() { return windowWidth; }

    // setter
    inline void setBorderX(int newX) { borderX = newX; }
    inline void setBorderY(int newY) { borderY = newY; }
    inline void setFieldHeight(double newFieldHeight) { fieldHeight = newFieldHeight; }

private:
    // functions
    void aboutOni();
    void analyseSetupString(QString string);
    void changeLayout(double factor);
    void createBoard();
    void createCardSlots();
    void drawBoard();
    void drawCardSlots();
    void loadGame();
    void positionNotation();
    void prepareGame();
    void saveGame();
    void saveGameAs();
    void unparentPieces();
    void setViewSize(double width, double height);

protected:
    // events
    void mouseMoveEvent(QMouseEvent *event) override;

public:
    // functions
    void newGame(QString setupString = "");

private slots:
    void on_actionAboutOni_triggered() { aboutOni(); }
    void on_actionAboutQt_triggered();
    void on_actionNew_triggered() { newGame(); }
    void on_actionLoad_triggered() { loadGame(); }
    void on_actionSave_triggered() { saveGame(); }
    void on_actionSaveAs_triggered() { saveGameAs(); }
    void on_actionTinyLayout_triggered() { changeLayout(0.3); }
    void on_actionSmallLayout_triggered() { changeLayout(0.5); }
    void on_actionNormalLayout_triggered() { changeLayout(0.7); }
    void on_actionLargeLayout_triggered() { changeLayout(0.9); }

private:
    QScreen *screen;
    QGraphicsScene *scene;
    double windowHeight, windowWidth, borderX, borderY, fieldHeight;
};

#endif // MAINWINDOW_H
