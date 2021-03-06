#ifndef DIALOGSAVE_H
#define DIALOGSAVE_H

#include <QDialog>

namespace Ui {
class DialogSave;
}

class DialogSave : public QDialog {
    Q_OBJECT

public:
    // constructor
    explicit DialogSave(QWidget *parent = nullptr);

    // destructor
    ~DialogSave();

    // getter
    QList<QString> getValues();

    // setter
    void setData(QJsonObject gameData);

private:
    // variables
    Ui::DialogSave *ui;

private slots:
    void on_radioButtonBlue_clicked();
    void on_radioButtonLine_clicked();
    void on_radioButtonRed_clicked();

private:
    // variables
    QList<QString> turns;
};

#endif // DIALOGSAVE_H
