#include "dialogdatabase.h"
#include "ui_dialogdatabase.h"

DialogDatabase::DialogDatabase(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogDatabase)
{
    ui->setupUi(this);
}

DialogDatabase::~DialogDatabase()
{
    delete ui;
}
