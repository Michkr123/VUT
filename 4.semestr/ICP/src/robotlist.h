/**
 * 
 * ICP Project
 * 
 * @file robotlist.h
 * 
 * @brief Header file representing class RobotList. Its logic is in robotlist.cpp.
 * 
 * @authors Michal Blažek <xblaze38>, Kryštof Michálek <xmicha94>
 * 
*/

#ifndef ROBOTLIST_H
#define ROBOTLIST_H

#include <QWidget>

namespace Ui {
class RobotList;
}

/**
 * @struct RobotSettings
 * @brief Structure representing the settings of a robot.
 */
struct RobotSettings {
    QString robotID;        /**< The ID of the robot. */
    int distance;           /**< The detection distance of the robot. */
    int rotation;           /**< The rotation angle of the robot. */
    bool rotationDirection; /**< The rotation direction of the robot (true for left, false for right). */
};

/**
 * @class RobotList
 * @brief Class representing the list of robots and theirs settings.
 */
class RobotList : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructs a RobotList object.
     * @param parent The parent widget (default is nullptr).
     */
    explicit RobotList(QWidget *parent = nullptr);
    /**
     * @brief Destructor for RobotList.
     */
    ~RobotList();

    /**
     * @brief Gets the count of robots in the list.
     * @return The count of robots in the list.
     */
    int getRobotCount();

    /**
     * @brief Gets the settings of the robot at the specified index.
     * @param robotIndex The index of the robot in the list.
     * @return The settings of the specified robot.
     */
    RobotSettings getRobotSettings(int robotIndex) const;

public slots:
    /**
     * @brief Spawns a new robot.
     */
    void spawnRobot();

    /**
     * @brief Updates the robot list.
     */
    void update();

    /**
     * @brief Clears the robot list.
     */
    void clearArray();

signals:
    /**
     * @brief Emits signal when the settings of a robot are changed.
     */
    void settingsChanged();

private:
    int i;  /**< Counter for robot IDs. */

    Ui::RobotList *ui;

    QList<QWidget *> robotWidgets;      /**< List of widget items representing robots. */
    QList<RobotSettings> settingsList;  /**< List of settings for each robot. */
};

#endif // ROBOTLIST_H
