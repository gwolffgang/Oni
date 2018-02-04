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
    explicit DialogSave(QWidget *parent = 0);
    ~DialogSave();

    // getter
    QList<QString> getValues();

    // setter
    void setData(QJsonObject gameData);

private:
    Ui::DialogSave *ui;

private slots:
    void on_radioButtonBlue_clicked();
    void on_radioButtonLine_clicked();
    void on_radioButtonRed_clicked();

};

#endif // DIALOGSAVE_H
