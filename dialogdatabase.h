#ifndef DIALOGDATABASE_H
#define DIALOGDATABASE_H

#include <QDialog>

namespace Ui {
class DialogDatabase;
}

class DialogDatabase : public QDialog
{
    Q_OBJECT

public:
    explicit DialogDatabase(QWidget *parent = 0);
    ~DialogDatabase();

private:
    Ui::DialogDatabase *ui;
};

#endif // DIALOGDATABASE_H
