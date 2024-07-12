/**
 * 
 * ICP Project
 * 
 * @file obstacle.cpp
 * 
 * @brief Instances of this class are obstacles that obstruct robots.
 * 
 * @author Michal Blažek <xblaze38>
 * 
*/

#include "obstacle.h"
#include <QApplication>
#include <QDebug>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QMenu>
#include <QMouseEvent>
#include <QObject>
#include <QPainter>
#include <QPen>
#include <QPainter>

/**
 * @brief Constructs an Obstacle object with given size.
 * @param parent The parent QGraphicsItem (default is nullptr).
 * @param width The width of the obstacle (default is 20).
 * @param height The height of the obstacle (default is 40).
 */
Obstacle::Obstacle(QGraphicsItem *parent, int width, int height) :
    QGraphicsRectItem(parent),
    mRotationAngle(0.0),
    mScaleFactor(1.0)
{
    setRect(200, 180, width, height);
    mWidth = width;
    mHeight = height;

    setFlag(QGraphicsItem::ItemIsMovable);

    setTransformOriginPoint(QPointF(200 + width / 2, 180 + height / 2));
}

/**
 * @brief Overrides paint event handling.
 * @param painter The painter object for painting operations.
 * @param option Style options for the item.
 * @param widget The widget to which the item is painted (default is nullptr).
 */
void Obstacle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setPen(QPen(Qt::black));
    painter->drawRect(boundingRect());

    QColor fillColor = QColor(150, 150, 150, 128);
    painter->fillRect(boundingRect(), fillColor);
}

/**
 * @brief Overrides mouse press event handling.
 * @param event The mouse press event.
 */
void Obstacle::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
    {
        // rotation menu
        QMenu menu;
        QAction *rotateLeftAction = menu.addAction("Rotate left");
        QAction *rotateRightAction = menu.addAction("Rotate right");
        QAction *scaleUpAction = menu.addAction("Scale up");
        QAction *scaleDownAction = menu.addAction("Scale down");

        QAction *selectedAction = menu.exec(event->screenPos());
        if (selectedAction == rotateLeftAction)
        {
            rotate(-45.0 / 2.0);
        }
        else if (selectedAction == rotateRightAction)
        {
            rotate(45.0 / 2.0);
        }
        else if (selectedAction == scaleUpAction)
        {
            scale(1.25);
        }
        else if (selectedAction == scaleDownAction)
        {
            scale(0.8);
        }
    }
    QGraphicsRectItem::mousePressEvent(event);
}

/**
 * @brief Rotates the obstacle by the specified angle.
 * @param angle The angle by which to rotate the obstacle.
 */
void Obstacle::rotate(qreal angle)
{
    mRotationAngle += angle;
    setRotation(mRotationAngle);
}

/**
 * @brief Scales the obstacle by the specified factor.
 * @param scalingFactor The scaling factor by which to scale the obstacle.
 */
void Obstacle::scale(qreal scalingFactor)
{
    mScaleFactor *= scalingFactor;
    setScale(mScaleFactor);
}

/**
 * @brief Gets the rotation angle of the obstacle.
 * @return The rotation angle of the obstacle.
 */
qreal Obstacle::rotationAngle() const
{
    return mRotationAngle;
}

/**
 * @brief Gets the scale factor of the obstacle.
 * @return The scale factor of the obstacle.
 */
qreal Obstacle::scaleFactor() const
{
    return mScaleFactor;
}

/**
 * @brief Changes the rotation angle of the obstacle after load.
 * @param rotation The new rotation angle to set.
 */
void Obstacle::changeRotationAngle(qreal rotation)
{
    mRotationAngle = rotation;
    rotate(0.0);
}

/**
 * @brief Changes the scale factor of the obstacle after load.
 * @param scaleToChange The scaling factor to apply.
 */
void Obstacle::changeScaleFactor(qreal scaleToChange)
{
    mScaleFactor = scaleToChange;
    scale(1.0);
}

/**
 * @brief Changes the position of the obstacle.
 * @param x The new x-coordinate.
 * @param y The new y-coordinate.
 */
void Obstacle::changePosition(qreal x, qreal y)
{
    setPos(pos() + QPointF(x, y));
}

/**
 * @brief Gets the width of the obstacle.
 * @return The width of the obstacle.
 */
int Obstacle::getWidth() const
{
    return mWidth;
}

/**
 * @brief Gets the height of the obstacle.
 * @return The height of the obstacle.
 */
int Obstacle::getHeight() const
{
    return mHeight;
}

/**
 * @brief Enables or disables movement of the obstacle.
 * @param enable If true, movement is enabled; otherwise, it's disabled.
 */
void Obstacle::setMovementEnable(bool enable)
{
    setFlag(QGraphicsItem::ItemIsMovable, enable);
}
