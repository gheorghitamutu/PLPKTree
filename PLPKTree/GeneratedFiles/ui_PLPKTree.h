/********************************************************************************
** Form generated from reading UI file 'PLPKTree.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PLPKTREE_H
#define UI_PLPKTREE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PLPKTreeClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *PLPKTreeClass)
    {
        if (PLPKTreeClass->objectName().isEmpty())
            PLPKTreeClass->setObjectName(QStringLiteral("PLPKTreeClass"));
        PLPKTreeClass->resize(600, 400);
        menuBar = new QMenuBar(PLPKTreeClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        PLPKTreeClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(PLPKTreeClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        PLPKTreeClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(PLPKTreeClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        PLPKTreeClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(PLPKTreeClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        PLPKTreeClass->setStatusBar(statusBar);

        retranslateUi(PLPKTreeClass);

        QMetaObject::connectSlotsByName(PLPKTreeClass);
    } // setupUi

    void retranslateUi(QMainWindow *PLPKTreeClass)
    {
        PLPKTreeClass->setWindowTitle(QApplication::translate("PLPKTreeClass", "PLPKTree", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PLPKTreeClass: public Ui_PLPKTreeClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PLPKTREE_H
