#include "dialogsave.h"
#include "ui_dialogsave.h"
#include "oni.h"

extern Oni *game;

DialogSave::DialogSave(QWidget *parent) : QDialog(parent), ui(new Ui::DialogSave), turns({}) {
    ui->setupUi(this);

    ui->lineEditNameRed->setText(game->getMatch()->getPlayerNameRed());
    ui->lineEditNameBlue->setText(game->getMatch()->getPlayerNameBlue());
    if (static_cast<int>(game->getMatch()->getRound()) != 0) ui->lineEditRound->setText(QString::number(game->getMatch()->getRound()));
    else ui->lineEditRound->setText("");
    ui->dateEdit->setDate(game->getMatch()->getDate());
    switch (game->getMatch()->getResult()) {
    case -1: ui->radioButtonBlue->setChecked(true); break;
    case 0: ui->radioButtonLine->setChecked(true); break;
    case 1: ui->radioButtonRed->setChecked(true);
    }
}

DialogSave::~DialogSave() {
    delete ui;
}

QList<QString> DialogSave::getValues() {
    QList<QString> list;
    list.append(ui->lineEditNameRed->text()); // 0
    list.append(ui->lineEditNameBlue->text()); // 1
    list.append(ui->lineEditNameEvent->text()); // 2
    list.append(ui->lineEditNameCity->text()); // 3
    list.append(ui->lineEditRound->text()); // 4
    list.append(ui->dateEdit->date().toString(Qt::ISODate)); // 5
    if (ui->radioButtonRed->isChecked()) list.append("1-0");
    else if (ui->radioButtonBlue->isChecked()) list.append("0-1");
    else list.append("*"); // 6
    for (int k = 0; k < turns.size(); k++) list.append(turns.at(k)); // 7 - END
    return list;
}

void DialogSave::setData(QJsonObject gameData) {
    if (gameData.contains("playerNameBlue") && gameData["playerNameBlue"].isString())
        ui->lineEditNameBlue->setText(gameData["playerNameBlue"].toString());
    if (gameData.contains("playerNameRed") && gameData["playerNameRed"].isString())
        ui->lineEditNameRed->setText(gameData["playerNameRed"].toString());
    if (gameData.contains("gameResult") && gameData["gameResult"].isDouble()) {
        switch (gameData["gameResult"].toInt()) {
        case -1: ui->radioButtonBlue->setChecked(true); break;
        case 0: ui->radioButtonLine->setChecked(true); break;
        case 1: ui->radioButtonRed->setChecked(true);
        }
    }
    if (gameData.contains("event") && gameData["event"].isString())
        ui->lineEditNameEvent->setText(gameData["event"].toString());
    if (gameData.contains("city") && gameData["city"].isString())
        ui->lineEditNameCity->setText(gameData["city"].toString());
    if (gameData.contains("date") && gameData["date"].isString()) {
        QDate date;
        date.fromString(gameData["date"].toString(),"YY.MM.dd");
        ui->dateEdit->setDate(date);
    }
    if (gameData.contains("round") && gameData["round"].isDouble())
        if (gameData["round"].toInt() != 0)
            ui->lineEditRound->setText(QString::number(gameData["round"].toDouble()));
    if (gameData.contains("turns") && gameData["turns"].isArray()) {
            QJsonArray turnsArray = gameData["turns"].toArray();
            turns.clear();
            for (int k = 0; k < turnsArray.size(); k++) turns.append(turnsArray[k].toString());
    }
}

void DialogSave::on_radioButtonBlue_clicked() {
    ui->radioButtonLine->setChecked(false);
    ui->radioButtonRed->setChecked(false);
}

void DialogSave::on_radioButtonLine_clicked() {
    ui->radioButtonBlue->setChecked(false);
    ui->radioButtonRed->setChecked(false);
}

void DialogSave::on_radioButtonRed_clicked() {
    ui->radioButtonBlue->setChecked(false);
    ui->radioButtonLine->setChecked(false);
}


