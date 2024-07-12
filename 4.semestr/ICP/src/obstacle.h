/**
 * 
 * ICP Project
 * 
 * @file obstacle.h
 * 
 * @brief Header file representing class Obstacle. Its logic is in obstacle.cpp.
 * 
 * @author Michal Blažek <xblaze38>
 * 
*/

#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <QGraphicsRectItem>
#include <QPaintEvent>

/**
 * @class Obstacle
 * @brief Class representing the obstacles in the scene.
 */
class Obstacle : public QGraphicsRectItem
{
public:
    /**
     * @brief Constructs an Obstacle object with given size.
     * @param parent The parent QGraphicsItem (default is nullptr).
     * @param width The width of the obstacle (default is 20).
     * @param height The height of the obstacle (default is 40).
     */
    Obstacle(QGraphicsItem *parent = nullptr, int width = 20, int height = 40);

    /**
     * @brief Gets the rotation angle of the obstacle.
     * @return The rotation angle of the obstacle.
     */
    qreal rotationAngle() const;
    /**
     * @brief Gets the scale factor of the obstacle.
     * @return The scale factor of the obstacle.
     */
    qreal scaleFactor() const;
    /**
     * @brief Changes the rotation angle of the obstacle after load.
     * @param rotation The new rotation angle to set.
     */
    void changeRotationAngle(qreal rotation);
    /**
     * @brief Changes the scale factor of the obstacle after load.
     * @param scaleToChange The scaling factor to apply.
     */
    void changeScaleFactor(qreal scaleToChange);
    /**
     * @brief Changes the position of the obstacle.
     * @param x The new x-coordinate.
     * @param y The new y-coordinate.
     */
    void changePosition(qreal x, qreal y);

    /**
     * @brief Gets the width of the obstacle.
     * @return The width of the obstacle.
     */
    int getWidth() const;
    /**
     * @brief Gets the height of the obstacle.
     * @return The height of the obstacle.
     */
    int getHeight() const;

    /**
     * @brief Enables or disables movement of the obstacle.
     * @param enable If true, movement is enabled; otherwise, it's disabled.
     */
    void setMovementEnable(bool enable);

protected:
    /**
     * @brief Overrides mouse press event handling.
     * @param event The mouse press event.
     */
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    /**
     * @brief Overrides paint event handling.
     * @param painter The painter object for painting operations.
     * @param option Style options for the item.
     * @param widget The widget to which the item is painted (default is nullptr).
     */
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    int mWidth;             /**< Obstacle width */
    int mHeight;            /**< Obstacle height */

    qreal mRotationAngle;   /**< Rotation angle of the obstacle */
    qreal mScaleFactor;     /**< Scale factor of the obstacle */
    /**
     * @brief Rotates the obstacle by the specified angle.
     * @param angle The angle by which to rotate the obstacle.
     */
    void rotate(qreal angle);
    /**
     * @brief Scales the obstacle by the specified factor.
     * @param scalingFactor The scaling factor by which to scale the obstacle.
     */
    void scale(qreal scalingFactor);
};

#endif // OBSTACLE_H
