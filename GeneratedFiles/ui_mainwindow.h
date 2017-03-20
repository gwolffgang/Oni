/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionNew;
    QAction *actionSave;
    QAction *actionLoad;
    QAction *actionExit;
    QAction *actionSmallLayout;
    QAction *actionNormalLayout;
    QAction *actionLargeLayout;
    QAction *actionFullScreen;
    QAction *actionTinyLayout;
    QWidget *centralWidget;
    QGraphicsView *view;
    QListWidget *listWidget;
    QMenuBar *menuBar;
    QMenu *menuGame;
    QMenu *menuLayout;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(984, 522);
        actionNew = new QAction(MainWindow);
        actionNew->setObjectName(QStringLiteral("actionNew"));
        actionSave = new QAction(MainWindow);
        actionSave->setObjectName(QStringLiteral("actionSave"));
        actionLoad = new QAction(MainWindow);
        actionLoad->setObjectName(QStringLiteral("actionLoad"));
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        actionSmallLayout = new QAction(MainWindow);
        actionSmallLayout->setObjectName(QStringLiteral("actionSmallLayout"));
        actionNormalLayout = new QAction(MainWindow);
        actionNormalLayout->setObjectName(QStringLiteral("actionNormalLayout"));
        actionLargeLayout = new QAction(MainWindow);
        actionLargeLayout->setObjectName(QStringLiteral("actionLargeLayout"));
        actionFullScreen = new QAction(MainWindow);
        actionFullScreen->setObjectName(QStringLiteral("actionFullScreen"));
        actionTinyLayout = new QAction(MainWindow);
        actionTinyLayout->setObjectName(QStringLiteral("actionTinyLayout"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        view = new QGraphicsView(centralWidget);
        view->setObjectName(QStringLiteral("view"));
        view->setGeometry(QRect(0, 0, 221, 121));
        listWidget = new QListWidget(centralWidget);
        new QListWidgetItem(listWidget);
        new QListWidgetItem(listWidget);
        new QListWidgetItem(listWidget);
        listWidget->setObjectName(QStringLiteral("listWidget"));
        listWidget->setGeometry(QRect(950, 300, 421, 161));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 984, 22));
        menuGame = new QMenu(menuBar);
        menuGame->setObjectName(QStringLiteral("menuGame"));
        menuLayout = new QMenu(menuBar);
        menuLayout->setObjectName(QStringLiteral("menuLayout"));
        MainWindow->setMenuBar(menuBar);

        menuBar->addAction(menuGame->menuAction());
        menuBar->addAction(menuLayout->menuAction());
        menuGame->addAction(actionNew);
        menuGame->addSeparator();
        menuGame->addAction(actionSave);
        menuGame->addAction(actionLoad);
        menuGame->addSeparator();
        menuGame->addAction(actionExit);
        menuLayout->addAction(actionTinyLayout);
        menuLayout->addAction(actionSmallLayout);
        menuLayout->addAction(actionNormalLayout);
        menuLayout->addAction(actionLargeLayout);
        menuLayout->addSeparator();
        menuLayout->addAction(actionFullScreen);

        retranslateUi(MainWindow);
        QObject::connect(actionFullScreen, SIGNAL(triggered()), MainWindow, SLOT(showFullScreen()));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
        actionNew->setText(QApplication::translate("MainWindow", "&New", Q_NULLPTR));
        actionNew->setShortcut(QApplication::translate("MainWindow", "Ctrl+N", Q_NULLPTR));
        actionSave->setText(QApplication::translate("MainWindow", "&Save", Q_NULLPTR));
        actionSave->setShortcut(QApplication::translate("MainWindow", "Ctrl+S", Q_NULLPTR));
        actionLoad->setText(QApplication::translate("MainWindow", "&Load", Q_NULLPTR));
        actionLoad->setShortcut(QApplication::translate("MainWindow", "Ctrl+L", Q_NULLPTR));
        actionExit->setText(QApplication::translate("MainWindow", "E&xit", Q_NULLPTR));
        actionExit->setShortcut(QApplication::translate("MainWindow", "Ctrl+X", Q_NULLPTR));
        actionSmallLayout->setText(QApplication::translate("MainWindow", "&Small", Q_NULLPTR));
        actionSmallLayout->setShortcut(QApplication::translate("MainWindow", "Ctrl+Alt+S", Q_NULLPTR));
        actionNormalLayout->setText(QApplication::translate("MainWindow", "&Normal", Q_NULLPTR));
        actionNormalLayout->setShortcut(QApplication::translate("MainWindow", "Ctrl+Alt+N", Q_NULLPTR));
        actionLargeLayout->setText(QApplication::translate("MainWindow", "&Large", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        actionLargeLayout->setToolTip(QApplication::translate("MainWindow", "Large", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        actionLargeLayout->setShortcut(QApplication::translate("MainWindow", "Ctrl+Alt+L", Q_NULLPTR));
        actionFullScreen->setText(QApplication::translate("MainWindow", "&FullScreen", Q_NULLPTR));
        actionFullScreen->setShortcut(QApplication::translate("MainWindow", "Ctrl+Alt+F", Q_NULLPTR));
        actionTinyLayout->setText(QApplication::translate("MainWindow", "&Tiny", Q_NULLPTR));
        actionTinyLayout->setShortcut(QApplication::translate("MainWindow", "Ctrl+Alt+T", Q_NULLPTR));

        const bool __sortingEnabled = listWidget->isSortingEnabled();
        listWidget->setSortingEnabled(false);
        QListWidgetItem *___qlistwidgetitem = listWidget->item(0);
        ___qlistwidgetitem->setText(QApplication::translate("MainWindow", "1. Sa2-a4 (TIGER) / md5-e5 (ELEPHANT)", Q_NULLPTR));
        QListWidgetItem *___qlistwidgetitem1 = listWidget->item(1);
        ___qlistwidgetitem1->setText(QApplication::translate("MainWindow", "2.          ...               / sb3xb4 (CRAB)", Q_NULLPTR));
        QListWidgetItem *___qlistwidgetitem2 = listWidget->item(2);
        ___qlistwidgetitem2->setText(QApplication::translate("MainWindow", "Sa1 Sb2 Md4 se2 sa5 me2 S-1 S-1 s-1 s-1 ROOSTER TIGER DRAGON CRAB ELEPHANT", Q_NULLPTR));
        listWidget->setSortingEnabled(__sortingEnabled);

        menuGame->setTitle(QApplication::translate("MainWindow", "Game", Q_NULLPTR));
        menuLayout->setTitle(QApplication::translate("MainWindow", "Layout", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
