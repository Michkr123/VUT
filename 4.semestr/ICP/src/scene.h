/**
 * 
 * ICP Project
 * 
 * @file scene.h
 * 
 * @brief Header file representing class Scene. Its logic is in scene.cpp.
 * 
 * @authors Michal Blažek <xblaze38>, Kryštof Michálek <xmicha94>
 * 
*/

#ifndef SCENE_H
#define SCENE_H

#include "obstacle.h"
#include "robot.h"
#include "rubbish.h"

#include <QGraphicsScene>

#define TIME_TO_SPAWN_RUBBISH 1000 /**< Time (milliseconds) to spawn rubbish. */

/**
 * @class Scene
 * @brief Class representing the graphics scene in the ICP Project.
 */
class Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    /**
     * @brief Constructs a Scene object.
     * @param parent The parent object (default is nullptr).
     */
    explicit Scene(QObject *parent = nullptr);

    /**
     * @brief Converts the scene to JSON format.
     * @return The scene data in JSON format.
     */
    QString toJSON();

    /**
     * @brief Starts the simulation.
     */
    void start();
    /**
     * @brief Pauses the simulation.
     */
    void pause();

    /**
     * @brief Handles the timeout event for the simulation.
     */
    void timeout();

    /**
     * @brief Moves the player robot forward.
     */
    void forward();
    /**
     * @brief Stops the player robot.
     */
    void stopPlayer();
    /**
     * @brief Rotates the player robot left.
     */
    void left();
    /**
     * @brief Rotates the player robot right.
     */
    void right();
    /**
     * @brief Signals the end of the left rotation.
     */
    void leftEnd();
    /**
     * @brief Signals the end of the right rotation.
     */
    void rightEnd();

    /**
     * @brief Spawns an obstacle on the scene with the specified dimensions.
     * @param width The width of the obstacle.
     * @param height The height of the obstacle.
     */
    void spawnObstacle(int width, int height);
    /**
     * @brief Spawns a robot on the scene.
     */
    void spawnRobot();
    /**
     * @brief Spawns a player robot on the scene.
     */
    void spawnRobotPlayer();
    /**
     * @brief Generates new rubbish items on the scene.
     */
    void generateNewRubbish();
    /**
     * @brief Starts or stops generating rubbish based on the specified flag.
     * @param generateRubbish Flag indicating whether to generate rubbish (true) or stop (false).
     */
    void spawnRubbish(bool generateRubbish);

    /**
     * @brief Spawns an obstacle on the scene from loaded data.
     * @param x The x-coordinate of the obstacle.
     * @param y The y-coordinate of the obstacle.
     * @param rotation The rotation angle of the obstacle.
     * @param scale The scaling factor of the obstacle.
     * @param width The width of the obstacle.
     * @param height The height of the obstacle.
     */
    void spawnObstacleFromLoad(qreal x, qreal y, qreal rotation, qreal scale, int width, int height);
    /**
     * @brief Spawns a robot on the scene from loaded data.
     * @param x The x-coordinate of the robot.
     * @param y The y-coordinate of the robot.
     * @param rotation The rotation angle of the robot.
     * @param player Flag indicating whether the robot is a player (true) or not (false).
     */
    void spawnRobotFromLoad(qreal x, qreal y, qreal rotation, bool player = false);

    /**
     * @brief Clears arrays after loading data.
     */
    void clearArraysAfterLoad();

    /**
     * @brief Resets the robot ID counter.
     */
    void resetID();

    /**
     * @brief Gets a pointer to the robot with the specified ID.
     * @param robotID The ID of the robot to retrieve.
     * @return A pointer to the robot with the specified ID.
     */
    Robot *getRobot(int robotID);
    /**
     * @brief Sets the simulation speed.
     * @param speed The simulation speed value.
     */
    void setSimulationSpeed(int speed);

signals:
    /**
     * @brief Signals that the player robot has been spawned.
     */
    void isPlayer();
    /**
     * @brief Signals that a robot has been added to the list.
     */
    void addRobotToList();

private:
    bool stop;              /**< Flag indicating whether the simulation is paused or stopped. */
    int robotID;            /**< Counter for assigning IDs to robots. */
    int rubbishID;          /**< Counter for assigning IDs to rubbish items. */
    bool playerRobot;       /**< Flag indicating whether a player robot exists. */

    QTimer *mTimer;         /**< Timer for controlling the simulation loop. */
    QTimer *timer;          /**< Timer for moving the player robot forward. */
    QTimer *timerLeft;      /**< Timer for rotating the player robot left. */
    QTimer *timerRight;     /**< Timer for rotating the player robot right. */
    QTimer *timerRotation;  /**< Timer for handling continuous rotation of the player robot. */
    QTimer *rubbishTimer;   /**< Timer for spawning rubbish items. */

    QList<Robot *> mRobots;         /**< List of all robots on the scene. */
    QList<Robot *> mPlayers;        /**< List of player robots on the scene. */
    QList<Obstacle *> mObstacles;   /**< List of obstacles on the scene. */
    QList<Rubbish *> mRubbish;      /**< List of rubbish items on the scene. */

    QList<QGraphicsLineItem *> endOfScene;  /**< List of lines representing the end of the scene. */

    /**
     * @brief Checks if an object is in the way of the player robot.
     * @param player Pointer to the player robot.
     * @return True if an object is in the way, otherwise false.
     */
    bool obstacleInWay(Robot *player);
    /**
     * @brief Checks if rubbish has been vacuumed by the player robot.
     * @param rubbish Pointer to the rubbish item.
     * @return True if the rubbish has been vacuumed, otherwise false.
     */
    bool rubbishVacuumed(Rubbish *rubbish);
    /**
     * @brief Checks if rubbish collides with an obstacle.
     * @param rubbish Pointer to the rubbish item.
     * @return True if the rubbish collides with an obstacle, otherwise false.
     */
    bool rubbishCollideWithObstacle(Rubbish *rubbish);

    int TIME_TO_REFRESH_TIMER; /**< Time (milliseconds) to refresh timers. */
};

#endif // SCENE_H
