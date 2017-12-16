#include "dialogabout.h"
#include "ui_dialogabout.h"

DialogAbout::DialogAbout(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAbout) {
    ui->setupUi(this);
    // hyperlinks (About Oni; Licenses)
    ui->aboutLabel->setOpenExternalLinks(true);
    ui->licensesLabel->setOpenExternalLinks(true);
    ui->link_M->setOpenExternalLinks(true);
    ui->link_S->setOpenExternalLinks(true);
}

DialogAbout::~DialogAbout() {
    delete ui;
}

void DialogAbout::on_aboutQt_clicked() {
    QApplication::aboutQt();
}

void DialogAbout::on_close_clicked() {
    this->close();
}
