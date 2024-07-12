/**
 * 
 * ICP Project
 * 
 * @file menu.cpp
 * 
 * @brief File representing menu bar. There are controls to operate with player robot or spawn object to scene or just start, stop, reset or end simulation.
 * 
 * @authors Michal Blažek <xblaze38>, Kryštof Michálek <xmicha94>
 * 
*/

#include "mainwindow.h"
#include "menu.h"
#include "ui_menu.h"
#include "scene.h"

#include <QPushButton>
#include <QDebug>

/**
 * @brief Constructs a Menu object.
 * @param parent The parent widget.
 */
Menu::Menu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Menu),
    height(40),
    width(20),
    generateRubbish(false)
{
    ui->setupUi(this);

    connect(ui->SpawnObstacleButton, &QPushButton::clicked, this, &Menu::onSpawnObstacleButtonClicked);
    connect(ui->SpawnRobotButton, &QPushButton::clicked, this, &Menu::onSpawnRobotButtonClicked);
    connect(ui->SpawnPlayerButton, &QPushButton::clicked, this, &Menu::onSpawnPlayerButtonClicked);
    connect(ui->spawnRubbishButton, &QPushButton::clicked, this, &Menu::onSpawnRubbishButtonClicked);

    connect(ui->ForwardButton, &QPushButton::clicked, this, &Menu::onForwardButtonClicked);
    connect(ui->StopButton, &QPushButton::clicked, this, &Menu::onStopButtonClicked);
    connect(ui->LeftButton, &QPushButton::pressed, this, &Menu::leftButtonPressed);
    connect(ui->RightButton, &QPushButton::pressed, this, &Menu::rightButtonPressed);
    connect(ui->LeftButton, &QPushButton::released, this, &Menu::leftButtonReleased);
    connect(ui->RightButton, &QPushButton::released, this, &Menu::rightButtonReleased);

    connect(ui->SaveButton, &QPushButton::clicked, this, &Menu::onSaveButtonClicked);
    connect(ui->LoadButton, &QPushButton::clicked, this, &Menu::onLoadButtonClicked);

    connect(ui->StartButton, &QPushButton::clicked, this, &Menu::onStartButtonClicked);
    connect(ui->PauseButton, &QPushButton::clicked, this, &Menu::onPauseButtonClicked);
    connect(ui->ContinueButton, &QPushButton::clicked, this, &Menu::onContinueButtonClicked);
    connect(ui->ResetButton, &QPushButton::clicked, this, &Menu::onResetButtonClicked);
    connect(ui->EndButton, &QPushButton::clicked, this, &Menu::onEndButtonClicked);

    connect(ui->ClearButton, &QPushButton::clicked, this, &Menu::onClearButtonClicked);

    connect(ui->ObstacleUpDownSlider, &QSlider::valueChanged, this, &Menu::changeObstacleSizeLabel);
    connect(ui->ObstacleLeftRightSlider, &QSlider::valueChanged, this, &Menu::changeObstacleSizeLabel);
    connect(ui->speedSlider, &QSlider::valueChanged, this, &Menu::simulationSpeedChange);


    ui->StartButton->show();
    ui->PauseButton->hide();
    ui->ContinueButton->hide();
    ui->ResetButton->hide();
    ui->EndButton->show();

    ui->SaveButton->show();

    ui->SpawnObstacleButton->show();
    ui->SpawnRobotButton->show();
    ui->SpawnPlayerButton->show();

    ui->StopButton->hide();
    ui->ForwardButton->hide();
    ui->LeftButton->hide();
    ui->RightButton->hide();

    ui->ClearButton->show();

    ui->ObstacleLeftRightSlider->show();
    ui->ObstacleUpDownSlider->show();
    ui->ObstacleSizeLabel->show();
    ui->speedSlider->hide();
    ui->speedLabel->hide();

    ui->SpawnPlayerButton->setEnabled(true);
}

/**
 * @brief Destructs the Menu object.
 */
Menu::~Menu()
{
    delete ui;
}

// Slot implementations for various button clicks

void Menu::onSpawnObstacleButtonClicked()
{
    emit spawnObstacle();
}

void Menu::onSpawnRobotButtonClicked()
{
    emit spawnRobot();
}

void Menu::onSpawnPlayerButtonClicked()
{
    emit spawnPlayer();
    ui->SpawnPlayerButton->setEnabled(false);
}

void Menu::onSpawnRubbishButtonClicked()
{
    // Toggle button for rubbish generation
    if (generateRubbish)
    {
        ui->spawnRubbishButton->setText("Generate rubbish");
        generateRubbish = false;
    }
    else
    {
        ui->spawnRubbishButton->setText("Stop generating rubbish");
        generateRubbish = true;
    }
    emit spawnRubbish(generateRubbish);
}

void Menu::onForwardButtonClicked()
{
    emit forward();
}

void Menu::onStopButtonClicked()
{
    emit stopPlayer();
}

void Menu::leftButtonPressed()
{
    emit left();
}

void Menu::rightButtonPressed()
{
    emit right();
}

void Menu::leftButtonReleased()
{
    emit leftEnd();
}

void Menu::rightButtonReleased()
{
    emit rightEnd();
}

void Menu::onSaveButtonClicked()
{
    emit save();
}

void Menu::onLoadButtonClicked()
{
    emit pause();
    emit load();

    ui->spawnRubbishButton->setText("Generate rubbish");
    generateRubbish = false;

    ui->StartButton->show();
    ui->PauseButton->hide();
    ui->ContinueButton->hide();
    ui->ResetButton->hide();
    ui->EndButton->show();

    ui->SaveButton->show();

    ui->SpawnObstacleButton->show();
    ui->SpawnRobotButton->show();
    ui->SpawnPlayerButton->show();

    ui->StopButton->hide();
    ui->ForwardButton->hide();
    ui->LeftButton->hide();
    ui->RightButton->hide();

    ui->ClearButton->show();

    ui->ObstacleLeftRightSlider->show();
    ui->ObstacleUpDownSlider->show();
    ui->ObstacleSizeLabel->show();
    ui->speedSlider->hide();
    ui->speedLabel->hide();
}

void Menu::onStartButtonClicked()
{
    emit save2();
    emit start();

    ui->StartButton->hide();
    ui->PauseButton->show();
    ui->ContinueButton->hide();
    ui->ResetButton->show();
    ui->EndButton->show();

    ui->SaveButton->hide();

    ui->SpawnObstacleButton->hide();
    ui->SpawnRobotButton->hide();
    ui->SpawnPlayerButton->hide();

    ui->StopButton->show();
    ui->ForwardButton->show();
    ui->LeftButton->show();
    ui->RightButton->show();

    ui->ClearButton->hide();

    ui->ObstacleLeftRightSlider->hide();
    ui->ObstacleUpDownSlider->hide();
    ui->ObstacleSizeLabel->hide();
    ui->speedSlider->show();
    ui->speedLabel->show();
}

void Menu::onResetButtonClicked()
{
    emit pause();
    emit load2();

    ui->spawnRubbishButton->setText("Generate rubbish");
    generateRubbish = false;

    ui->StartButton->show();
    ui->PauseButton->hide();
    ui->ContinueButton->hide();
    ui->ResetButton->hide();
    ui->EndButton->show();

    ui->SaveButton->show();

    ui->SpawnObstacleButton->show();
    ui->SpawnRobotButton->show();
    ui->SpawnPlayerButton->show();

    ui->StopButton->hide();
    ui->ForwardButton->hide();
    ui->LeftButton->hide();
    ui->RightButton->hide();

    ui->ClearButton->show();

    ui->ObstacleLeftRightSlider->show();
    ui->ObstacleUpDownSlider->show();
    ui->ObstacleSizeLabel->show();
    ui->speedSlider->hide();
    ui->speedLabel->hide();
}

void Menu::onPauseButtonClicked()
{
    emit pause();

    ui->StartButton->hide();
    ui->PauseButton->hide();
    ui->ContinueButton->show();
    ui->ResetButton->show();
    ui->EndButton->show();

    ui->SaveButton->hide();

    ui->SpawnObstacleButton->show();
    ui->SpawnRobotButton->show();
    ui->SpawnPlayerButton->show();

    ui->StopButton->hide();
    ui->ForwardButton->hide();
    ui->LeftButton->hide();
    ui->RightButton->hide();

    ui->ClearButton->hide();

    ui->ObstacleLeftRightSlider->show();
    ui->ObstacleUpDownSlider->show();
    ui->ObstacleSizeLabel->show();
    ui->speedSlider->hide();
    ui->speedLabel->hide();
}

void Menu::onContinueButtonClicked()
{
    emit start();

    ui->StartButton->hide();
    ui->PauseButton->show();
    ui->ContinueButton->hide();
    ui->ResetButton->show();
    ui->EndButton->show();

    ui->SaveButton->hide();

    ui->SpawnObstacleButton->hide();
    ui->SpawnRobotButton->hide();
    ui->SpawnPlayerButton->hide();

    ui->StopButton->show();
    ui->ForwardButton->show();
    ui->LeftButton->show();
    ui->RightButton->show();

    ui->ClearButton->hide();

    ui->ObstacleLeftRightSlider->hide();
    ui->ObstacleUpDownSlider->hide();
    ui->ObstacleSizeLabel->hide();
    ui->speedSlider->show();
    ui->speedLabel->show();
}

void Menu::onEndButtonClicked()
{
    emit load2();
    exit(0);
}

void Menu::onClearButtonClicked()
{
    ui->SpawnPlayerButton->setEnabled(true);

    ui->spawnRubbishButton->setText("Generate rubbish");
    generateRubbish = false;
    emit clear();
}

void Menu::changeObstacleSizeLabel()
{
    width = ui->ObstacleLeftRightSlider->value();
    height = ui->ObstacleUpDownSlider->value();
    QString text = QString::number(width) + "x" + QString::number(height);
    ui->ObstacleSizeLabel->setText(text);
}

void Menu::simulationSpeedChange()
{
    emit simulationSpeed();
}

/**
 * @brief Retrieves the simulation speed.
 * @return The simulation speed.
 */
int Menu::getSimulationSpeed()
{
    return ui->speedSlider->value();
}

/**
 * @brief Checks if the player is currently active.
 */
void Menu::isPlayer()
{
    ui->SpawnPlayerButton->setEnabled(false);
}

/**
 * @brief Retrieves the height of the obstacle before spawning.
 * @return The height of the obstacle.
 */
int Menu::getHeight()
{
    return height;
}

/**
 * @brief Retrieves the width of the obstacle before spawning.
 * @return The width of the obstacle.
 */
int Menu::getWidth()
{
    return width;
}
