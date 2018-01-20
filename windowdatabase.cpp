#include "windowdatabase.h"
#include "ui_windowdatabase.h"

WindowDatabase::WindowDatabase(QWidget *parent) : QWidget(parent), ui(new Ui::WindowDatabase) {
    ui->setupUi(this);
}

WindowDatabase::~WindowDatabase() {
    delete ui;
}
