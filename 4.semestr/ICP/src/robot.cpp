/**
 * 
 * ICP Project
 * 
 * @file robot.cpp
 * 
 * @brief Instances of this class are robots or player robot. Player robot is just robot but has id -1 and is controlled by buttons in menu.
 * 
 * @authors Michal Blažek <xblaze38>, Kryštof Michálek <xmicha94>
 * 
*/

#include "robot.h"
#include "scene.h"
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QMenu>
#include <QPainter>
#include <QPen>
#include <QtMath>
#include <QGraphicsItemGroup>
#include <QThread>

/**
 * @brief Constructs a Robot object with the specified ID.
 * @param id The ID of the robot.
 * @param parent The parent QGraphicsItem (default is nullptr).
 */
Robot::Robot(int id, QGraphicsItem *parent) :
    QGraphicsItemGroup(parent),
    mRotationAngle(0.0),
    robotID(id)
{
    mLine = new QGraphicsLineItem(200, 250, 220, 250, this);
    mEllipse = new QGraphicsEllipseItem(180, 230, 40, 40, this);
    mRect = new QGraphicsRectItem(200, 230, 80, 40, this);

    if (id < 0)
    {
        QPen playerPen(Qt::blue);
        mLine->setPen(playerPen);
        mEllipse->setPen(playerPen);
        mRect->setPen(playerPen);
    }

    mRect->setOpacity(0.3);

    addToGroup(mLine);
    addToGroup(mEllipse);
    addToGroup(mRect);

    setFlag(QGraphicsItem::ItemIsMovable);

    setTransformOriginPoint(QPointF(200,250));
}

/**
 * @brief Gets the body of the robot.
 * @return The QGraphicsEllipseItem representing the body of the robot.
 */
QGraphicsEllipseItem *Robot::getBody() const
{
    return mEllipse;
}

/**
 * @brief Gets the ID of the robot.
 * @return The ID of the robot.
 */
int Robot::getRobotID() const
{
    return robotID;
}

/**
 * @brief Overrides mouse press event handling.
 * @param event The mouse press event.
 */
void Robot::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
    {
        // rotation menu
        QMenu menu;

        menu.addSection("RobotID: " + QString::number(robotID));

        QAction *rotateLeftAction = menu.addAction("Rotate left");
        QAction *rotateRightAction = menu.addAction("Rotate right");

        QAction *selectedAction = menu.exec(event->screenPos());
        if (selectedAction == rotateLeftAction)
        {
            rotate(-45.0);
        }
        else if (selectedAction == rotateRightAction)
        {
            rotate(45.0);
        }
    }
    QGraphicsItemGroup::mousePressEvent(event);
}

/**
 * @brief Rotates the robot by the specified angle.
 * @param angle The angle by which to rotate the robot.
 */
void Robot::rotate(qreal angle)
{
    mRotationAngle += angle;
    setRotation(mRotationAngle);
}

/**
 * @brief Gets the rotation angle of the robot.
 * @return The rotation angle of the robot.
 */
qreal Robot::rotationAngle() const
{
    return mRotationAngle;
}

/**
 * @brief Changes the rotation angle of the robot.
 * @param rotation The new rotation angle to set.
 */
void Robot::changeRotationAngle(qreal rotation)
{
    mRotationAngle = rotation;
    rotate(0.0);
}

/**
 * @brief Sets the angle of the robot.
 * @param newAngle The new angle to set.
 */
void Robot::setAngle(int newAngle)
{
    angle = newAngle;
}

/**
 * @brief Gets the angle of the robot.
 * @return The angle of the robot.
 */
int Robot::getAngle()
{
    return angle;
}

/**
 * @brief Sets the remaining angle of the robot.
 * @param angle The remaining angle to set.
 */
void Robot::setRemainingAngle(int angle)
{
    remainingAngle = angle;
}

/**
 * @brief Gets the remaining angle of the robot.
 * @return The remaining angle of the robot.
 */
int Robot::getRemainingAngle()
{
    return remainingAngle;
}

/**
 * @brief Sets the detection distance of the robot.
 * @param length The detection distance to set.
 */
void Robot::setDetectionDistance(int length)
{
    removeFromGroup(mRect);
    mRect->setRect(200, 230, length, 40);
    addToGroup(mRect);
}

/**
 * @brief Enables or disables movement of the robot.
 * @param enable If true, movement is enabled; otherwise, it's disabled.
 */
void Robot::setMovementEnable(bool enable)
{
    setFlag(QGraphicsItem::ItemIsMovable, enable);
}

/**
 * @brief Changes the position of the robot.
 * @param x The new x-coordinate.
 * @param y The new y-coordinate.
 */
void Robot::changePosition(qreal x, qreal y)
{
    setPos(pos() + QPointF(x, y));
}

/**
 * @brief Moves the robot forward.
 */
void Robot::moveForward()
{
    qreal angleInRad = qDegreesToRadians(mRotationAngle);
    QPointF direction(qCos(angleInRad), qSin(angleInRad));

    QPointF newPosition = pos() + direction;

    setPos(newPosition);
}
