#ifndef DIALOGSAVE_H
#define DIALOGSAVE_H

#include <QDialog>

namespace Ui {
class DialogSave;
}

class DialogSave : public QDialog {
    Q_OBJECT

private:
    Ui::DialogSave *ui;

private slots:
    void on_buttonBoxSave_accepted();

public:
    explicit DialogSave(QWidget *parent = 0);
    ~DialogSave();

    // getter
    QList<QString> getValues();
};

#endif // DIALOGSAVE_H
