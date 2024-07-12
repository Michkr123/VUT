/**
 * 
 * ICP Project
 * 
 * @file scene.cpp
 * 
 * @brief This represents scene where all the object are spawning, moving, rotating... It contains information about all the objects in lists.
 * 
 * @authors Michal Blažek <xblaze38>, Kryštof Michálek <xmicha94>
 * 
*/

#include "scene.h"
#include <QDebug>
#include <QGraphicsRectItem>
#include <QGraphicsLineItem>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "robot.h"
#include "obstacle.h"
#include "rubbish.h"
#include <QTimer>

/**
 * @brief Constructs a Scene object.
 * @param parent The parent object (default is nullptr).
 */
Scene::Scene(QObject *parent)
    : QGraphicsScene{parent},
      stop(false),
      robotID(0),
      rubbishID(0),
      playerRobot(false),
      TIME_TO_REFRESH_TIMER(25)
{
    mTimer = new QTimer(this);
    timer = new QTimer(this);
    timerLeft = new QTimer(this);
    timerRight = new QTimer(this);
    timerRotation = new QTimer(this);
    rubbishTimer = new QTimer(this);
    connect(mTimer, &QTimer::timeout, this, &Scene::timeout);

    endOfScene.append(new QGraphicsLineItem(0, 0, 0, 690));
    endOfScene.append(new QGraphicsLineItem(0, 0, 990, 0));
    endOfScene.append(new QGraphicsLineItem(0, 690, 990, 690));
    endOfScene.append(new QGraphicsLineItem(990, 0, 990, 690));
}

/**
 * @brief Converts the scene to JSON format.
 * @return The scene data in JSON format.
 */
QString Scene::toJSON()
{
    QJsonArray arr;
    for (auto *obstacle : mObstacles)
    {
        // rotation
        qreal rotationAngle = obstacle->rotationAngle();

        // scale
        qreal scaleFactor = obstacle->scaleFactor();

        // type
        QString type = "Obstacle";

        // size
        int width = obstacle->getWidth();
        int height = obstacle->getHeight();

        arr.append(QJsonObject({
            {"x", obstacle->x()},
            {"y", obstacle->y()},
            {"object", type},
            {"rotation", rotationAngle},
            {"scale", scaleFactor},
            {"width", width},
            {"height", height}
        }));
    }
    for (auto *robot : mRobots)
    {
        // rotation
        qreal rotationAngle = robot->rotationAngle();

        // type
        QString type = "Robot";

        arr.append(QJsonObject({
            {"x", robot->x()},
            {"y", robot->y()},
            {"object", type},
            {"rotation", rotationAngle}
        }));
    }
    for (auto *player : mPlayers)
    {
        // rotation
        qreal rotationAngle = player->rotationAngle();

        // type
        QString type = "Player";

        arr.append(QJsonObject({
            {"x", player->x()},
            {"y", player->y()},
            {"object", type},
            {"rotation", rotationAngle}
        }));
    }

    QJsonObject obj{{"items", arr}};
    QJsonDocument doc;
    doc.setObject(obj);

    return doc.toJson(QJsonDocument::Indented);
}

/**
 * @brief Starts the simulation.
 */
void Scene::start()
{
    for (Robot *robot : mRobots)
    {
        robot->setMovementEnable(false);
    }
    for (Robot *player : mPlayers)
    {
        emit isPlayer();
        player->setMovementEnable(false);
    }
    for (Obstacle *obstacle : mObstacles)
    {
        obstacle->setMovementEnable(false);
    }
    mTimer->start(TIME_TO_REFRESH_TIMER);
    timer->start(TIME_TO_REFRESH_TIMER);
}

/**
 * @brief Pauses the simulation.
 */
void Scene::pause()
{
    for (Robot *robot : mRobots)
    {
        robot->setMovementEnable(true);
    }
    for (Robot *player : mPlayers)
    {
        emit isPlayer();
        player->setMovementEnable(true);
    }
    for (Obstacle *obstacle : mObstacles)
    {
        obstacle->setMovementEnable(true);
    }
    mTimer->stop();
    timer->stop();
}

/**
 * @brief Handles the timeout event for the simulation.
 */
void Scene::timeout()
{
    for (Robot *robot : mRobots)
    {
        if (!obstacleInWay(robot) && robot->getRemainingAngle() == robot->getAngle())
        {
            robot->moveForward();
        }
        else
        {
            if(robot->getAngle() > 0)
            {
                robot->rotate(1.0);
                robot->setRemainingAngle((robot->getRemainingAngle()) - 1);
            }
            else
            {
                robot->rotate(-1.0);
                robot->setRemainingAngle((robot->getRemainingAngle()) + 1);
            }

            if(robot->getRemainingAngle() == 0)
            {
                robot->setRemainingAngle(robot->getAngle());
            }

        }
    }
    QList<Rubbish *> rubbishToDelete;
    for (Rubbish *rubbish : mRubbish)
    {
        if(rubbishVacuumed(rubbish))
        {
            rubbishToDelete.append(rubbish);
        }
        if(rubbishCollideWithObstacle(rubbish))
        {
            rubbishToDelete.append(rubbish);
        }
    }

    for (Rubbish *rubbish : rubbishToDelete)
    {
        removeItem(rubbish);
        mRubbish.removeOne(rubbish);
        delete rubbish;
    }

}

/**
 * @brief Moves the player robot forward.
 */
void Scene::forward()
{
    stop = false;
    for (Robot *player : mPlayers)
    {
        disconnect(timer, &QTimer::timeout, nullptr, nullptr);
        connect(timer, &QTimer::timeout, [=]()
        {
            if (!obstacleInWay(player) && !stop)
            {
                player->moveForward();
            }
            else
            {
                timer->stop();
            }
        });
        timer->start(TIME_TO_REFRESH_TIMER);
    }
}

/**
 * @brief Stops the player robot.
 */
void Scene::stopPlayer()
{
    stop = true;
}

/**
 * @brief Rotates the player robot left.
 */
void Scene::left()
{
    for (Robot *player : mPlayers)
    {
        disconnect(timerLeft, &QTimer::timeout, nullptr, nullptr);
        connect(timerLeft, &QTimer::timeout, [=]()
        {
            player->rotate(-1.0);
        });
        timerLeft->start(TIME_TO_REFRESH_TIMER);
    }
}

/**
 * @brief Rotates the player robot right.
 */
void Scene::right()
{
    for (Robot *player : mPlayers)
    {
        disconnect(timerRight, &QTimer::timeout, nullptr, nullptr);
        connect(timerRight, &QTimer::timeout, [=]()
        {
            player->rotate(1.0);
        });
        timerRight->start(TIME_TO_REFRESH_TIMER);
    }
}

/**
 * @brief Signals the end of the left rotation.
 */
void Scene::leftEnd()
{
    timerLeft->stop();
}

/**
 * @brief Signals the end of the right rotation.
 */
void Scene::rightEnd()
{
    timerRight->stop();
}

/**
 * @brief Spawns an obstacle on the scene with the specified dimensions.
 * @param width The width of the obstacle.
 * @param height The height of the obstacle.
 */
void Scene::spawnObstacle(int width, int height)
{
    Obstacle *obstacle = new Obstacle(nullptr, width, height);
    addItem(obstacle);
    mObstacles.append(obstacle);
}

/**
 * @brief Generates new rubbish items on the scene.
 */
void Scene::generateNewRubbish()
{
    Rubbish *rubbish = new Rubbish(rubbishID++);
    //    addItem(rubbish);
    //    mRubbish.append(rubbish);
    for (Obstacle *obstacle : mObstacles)
    {
        if(rubbish->collidesWithItem(obstacle))
        {
            delete rubbish;
            generateNewRubbish();
            return;
        }
    }
    addItem(rubbish);
    mRubbish.append(rubbish);
}

/**
 * @brief Starts or stops generating rubbish based on the specified flag.
 * @param generateRubbish Flag indicating whether to generate rubbish (true) or stop (false).
 */
void Scene::spawnRubbish(bool generateRubbish)
{
    if (generateRubbish)
    {
        disconnect(rubbishTimer, &QTimer::timeout, nullptr, nullptr);
        connect(rubbishTimer, &QTimer::timeout, this, &Scene::generateNewRubbish);
        rubbishTimer->start(TIME_TO_SPAWN_RUBBISH);
    }
    else
    {
        rubbishTimer->stop();
    }
}

/**
 * @brief Spawns an obstacle on the scene from loaded data.
 * @param x The x-coordinate of the obstacle.
 * @param y The y-coordinate of the obstacle.
 * @param rotation The rotation angle of the obstacle.
 * @param scale The scaling factor of the obstacle.
 * @param width The width of the obstacle.
 * @param height The height of the obstacle.
 */
void Scene::spawnObstacleFromLoad(qreal x, qreal y, qreal rotation, qreal scale, int width, int height)
{
    Obstacle *obstacle = new Obstacle(nullptr, width, height);
    obstacle->changePosition(x, y);
    obstacle->changeRotationAngle(rotation);
    obstacle->changeScaleFactor(scale);

    addItem(obstacle);
    mObstacles.append(obstacle);
}

/**
 * @brief Spawns a robot on the scene.
 */
void Scene::spawnRobot()
{
    Robot *robot = new Robot(robotID++);

    addItem(robot);
    mRobots.append(robot);
    emit addRobotToList();
}

/**
 * @brief Spawns a robot on the scene from loaded data.
 * @param x The x-coordinate of the robot.
 * @param y The y-coordinate of the robot.
 * @param rotation The rotation angle of the robot.
 * @param player Flag indicating whether the robot is a player (true) or not (false).
 */
void Scene::spawnRobotFromLoad(qreal x, qreal y, qreal rotation, bool player)
{
    if (player)
    {
        Robot *player = new Robot(-1);
        player->changePosition(x, y);
        player->changeRotationAngle(rotation);
        addItem(player);
        mPlayers.append(player);
        playerRobot = true;
    }
    else
    {
        Robot *robot = new Robot(robotID++);
        robot->changePosition(x, y);
        robot->changeRotationAngle(rotation);
        addItem(robot);
        mRobots.append(robot);
        emit addRobotToList();
    }
}

/**
 * @brief Clears arrays after loading data.
 */
void Scene::clearArraysAfterLoad()
{
    mPlayers.clear();
    mRobots.clear();
    mObstacles.clear();
    playerRobot = false;
    rubbishTimer->stop();
    mRubbish.clear();
    stopPlayer();
    disconnect(timer, &QTimer::timeout, nullptr, nullptr);
}

/**
 * @brief Spawns a player robot on the scene.
 */
void Scene::spawnRobotPlayer()
{
    if (!playerRobot)
    {
        Robot *player = new Robot(-1);
        player->changePosition(0, 60);
        addItem(player);
        mPlayers.append(player);
        playerRobot = true;
    }
}

/**
 * @brief Checks if an object is in the way of the player robot.
 * @param player Pointer to the player robot.
 * @return True if an object is in the way, otherwise false.
 */
bool Scene::obstacleInWay(Robot *robot)
{
    for (Obstacle *obstacle : mObstacles)
    {
        if (robot->collidesWithItem(obstacle))
        {
            return true;
        }
    }
    for (QGraphicsLineItem *lineAtTheEnd : endOfScene)
    {
        if (robot->collidesWithItem(lineAtTheEnd))
        {
            return true;
        }
    }
    for (Robot *otherRobot : mRobots)
    {
        if (robot == otherRobot)
        {
            continue;
        }
        if (robot->collidesWithItem(otherRobot->getBody()))
        {
            return true;
        }
    }
    for (Robot *player : mPlayers)
    {
        if (robot == player)
        {
            continue;
        }
        if (robot->collidesWithItem(player->getBody()))
        {
            return true;
        }
    }
    return false;
}

/**
 * @brief Checks if rubbish has been vacuumed by the player robot.
 * @param rubbish Pointer to the rubbish item.
 * @return True if the rubbish has been vacuumed, otherwise false.
 */
bool Scene::rubbishVacuumed(Rubbish *rubbish)
{
    for (Robot *robot : mRobots)
    {
        if(rubbish->collidesWithItem(robot->getBody()))
        {
            return true;
        }
    }
    for (Robot *player : mPlayers)
    {
        if(rubbish->collidesWithItem(player->getBody()))
        {
            return true;
        }
    }
    return false;
}

/**
 * @brief Checks if rubbish collides with an obstacle.
 * @param rubbish Pointer to the rubbish item.
 * @return True if the rubbish collides with an obstacle, otherwise false.
 */
bool Scene::rubbishCollideWithObstacle(Rubbish *rubbish)
{
    for(Obstacle *obstacle : mObstacles)
    {
        if(rubbish->collidesWithItem(obstacle))
        {
            return true;
        }
    }
    return false;
}

/**
 * @brief Resets the robot ID counter.
 */
void Scene::resetID()
{
    robotID = 0;
}

/**
 * @brief Sets the simulation speed.
 * @param speed The simulation speed value.
 */
void Scene::setSimulationSpeed(int speed)
{
    TIME_TO_REFRESH_TIMER = 50 - speed;
    mTimer->setInterval(TIME_TO_REFRESH_TIMER);
    timer->setInterval(TIME_TO_REFRESH_TIMER);
    timerLeft->setInterval(TIME_TO_REFRESH_TIMER);
    timerRight->setInterval(TIME_TO_REFRESH_TIMER);
    timerRotation->setInterval(TIME_TO_REFRESH_TIMER);
}

/**
 * @brief Gets a pointer to the robot with the specified ID.
 * @param robotID The ID of the robot to retrieve.
 * @return A pointer to the robot with the specified ID.
 */
Robot *Scene::getRobot(int robotID)
{
    for (Robot *robot : mRobots)
    {
        if (robotID == robot->getRobotID())
        {
            return robot;
        }
    }
    return NULL;
}
