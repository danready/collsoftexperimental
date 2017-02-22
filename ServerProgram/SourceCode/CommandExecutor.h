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

//This file contains the declarations of the functions defined in CommandExecutor.c .
//The functions in this files call the funciotions contained in DriverFunction.c and 
//are called by Main.c  

#ifndef COMMANDEXECUTOR_H
#define COMMANDEXECUTOR_H

#include "Input.h"
#include "serialib.h"
#include "BasicModbusLibrary.h"
#include "ApplicationSetup.h"
#include "OutputModule.h"
#include "LogFile.h"
#include "TcpUser.h"
#include "CommunicationObject.h"
#include "DefineGeneral.h"
#include "Utils.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <iostream>
#include <thread>
#include <regex.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <iostream>
#include <vector>


//This struct is very useful to track some information about server activity.
//If GeneralStatus.assoc_file_status == 1 means the user has already executed the procedure to check
//the association between the serial numbers contained in the SerialDrvLog.txt file and the real situation.
//If GeneralStatus.par_file_status = 1 means the user has already executed the procedure to check
//the association between the parameters contained in the FileParLog.txt file and the real situation.
//If GeneralStatus.encoder_file_status = 1 means the user has already executed the procedure to check
//the association between the encoder values contained in the EncoderLog.txt file and the real situation.	

typedef struct{
bool assoc_file_status;
bool par_file_status;
bool encoder_file_status;
} file_check_status;

extern file_check_status GeneralStatus;

//These arrays record the actual serial number of the drivers.
extern unsigned int SerialNumberArray[MAXIMUM_DRIVER];
extern unsigned int SerialNumberArrayParagorn[MAXIMUM_DRIVER];
extern unsigned int SerialNumberArrayTmp[MAXIMUM_DRIVER];

//These arrays record the actual parameters of the drivers. See utils.h for the declaration of ParameterStruct.
extern ParameterStruct ParameterArray[MAXIMUM_DRIVER];
extern ParameterStruct ParameterArrayParagorn[MAXIMUM_DRIVER];
extern ParameterStruct ParameterArrayTmp[MAXIMUM_DRIVER];

//This array records the values used to accomplished the check_position procedure for each driver. 
//See utils.h for the declaration of EncoderStruct.
extern EncoderStruct EncoderArrayValue[MAXIMUM_DRIVER];

//This variable records if the user has just loaded some consistent values from the log file used to
//to accomplished the check_position procedure.
extern bool loading_encoder_from_file_okay; //Be careful: global variable!

//Help procedure. It is launched by the help command and prints the list of available commands. 
void HelpCommand ();

//All these procedure are lauched by the correspondent command in the Main.c file.
//The correspondent command will be indicated above every procedure. See CollSoftServerCommand.txt
//for the description of every command.

//check_drv_assoc
void CheckDrvAssoc (CommunicationObject& mioTCP, Input* mioinput, modbus_t* ctx);
//check_par_assoc
void CheckParAssoc (CommunicationObject& mioTCP, Input* mioinput, modbus_t* ctx);
//check_encode_assoc
void CheckEncodeAssoc (CommunicationObject& mioTCP, Input* mioinput, modbus_t* ctx);
//get_par drvnum
void GetPar (modbus_t* ctx, int get_par_drv);
//set_par drvnum max_vel acceleration deceleration PhaseCurrent AnalogOutput0
void SetPar (modbus_t* ctx, int set_par_drv, char* buffer);
//setmult_par max_vel acceleration deceleration PhaseCurrent AnalogOutput0 drvnum1 drvnum2 drvnum3 drvnum....
void SetParMult (modbus_t* ctx, int set_par_drv, char* buffer);
//homing drvnum
void Homing(modbus_t* ctx, int homing_drv);
//get_mov_par drvnum
void GetMovePar(modbus_t* ctx, int mov_par_drv);
//move_to drvnum targetposition
void MoveTo(modbus_t* ctx, int moveto_drv_num, int moveto_value);
//encode drvnum
void Encode(modbus_t* ctx, int encode_drv_num, EncoderStruct& drv_parameters);
//set_status_state drvnum status_state
void SetStatusStateSoftwareCmd (modbus_t* ctx, int set_status_state_drv, uint16_t set_status_state_value);
//get_status_state drvnum
void GetStatusStateSoftwareCmd (modbus_t* ctx, int get_status_state_drv);
//set_request_state drvnum status_state
void SetRequestStateSoftwareCmd (modbus_t* ctx, int set_request_state_drv, uint16_t request_state_value);
//get_request_state drvnum
void GetRequestStateSoftwareCmd (modbus_t* ctx, int get_request_state_drv);
//save_eprom drvnum
void SaveEprom (modbus_t* ctx, int save_eprom_drv);
//check_fault drvnum
void CheckFault (modbus_t* ctx, int check_fault_drv);
//set_home_done drvnum home_done
void SetHomeDoneSoftwareCmd (modbus_t* ctx, int set_home_done_drv, uint16_t set_home_done_value);
//get_home_done drvnum
void GetHomeDoneSoftwareCmd (modbus_t* ctx, int get_home_done_drv);
//set_encoder_max drvnum encoder_max
void SetEncoderMaxSoftwareCmd (modbus_t* ctx, int set_encoder_max_drv, uint16_t set_encoder_max_value);
//get_encoder_max drvnum
void GetEncoderMaxSoftwareCmd (modbus_t* ctx, int get_encoder_max_drv);
//set_encoder_min drvnum encoder_min
void SetEncoderMinSoftwareCmd (modbus_t* ctx, int set_encoder_min_drv, uint16_t set_encoder_min_value);
//get_encoder_min drvnum
void GetEncoderMinSoftwareCmd (modbus_t* ctx, int get_encoder_min_drv);
//set_delta_analog_pos drvnum delta_analog_pos
void SetDeltaAnalogPosSoftwareCmd (modbus_t* ctx, int set_delta_analog_pos_drv, uint16_t set_delta_analog_pos_value);
//set_delta_analog_neg drvnum delta_analog_neg
void GetDeltaAnalogPosSoftwareCmd (modbus_t* ctx, int get_delta_analog_pos_drv);
//get_delta_analog_neg drvnum
void SetDeltaAnalogNegSoftwareCmd (modbus_t* ctx, int set_delta_analog_neg_drv, int16_t set_delta_analog_neg_value);
//get_delta_analog_pos drvnum
void GetDeltaAnalogNegSoftwareCmd (modbus_t* ctx, int get_delta_analog_neg_drv);
//set_phase_current_user drvnum phase_current_user
void SetPhaseCurrentUserSoftwareCmd (modbus_t* ctx, int set_phase_current_user_drv, uint16_t set_phase_current_user_value);
//get_phase_current_user drvnum
void GetPhaseCurrentUserSoftwareCmd (modbus_t* ctx, int get_phase_current_user_drv);
//set_delay_check_rot drvnum delay_check_rot
void SetDelayCheckRotSoftwareCmd (modbus_t* ctx, int set_delay_check_rot_drv, uint16_t set_delay_check_rot_value);
//get_delay_check_rot drvnum
void GetDelayCheckRotSoftwareCmd (modbus_t* ctx, int get_delay_check_rot_drv);
//set_max_target_position drvnum max_target_position
void SetMaxTargetPositionSoftwareCmd (modbus_t* ctx, int set_max_target_position_drv, int set_max_target_position_value);
//get_max_target_position drvnum
void GetMaxTargetPositionSoftwareCmd (modbus_t* ctx, int get_max_target_position_drv);
//
//check_position drvnum
/////////////////////////////////////
//Return values:
//0 all okay
//-1 real position mismatch with estimated position
//-2 problem communicating with the driver
//everything > 0 the driver is blocked in an invalid state

//The output of this function is sent to all the clients connected to the applications
//This procedure is launched by get_all_parameter command. 
int CheckPositionEncoderToAll (modbus_t* ctx, int position_encoder_drv_num);

//The output of this function is sent only to the client that requires it.
int CheckPositionEncoderSingle (modbus_t* ctx, int position_encoder_drv_num);

/////////////////////////////////////

//Send failed response for "get_par drvnum" command. 
void SendFailedGetPar(int drv_num);
//Send failed response for "get_mov_par drvnum" command.
void SendFailedGetMovPar(int drv_num);
//Send failed response for "check_position drvnum" command.
void SendFailedGetStatusPos(int drv_num);

//Fills EncoderArrayValue array whith the values found in EncoderLog.txt file.
void LoadEncoderFromFile();

//Prints the values of the EncoderArrayValue array. 
void ReadActualEncoderValue();

void CheckDriverStatus(modbus_t* ctx, int check_driver_status_drv);

//This function return true if the status_state variable of the driver is: 0,6,4,5.
//Return false otherwise.
bool VerifyStatusState(uint16_t status_state);

#endif
