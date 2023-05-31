#include "ServoUtility.h"
#include <QCoreApplication>
#include <QTimer>

ServoUtility::ServoUtility() : portHandler(dynamixel::PortHandler::getPortHandler(DEVICENAME)), packetHandler(dynamixel::PacketHandler::getPacketHandler(PROTOCOL_VERSION)), groupSyncWrite(portHandler, packetHandler, ADDR_PRO_GOAL_POSITION, LEN_PRO_GOAL_POSITION), groupSyncRead(portHandler, packetHandler, ADDR_PRO_PRESENT_POSITION, LEN_PRO_PRESENT_POSITION) {
    dxl_comm_result = COMM_TX_FAIL;
    dxl_error = 0;
    dxl_addparam_result = false;
    dxl_getdata_result = false;
    for (int i = 0; i < 4; i++) {
        param_goal_position[i] = 0;
    }
}

ServoUtility::~ServoUtility()
{
}

bool ServoUtility::openPort() {
    if (portHandler->openPort())
    {
        qDebug() << "Succeeded to open the port!";
        return true;
    }
    else
    {
        qDebug() << "Failed to open the port!";
        return false;
    }
}

void ServoUtility::closePort() {
    portHandler->closePort();
}

bool ServoUtility::setBaudRate(int baud_rate) {
    if (portHandler->setBaudRate(baud_rate))
    {
        qDebug() << "Succeeded to change the baudrate!";
        return true;
    }
    else
    {
        qDebug() << "Failed to change the baudrate!";
        return false;
    }
}

void ServoUtility::enableTorque(int dxl_id) {
    dxl_comm_result = packetHandler->write1ByteTxRx(portHandler, dxl_id, ADDR_PRO_TORQUE_ENABLE, TORQUE_ENABLE, &dxl_error);
    if (dxl_comm_result != COMM_SUCCESS)
    {
        qDebug() << packetHandler->getTxRxResult(dxl_comm_result);
        QTimer::singleShot(0, QCoreApplication::instance(), &QCoreApplication::quit);
        return;
    }
    else if (dxl_error != 0)
    {
        qDebug() << packetHandler->getRxPacketError(dxl_error);
        QTimer::singleShot(0, QCoreApplication::instance(), &QCoreApplication::quit);
        return;
    }
    else
    {
        qDebug() << "Dynamixel#" << dxl_id << "has been successfully connected";
    }
}

void ServoUtility::disableTorque(int dxl_id) {
    dxl_comm_result = packetHandler->write1ByteTxRx(portHandler, dxl_id, ADDR_PRO_TORQUE_ENABLE, TORQUE_DISABLE, &dxl_error);
    if (dxl_comm_result != COMM_SUCCESS)
    {
        qDebug() << packetHandler->getTxRxResult(dxl_comm_result);
        QTimer::singleShot(0, QCoreApplication::instance(), &QCoreApplication::quit);
        return;
    }
    else if (dxl_error != 0)
    {
        qDebug() << packetHandler->getRxPacketError(dxl_error);
        QTimer::singleShot(0, QCoreApplication::instance(), &QCoreApplication::quit);
        return;
    }
}

void ServoUtility::setVelocity(int dxl_id, int velocity) {
    dxl_comm_result = packetHandler->write4ByteTxRx(portHandler, dxl_id, ADDR_PRO_PROFILE_VELOCITY, velocity, &dxl_error);
    if (dxl_comm_result != COMM_SUCCESS)
    {
        qDebug() << packetHandler->getTxRxResult(dxl_comm_result);
        QTimer::singleShot(0, QCoreApplication::instance(), &QCoreApplication::quit);
        return;
    }
    else if (dxl_error != 0)
    {
        qDebug() << packetHandler->getRxPacketError(dxl_error);
        QTimer::singleShot(0, QCoreApplication::instance(), &QCoreApplication::quit);
        return;
    }
}
void ServoUtility::setOperatingMode(int dxl_id, uint8_t operating_mode) {
    dxl_comm_result = packetHandler->write1ByteTxRx(portHandler, dxl_id, ADDR_OPERATING_MODE, operating_mode, &dxl_error);
    if (dxl_comm_result != COMM_SUCCESS)
    {
        qDebug() << packetHandler->getTxRxResult(dxl_comm_result);
        QTimer::singleShot(0, QCoreApplication::instance(), &QCoreApplication::quit);
        return;
    }
    else if (dxl_error != 0)
    {
        qDebug() << packetHandler->getRxPacketError(dxl_error);
        QTimer::singleShot(0, QCoreApplication::instance(), &QCoreApplication::quit);
        return;
    }
}

void ServoUtility::resetVelocity(int dxl_id) {
    dxl_comm_result = packetHandler->write4ByteTxRx(portHandler, dxl_id, ADDR_PRO_PROFILE_VELOCITY, DXL_VELOCITY_RESET_VALUE, &dxl_error);
    if (dxl_comm_result != COMM_SUCCESS)
    {
        qDebug() << packetHandler->getTxRxResult(dxl_comm_result);
        QTimer::singleShot(0, QCoreApplication::instance(), &QCoreApplication::quit);
        return;
    }
    else if (dxl_error != 0)
    {
        qDebug() << packetHandler->getRxPacketError(dxl_error);
        QTimer::singleShot(0, QCoreApplication::instance(), &QCoreApplication::quit);
        return;
    }
}

void ServoUtility::syncWritePosition(int dxl_num, int dxl_ids[], int dxls_goal_position[]) {
    // Add servos goal position value to the Syncwrite storage
    for (int i = 0; i < dxl_num; i++) {
        param_goal_position[0] = DXL_LOBYTE(DXL_LOWORD(dxls_goal_position[i]));
        param_goal_position[1] = DXL_HIBYTE(DXL_LOWORD(dxls_goal_position[i]));
        param_goal_position[2] = DXL_LOBYTE(DXL_HIWORD(dxls_goal_position[i]));
        param_goal_position[3] = DXL_HIBYTE(DXL_HIWORD(dxls_goal_position[i]));

        dxl_addparam_result = groupSyncWrite.addParam(dxl_ids[i], param_goal_position);
        if (dxl_addparam_result != true)
        {
            qDebug() << QString("[ID:%1] groupSyncWrite addparam failed").arg(dxl_ids[i]);
            QTimer::singleShot(0, QCoreApplication::instance(), &QCoreApplication::quit);
            return;
        }
    }
    // Syncwrite goal position
    mtx.lock();
    dxl_comm_result = groupSyncWrite.txPacket();
    mtx.unlock();
    if (dxl_comm_result != COMM_SUCCESS) {
        qDebug() << packetHandler->getTxRxResult(dxl_comm_result);
        QTimer::singleShot(0, QCoreApplication::instance(), &QCoreApplication::quit);
        return;
    }

    // Clear syncwrite parameter storage
    groupSyncWrite.clearParam();
}

void ServoUtility::syncReadPosition(int dxl_num, int dxl_ids[], int32_t dxls_present_position[], int dxls_goal_position[]) {
    bool anyServoMoving;
    // Add parameter storage for servos present position value
    for (int i = 0; i < dxl_num; i++) {
        dxl_addparam_result = groupSyncRead.addParam(dxl_ids[i]);
        if (dxl_addparam_result != true)
        {
            qDebug() << QString("[ID:%1] groupSyncRead addparam failed").arg(dxl_ids[i]);
            QTimer::singleShot(0, QCoreApplication::instance(), &QCoreApplication::quit);
            return;
        }
    }
    do
    {
        // Syncread present position
        mtx.lock();
        dxl_comm_result = groupSyncRead.txRxPacket();
        mtx.unlock();
        if (dxl_comm_result != COMM_SUCCESS)
        {
            qDebug() << packetHandler->getTxRxResult(dxl_comm_result);
            QTimer::singleShot(0, QCoreApplication::instance(), &QCoreApplication::quit);
            return;
        }
        else {
            for (int i = 0; i < dxl_num; i++) {
                if (groupSyncRead.getError(dxl_ids[i], &dxl_error)) {
                    qDebug() << QString("[ID:%1] %2").arg(dxl_ids[i]).arg(packetHandler->getRxPacketError(dxl_error));
                    QTimer::singleShot(0, QCoreApplication::instance(), &QCoreApplication::quit);
                    return;
                }
            }
        }
        // Check if groupsyncread data of servos is available
        for (int i = 0; i < dxl_num; i++) {
            dxl_getdata_result = groupSyncRead.isAvailable(dxl_ids[i], ADDR_PRO_PRESENT_POSITION, LEN_PRO_PRESENT_POSITION);
            if (dxl_getdata_result != true)
            {
                qDebug() << QString("[ID:%1] groupSyncRead getdata failed").arg(dxl_ids[i]);;
                QTimer::singleShot(0, QCoreApplication::instance(), &QCoreApplication::quit);
                return;
            }
        }
        // Get servos present position value
        for (int i = 0; i < dxl_num; i++) {
            dxls_present_position[i] = groupSyncRead.getData(dxl_ids[i], ADDR_PRO_PRESENT_POSITION, LEN_PRO_PRESENT_POSITION);
        }

        // Print servos present position value
        QString output;
        for (int i = 0; i < dxl_num; i++) {
            if (i == dxl_num - 1) {
                output.append(QString("[ID:%1] GoalPos:%2  PresPos:%3").arg(dxl_ids[i]).arg(dxls_goal_position[i]).arg(dxls_present_position[i]));
            } else {
                output.append(QString("[ID:%1] GoalPos:%2  PresPos:%3  ").arg(dxl_ids[i]).arg(dxls_goal_position[i]).arg(dxls_present_position[i]));
            }
        }
        qDebug() << output;
        // check if any servo is still moving
        anyServoMoving = false;
        for (int i = 0; i < dxl_num; i++) {
            if (abs(dxls_goal_position[i] - dxls_present_position[i]) > DXL_MOVING_STATUS_THRESHOLD) {
                anyServoMoving = true;
                break;
            }
        }
        Sleep(150);
    } while (anyServoMoving);
    groupSyncRead.clearParam();
}

void ServoUtility::writePositionWithDelay(int dxl_id, int dxl_goal_position, int delay) {
    mtx.lock();
    dxl_comm_result = packetHandler->write4ByteTxRx(portHandler, dxl_id, ADDR_PRO_GOAL_POSITION, dxl_goal_position, &dxl_error);
    mtx.unlock();
    if (dxl_comm_result != COMM_SUCCESS) {
        printf("%s\n", packetHandler->getTxRxResult(dxl_comm_result));
    }
    else if (dxl_error != 0) {
        printf("%s\n", packetHandler->getRxPacketError(dxl_error));
    }

    if (delay > 0) {
        Sleep(delay);
    }
}

void ServoUtility::syncWritePositionWithDelay(int dxl_num, int dxl_ids[], int dxl_goal_position, int delay) {
    for (int i = 0; i < dxl_num; i++) {
        if (i != dxl_num - 1) {
            writePositionWithDelay(dxl_ids[i], dxl_goal_position, delay);
        }
        else {
            writePositionWithDelay(dxl_ids[i], dxl_goal_position, 0);
        }
    }
}

void ServoUtility::resetPosition(int dxl_ids[], int dxl_goal_position) {
    int32_t dxls_present_position[NUM_OF_DXL];
    int dxls_goal_position[NUM_OF_DXL];
    for (int i = 0; i < NUM_OF_DXL; i++) {
        dxls_goal_position[i] = dxl_goal_position;
        dxls_present_position[i] = 0;
    }
    std::thread thread_sync_read(&ServoUtility::syncReadPosition, this, NUM_OF_DXL, dxl_ids, dxls_present_position, dxls_goal_position);
    std::thread thread_sync_write(&ServoUtility::syncWritePosition, this, NUM_OF_DXL, dxl_ids, dxls_goal_position);
    thread_sync_write.join();
    thread_sync_read.join();
}

void ServoUtility::resetPositionCustom(int dxl_ids[], int flag, int firstReset) {
    int32_t dxls_present_position[NUM_OF_DXL];
    int dxls_goal_position[NUM_OF_DXL];
    if (flag == 0) {
        for (int i = 0; i < NUM_OF_DXL; i++) {
            if (firstReset == 1) {
                if (i < 3) {
                    dxls_goal_position[i] = 2048;
                } else {
                    dxls_goal_position[i] = -2048;
                }
            } else {
                if (i < 3) {
                    dxls_goal_position[i] = 6144;
                } else {
                    dxls_goal_position[i] = -2048;
                }
            }
            dxls_present_position[i] = 0;
        }
    } else {
        for (int i = 0; i < NUM_OF_DXL; i++) {
            if (firstReset == 1) {
                if (i < 3) {
                    dxls_goal_position[i] = -2048;
                } else {
                    dxls_goal_position[i] = 2048;
                }
            } else {
                if (i < 3) {
                    dxls_goal_position[i] = -2048;
                } else {
                    dxls_goal_position[i] = 6144;
                }
            }
            dxls_present_position[i] = 0;
        }
    }
    std::thread thread_sync_read(&ServoUtility::syncReadPosition, this, NUM_OF_DXL, dxl_ids, dxls_present_position, dxls_goal_position);
    std::thread thread_sync_write(&ServoUtility::syncWritePosition, this, NUM_OF_DXL, dxl_ids, dxls_goal_position);
    thread_sync_write.join();
    thread_sync_read.join();
}

void ServoUtility::quitCustom(int dxl_ids[], int flag) {
    int32_t dxls_present_position[NUM_OF_DXL];
    int dxls_goal_position[NUM_OF_DXL];
    if (flag == 0) {
        for (int i = 0; i < NUM_OF_DXL; i++) {
            if (i < 3) {
                dxls_goal_position[i] = 4096;
            } else {
                dxls_goal_position[i] = 0;
            }

            dxls_present_position[i] = 0;
        }
    } else {
        for (int i = 0; i < NUM_OF_DXL; i++) {
            if (i < 3) {
                dxls_goal_position[i] = 0;
            } else {
                dxls_goal_position[i] = 4096;
            }
            dxls_present_position[i] = 0;
        }
    }
    std::thread thread_sync_read(&ServoUtility::syncReadPosition, this, NUM_OF_DXL, dxl_ids, dxls_present_position, dxls_goal_position);
    std::thread thread_sync_write(&ServoUtility::syncWritePosition, this, NUM_OF_DXL, dxl_ids, dxls_goal_position);
    thread_sync_write.join();
    thread_sync_read.join();
}



