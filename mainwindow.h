#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <conio.h>
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <thread>
#include <mutex>
#include "skinslipsubwindow.h"
#include "skinslipdelaysubwindow.h"
#include "squeezesubwindow.h"
#include "pushsubwindow.h"
#include "continuousskinslipsubwindow.h"
#include "continuousskinslipdelaysubwindow.h"

#include "dynamixel_sdk.h"
#include "ServoUtility.h"

// Control table address
#define ADDR_PRO_TORQUE_ENABLE          64                 // Control table address is different in Dynamixel model
#define ADDR_PRO_GOAL_POSITION          116
#define ADDR_PRO_PRESENT_POSITION       132
#define ADDR_PRO_PROFILE_VELOCITY       112
#define ADDR_OPERATING_MODE             11

// Data Byte Length
#define LEN_PRO_GOAL_POSITION           4
#define LEN_PRO_PRESENT_POSITION        4

// Protocol version
#define PROTOCOL_VERSION                2.0                 // See which protocol version is used in the Dynamixel

// Default setting
#define DXL1_ID                         1                   // Dynamixel#1 ID: 1
#define DXL2_ID                         2                   // Dynamixel#2 ID: 2
#define DXL3_ID                         3                   // Dynamixel#3 ID: 3
#define DXL4_ID                         4                   // Dynamixel#4 ID: 4
#define DXL5_ID                         5                   // Dynamixel#5 ID: 5
#define DXL6_ID                         6                   // Dynamixel#5 ID: 6
#define NUM_OF_DXL                      6                   // Number of Dynamixels
#define BAUDRATE                        1000000
#define DEVICENAME                      "COM5"              // Check which port is being used on your controller
                                                            // ex) Windows: "COM1"   Linux: "/dev/ttyUSB0" Mac: "/dev/tty.usbserial-*"

#define TORQUE_ENABLE                   1                   // Value for enabling the torque
#define TORQUE_DISABLE                  0                   // Value for disabling the torque
#define DXL_MINIMUM_POSITION_VALUE      0                   // Dynamixel will rotate between this value
#define DXL_MAXIMUM_POSITION_VALUE      1024                // and this value (note that the Dynamixel would not move when the position value is out of movable range. Check e-manual about the range of the Dynamixel you use.)
#define DXL_MOVING_STATUS_THRESHOLD     20                  // Dynamixel moving status threshold
#define DXL_VELOCITY_VALUE              100                 // rotating velocity of Dynamixels
#define DXL_VELOCITY_RESET_VALUE        0                   // reset value of velocity for Dynamixels
#define ESC_ASCII_VALUE                 0x1b

class SkinSlipSubWindow;
class SkinSlipDelaySubWindow;
class SqueezeSubWindow;
class PushSubWindow;
class ContinuousSkinSlipSubWindow;
class ContinuousSkinSlipDelaySubWindow;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    ServoUtility servoUtility;
    int dxl_ids[NUM_OF_DXL] = { DXL1_ID, DXL2_ID, DXL3_ID, DXL4_ID, DXL5_ID, DXL6_ID };

private slots:
    void on_skinSlipBtn_clicked();

    void on_skinSlipDelayBtn_clicked();

    void on_squeezeBtn_clicked();

    void on_pushBtn_clicked();

    void on_continuousSkinSlipBtn_clicked();

    void on_continuousSkinSlipDelayBtn_clicked();

private:
    Ui::MainWindow *ui;
    SkinSlipSubWindow *skinSlipSubWindow;
    SkinSlipDelaySubWindow *skinSlipDelaySubWindow;
    SqueezeSubWindow *squeezeSubWindow;
    PushSubWindow *pushSubWindow;
    ContinuousSkinSlipSubWindow *continuousSkinSlipSubWindow;
    ContinuousSkinSlipDelaySubWindow *continuousSkinSlipDelaySubWindow;
};

#endif // MAINWINDOW_H
