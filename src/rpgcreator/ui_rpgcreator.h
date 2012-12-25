/********************************************************************************
** Form generated from reading UI file 'rpgcreator.ui'
**
** Created: Tue Dec 25 21:51:04 2012
**      by: Qt User Interface Compiler version 4.8.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RPGCREATOR_H
#define UI_RPGCREATOR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QStatusBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RpgCreator
{
public:
    QWidget *centralWidget;
    QPushButton *btnStatistics;
    QPushButton *btnMap;
    QPushButton *btnLoad;
    QPushButton *btnSave;
    QMenuBar *menuBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *RpgCreator)
    {
        if (RpgCreator->objectName().isEmpty())
            RpgCreator->setObjectName(QString::fromUtf8("RpgCreator"));
        RpgCreator->resize(400, 300);
        centralWidget = new QWidget(RpgCreator);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        btnStatistics = new QPushButton(centralWidget);
        btnStatistics->setObjectName(QString::fromUtf8("btnStatistics"));
        btnStatistics->setGeometry(QRect(200, 80, 192, 27));
        btnMap = new QPushButton(centralWidget);
        btnMap->setObjectName(QString::fromUtf8("btnMap"));
        btnMap->setGeometry(QRect(10, 80, 193, 27));
        btnLoad = new QPushButton(centralWidget);
        btnLoad->setObjectName(QString::fromUtf8("btnLoad"));
        btnLoad->setGeometry(QRect(10, 0, 192, 27));
        btnSave = new QPushButton(centralWidget);
        btnSave->setObjectName(QString::fromUtf8("btnSave"));
        btnSave->setGeometry(QRect(200, 0, 192, 27));
        RpgCreator->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(RpgCreator);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 400, 25));
        RpgCreator->setMenuBar(menuBar);
        statusBar = new QStatusBar(RpgCreator);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        RpgCreator->setStatusBar(statusBar);

        retranslateUi(RpgCreator);

        QMetaObject::connectSlotsByName(RpgCreator);
    } // setupUi

    void retranslateUi(QMainWindow *RpgCreator)
    {
        RpgCreator->setWindowTitle(QApplication::translate("RpgCreator", "RpgCreator", 0, QApplication::UnicodeUTF8));
        btnStatistics->setText(QApplication::translate("RpgCreator", "Change Statistics", 0, QApplication::UnicodeUTF8));
        btnMap->setText(QApplication::translate("RpgCreator", "Edit Map", 0, QApplication::UnicodeUTF8));
        btnLoad->setText(QApplication::translate("RpgCreator", "Load File", 0, QApplication::UnicodeUTF8));
        btnSave->setText(QApplication::translate("RpgCreator", "Save File", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class RpgCreator: public Ui_RpgCreator {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RPGCREATOR_H
