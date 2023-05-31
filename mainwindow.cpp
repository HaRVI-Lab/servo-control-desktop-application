#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QTimer>
#include <QCoreApplication>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    skinSlipSubWindow(new SkinSlipSubWindow(this)),
    skinSlipDelaySubWindow(new SkinSlipDelaySubWindow(this)),
    squeezeSubWindow(new SqueezeSubWindow(this)),
    pushSubWindow(new PushSubWindow(this)),
    continuousSkinSlipSubWindow(new ContinuousSkinSlipSubWindow(this)),
    continuousSkinSlipDelaySubWindow(new ContinuousSkinSlipDelaySubWindow(this))
{
    ui->setupUi(this);

    // Open port
    if (!servoUtility.openPort())
    {
        QTimer::singleShot(0, QCoreApplication::instance(), &QCoreApplication::quit);
        return;
    }

    // Set port baudrate
    if (!servoUtility.setBaudRate(BAUDRATE)) {
        QTimer::singleShot(0, QCoreApplication::instance(), &QCoreApplication::quit);
        return;
    }

    // Enable servos torque
    for (int i = 0; i < NUM_OF_DXL; i++) {
        servoUtility.enableTorque(dxl_ids[i]);
    }

    // Change servos velocity
    for (int i = 0; i < NUM_OF_DXL; i++) {
        servoUtility.setVelocity(dxl_ids[i], DXL_VELOCITY_VALUE);
    }

    // Reset servos initial position
    qDebug() << "Resetting servos positions...";
    servoUtility.resetPosition(dxl_ids, 0);
    qDebug() << "Servos positions have been reset!";

}

MainWindow::~MainWindow()
{
    for (int i = 0; i < NUM_OF_DXL; i++) {
        servoUtility.disableTorque(dxl_ids[i]);
        servoUtility.setOperatingMode(dxl_ids[i], 3);
        servoUtility.enableTorque(dxl_ids[i]);
        servoUtility.setVelocity(dxl_ids[i], DXL_VELOCITY_VALUE);
    }
    servoUtility.resetPosition(dxl_ids, 0);
    for (int i = 0; i < NUM_OF_DXL; i++) {
        servoUtility.disableTorque(dxl_ids[i]);
    }
    servoUtility.closePort();

    delete ui;
    delete skinSlipSubWindow;
    delete skinSlipDelaySubWindow;
    delete pushSubWindow;
    delete squeezeSubWindow;
    delete continuousSkinSlipDelaySubWindow;
    delete continuousSkinSlipSubWindow;
}

void MainWindow::on_skinSlipBtn_clicked()
{
    this->hide();
    skinSlipSubWindow->show();
}

void MainWindow::on_skinSlipDelayBtn_clicked()
{
    this->hide();
    skinSlipDelaySubWindow->show();
}

void MainWindow::on_squeezeBtn_clicked()
{
    this->hide();
    squeezeSubWindow->show();
}

void MainWindow::on_pushBtn_clicked()
{
    this->hide();
    pushSubWindow->show();
}

void MainWindow::on_continuousSkinSlipBtn_clicked()
{
    this->hide();
    continuousSkinSlipSubWindow->show();
}

void MainWindow::on_continuousSkinSlipDelayBtn_clicked()
{
    this->hide();
    continuousSkinSlipDelaySubWindow->show();
}
