/********************************************************************************
** Form generated from reading UI file 'menu.ui'
**
** Created by: Qt User Interface Compiler version 5.15.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MENU_H
#define UI_MENU_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Menu
{
public:
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout_4;
    QSpacerItem *verticalSpacer_6;
    QPushButton *SpawnObstacleButton;
    QSlider *ObstacleUpDownSlider;
    QSlider *ObstacleLeftRightSlider;
    QSpacerItem *verticalSpacer_5;
    QVBoxLayout *verticalLayout_5;
    QSpacerItem *verticalSpacer_8;
    QLabel *ObstacleSizeLabel;
    QSpacerItem *verticalSpacer_9;
    QPushButton *SpawnRobotButton;
    QSpacerItem *verticalSpacer_7;
    QSpacerItem *verticalSpacer_10;
    QPushButton *SpawnPlayerButton;
    QVBoxLayout *verticalLayout_2;
    QSpacerItem *verticalSpacer;
    QPushButton *ForwardButton;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *LeftButton;
    QPushButton *RightButton;
    QPushButton *StopButton;
    QSpacerItem *verticalSpacer_2;
    QSpacerItem *horizontalSpacer_2;
    QHBoxLayout *horizontalLayout_4;
    QVBoxLayout *verticalLayout_6;
    QSpacerItem *verticalSpacer_12;
    QLabel *speedLabel;
    QSlider *speedSlider;
    QSpacerItem *verticalSpacer_11;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *spawnRubbishButton;
    QSpacerItem *horizontalSpacer;
    QPushButton *ClearButton;
    QVBoxLayout *verticalLayout_3;
    QSpacerItem *verticalSpacer_4;
    QPushButton *LoadButton;
    QPushButton *SaveButton;
    QSpacerItem *verticalSpacer_3;
    QVBoxLayout *verticalLayout;
    QPushButton *StartButton;
    QPushButton *PauseButton;
    QPushButton *ContinueButton;
    QPushButton *ResetButton;
    QPushButton *EndButton;

    void setupUi(QWidget *Menu)
    {
        if (Menu->objectName().isEmpty())
            Menu->setObjectName(QString::fromUtf8("Menu"));
        Menu->resize(1201, 243);
        horizontalLayout = new QHBoxLayout(Menu);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        verticalSpacer_6 = new QSpacerItem(20, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_4->addItem(verticalSpacer_6);

        SpawnObstacleButton = new QPushButton(Menu);
        SpawnObstacleButton->setObjectName(QString::fromUtf8("SpawnObstacleButton"));
        SpawnObstacleButton->setMaximumSize(QSize(115, 16777215));

        verticalLayout_4->addWidget(SpawnObstacleButton);

        ObstacleUpDownSlider = new QSlider(Menu);
        ObstacleUpDownSlider->setObjectName(QString::fromUtf8("ObstacleUpDownSlider"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ObstacleUpDownSlider->sizePolicy().hasHeightForWidth());
        ObstacleUpDownSlider->setSizePolicy(sizePolicy);
        ObstacleUpDownSlider->setMinimumSize(QSize(0, 70));
        ObstacleUpDownSlider->setMaximumSize(QSize(100, 70));
        ObstacleUpDownSlider->setMinimum(10);
        ObstacleUpDownSlider->setMaximum(150);
        ObstacleUpDownSlider->setValue(40);
        ObstacleUpDownSlider->setOrientation(Qt::Vertical);

        verticalLayout_4->addWidget(ObstacleUpDownSlider);

        ObstacleLeftRightSlider = new QSlider(Menu);
        ObstacleLeftRightSlider->setObjectName(QString::fromUtf8("ObstacleLeftRightSlider"));
        sizePolicy.setHeightForWidth(ObstacleLeftRightSlider->sizePolicy().hasHeightForWidth());
        ObstacleLeftRightSlider->setSizePolicy(sizePolicy);
        ObstacleLeftRightSlider->setMinimumSize(QSize(115, 0));
        ObstacleLeftRightSlider->setMinimum(10);
        ObstacleLeftRightSlider->setMaximum(150);
        ObstacleLeftRightSlider->setValue(20);
        ObstacleLeftRightSlider->setOrientation(Qt::Horizontal);

        verticalLayout_4->addWidget(ObstacleLeftRightSlider);

        verticalSpacer_5 = new QSpacerItem(20, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_4->addItem(verticalSpacer_5);


        horizontalLayout->addLayout(verticalLayout_4);

        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        verticalSpacer_8 = new QSpacerItem(20, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_5->addItem(verticalSpacer_8);

        ObstacleSizeLabel = new QLabel(Menu);
        ObstacleSizeLabel->setObjectName(QString::fromUtf8("ObstacleSizeLabel"));
        sizePolicy.setHeightForWidth(ObstacleSizeLabel->sizePolicy().hasHeightForWidth());
        ObstacleSizeLabel->setSizePolicy(sizePolicy);
        ObstacleSizeLabel->setMaximumSize(QSize(60, 20));

        verticalLayout_5->addWidget(ObstacleSizeLabel);

        verticalSpacer_9 = new QSpacerItem(20, 25, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_5->addItem(verticalSpacer_9);

        SpawnRobotButton = new QPushButton(Menu);
        SpawnRobotButton->setObjectName(QString::fromUtf8("SpawnRobotButton"));

        verticalLayout_5->addWidget(SpawnRobotButton);

        verticalSpacer_7 = new QSpacerItem(20, 49, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_5->addItem(verticalSpacer_7);

        verticalSpacer_10 = new QSpacerItem(20, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_5->addItem(verticalSpacer_10);


        horizontalLayout->addLayout(verticalLayout_5);

        SpawnPlayerButton = new QPushButton(Menu);
        SpawnPlayerButton->setObjectName(QString::fromUtf8("SpawnPlayerButton"));

        horizontalLayout->addWidget(SpawnPlayerButton);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalSpacer = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);

        ForwardButton = new QPushButton(Menu);
        ForwardButton->setObjectName(QString::fromUtf8("ForwardButton"));

        verticalLayout_2->addWidget(ForwardButton);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        LeftButton = new QPushButton(Menu);
        LeftButton->setObjectName(QString::fromUtf8("LeftButton"));

        horizontalLayout_2->addWidget(LeftButton);

        RightButton = new QPushButton(Menu);
        RightButton->setObjectName(QString::fromUtf8("RightButton"));

        horizontalLayout_2->addWidget(RightButton);


        verticalLayout_2->addLayout(horizontalLayout_2);

        StopButton = new QPushButton(Menu);
        StopButton->setObjectName(QString::fromUtf8("StopButton"));

        verticalLayout_2->addWidget(StopButton);

        verticalSpacer_2 = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_2);


        horizontalLayout->addLayout(verticalLayout_2);

        horizontalSpacer_2 = new QSpacerItem(20, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        verticalSpacer_12 = new QSpacerItem(20, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_6->addItem(verticalSpacer_12);

        speedLabel = new QLabel(Menu);
        speedLabel->setObjectName(QString::fromUtf8("speedLabel"));
        speedLabel->setMaximumSize(QSize(122, 20));

        verticalLayout_6->addWidget(speedLabel);

        speedSlider = new QSlider(Menu);
        speedSlider->setObjectName(QString::fromUtf8("speedSlider"));
        speedSlider->setMaximumSize(QSize(122, 16777215));
        speedSlider->setMinimum(1);
        speedSlider->setMaximum(48);
        speedSlider->setValue(25);
        speedSlider->setOrientation(Qt::Horizontal);

        verticalLayout_6->addWidget(speedSlider);

        verticalSpacer_11 = new QSpacerItem(20, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_6->addItem(verticalSpacer_11);


        horizontalLayout_4->addLayout(verticalLayout_6);

        horizontalSpacer_3 = new QSpacerItem(20, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_3);

        spawnRubbishButton = new QPushButton(Menu);
        spawnRubbishButton->setObjectName(QString::fromUtf8("spawnRubbishButton"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(spawnRubbishButton->sizePolicy().hasHeightForWidth());
        spawnRubbishButton->setSizePolicy(sizePolicy1);
        spawnRubbishButton->setMinimumSize(QSize(173, 0));
        spawnRubbishButton->setMaximumSize(QSize(173, 16777215));

        horizontalLayout_4->addWidget(spawnRubbishButton);


        horizontalLayout->addLayout(horizontalLayout_4);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        ClearButton = new QPushButton(Menu);
        ClearButton->setObjectName(QString::fromUtf8("ClearButton"));

        horizontalLayout->addWidget(ClearButton);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer_4);

        LoadButton = new QPushButton(Menu);
        LoadButton->setObjectName(QString::fromUtf8("LoadButton"));

        verticalLayout_3->addWidget(LoadButton);

        SaveButton = new QPushButton(Menu);
        SaveButton->setObjectName(QString::fromUtf8("SaveButton"));

        verticalLayout_3->addWidget(SaveButton);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer_3);


        horizontalLayout->addLayout(verticalLayout_3);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        StartButton = new QPushButton(Menu);
        StartButton->setObjectName(QString::fromUtf8("StartButton"));

        verticalLayout->addWidget(StartButton);

        PauseButton = new QPushButton(Menu);
        PauseButton->setObjectName(QString::fromUtf8("PauseButton"));

        verticalLayout->addWidget(PauseButton);

        ContinueButton = new QPushButton(Menu);
        ContinueButton->setObjectName(QString::fromUtf8("ContinueButton"));

        verticalLayout->addWidget(ContinueButton);

        ResetButton = new QPushButton(Menu);
        ResetButton->setObjectName(QString::fromUtf8("ResetButton"));

        verticalLayout->addWidget(ResetButton);

        EndButton = new QPushButton(Menu);
        EndButton->setObjectName(QString::fromUtf8("EndButton"));

        verticalLayout->addWidget(EndButton);


        horizontalLayout->addLayout(verticalLayout);


        retranslateUi(Menu);

        QMetaObject::connectSlotsByName(Menu);
    } // setupUi

    void retranslateUi(QWidget *Menu)
    {
        Menu->setWindowTitle(QCoreApplication::translate("Menu", "Form", nullptr));
        SpawnObstacleButton->setText(QCoreApplication::translate("Menu", "Spawn obstacle", nullptr));
        ObstacleSizeLabel->setText(QCoreApplication::translate("Menu", "20x40", nullptr));
        SpawnRobotButton->setText(QCoreApplication::translate("Menu", "Spawn robot", nullptr));
        SpawnPlayerButton->setText(QCoreApplication::translate("Menu", "Spawn player robot", nullptr));
        ForwardButton->setText(QCoreApplication::translate("Menu", "Forward", nullptr));
        LeftButton->setText(QCoreApplication::translate("Menu", "Left", nullptr));
        RightButton->setText(QCoreApplication::translate("Menu", "Right", nullptr));
        StopButton->setText(QCoreApplication::translate("Menu", "Stop", nullptr));
        speedLabel->setText(QCoreApplication::translate("Menu", "Simulation speed:", nullptr));
        spawnRubbishButton->setText(QCoreApplication::translate("Menu", "Generate rubbish", nullptr));
        ClearButton->setText(QCoreApplication::translate("Menu", "Clear scene", nullptr));
        LoadButton->setText(QCoreApplication::translate("Menu", "Load", nullptr));
        SaveButton->setText(QCoreApplication::translate("Menu", "Save", nullptr));
        StartButton->setText(QCoreApplication::translate("Menu", "Start", nullptr));
        PauseButton->setText(QCoreApplication::translate("Menu", "Pause", nullptr));
        ContinueButton->setText(QCoreApplication::translate("Menu", "Continue", nullptr));
        ResetButton->setText(QCoreApplication::translate("Menu", "Reset", nullptr));
        EndButton->setText(QCoreApplication::translate("Menu", "End", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Menu: public Ui_Menu {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MENU_H
