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

    // functions
    void changeLayout(double factor, bool resize = false);
    void drawBoard(bool redraw = false);
    void drawCardSlots();
    void redrawWindow();
    void resizeWindow();
    void setViewSize(double width, double height);
    void start();

public slots:

    void loadGame();
    void newGame();
    void saveGame();
    void saveGameAs();

private:
    QScreen *screen;
    QGraphicsScene *scene;
    double windowHeight, windowWidth, borderX, borderY, fieldHeight;
};

#endif // MAINWINDOW_H
