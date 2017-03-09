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
    inline QGraphicsScene *getScene() { return scene; }

    // functions
    void setViewSize(double width, double height);
    void drawBoard();
    void drawCardSlots();

private:
    QGraphicsScene *scene;
};

#endif // MAINWINDOW_H
