#include "squeezesubwindow.h"
#include "ui_squeezesubwindow.h"

SqueezeSubWindow::SqueezeSubWindow(MainWindow *mainWinPtr, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SqueezeSubWindow),
    mainWindowPtr(mainWinPtr)
{
    ui->setupUi(this);
    ui->start_btn->setEnabled(false);
}

SqueezeSubWindow::~SqueezeSubWindow()
{
    delete ui;
}

void SqueezeSubWindow::on_angle_slider_valueChanged(int value)
{
    ui->angle_val->setText(QString::number(value));
}

void SqueezeSubWindow::on_velocity_slider_valueChanged(int value)
{
    ui->velocity_val->setText(QString::number(value));
}

void SqueezeSubWindow::on_reset_btn_clicked()
{
    ui->angle_slider->setEnabled(false);
    ui->velocity_slider->setEnabled(false);
    ui->quit_btn->setEnabled(false);
    ui->reset_btn->setEnabled(false);

    for (int i = 0; i < NUM_OF_DXL; i++) {
        mainWindowPtr->servoUtility.disableTorque(mainWindowPtr->dxl_ids[i]);
        mainWindowPtr->servoUtility.setOperatingMode(mainWindowPtr->dxl_ids[i], 4);
        mainWindowPtr->servoUtility.enableTorque(mainWindowPtr->dxl_ids[i]);
        mainWindowPtr->servoUtility.setVelocity(mainWindowPtr->dxl_ids[i], DXL_VELOCITY_VALUE);
    }
    qDebug() << "Resetting servos positions...";
    mainWindowPtr->servoUtility.resetPositionCustom(mainWindowPtr->dxl_ids, 0, firstReset);
    firstReset = 0;
    qDebug() << "Servos positions have been reset!";
    for (int i = 0; i < NUM_OF_DXL; i++) {
        mainWindowPtr->servoUtility.disableTorque(mainWindowPtr->dxl_ids[i]);
        mainWindowPtr->servoUtility.setOperatingMode(mainWindowPtr->dxl_ids[i], 3);
        mainWindowPtr->servoUtility.enableTorque(mainWindowPtr->dxl_ids[i]);
        mainWindowPtr->servoUtility.setVelocity(mainWindowPtr->dxl_ids[i], ui->velocity_slider->value());
    }
    ui->start_btn->setEnabled(true);
}

void SqueezeSubWindow::on_start_btn_clicked()
{
    ui->start_btn->setEnabled(false);
    int32_t dxls_present_position[NUM_OF_DXL];
    int dxls_goal_position[NUM_OF_DXL];
    for (int i = 0; i < NUM_OF_DXL; i++) {
        dxls_present_position[i] = 0;
        if (i < NUM_OF_DXL / 2) {
            dxls_goal_position[i] = ui->angle_slider->value();
        }
        else {
            dxls_goal_position[i] = 2048 - (ui->angle_slider->value() - 2048);
        }
    }
    std::thread thread_sync_read(&ServoUtility::syncReadPosition, &mainWindowPtr->servoUtility, NUM_OF_DXL, mainWindowPtr->dxl_ids, dxls_present_position, dxls_goal_position);
    std::thread thread_sync_write(&ServoUtility::syncWritePosition, &mainWindowPtr->servoUtility, NUM_OF_DXL, mainWindowPtr->dxl_ids, dxls_goal_position);
    thread_sync_write.join();
    thread_sync_read.join();
    // Change servos velocity
    for (int i = 0; i < NUM_OF_DXL; i++) {
        mainWindowPtr->servoUtility.setVelocity(mainWindowPtr->dxl_ids[i], DXL_VELOCITY_VALUE);
    }
    ui->angle_slider->setEnabled(true);
    ui->velocity_slider->setEnabled(true);
    ui->reset_btn->setEnabled(true);
    ui->quit_btn->setEnabled(true);
    qDebug() << "Finished performing squeeze!";
}

void SqueezeSubWindow::on_quit_btn_clicked()
{
    ui->start_btn->setEnabled(false);
    if (firstReset == 1) {
        // Change servos velocity
        for (int i = 0; i < NUM_OF_DXL; i++) {
            mainWindowPtr->servoUtility.setVelocity(mainWindowPtr->dxl_ids[i], DXL_VELOCITY_VALUE);
        }
        qDebug() << "Resetting servos positions...";
        mainWindowPtr->servoUtility.resetPosition(mainWindowPtr->dxl_ids, 0);
        qDebug() << "Servos positions have been reset!";
    } else {
        for (int i = 0; i < NUM_OF_DXL; i++) {
            mainWindowPtr->servoUtility.disableTorque(mainWindowPtr->dxl_ids[i]);
            mainWindowPtr->servoUtility.setOperatingMode(mainWindowPtr->dxl_ids[i], 4);
            mainWindowPtr->servoUtility.enableTorque(mainWindowPtr->dxl_ids[i]);
            mainWindowPtr->servoUtility.setVelocity(mainWindowPtr->dxl_ids[i], DXL_VELOCITY_VALUE);
        }
        qDebug() << "Resetting servos positions...";
        mainWindowPtr->servoUtility.quitCustom(mainWindowPtr->dxl_ids, 0);
        firstReset = 1;
        qDebug() << "Servos positions have been reset!";
        for (int i = 0; i < NUM_OF_DXL; i++) {
            mainWindowPtr->servoUtility.disableTorque(mainWindowPtr->dxl_ids[i]);
            mainWindowPtr->servoUtility.setOperatingMode(mainWindowPtr->dxl_ids[i], 3);
            mainWindowPtr->servoUtility.enableTorque(mainWindowPtr->dxl_ids[i]);
            mainWindowPtr->servoUtility.setVelocity(mainWindowPtr->dxl_ids[i], DXL_VELOCITY_VALUE);
        }
    }

    ui->angle_slider->setValue(2600);
    ui->velocity_slider->setValue(100);
    this->hide();
    mainWindowPtr->show();
}
