// GNU General Public License Agreement
// Copyright (C) 2015-2016 Daniele Berto daniele.fratello@gmail.com
//
// CollSoft is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free Software Foundation.
// You must retain a copy of this licence in all copies.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
// PARTICULAR PURPOSE. See the GNU General Public License for more details.
// ---------------------------------------------------------------------------------

/*
 * This class manages the main window.
 * Author: Daniele Berto
 * daniele.fratello@gmail.com
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "logfileexpertgui.h"
#include <QMainWindow>
#include <QTextEdit>
#include <QTcpSocket>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(char* user = "", QWidget *parent = 0);
    ~MainWindow();

private slots:
    void readTcpData();
    void Errore(QAbstractSocket::SocketError sock_error);

    void readTcpData_existence();
    void Errore_existence(QAbstractSocket::SocketError sock_error);

    void on_CommandStringButton_clicked();

    void on_ConnectToServerButton_clicked();

    void on_ConnectProgrammer_clicked();

    void on_CheckDrvAssoc_clicked();

    void on_CheckExistence_clicked();

    void on_ConnectExistence_clicked();

    void on_KillProcess_CollSoft_clicked();

    void on_NewProcess_CollSoft_clicked();

    void on_DisconnectExistence_clicked();

    void on_DisconnectToServerButton_clicked();

    void closing();

    void on_ReadSerialDrvLog_clicked();

    void on_ReadFileParLog_clicked();

    void on_CheckParAssoc_clicked();

    void on_SetParameterDriver_1_clicked();

    void on_SetParameterDriver_2_clicked();

    void on_SetParameterDriver_3_clicked();

    void on_SetParameterDriver_4_clicked();

    void on_SetParameterDriver_5_clicked();

    void on_SetParameterDriver_6_clicked();

    void on_SetParameterDriver_7_clicked();

    void on_SetParameterDriver_8_clicked();

    void on_SetParameterDriver_9_clicked();

    void on_SetParameterDriver_10_clicked();

    void on_SetParameterDriver_11_clicked();

    void on_SetParameterDriver_12_clicked();

    void on_SetParameterDriver_13_clicked();

    void on_SetParameterDriver_14_clicked();

    void GetParameterDriver_clicked(int drv_num);

    void Homing_clicked(int drv_num);

    void GetParameterMov_clicked(int drv_num);

    void MoveTo_clicked(int drv_num);

    void CheckPositionButton_clicked(int drv_num);

    void on_GetAllParameter_clicked();

    void on_checkBox_all_clicked();

    void on_Homing_all_clicked();

    void on_MoveTo_all_clicked();

    void on_SetParameterDriver_all_clicked();

    void on_CheckEncoderAssoc_clicked();

    void on_ReadEncoderLog_clicked();

    void on_ReadGeneralLog_clicked();

    void on_CheckInternalStatus_clicked();

    void on_actionQuit_triggered();

    void on_actionAbout_triggered();

    void on_LoadEncoderFromFileButton_clicked();

    void on_ReadActualEncoderValue_clicked();



    void on_DeviceListButton_clicked();

private:
    Ui::MainWindow *ui;

    //Socket to communicate with CollSoft server
    QTcpSocket* _pSocket;

    //Socket to communicate with CheckExistence server
    QTcpSocket* _pSocket_existence;

    //Singleton to write relevant running information to the log file.
    //The log file path is hard coded in the singleton constructor
    LogFileExpertGUI* main_window_log_file;

    //This function check the connection status of the ExpertGUI with the Collsoft server and the CheckExistence server.
    //It prints "OK" if the connection is okay.
    void CheckGeneralStatus();

    //This function print the message contained in Message QString at the bottom of the QTextEdit pointed by MexArea
    void AppendMessageToQtextEdit(QTextEdit* MexArea, QString Message);

};

#endif // MAINWINDOW_H
