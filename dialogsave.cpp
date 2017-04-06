#include "dialogsave.h"
#include "ui_dialogsave.h"
#include "oni.h"

extern Oni *game;

void DialogSave::on_buttonBoxSave_accepted() {
}

DialogSave::DialogSave(QWidget *parent) : QDialog(parent), ui(new Ui::DialogSave) {
    ui->setupUi(this);
}

DialogSave::~DialogSave() {
    delete ui;
}

QList<QString> DialogSave::getValues() {
    QList<QString> list;
    list.append(ui->lineEditNameRed->text());
    list.append(ui->lineEditNameBlue->text());
    return list;
}
