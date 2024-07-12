/**
 * 
 * ICP Project
 * 
 * @file rubbish.h
 * 
 * @brief Header file representing class Rubbish. Its logic is in rubbish.cpp.
 * 
 * @author Kryštof Michálek <xmicha94>
 * 
*/

#ifndef RUBBISH_H
#define RUBBISH_H

#include <QGraphicsItem>
#include <QPainter>


/**
 * @class Rubbish
 * @brief Class representing the rubbish in the scene.
 */
class Rubbish : public QGraphicsItem
{
public:
    /**
     * @brief Constructs a Rubbish object with the specified ID.
     * @param id The ID of the rubbish item.
     * @param parent The parent graphics item (default is nullptr).
     */
    Rubbish(int id, QGraphicsItem *parent = nullptr);

    /**
     * @brief Returns the bounding rectangle of the rubbish item.
     * @return The bounding rectangle of the rubbish item.
     */
    QRectF boundingRect() const override;

    /**
     * @brief Gets the ID of the rubbish item.
     * @return The ID of the rubbish item.
     */
    int getRubbishID();

protected:
    /**
     * @brief Paints the rubbish item on the graphics scene.
     * @param painter The painter object used for drawing.
     * @param option Style options for the item (not used).
     * @param widget The widget on which the item is drawn (not used).
     */
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    int rubbishID; /**< The ID of the rubbish item. */
    int shape;     /**< The shape of the rubbish item. */

    /**
     * @brief Generates the path for drawing a star-shaped rubbish item.
     * @param path Reference to the QPainterPath object to store the generated path.
     */
    void generateStarPath(QPainterPath &path);
};

#endif // RUBBISH_H
