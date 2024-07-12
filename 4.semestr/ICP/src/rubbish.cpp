/**
 * 
 * ICP Project
 * 
 * @file rubbish.cpp
 * 
 * @brief There is randomly generating rubbish in scene when it is enabled from button in menu.
 * 
 * @author Kryštof Michálek <xmicha94>
 * 
*/

#include "rubbish.h"
#include <QPainter>
#include <QLinearGradient>
#include <cstdlib>
#include <cmath>

/**
 * @brief Constructs a Rubbish object with the specified ID.
 * @param id The ID of the rubbish item.
 * @param parent The parent graphics item (default is nullptr).
 */
Rubbish::Rubbish(int id, QGraphicsItem *parent) :
    QGraphicsItem(parent),
    rubbishID(id)
{
    // Seed
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // Generate random coordinates within the scene's dimensions
    qreal x = static_cast<qreal>(std::rand()) / RAND_MAX * 950 + 20;
    qreal y = static_cast<qreal>(std::rand()) / RAND_MAX * 600 + 40;

    // Set the position of the rubbish item
    setPos(x, y);

    // Generate a random rotation angle between 0 and 360 degrees
    qreal rotationAngle = static_cast<qreal>(std::rand()) / RAND_MAX * 360;
    setRotation(rotationAngle);

    // Choose shape of rubbish
    QList<int> possibleShapes {3, 4, 5, 6, 7, 9};
    int index = std::rand() % possibleShapes.size();
    shape = possibleShapes[index];
}

/**
 * @brief Returns the bounding rectangle of the rubbish item.
 * @return The bounding rectangle of the rubbish item.
 */
QRectF Rubbish::boundingRect() const
{
    return QRectF(0, 0, 20, 20);
}

/**
 * @brief Paints the rubbish item on the graphics scene.
 * @param painter The painter object used for drawing.
 * @param option Style options for the item (not used).
 * @param widget The widget on which the item is drawn (not used).
 */
void Rubbish::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    // Draw a star-like shape
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(Qt::NoPen);

    // Define the base color for the star
    QColor baseColor = QColor(200, 200, 200);

    // Create a gradient for shading
    QLinearGradient gradient(boundingRect().topLeft(), boundingRect().bottomRight());
    gradient.setColorAt(0, baseColor.lighter(150));
    gradient.setColorAt(1, baseColor);

    // Draw the base star shape
    QPainterPath starPath;
    generateStarPath(starPath);
    painter->setBrush(gradient);
    painter->drawPath(starPath);
}

/**
 * @brief Generates the path for drawing a star-shaped rubbish item.
 * @param path Reference to the QPainterPath object to store the generated path.
 */
void Rubbish::generateStarPath(QPainterPath &path)
{
    // Define the bounding box of the Rubbish item
    QRectF boundingBox = boundingRect();

    // Calculate the center of the bounding box
    qreal centerX = boundingBox.center().x();
    qreal centerY = boundingBox.center().y();

    // Calculate the outer and inner radius of the star
    qreal outerRadius = boundingBox.width() / 2;
    qreal innerRadius = outerRadius * 0.4;

    // Calculate the angle step for each point of the star
    qreal angleStep = 2 * M_PI / shape;

    // Move to the first point of the star
    qreal x = centerX + outerRadius * cos(-M_PI / 2);
    qreal y = centerY + outerRadius * sin(-M_PI / 2);
    path.moveTo(x, y);

    // Generate the points of the star
    for (int i = 1; i <= 5; ++i) {
        qreal angle = -M_PI / 2 + angleStep * i;
        x = centerX + (i % 2 ? outerRadius : innerRadius) * cos(angle);
        y = centerY + (i % 2 ? outerRadius : innerRadius) * sin(angle);
        path.lineTo(x, y);
    }

    // Close the path
    path.closeSubpath();
}

/**
 * @brief Gets the ID of the rubbish item.
 * @return The ID of the rubbish item.
 */
int Rubbish::getRubbishID()
{
    return rubbishID;
}
