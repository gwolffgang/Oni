#include <QApplication>
#include "aboutwindow.h"
#include "ui_aboutwindow.h"
#include <QTextBrowser>

AboutWindow::AboutWindow(QWidget *parent) : QDialog(parent), ui(new Ui::AboutWindow) {
    ui->setupUi(this);
    ui->textLicenses->setOpenExternalLinks(true);
    ui->textAbout->setOpenExternalLinks(true);
}

AboutWindow::~AboutWindow() {
    delete ui;
}

void AboutWindow::on_aboutQt_clicked() {
    QApplication::aboutQt();
}
