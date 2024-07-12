/**
 * 
 * ICP Project
 * 
 * @file mainwindow.h
 * 
 * @brief Header file representing class MainWindow. Its logic is in mainwindow.cpp.
 * 
 * @authors Michal Blažek <xblaze38>, Kryštof Michálek <xmicha94>
 * 
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "scene.h"
#include "robot.h"
#include "robotlist.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/**
 * @class MainWindow
 * @brief Class representing the window project.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Constructs the MainWindow object.
     * @param parent The parent widget.
     */
    MainWindow(QWidget *parent = nullptr);
    /**
     * @brief Destructs the MainWindow object.
     */
    ~MainWindow();

private slots:
    /**
     * @brief Event handler for close event.
     * @param event The close event object.
     */
    void closeEvent(QCloseEvent *event) override;

    /**
     * @brief Updates robot settings for each robot.
    */
    void getRobotSettings();
    /**
     * @brief Calls function to spawn obstacle with certain size.
     */
    void spawnObstacle();

private:
    /**
     * @brief Initializes the scene.
     * @return Pointer to the initialized scene.
     */
    Scene *initScene();
    /**
     * @brief Initializes the robot list.
     * @return Pointer to the initialized robot list.
     */
    RobotList *initRobotList();
    /**
     * @brief Pointer to robot list.
    */
    RobotList *robotList;

    /**
     * @brief Saves configuration to file.
    */
    void save();
    /**
     * @brief Loads configuration from file.
    */
    void load();
    /**
     * @brief Saves configuration to temporary file.
    */
    void save2();
    /**
     * @brief Loads configuration from temporary file.
    */
    void load2();
    /**
     * @brief Provides saving logic.
     * @param saveStart Bool variable switching between temporary (true) and permanent (false) files.
    */
    void saveFile(bool saveStart = false);
    /**
     * @brief Provides loading logic.
     * @param loadStart Bool variable switching between temporary (true) and permanent (false) files.
    */
    void loadFile(bool loadStart = false);

    /**
     * @brief Clears scene to empty.
    */
    void clear();

    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
