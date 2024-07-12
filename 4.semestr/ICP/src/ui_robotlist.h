/********************************************************************************
** Form generated from reading UI file 'robotlist.ui'
**
** Created by: Qt User Interface Compiler version 5.15.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ROBOTLIST_H
#define UI_ROBOTLIST_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RobotList
{
public:
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *RobotList)
    {
        if (RobotList->objectName().isEmpty())
            RobotList->setObjectName(QString::fromUtf8("RobotList"));
        RobotList->setWindowModality(Qt::ApplicationModal);
        RobotList->resize(522, 653);
        RobotList->setMinimumSize(QSize(520, 635));
        RobotList->setLayoutDirection(Qt::LeftToRight);
        verticalLayout = new QVBoxLayout(RobotList);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        retranslateUi(RobotList);

        QMetaObject::connectSlotsByName(RobotList);
    } // setupUi

    void retranslateUi(QWidget *RobotList)
    {
        RobotList->setWindowTitle(QCoreApplication::translate("RobotList", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RobotList: public Ui_RobotList {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ROBOTLIST_H
