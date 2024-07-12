/**
 * 
 * ICP Project
 * 
 * @file mainwindow.cpp
 * 
 * @brief File representing window. It is primarily used to connect signals and slot from different objects.
 * 
 * @authors Michal Blažek <xblaze38>, Kryštof Michálek <xmicha94>
 * 
*/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "robotlist.h"
#include "scene.h"
#include <QGraphicsItem>
#include "menu.h"
#include <QByteArray>
#include <QDebug>
#include <QFile>
#include <QIODevice>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTimer>

/**
 * @brief Constructs the MainWindow object.
 * @param parent The parent widget.
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    resize(1600, 900);
    Scene *scene = initScene();
    robotList = initRobotList();

    connect(ui->menu, &Menu::spawnObstacle, this, &MainWindow::spawnObstacle);
    connect(ui->menu, &Menu::spawnRobot, scene, &Scene::spawnRobot);
    connect(scene, &Scene::addRobotToList, robotList, &RobotList::spawnRobot);
    connect(ui->menu, &Menu::spawnPlayer, scene, &Scene::spawnRobotPlayer);
    connect(ui->menu, &Menu::spawnRubbish, scene, &Scene::spawnRubbish);

    connect(ui->menu, &Menu::forward, scene, &Scene::forward);
    connect(ui->menu, &Menu::stopPlayer, scene, &Scene::stopPlayer);
    connect(ui->menu, &Menu::left, scene, &Scene::left);
    connect(ui->menu, &Menu::right, scene, &Scene::right);
    connect(ui->menu, &Menu::leftEnd, scene, &Scene::leftEnd);
    connect(ui->menu, &Menu::rightEnd, scene, &Scene::rightEnd);

    connect(ui->menu, &Menu::save, this, &MainWindow::save);
    connect(ui->menu, &Menu::load, this, &MainWindow::load);
    connect(ui->menu, &Menu::save2, this, &MainWindow::save2);
    connect(ui->menu, &Menu::load2, this, &MainWindow::load2);

    connect(ui->menu, &Menu::start, scene, &Scene::start);
    connect(ui->menu, &Menu::start, this, &MainWindow::getRobotSettings);
    connect(ui->SaveButton, &QPushButton::clicked, this, &MainWindow::getRobotSettings);
    connect(ui->menu, &Menu::pause, scene, &Scene::pause);

    connect(ui->menu, &Menu::clear, this, &MainWindow::clear);

    connect(scene, &Scene::isPlayer, ui->menu, &Menu::isPlayer);
    connect(ui->menu, &Menu::simulationSpeed, scene, [=]() {
        scene->setSimulationSpeed(ui->menu->getSimulationSpeed());
    });
}

/**
 * @brief Destructs the MainWindow object.
 */
MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @brief Calls function to spawn obstacle with certain size.
 */
void MainWindow::spawnObstacle()
{
    int width = ui->menu->getWidth();
    int height = ui->menu->getHeight();

    Scene *scene = dynamic_cast<Scene *>(ui->graphicsView->scene());
    if (scene) {
        scene->spawnObstacle(width, height);
    } else {
        qDebug() << "Failed to get the scene for spawning an obstacle";
    }
}

/**
 * @brief Event handler for close event.
 * @param event The close event object.
 */
void MainWindow::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);

    load2();
    exit(0);
}

/**
 * @brief Initializes the scene.
 * @return Pointer to the initialized scene.
 */
Scene *MainWindow::initScene()
{
    Scene *scene = new Scene(ui->graphicsView);
    ui->graphicsView->setScene(scene);

    ui->graphicsView->setSceneRect(0, 0, 990, 690);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView->setResizeAnchor(QGraphicsView::NoAnchor);
    return scene;
}

/**
 * @brief Initializes the robot list.
 * @return Pointer to the initialized robot list.
 */
RobotList *MainWindow::initRobotList()
{
    RobotList *robotList = new RobotList(ui->robotList);
    return robotList;
}

/**
 * @brief Saves configuration to file.
*/
void MainWindow::save()
{
    saveFile();
}

/**
 * @brief Saves configuration to temporary file.
*/
void MainWindow::save2()
{
    saveFile(true);
}

/**
 * @brief Provides saving logic.
 * @param saveStart Bool variable switching between temporary (true) and permanent (false) files.
*/
void MainWindow::saveFile(bool saveStart)
{
    Scene *scene = dynamic_cast<Scene *>(ui->graphicsView->scene());
    if (scene)
    {
        QString filename = "savedScene.json";
        if (saveStart)
        {
            filename = "tmpSavedScene.json";
        }

        QFile file(filename);
        if (file.exists() && !file.remove())
        {
            qDebug() << "Failed to remove existing file:" << file.errorString();
            return;
        }
        if (!file.open(QIODevice::WriteOnly))
        {
            qDebug() << "Failed to open file for writing:" << file.errorString();
            return;
        }


        qint64 bytesWritten = file.write(scene->toJSON().toUtf8());
        if (bytesWritten == -1)
        {
            qDebug() << "Failed to write JSON data to file:" << file.errorString();
            return;
        }
        file.close();
    }
}

/**
 * @brief Loads configuration from file.
*/
void MainWindow::load()
{
    loadFile();
}

/**
 * @brief Loads configuration from temporary file.
*/
void MainWindow::load2()
{
    loadFile(true);
}

/**
 * @brief Provides loading logic.
 * @param loadStart Bool variable switching between temporary (true) and permanent (false) files.
*/
void MainWindow::loadFile(bool loadStart)
{
    Scene *scene = dynamic_cast<Scene *>(ui->graphicsView->scene());
    if (scene)
    {
        QString filename = "savedScene.json";
        if (loadStart)
        {
            filename = "tmpSavedScene.json";
        }

        QFile file(filename);

        if (!file.open(QIODevice::ReadOnly))
        {
            return;
        }

        scene->clear();
        scene->clearArraysAfterLoad();
        robotList->clearArray();
        scene->resetID();

        QByteArray dataFromFile = file.readAll();
        file.close();

        QJsonDocument doc = QJsonDocument::fromJson(dataFromFile);
        if (doc.isNull())
        {
            return;
        }
        if (!doc.isObject())
        {
            return;
        }

        QJsonObject obj = doc.object();
        QJsonArray arr = obj["items"].toArray();

        for (const QJsonValue &value : arr)
        {
            QJsonObject parameter = value.toObject();
            qreal x = parameter["x"].toDouble();
            qreal y = parameter["y"].toDouble();
            QString objectType = parameter["object"].toString();
            qreal rotation = parameter["rotation"].toDouble();
            qreal scale = parameter["scale"].toDouble();
            int width = parameter["width"].toInt();
            int height = parameter["height"].toInt();

            if (objectType == "Obstacle")
            {
                scene->spawnObstacleFromLoad(x, y, rotation, scale, width, height);
            }
            else if (objectType == "Robot")
            {
                scene->spawnRobotFromLoad(x, y, rotation);
            }
            else if (objectType == "Player")
            {
                scene->spawnRobotFromLoad(x, y, rotation, true);
            }
        }

        if (loadStart)
        {
            QFile::remove("tmpSavedScene.json");
        }
    }
}

/**
 * @brief Clears scene to empty.
*/
void MainWindow::clear()
{
    Scene *scene = dynamic_cast<Scene *>(ui->graphicsView->scene());
    if (scene)
    {
        QFile file("tmpSavedScene.json");
        if (file.exists() && !file.remove())
        {
            return;
        }
        if (!file.open(QIODevice::WriteOnly))
        {
            return;
        }
        file.close();
        load2();
    }
}

/**
 * @brief Updates robot settings for each robot.
*/
void MainWindow::getRobotSettings()
{
    Scene *scene = dynamic_cast<Scene *>(ui->graphicsView->scene());
    if (scene)
    {
        int robotCount = robotList->getRobotCount();
        for (int robotIndex = 0; robotIndex < robotCount; robotIndex++)
        {
            RobotSettings settings = robotList->getRobotSettings(robotIndex);

            bool ok;
            int robotID = settings.robotID.toInt(&ok);
            if (!ok)
            {
                qDebug() << "Conversion failed!";
                continue;
            }

            Robot *robot = scene->getRobot(robotID);
            if (robot)
            {
                if(settings.rotationDirection)
                {
                    robot->setAngle(-settings.rotation);
                }
                else
                {
                    robot->setAngle(settings.rotation);
                }
                robot->setRemainingAngle(robot->getAngle());
                robot->setDetectionDistance(settings.distance);
            }

        }
    }
}
