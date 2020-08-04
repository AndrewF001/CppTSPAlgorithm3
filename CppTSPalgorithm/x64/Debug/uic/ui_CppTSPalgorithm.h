/********************************************************************************
** Form generated from reading UI file 'CppTSPalgorithm.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CPPTSPALGORITHM_H
#define UI_CPPTSPALGORITHM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CppTSPalgorithmClass
{
public:
    QWidget *centralWidget;
    QPushButton *StartBtn;
    QLabel *label;
    QSpinBox *DelayBox;
    QPushButton *DotsBtn;
    QLabel *label_2;
    QSpinBox *DotsBox;
    QCheckBox *ShowGridCheck;
    QCheckBox *FastModeCheck;
    QLabel *label_3;
    QLabel *TimerLabS;
    QLabel *label_4;
    QSpinBox *FPSBox;
    QLabel *label_5;
    QLabel *TimerLabNS;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *CppTSPalgorithmClass)
    {
        if (CppTSPalgorithmClass->objectName().isEmpty())
            CppTSPalgorithmClass->setObjectName(QString::fromUtf8("CppTSPalgorithmClass"));
        CppTSPalgorithmClass->resize(1850, 950);
        CppTSPalgorithmClass->setMinimumSize(QSize(1850, 950));
        centralWidget = new QWidget(CppTSPalgorithmClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        StartBtn = new QPushButton(centralWidget);
        StartBtn->setObjectName(QString::fromUtf8("StartBtn"));
        StartBtn->setGeometry(QRect(10, 10, 75, 31));
        label = new QLabel(centralWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(100, 10, 81, 31));
        DelayBox = new QSpinBox(centralWidget);
        DelayBox->setObjectName(QString::fromUtf8("DelayBox"));
        DelayBox->setGeometry(QRect(190, 10, 81, 31));
        DelayBox->setMaximum(10000);
        DotsBtn = new QPushButton(centralWidget);
        DotsBtn->setObjectName(QString::fromUtf8("DotsBtn"));
        DotsBtn->setGeometry(QRect(370, 10, 101, 31));
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(480, 10, 41, 31));
        DotsBox = new QSpinBox(centralWidget);
        DotsBox->setObjectName(QString::fromUtf8("DotsBox"));
        DotsBox->setGeometry(QRect(530, 10, 91, 31));
        DotsBox->setMinimum(10);
        DotsBox->setMaximum(1000000);
        ShowGridCheck = new QCheckBox(centralWidget);
        ShowGridCheck->setObjectName(QString::fromUtf8("ShowGridCheck"));
        ShowGridCheck->setGeometry(QRect(700, 10, 121, 31));
        FastModeCheck = new QCheckBox(centralWidget);
        FastModeCheck->setObjectName(QString::fromUtf8("FastModeCheck"));
        FastModeCheck->setGeometry(QRect(830, 10, 111, 31));
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(1210, 10, 91, 31));
        TimerLabS = new QLabel(centralWidget);
        TimerLabS->setObjectName(QString::fromUtf8("TimerLabS"));
        TimerLabS->setGeometry(QRect(1310, 10, 101, 31));
        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(980, 10, 47, 31));
        FPSBox = new QSpinBox(centralWidget);
        FPSBox->setObjectName(QString::fromUtf8("FPSBox"));
        FPSBox->setGeometry(QRect(1030, 10, 81, 31));
        FPSBox->setMaximum(144);
        FPSBox->setSingleStep(0);
        FPSBox->setValue(60);
        label_5 = new QLabel(centralWidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(1420, 10, 91, 31));
        TimerLabNS = new QLabel(centralWidget);
        TimerLabNS->setObjectName(QString::fromUtf8("TimerLabNS"));
        TimerLabNS->setGeometry(QRect(1520, 10, 181, 31));
        CppTSPalgorithmClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(CppTSPalgorithmClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1850, 22));
        CppTSPalgorithmClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(CppTSPalgorithmClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        CppTSPalgorithmClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(CppTSPalgorithmClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        CppTSPalgorithmClass->setStatusBar(statusBar);

        retranslateUi(CppTSPalgorithmClass);

        QMetaObject::connectSlotsByName(CppTSPalgorithmClass);
    } // setupUi

    void retranslateUi(QMainWindow *CppTSPalgorithmClass)
    {
        CppTSPalgorithmClass->setWindowTitle(QCoreApplication::translate("CppTSPalgorithmClass", "CppTSPalgorithm", nullptr));
        StartBtn->setText(QCoreApplication::translate("CppTSPalgorithmClass", "Start", nullptr));
        label->setText(QCoreApplication::translate("CppTSPalgorithmClass", "Delay(MS):", nullptr));
        DotsBtn->setText(QCoreApplication::translate("CppTSPalgorithmClass", "Place Dots", nullptr));
        label_2->setText(QCoreApplication::translate("CppTSPalgorithmClass", "Dots:", nullptr));
        ShowGridCheck->setText(QCoreApplication::translate("CppTSPalgorithmClass", "Show Grid", nullptr));
        FastModeCheck->setText(QCoreApplication::translate("CppTSPalgorithmClass", "Fast Mode", nullptr));
        label_3->setText(QCoreApplication::translate("CppTSPalgorithmClass", "Timer(S):", nullptr));
        TimerLabS->setText(QCoreApplication::translate("CppTSPalgorithmClass", "0", nullptr));
        label_4->setText(QCoreApplication::translate("CppTSPalgorithmClass", "FPS:", nullptr));
        label_5->setText(QCoreApplication::translate("CppTSPalgorithmClass", "Timer(NS):", nullptr));
        TimerLabNS->setText(QCoreApplication::translate("CppTSPalgorithmClass", "0", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CppTSPalgorithmClass: public Ui_CppTSPalgorithmClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CPPTSPALGORITHM_H
