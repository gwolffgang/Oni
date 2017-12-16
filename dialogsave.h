#ifndef DIALOGSAVE_H
#define DIALOGSAVE_H

#include <QDialog>

namespace Ui {
class DialogSave;
}

class DialogSave : public QDialog {
    Q_OBJECT

public:
    explicit DialogSave(QWidget *parent = 0);
    ~DialogSave();

    // getter
    QList<QString> getValues();

private:
    Ui::DialogSave *ui;

private slots:
    inline void on_buttonBoxSave_accepted() {}
};

#endif // DIALOGSAVE_H
