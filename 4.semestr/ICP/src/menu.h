/**
 * 
 * ICP Project
 * 
 * @file menu.h
 * 
 * @brief Header file representing class Menu. Its logic is in menu.cpp.
 * 
 * @authors Michal Blažek <xblaze38>, Kryštof Michálek <xmicha94>
 * 
*/

#ifndef MENU_H
#define MENU_H

#include <QWidget>

namespace Ui {
class Menu;
}

/**
 * @class Menu
 * @brief Class representing the menu bar.
 */
class Menu : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructs a Menu object.
     * @param parent The parent widget.
     */
    explicit Menu(QWidget *parent = nullptr);
    /**
     * @brief Destructs the Menu object.
     */
    ~Menu();

    /**
     * @brief Checks if the player is currently active.
     */
    void isPlayer(); 
    /**
     * @brief Retrieves the height of the obstacle before spawning.
     * @return The height of the obstacle.
     */
    int getHeight();
    /**
     * @brief Retrieves the width of the obstacle before spawning.
     * @return The width of the obstacle.
     */
    int getWidth();
    /**
     * @brief Retrieves the simulation speed.
     * @return The simulation speed.
     */
    int getSimulationSpeed();

signals:
    // Emitted signals to communicate with other parts

    void spawnObstacle();
    void spawnRobot();
    void spawnPlayer();
    void spawnRubbish(bool generateRubbish);

    void forward();
    void stopPlayer();
    void left();
    void right();
    void leftEnd();
    void rightEnd();

    void save();
    void load();
    void save2();
    void load2();

    void pause();
    void start();
    void end();

    void clear();

    void simulationSpeed();

private slots:
    // Slots to handle events like buttons clicked

    void onSpawnObstacleButtonClicked();
    void onSpawnRobotButtonClicked();
    void onSpawnPlayerButtonClicked();
    void onSpawnRubbishButtonClicked();

    void onForwardButtonClicked();
    void onStopButtonClicked();
    void leftButtonPressed();
    void rightButtonPressed();
    void leftButtonReleased();
    void rightButtonReleased();

    void onSaveButtonClicked();
    void onLoadButtonClicked();

    void onStartButtonClicked();
    void onResetButtonClicked();
    void onPauseButtonClicked();
    void onContinueButtonClicked();
    void onEndButtonClicked();

    void onClearButtonClicked();

    void changeObstacleSizeLabel();

    void simulationSpeedChange();


private:
    Ui::Menu *ui;
    int height; /**< Height of the obstacle */
    int width;  /**< Width of the obstacle */
    bool generateRubbish;   /**< Flag allowing to generate rubbish */
};

#endif // MENU_H
