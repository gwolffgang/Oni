#ifndef DIALOGABOUT_H
#define DIALOGABOUT_H

#include <QDialog>

namespace Ui {
class DialogAbout;
}

class DialogAbout : public QDialog {
    Q_OBJECT

public:
    // constructor
    explicit DialogAbout(QWidget *parent = nullptr);

    // destructor
    ~DialogAbout();

private:
    // variables
    Ui::DialogAbout *ui;

private slots:
    void on_aboutQt_clicked();
    inline void on_close_clicked() { close(); }
};

#endif // DIALOGABOUT_H
