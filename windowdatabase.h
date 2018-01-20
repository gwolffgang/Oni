#ifndef WINDOWDATABASE_H
#define WINDOWDATABASE_H

#include <QWidget>

namespace Ui {
class WindowDatabase;
}

class WindowDatabase : public QWidget
{
    Q_OBJECT

public:
    explicit WindowDatabase(QWidget *parent = 0);
    ~WindowDatabase();

private:
    Ui::WindowDatabase *ui;
};

#endif // WINDOWDATABASE_H
