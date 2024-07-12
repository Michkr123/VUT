/**
 * 
 * ICP Project
 * 
 * @file robot.h
 * 
 * @brief Header file representing class Robot. Its logic is in robot.cpp.
 * 
 * @authors Michal Blažek <xblaze38>, Kryštof Michálek <xmicha94>
 * 
*/

#ifndef ROBOT_H
#define ROBOT_H

#include <QGraphicsItemGroup>
#include <QGraphicsItem>

/**
 * @class Robot
 * @brief Class representing the robots in the scene.
 */
class Robot : public QGraphicsItemGroup
{
    friend class Scene;

public:
    /**
     * @brief Constructs a Robot object with the specified ID.
     * @param id The ID of the robot.
     * @param parent The parent QGraphicsItem (default is nullptr).
     */
    Robot(int id, QGraphicsItem *parent = nullptr);

    /**
     * @brief Gets the body of the robot.
     * @return The QGraphicsEllipseItem representing the body of the robot.
     */
    QGraphicsEllipseItem *getBody() const;
    /**
     * @brief Gets the ID of the robot.
     * @return The ID of the robot.
     */
    int getRobotID() const;
    /**
     * @brief Gets the rotation angle of the robot.
     * @return The rotation angle of the robot.
     */
    qreal rotationAngle() const;
    /**
     * @brief Changes the rotation angle of the robot.
     * @param rotation The new rotation angle to set.
     */
    void changeRotationAngle(qreal rotation);
    /**
     * @brief Enables or disables movement of the robot.
     * @param enable If true, movement is enabled; otherwise, it's disabled.
     */
    void setMovementEnable(bool enable);
    /**
     * @brief Changes the position of the robot.
     * @param x The new x-coordinate.
     * @param y The new y-coordinate.
     */
    void changePosition(qreal x, qreal y);
    /**
     * @brief Sets the angle of the robot.
     * @param newAngle The new angle to set.
     */
    void setAngle(int newAngle);
    /**
     * @brief Gets the angle of the robot.
     * @return The angle of the robot.
     */
    int getAngle();
    /**
     * @brief Gets the remaining angle of the robot.
     * @return The remaining angle of the robot.
     */
    int getRemainingAngle();
    /**
     * @brief Sets the remaining angle of the robot.
     * @param angle The remaining angle to set.
     */
    void setRemainingAngle(int angle);
    /**
     * @brief Sets the detection distance of the robot.
     * @param length The detection distance to set.
     */
    void setDetectionDistance(int length);

protected:
    /**
     * @brief Overrides mouse press event handling.
     * @param event The mouse press event.
     */
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

public slots:
    /**
     * @brief Moves the robot forward.
     */
    void moveForward();

private:
    qreal mRotationAngle;       /**< Rotation angle of the robot. */
    int robotID;                /**< ID of the robot. */
    int angle;                  /**< Angle of the robot. */
    int rectLength;             /**< Length of the rectangle representing the robot's body. */
    int remainingAngle;         /**< Remaining angle of the robot. */

    QGraphicsLineItem *mLine;       /**< Line representing the direction of movement. */
    QGraphicsEllipseItem *mEllipse; /**< Ellipse representing the body of the robot. */
    QGraphicsRectItem *mRect;       /**< Rectangle representing the body of the robot. */

    /**
     * @brief Rotates the robot by the specified angle.
     * @param angle The angle by which to rotate the robot.
     */
    void rotate(qreal angle);
};

#endif // ROBOT_H
