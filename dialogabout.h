#ifndef DIALOGABOUT_H
#define DIALOGABOUT_H

#include <QDialog>

namespace Ui {
class DialogAbout;
}

class DialogAbout : public QDialog {
    Q_OBJECT

public:
    explicit DialogAbout(QWidget *parent = 0);
    ~DialogAbout();

private:
    Ui::DialogAbout *ui;

private slots:
    void on_aboutQt_clicked();
    void on_close_clicked();
};

#endif // DIALOGABOUT_H
