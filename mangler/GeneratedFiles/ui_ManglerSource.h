/********************************************************************************
** Form generated from reading UI file 'ManglerSource.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MANGLERSOURCE_H
#define UI_MANGLERSOURCE_H

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

class Ui_ManglerSourceClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *ManglerSourceClass)
    {
        if (ManglerSourceClass->objectName().isEmpty())
            ManglerSourceClass->setObjectName(QStringLiteral("ManglerSourceClass"));
        ManglerSourceClass->resize(600, 400);
        menuBar = new QMenuBar(ManglerSourceClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        ManglerSourceClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(ManglerSourceClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        ManglerSourceClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(ManglerSourceClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        ManglerSourceClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(ManglerSourceClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        ManglerSourceClass->setStatusBar(statusBar);

        retranslateUi(ManglerSourceClass);

        QMetaObject::connectSlotsByName(ManglerSourceClass);
    } // setupUi

    void retranslateUi(QMainWindow *ManglerSourceClass)
    {
        ManglerSourceClass->setWindowTitle(QApplication::translate("ManglerSourceClass", "ManglerSource", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class ManglerSourceClass: public Ui_ManglerSourceClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MANGLERSOURCE_H
