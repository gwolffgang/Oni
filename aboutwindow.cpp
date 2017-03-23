#include <QApplication>
#include "aboutwindow.h"
#include "ui_aboutwindow.h"

AboutWindow::AboutWindow(QWidget *parent) : QDialog(parent), ui(new Ui::AboutWindow) {
    ui->setupUi(this);
    // hyperlinks (About Oni; Licenses)
    ui->aboutLabel->setOpenExternalLinks(true);
    ui->licensesLabel->setOpenExternalLinks(true);
    ui->link_M->setOpenExternalLinks(true);
    ui->link_S->setOpenExternalLinks(true);
}

AboutWindow::~AboutWindow() {
    delete ui;
}

void AboutWindow::on_aboutQt_clicked() {
    QApplication::aboutQt();
}
