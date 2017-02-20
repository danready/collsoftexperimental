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
#include "DriverFunction.h"

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

using namespace std;

//All these function are declared in DriverFunction.h file.
//The following functions interact with the drivers using the functions defined in
//BasicModbusLibrary.c, a simplified version of libmodbus v.3.0.6 by Stephane Raimbault.
//
//These functions does not set the slave number, so you have to set it before
//calling them.
//
//Usually, the following functions call only modbus_read_register or modbus_write_register and modbus_strerror.

//N.B. The return value and the argument of the functions depend to the dimension of the driver
//register being written.

//N.B. Register address' are defined in DefineGeneral.h .

//N.B. In each function is very important usleep(SLEEPMODBUS) in order to give the programmer the time to send the data.
	
//N.B. Setting functions return the error status.

//Utility function used to determine the number of the argument of a command.
int FindNumArg (char *yytext)
{
	int i = 0;
	char *punt;
    int lunghezza = 0;	
	
	while (i<strlen(yytext)) {
	while (yytext[i] != ' ' && yytext[i] != '\t' && i<strlen(yytext))
		i++;
	while ((yytext[i] == ' ' || yytext[i] == '\t') && i<strlen(yytext))
		i++;
	lunghezza++;
	}
	
	return lunghezza - 1;

}

//Function used to connect the server with the programmer. STATE_CONNECT is a global_variable
//that records if the operation is successfully done.
//path is the absolute path of the device (ex. "/dev/ttyUSB0").
modbus_t* Connect(modbus_t *ctx, bool* STATE_CONNECT, char* path)
{
	//Singleton used to manage the output of the server.
	OutputModule *output_module;
	output_module = OutputModule::Instance ();
	
	output_module->Output("Connect: MODBUS CONNECTION start\n");
	
	//Creating libmodbus context.
	ctx = modbus_new_rtu(path, 9600, 'N', 8, 1);

	//If an error occurred
	if (ctx == NULL) {
		output_module->Output("Connect: MODBUS CONTEXT: ERROR - Unable to create the libmodbus context\n");
	} else {
		//If success
		output_module->Output("Connect: MODBUS CONTEXT: SUCCESS!\n");
	}

	//Try to connect to ctx modbus_t resource.
	if (modbus_connect(ctx) == -1) {
		
		//Set STATE_CONNECT global variable to failed status.
		*STATE_CONNECT = 0;
		
		//recording the error.
		string tmp_string(modbus_strerror(errno));
		
		//printing the error
		output_module->Output("Connect: MODBUS CONNECTION ERROR: " + tmp_string + "\n");
		
		//clearing ctx modbus_t resource
		modbus_free(ctx);
	} else {
		
		int errore = 0;
		
		//Set STATE_CONNECT global variable to success status.		
		*STATE_CONNECT = 1;
		
		output_module->Output("Connect: MODBUS CONNECTION SUCCESS\n");
		
		//setting the default slave.
		modbus_set_slave(ctx, DEFAULT_SLAVE);
		
		//recording the possible error.		
		string tmp_string(modbus_strerror(errno));
				
		output_module->Output("Connect: MODBUS SET_SLAVE ERROR: " + tmp_string + "\n");
		
		output_module->Output("Connect: MODBUS CONNECTION done\n");
	}

	//return ctx modbus_t resource.
	return ctx;
    
}

//Function used to read the serial number of the driver.
//rc_arg will contain the status of the operation. 
unsigned int ReadSerialNumber(modbus_t *ctx, int* rc_arg)
{	
	
	unsigned int SerialNumber = 0;

	//Singleton to manage the output of the program.
	OutputModule *output_module;
	output_module = OutputModule::Instance ();	

	//Recording the function name.
	string tmp_string(__func__);
	
	string tmp_errno;	
	
	output_module->Output("Exp: \n");

	output_module->Output(tmp_string + "Exp: Reading SerialNumber...\n");

	//This buffer will be used to store the data received by the driver.
	uint16_t data[STANDARDBUFFERLIMIT];
	
	//collect success/error status.
	int rc;

	//clearing data.
	bzero(data, STANDARDBUFFERLIMIT);
	
	//reading Serial Number register and put the data read in "data" array..
	rc = modbus_read_registers(ctx, SERIAL_NUMBER, 2, &data[0]);
	
	//collecting the status.
	*rc_arg = rc;
	
	//sleeping a while in order to give the programmer the time to send the data.
	usleep(SLEEPMODBUS);
	
	//If an error occurred.
	if (rc == -1) 
	{
		//recording the error.
		tmp_errno = modbus_strerror(errno);
		output_module->Output("Exp: READING READBACK REMOTE STATE FAILED " + tmp_string + "\t" + tmp_errno + "\n");
		
		return 0;
	}
	else
	{
		//If success, storing serial number in SerialNumber variable.
		SerialNumber = SerialNumber | data[1];
		SerialNumber = SerialNumber | (data[0] << 16);
		
		output_module->Output("Exp: " + tmp_string + ": Reading Serial Number DONE rc: " + to_string(rc) + " \t Serial Number content is: " + to_string(SerialNumber) + "\n");
		
		return SerialNumber;
	}
	
}

//Function used to set the max_vel parameter.
//header is the prefix of every output printed by the function.
//The return value is the error status.
int SetMaxVel (modbus_t *ctx, uint16_t max_vel, string header)
{
	//Singleton to manage the output of the program.
	OutputModule *output_module;
	output_module = OutputModule::Instance ();	
	
	//Recording the function name.	
	string tmp_string(__func__);
	
	//Variable used to store the possible error.
	string tmp_errno;
	
	//This buffer will be used to store the data being sent to the driver.	
	uint16_t data[STANDARDBUFFERLIMIT];
	
	//collect success/error status.	
	int rc;

	//clearing data.
	bzero(data, STANDARDBUFFERLIMIT);
	
	//Storing max_vel that will be sent to the driver.
	data[0] = max_vel;
	
	//Sending data to the driver.
	rc = modbus_write_registers(ctx, MAX_VEL, 1, &data[0]);
	
	//sleeping a while in order to give the programmer the time to send the data.	
	usleep(SLEEPMODBUS);
	
	//If an error occurred.	
	if (rc == -1) 
	{
		//Recording the error status.
		tmp_errno = modbus_strerror(errno);
		
		//Printing the error status.
		output_module->Output(header + "READING READBACK REMOTE STATE FAILED " + tmp_string + "\t" + tmp_errno + "\n");
	}
	
	//Returning the error status.
	return rc;
	
}

//Function used to set the vel_home parameter.
//header is the prefix of every output printed by the function.
//The return value is the error status.
int SetVelHome (modbus_t *ctx, int16_t vel_home, string header)
{
	//Singleton to manage the output of the program.	
	OutputModule *output_module;
	output_module = OutputModule::Instance ();
	
	//Recording the function name.		
	string tmp_string(__func__);
	
	//Variable used to store the possible error.	
	string tmp_errno;
	
	//This buffer will be used to store the data being sent to the driver.			
	uint16_t data[STANDARDBUFFERLIMIT];
	
	//collect success/error status.		
	int rc;

	//clearing data.
	bzero(data, STANDARDBUFFERLIMIT);
	
	//Storing vel_home that will be sent to the driver.	
	data[0] = vel_home;
	
	//Sending data to the driver.	
	rc = modbus_write_registers(ctx, VEL_HOME, 1, &data[0]);
	
	//sleeping a while in order to give the programmer the time to send the data.		
	usleep(SLEEPMODBUS);
	
	//If an error occurred.		
	if (rc == -1) 
	{
		//Recording the error status.		
		tmp_errno = modbus_strerror(errno);
		
		//Printing the error status.		
		output_module->Output(header + "READING READBACK REMOTE STATE FAILED " + tmp_string + "\t" + tmp_errno + "\n");
	}
	
	//Returning the error status.	
	return rc;
	
}

//Function used to set the acceleration parameter.
//header is the prefix of every output printed by the function.
//The return value is the error status.
int SetAcceleration (modbus_t *ctx, uint16_t acceleration, string header)
{
	//Singleton to manage the output of the program.
	OutputModule *output_module;
	output_module = OutputModule::Instance ();
	
	//Recording the function name.			
	string tmp_string(__func__);
	
	//Variable used to store the possible error.		
	string tmp_errno;	
		
	//This buffer will be used to store the data being sent to the driver.		
	uint16_t data[STANDARDBUFFERLIMIT];
	
	//collect success/error status.		
	int rc;

	//clearing data.
	bzero(data, STANDARDBUFFERLIMIT);
	
	//Storing acceleration that will be sent to the driver.		
	data[0] = acceleration;
	
	//Sending data to the driver.		
	rc = modbus_write_registers(ctx, ACCELERATION, 1, &data[0]);
	
	//sleeping a while in order to give the programmer the time to send the data.	
	usleep(SLEEPMODBUS);
	
	//If an error occurred.		
	if (rc == -1) 
	{
		//Recording the error status.		
		tmp_errno = modbus_strerror(errno);
		
		//Printing the error status.		
		output_module->Output(header + "READING READBACK REMOTE STATE FAILED " + tmp_string + "\t" + tmp_errno + "\n");
	}

	//Returning the error status.		
	return rc;
	
}

//Function used to set the acceleration parameter.
//header is the prefix of every output printed by the function.
//The return value is the error status.
int SetDeceleration (modbus_t *ctx, uint16_t deceleration, string header)
{
	//Singleton to manage the output of the program.	
	OutputModule *output_module;
	output_module = OutputModule::Instance ();
	
	//Recording the function name.			
	string tmp_string(__func__);
	
	//Variable used to store the possible error.		
	string tmp_errno;

	//This buffer will be used to store the data being sent to the driver.			
	uint16_t data[STANDARDBUFFERLIMIT];
	
	//collect success/error status.		
	int rc;

	//clearing data.
	bzero(data, STANDARDBUFFERLIMIT);
	
	//Storing deceleration that will be sent to the driver.		
	data[0] = deceleration;
	
	//Sending data to the driver.			
	rc = modbus_write_registers(ctx, DECELERATION, 1, &data[0]);
	
	//sleeping a while in order to give the programmer the time to send the data.		
	usleep(SLEEPMODBUS);
	
	//If an error occurred.			
	if (rc == -1) 
	{
		//Recording the error status.				
		tmp_errno = modbus_strerror(errno);
		
		//Printing the error status.		
		output_module->Output(header + "READING READBACK REMOTE STATE FAILED " + tmp_string + "\t" + tmp_errno + "\n");
	}
	
	//Returning the error status.	
	return rc;
	
}

//Function used to set the phase_current parameter.
//header is the prefix of every output printed by the function.
//The return value is the error status.
int SetPhaseCurrent (modbus_t *ctx, uint16_t phase_current, string header)
{
	//Singleton to manage the output of the program.	
	OutputModule *output_module;
	output_module = OutputModule::Instance ();
	
	//Recording the function name.		
	string tmp_string(__func__);
	
	//Variable used to store the possible error.	
	string tmp_errno;

	//This buffer will be used to store the data being sent to the driver.			
	uint16_t data[STANDARDBUFFERLIMIT];
	
	//collect success/error status.		
	int rc;

	//clearing data.
	bzero(data, STANDARDBUFFERLIMIT);
	
	//Storing phase_current that will be sent to the driver.	
	data[0] = phase_current;
	
	//Sending data to the driver.	
	rc = modbus_write_registers(ctx, PHASE_CURRENT, 1, &data[0]);
	
	//sleeping a while in order to give the programmer the time to send the data.	
	usleep(SLEEPMODBUS);
	
	//If an error occurred.		
	if (rc == -1) 
	{
		//Recording the error status.			
		tmp_errno = modbus_strerror(errno);
		
		//Printing the error status.			
		output_module->Output(header + "READING READBACK REMOTE STATE FAILED " + tmp_string + "\t" + tmp_errno + "\n");
	}
	
	//Returning the error status.	
	return rc;
	
}

//Function used to set the analog_output0 parameter.
//header is the prefix of every output printed by the function.
//The return value is the error status.
int SetAnalogOutput0 (modbus_t *ctx, uint16_t analog_output0, string header)
{
	//Singleton to manage the output of the program.		
	OutputModule *output_module;
	output_module = OutputModule::Instance ();
	
	//Recording the function name.			
	string tmp_string(__func__);
	
	//Variable used to store the possible error.		
	string tmp_errno;

	//This buffer will be used to store the data being sent to the driver.		
	uint16_t data[STANDARDBUFFERLIMIT];
	
	//collect success/error status.		
	int rc;

	//clearing data.
	bzero(data, STANDARDBUFFERLIMIT);
	
	//Storing analog_output0 that will be sent to the driver.		
	data[0] = analog_output0;
	
	//Sending data to the driver.	
	rc = modbus_write_registers(ctx, ANALOG_OUTPUT_0, 1, &data[0]);
	
	//sleeping a while in order to give the programmer the time to send the data.	
	usleep(SLEEPMODBUS);
	
	//If an error occurred.	
	if (rc == -1) 
	{
		//Recording the error status.		
		tmp_errno = modbus_strerror(errno);
		
		//Printing the error status.		
		output_module->Output(header + "READING READBACK REMOTE STATE FAILED " + tmp_string + "\t" + tmp_errno + "\n");
	}
	
	//Returning the error status.	
	return rc;
	
}

//Function used to set the status_state register.
//header is the prefix of every output printed by the function.
//The return value is the error status.
int SetStatusState (modbus_t *ctx, uint16_t status_state, string header)
{
	//Singleton to manage the output of the program.		
	OutputModule *output_module;
	output_module = OutputModule::Instance ();
	
	//Recording the function name.			
	string tmp_string(__func__);
	
	//Variable used to store the possible error.		
	string tmp_errno;
		
	//This buffer will be used to store the data being sent to the driver.		
	uint16_t data[STANDARDBUFFERLIMIT];
	
	//collect success/error status.	
	int rc;

	//clearing data.
	bzero(data, STANDARDBUFFERLIMIT);
	
	//Storing status_state that will be sent to the driver.		
	data[0] = status_state;
	
	//Sending data to the driver.	
	rc = modbus_write_registers(ctx, STATUS_STATE, 1, &data[0]);
	
	//sleeping a while in order to give the programmer the time to send the data.	
	usleep(SLEEPMODBUS);
	
	//If an error occurred.		
	if (rc == -1) 
	{
		//Recording the error status.		
		tmp_errno = modbus_strerror(errno);
		
		//Printing the error status.		
		output_module->Output(header + "READING READBACK REMOTE STATE FAILED " + tmp_string + "\t" + tmp_errno + "\n");
	}
	
	//Returning the error status.	
	return rc;
	
}

//Function used to set the request_state register.
//header is the prefix of every output printed by the function.
//The return value is the error status.
int SetRequestState (modbus_t *ctx, uint16_t request_state, string header)
{
	//Singleton to manage the output of the program.	
	OutputModule *output_module;
	output_module = OutputModule::Instance ();
	
	//Recording the function name.	
	string tmp_string(__func__);
	
	//Variable used to store the possible error.	
	string tmp_errno;
		
	//This buffer will be used to store the data being sent to the driver.				
	uint16_t data[STANDARDBUFFERLIMIT];
	
	//collect success/error status.		
	int rc;

	//clearing data.
	bzero(data, STANDARDBUFFERLIMIT);
	
	//Storing request_state that will be sent to the driver.
	data[0] = request_state;
	
	//Sending data to the driver.	
	rc = modbus_write_registers(ctx, REQUEST_STATE, 1, &data[0]);
	
	//sleeping a while in order to give the programmer the time to send the data.	
	usleep(SLEEPMODBUS);
	
	//If an error occurred.	
	if (rc == -1) 
	{
		//Recording the error status.		
		tmp_errno = modbus_strerror(errno);
		
		//Printing the error status.		
		output_module->Output(header + "READING READBACK REMOTE STATE FAILED " + tmp_string + "\t" + tmp_errno + "\n");
	}
	
	//Returning the error status.	
	return rc;
	
}

//Function used to set the target_position register.
//header is the prefix of every output printed by the function.
//The return value is the error status.
int SetTargetPosition(modbus_t *ctx, unsigned int target_position, string header) //DB 20160420
{
	//Singleton to manage the output of the program.		
	OutputModule *output_module;
	output_module = OutputModule::Instance ();
	
	//Recording the function name.		
	string tmp_string(__func__);
	
	//Variable used to store the possible error.	
	string tmp_errno;	
	
	//This buffer will be used to store the data being sent to the driver.	
	uint16_t data[STANDARDBUFFERLIMIT];
	
	//collect success/error status.	
	int rc;

	//clearing data.
	bzero(data, STANDARDBUFFERLIMIT);
	
	//Storing target_position that will be sent to the driver.	
	data[1]= target_position & 0x0000FFFF;

	data[0]= (target_position & 0xFFFF0000) >> 16;	
	
	//Sending data to the driver.	
	rc = modbus_write_registers(ctx, TARGET_POSITION, 2, data);
	
	//sleeping a while in order to give the programmer the time to send the data.	
	usleep(SLEEPMODBUS);
	
	//If an error occurred.		
	if (rc == -1)
	{
		//Recording the error status.		
		tmp_errno = modbus_strerror(errno);
		
		//Printing the error status.		
		output_module->Output(header + "READING READBACK REMOTE STATE FAILED " + tmp_string + "\t" + tmp_errno + "\n");
	}

	//Returning the error status.	
	return rc;
	
}

//Function used to set the home_done variable.
//header is the prefix of every output printed by the function.
//The return value is the error status.
int SetHomeDone (modbus_t *ctx, uint16_t home_done, string header)
{
	//Singleton to manage the output of the program.	
	OutputModule *output_module;
	output_module = OutputModule::Instance ();
	
	//Recording the function name.		
	string tmp_string(__func__);
	
	//Variable used to store the possible error.	
	string tmp_errno;

	//This buffer will be used to store the data being sent to the driver.			
	uint16_t data[STANDARDBUFFERLIMIT];
	
	//collect success/error status.		
	int rc;

	//clearing data.
	bzero(data, STANDARDBUFFERLIMIT);
	
	//Storing home_done that will be sent to the driver.	
	data[0] = home_done;
	
	//Sending data to the driver.	
	rc = modbus_write_registers(ctx, HOME_DONE, 1, &data[0]);
	
	//sleeping a while in order to give the programmer the time to send the data.	
	usleep(SLEEPMODBUS);
	
	//If an error occurred.		
	if (rc == -1) 
	{
		//Recording the error status.			
		tmp_errno = modbus_strerror(errno);
		
		//Printing the error status.			
		output_module->Output(header + "READING READBACK REMOTE STATE FAILED " + tmp_string + "\t" + tmp_errno + "\n");
	}
	
	//Returning the error status.	
	return rc;
	
}

//Function used to set the Max_TargetPos variable.
//header is the prefix of every output printed by the function.
//The return value is the error status.
int SetMaxTargetPosition(modbus_t *ctx, unsigned int max_target_position, string header) //DB 20160420
{
	//Singleton to manage the output of the program.		
	OutputModule *output_module;
	output_module = OutputModule::Instance ();
	
	//Recording the function name.		
	string tmp_string(__func__);
	
	//Variable used to store the possible error.	
	string tmp_errno;	
	
	//This buffer will be used to store the data being sent to the driver.	
	uint16_t data[STANDARDBUFFERLIMIT];
	
	//collect success/error status.	
	int rc;

	//clearing data.
	bzero(data, STANDARDBUFFERLIMIT);
	
	//Storing target_position that will be sent to the driver.	
	data[1]= max_target_position & 0x0000FFFF;

	data[0]= (max_target_position & 0xFFFF0000) >> 16;	
	
	//Sending data to the driver.	
	rc = modbus_write_registers(ctx, MAX_TARGET_POS, 2, data);
	
	//sleeping a while in order to give the programmer the time to send the data.	
	usleep(SLEEPMODBUS);
	
	//If an error occurred.		
	if (rc == -1)
	{
		//Recording the error status.		
		tmp_errno = modbus_strerror(errno);
		
		//Printing the error status.		
		output_module->Output(header + "READING READBACK REMOTE STATE FAILED " + tmp_string + "\t" + tmp_errno + "\n");
	}

	//Returning the error status.	
	return rc;
	
}

//Function used to set the encoder_min variable.
//header is the prefix of every output printed by the function.
//The return value is the error status.
int SetEncoderMin (modbus_t *ctx, uint16_t encoder_min, string header)
{
	//Singleton to manage the output of the program.	
	OutputModule *output_module;
	output_module = OutputModule::Instance ();
	
	//Recording the function name.		
	string tmp_string(__func__);
	
	//Variable used to store the possible error.	
	string tmp_errno;

	//This buffer will be used to store the data being sent to the driver.			
	uint16_t data[STANDARDBUFFERLIMIT];
	
	//collect success/error status.		
	int rc;

	//clearing data.
	bzero(data, STANDARDBUFFERLIMIT);
	
	//Storing encoder_min that will be sent to the driver.	
	data[0] = encoder_min;
	
	//Sending data to the driver.	
	rc = modbus_write_registers(ctx, ENCODER_MIN, 1, &data[0]);
	
	//sleeping a while in order to give the programmer the time to send the data.	
	usleep(SLEEPMODBUS);
	
	//If an error occurred.		
	if (rc == -1) 
	{
		//Recording the error status.			
		tmp_errno = modbus_strerror(errno);
		
		//Printing the error status.			
		output_module->Output(header + "READING READBACK REMOTE STATE FAILED " + tmp_string + "\t" + tmp_errno + "\n");
	}
	
	//Returning the error status.	
	return rc;
	
}

//Function used to set the encoder_max variable.
//header is the prefix of every output printed by the function.
//The return value is the error status.
int SetEncoderMax (modbus_t *ctx, uint16_t encoder_max, string header)
{
	//Singleton to manage the output of the program.	
	OutputModule *output_module;
	output_module = OutputModule::Instance ();
	
	//Recording the function name.		
	string tmp_string(__func__);
	
	//Variable used to store the possible error.	
	string tmp_errno;

	//This buffer will be used to store the data being sent to the driver.			
	uint16_t data[STANDARDBUFFERLIMIT];
	
	//collect success/error status.		
	int rc;

	//clearing data.
	bzero(data, STANDARDBUFFERLIMIT);
	
	//Storing encoder_max that will be sent to the driver.	
	data[0] = encoder_max;
	
	//Sending data to the driver.	
	rc = modbus_write_registers(ctx, ENCODER_MAX, 1, &data[0]);
	
	//sleeping a while in order to give the programmer the time to send the data.	
	usleep(SLEEPMODBUS);
	
	//If an error occurred.		
	if (rc == -1) 
	{
		//Recording the error status.			
		tmp_errno = modbus_strerror(errno);
		
		//Printing the error status.			
		output_module->Output(header + "READING READBACK REMOTE STATE FAILED " + tmp_string + "\t" + tmp_errno + "\n");
	}
	
	//Returning the error status.	
	return rc;
	
}

//Function used to set the phase_current_user variable.
//header is the prefix of every output printed by the function.
//The return value is the error status.
int SetPhaseCurrentUser (modbus_t *ctx, uint16_t phase_current_user, string header)
{
	//Singleton to manage the output of the program.	
	OutputModule *output_module;
	output_module = OutputModule::Instance ();
	
	//Recording the function name.		
	string tmp_string(__func__);
	
	//Variable used to store the possible error.	
	string tmp_errno;

	//This buffer will be used to store the data being sent to the driver.			
	uint16_t data[STANDARDBUFFERLIMIT];
	
	//collect success/error status.		
	int rc;

	//clearing data.
	bzero(data, STANDARDBUFFERLIMIT);
	
	//Storing phase_current_user that will be sent to the driver.	
	data[0] = phase_current_user;
	
	//Sending data to the driver.	
	rc = modbus_write_registers(ctx, PHASE_CURRENT_USER, 1, &data[0]);
	
	//sleeping a while in order to give the programmer the time to send the data.	
	usleep(SLEEPMODBUS);
	
	//If an error occurred.		
	if (rc == -1) 
	{
		//Recording the error status.			
		tmp_errno = modbus_strerror(errno);
		
		//Printing the error status.			
		output_module->Output(header + "READING READBACK REMOTE STATE FAILED " + tmp_string + "\t" + tmp_errno + "\n");
	}
	
	//Returning the error status.	
	return rc;
	
}

//Function used to set the delta_analog_pos variable.
//header is the prefix of every output printed by the function.
//The return value is the error status.
int SetDeltaAnalogPos (modbus_t *ctx, uint16_t delta_analog_pos, string header)
{
	//Singleton to manage the output of the program.	
	OutputModule *output_module;
	output_module = OutputModule::Instance ();
	
	//Recording the function name.		
	string tmp_string(__func__);
	
	//Variable used to store the possible error.	
	string tmp_errno;

	//This buffer will be used to store the data being sent to the driver.			
	uint16_t data[STANDARDBUFFERLIMIT];
	
	//collect success/error status.		
	int rc;

	//clearing data.
	bzero(data, STANDARDBUFFERLIMIT);
	
	//Storing delta_analog_pos that will be sent to the driver.	
	data[0] = delta_analog_pos;
	
	//Sending data to the driver.	
	rc = modbus_write_registers(ctx, DELTA_ANALOG_POS, 1, &data[0]);
	
	//sleeping a while in order to give the programmer the time to send the data.	
	usleep(SLEEPMODBUS);
	
	//If an error occurred.		
	if (rc == -1) 
	{
		//Recording the error status.			
		tmp_errno = modbus_strerror(errno);
		
		//Printing the error status.			
		output_module->Output(header + "READING READBACK REMOTE STATE FAILED " + tmp_string + "\t" + tmp_errno + "\n");
	}
	
	//Returning the error status.	
	return rc;
	
}

//Function used to set the delta_analog_neg variable.
//header is the prefix of every output printed by the function.
//The return value is the error status.
int SetDeltaAnalogNeg (modbus_t *ctx, int16_t delta_analog_neg, string header)
{
	//Singleton to manage the output of the program.	
	OutputModule *output_module;
	output_module = OutputModule::Instance ();
	
	//Recording the function name.		
	string tmp_string(__func__);
	
	//Variable used to store the possible error.	
	string tmp_errno;

	//This buffer will be used to store the data being sent to the driver.			
	uint16_t data[STANDARDBUFFERLIMIT];
	
	//collect success/error status.		
	int rc;

	//clearing data.
	bzero(data, STANDARDBUFFERLIMIT);
	
	//Storing delta_analog_neg that will be sent to the driver.	
	data[0] = delta_analog_neg;
	
	//Sending data to the driver.	
	rc = modbus_write_registers(ctx, DELTA_ANALOG_NEG, 1, &data[0]);
	
	//sleeping a while in order to give the programmer the time to send the data.	
	usleep(SLEEPMODBUS);
	
	//If an error occurred.		
	if (rc == -1) 
	{
		//Recording the error status.			
		tmp_errno = modbus_strerror(errno);
		
		//Printing the error status.			
		output_module->Output(header + "READING READBACK REMOTE STATE FAILED " + tmp_string + "\t" + tmp_errno + "\n");
	}
	
	//Returning the error status.	
	return rc;
	
}

//Function used to set the delay_check_rot variable.
//header is the prefix of every output printed by the function.
//The return value is the error status.
int SetDelayCheckRot (modbus_t *ctx, uint16_t delay_check_rot, string header)
{
	//Singleton to manage the output of the program.	
	OutputModule *output_module;
	output_module = OutputModule::Instance ();
	
	//Recording the function name.		
	string tmp_string(__func__);
	
	//Variable used to store the possible error.	
	string tmp_errno;

	//This buffer will be used to store the data being sent to the driver.			
	uint16_t data[STANDARDBUFFERLIMIT];
	
	//collect success/error status.		
	int rc;

	//clearing data.
	bzero(data, STANDARDBUFFERLIMIT);
	
	//Storing delay_check_rot that will be sent to the driver.	
	data[0] = delay_check_rot;
	
	//Sending data to the driver.	
	rc = modbus_write_registers(ctx, DELAY_CHECKROT, 1, &data[0]);
	
	//sleeping a while in order to give the programmer the time to send the data.	
	usleep(SLEEPMODBUS);
	
	//If an error occurred.		
	if (rc == -1) 
	{
		//Recording the error status.			
		tmp_errno = modbus_strerror(errno);
		
		//Printing the error status.			
		output_module->Output(header + "READING READBACK REMOTE STATE FAILED " + tmp_string + "\t" + tmp_errno + "\n");
	}
	
	//Returning the error status.	
	return rc;
	
}

//Function used to read the max_vel of the driver.
//rc_arg will contain the status of the operation. 
uint16_t ReadMaxVel(modbus_t *ctx, int* rc_arg, string header)
{
	//Singleton to manage the output of the program.	
	OutputModule *output_module;
	output_module = OutputModule::Instance ();
	
	//Recording the function name.	
	string tmp_string(__func__);
	
	//Variable used to store the possible error.		
	string tmp_errno;	
	
	output_module->Output(header + "\n");

	output_module->Output(header + tmp_string + "Reading MaxVel...\n");
	
	//This buffer will be used to store the data received by the driver.	
	uint16_t data[STANDARDBUFFERLIMIT];
	
	//collect success/error status.	
	int rc;

	//clearing data.
	bzero(data, STANDARDBUFFERLIMIT);
	
	data[0] = 0;
	
	//reading max_vel register and put the data read in "data" array.	
	rc = modbus_read_registers(ctx, MAX_VEL, 1, &data[0]);
	
	//collecting the status.	
	*rc_arg = rc;
	
	//sleeping a while in order to give the programmer the time to send the data.	
	usleep(SLEEPMODBUS);
	
	//If an error occurred.	
	if (rc == -1) 
	{
		//recording the error.		
		tmp_errno = modbus_strerror(errno);
		output_module->Output(header + "READING READBACK REMOTE STATE FAILED " + tmp_string + "\t" + tmp_errno + "\n");
		
		return 0;
	}
	else
	{
		//If success, printing and returning the value read.		
		output_module->Output(header + tmp_string + ": Reading MaxVel DONE rc: " + to_string(rc) + " \t MaxVel content is: " + to_string((int)data[0]) + "\n");
		
		return data[0];
	}	
	
}

//Function used to read the vel_home of the driver.
//rc_arg will contain the status of the operation. 
int16_t ReadVelHome(modbus_t *ctx, int* rc_arg, string header)
{
	//Singleton to manage the output of the program.	
	OutputModule *output_module;
	output_module = OutputModule::Instance ();
	
	//Recording the function name.		
	string tmp_string(__func__);
	
	//Variable used to store the possible error.		
	string tmp_errno;	
	
	output_module->Output(header + "\n");

	output_module->Output(header + tmp_string + "Reading VelHome...\n");
	
	//This buffer will be used to store the data received by the driver.	
	uint16_t data[STANDARDBUFFERLIMIT];
	
	//collect success/error status.		
	int rc;

	//clearing data.
	bzero(data, STANDARDBUFFERLIMIT);
	data[0] = 0;
	
	//reading vel_home register and put the data read in "data" array.		
	rc = modbus_read_registers(ctx, VEL_HOME, 1, &data[0]);
	
	//collecting the status.	
	*rc_arg = rc;
	
	//sleeping a while in order to give the programmer the time to send the data.		
	usleep(SLEEPMODBUS);
	
	//If an error occurred.	
	if (rc == -1) 
	{
		//recording the error.			
		tmp_errno = modbus_strerror(errno);
		output_module->Output(header + "READING READBACK REMOTE STATE FAILED " + tmp_string + "\t" + tmp_errno + "\n");
		
		return 0;
	}
	else
	{
		//If success, printing and returning the value read.		
		output_module->Output(header + tmp_string + ": Reading VelHome DONE rc: " + to_string(rc) + " \t VelHome content is: " + to_string((int)data[0]) + "\n");
		
		return data[0];
	}	
	
}

//Function used to read the acceleration of the driver.
//rc_arg will contain the status of the operation. 
uint16_t ReadAcceleration(modbus_t *ctx, int* rc_arg, string header)
{
	//Singleton to manage the output of the program.		
	OutputModule *output_module;
	output_module = OutputModule::Instance ();
	
	//Recording the function name.			
	string tmp_string(__func__);
	string tmp_errno;	
	
	output_module->Output(header + "\n");

	output_module->Output(header + tmp_string + "Reading Acceleration...\n");
	
	//This buffer will be used to store the data received by the driver.	
	uint16_t data[STANDARDBUFFERLIMIT];
	
	//collect success/error status.		
	int rc;

	//clearing data.
	bzero(data, STANDARDBUFFERLIMIT);
	data[0] = 0;
	
	//reading acceleration register and put the data read in "data" array.		
	rc = modbus_read_registers(ctx, ACCELERATION, 1, &data[0]);
	
	//collecting the status.	
	*rc_arg = rc;
	
	//sleeping a while in order to give the programmer the time to send the data.		
	usleep(SLEEPMODBUS);
	
	//If an error occurred.		
	if (rc == -1) 
	{
		//recording the error.			
		tmp_errno = modbus_strerror(errno);
		output_module->Output(header + "READING READBACK REMOTE STATE FAILED " + tmp_string + "\t" + tmp_errno + "\n");
		
		return 0;
	}
	else
	{
		//If success, printing and returning the value read.				
		output_module->Output(header + tmp_string + ": Reading Acceleration DONE rc: " + to_string(rc) + " \t Acceleration content is: " + to_string((int)data[0]) + "\n");
		return data[0];
	}	
	
}

//Function used to read the deceleration of the driver.
//rc_arg will contain the status of the operation. 
uint16_t ReadDeceleration(modbus_t *ctx, int* rc_arg, string header)
{
	
	//Singleton to manage the output of the program.		
	OutputModule *output_module;
	output_module = OutputModule::Instance ();
	
	//Recording the function name.			
	string tmp_string(__func__);
	string tmp_errno;	
	
	output_module->Output(header + "\n");

	output_module->Output(header + tmp_string + "Reading Deceleration...\n");
	
	//This buffer will be used to store the data received by the driver.	
	uint16_t data[STANDARDBUFFERLIMIT];
	
	//collect success/error status.		
	int rc;

	//clearing data.
	bzero(data, STANDARDBUFFERLIMIT);
	data[0] = 0;

	//reading deceleration register and put the data read in "data" array.		
	rc = modbus_read_registers(ctx, DECELERATION, 1, &data[0]);
	
	//collecting the status.		
	*rc_arg = rc;
	
	//sleeping a while in order to give the programmer the time to send the data.		
	usleep(SLEEPMODBUS);
	
	//If an error occurred.		
	if (rc == -1) 
	{
		//recording the error.			
		tmp_errno = modbus_strerror(errno);
		output_module->Output(header + "READING READBACK REMOTE STATE FAILED " + tmp_string + "\t" + tmp_errno + "\n");
		
		return 0;
	}
	else
	{
		//If success, printing and returning the value read.		
		output_module->Output(header + tmp_string + ": Reading Deceleration DONE rc: " + to_string(rc) + " \t Deceleration content is: " + to_string((int)data[0]) + "\n");
		return data[0];
	}	
	
}

//Function used to read the phase_current of the driver.
//rc_arg will contain the status of the operation. 
uint16_t ReadPhaseCurrent(modbus_t *ctx, int* rc_arg, string header)
{
	
	//Singleton to manage the output of the program.		
	OutputModule *output_module;
	output_module = OutputModule::Instance ();
	
	//Recording the function name.			
	string tmp_string(__func__);
	string tmp_errno;	
	
	output_module->Output(header + "\n");

	output_module->Output(header + tmp_string + "Reading PhaseCurrent...\n");
	
	//This buffer will be used to store the data received by the driver.	
	uint16_t data[STANDARDBUFFERLIMIT];
	
	//collect success/error status.		
	int rc;

	//clearing data.
	bzero(data, STANDARDBUFFERLIMIT);
	data[0] = 0;
	
	//reading phase_current register and put the data read in "data" array.		
	rc = modbus_read_registers(ctx, PHASE_CURRENT, 1, &data[0]);
	
	//collecting the status.		
	*rc_arg = rc;
	
	//sleeping a while in order to give the programmer the time to send the data.		
	usleep(SLEEPMODBUS);
	
	//If an error occurred.		
	if (rc == -1) 
	{
		//recording the error.			
		tmp_errno = modbus_strerror(errno);
		output_module->Output(header + "READING READBACK REMOTE STATE FAILED " + tmp_string + "\t" + tmp_errno + "\n");
		
		return 0;
	}
	else
	{
		//If success, printing and returning the value read.				
		output_module->Output(header + tmp_string + ": Reading PhaseCurrent DONE rc: " + to_string(rc) + " \t PhaseCurrent content is: " + to_string((int)data[0]) + "\n");
		return data[0];
	}	
	
}

//Function used to read the analog_output0 of the driver.
//rc_arg will contain the status of the operation. 
uint16_t ReadAnalogOutput0(modbus_t *ctx, int* rc_arg, string header)
{

	//Singleton to manage the output of the program.		
	OutputModule *output_module;
	output_module = OutputModule::Instance ();
	
	//Recording the function name.			
	string tmp_string(__func__);
	string tmp_errno;	
	
	output_module->Output(header + "\n");

	output_module->Output(header + tmp_string + "Reading analog_output0...\n");
	
	//This buffer will be used to store the data received by the driver.	
	uint16_t data[STANDARDBUFFERLIMIT];
	
	//collect success/error status.		
	int rc;

	//clearing data.
	bzero(data, STANDARDBUFFERLIMIT);
	data[0] = 0;
	
	//reading analog_output0 register and put the data read in "data" array.	
	rc = modbus_read_registers(ctx, ANALOG_OUTPUT_0, 1, &data[0]);
	
	//collecting the status.		
	*rc_arg = rc;
	
	//sleeping a while in order to give the programmer the time to send the data.		
	usleep(SLEEPMODBUS);
	
	//If an error occurred.		
	if (rc == -1) 
	{
		//recording the error.			
		tmp_errno = modbus_strerror(errno);
		output_module->Output(header + "READING READBACK REMOTE STATE FAILED " + tmp_string + "\t" + tmp_errno + "\n");
		
		return 0;
	}
	else
	{
		//If success, printing and returning the value read.				
		output_module->Output(header + tmp_string + ": Reading AnalogOutput DONE rc: " + to_string(rc) + " \t AnalogOutput content is: " + to_string((int)data[0]) + "\n");
		return data[0];
	}	
	
}

//Function used to read the status_state register of the driver.
//rc_arg will contain the status of the operation. 
uint16_t ReadStatusState(modbus_t *ctx, int* rc_arg, string header)
{
	//Singleton to manage the output of the program.		
	OutputModule *output_module;
	output_module = OutputModule::Instance ();
	
	//Recording the function name.			
	string tmp_string(__func__);
	string tmp_errno;	
	
	output_module->Output(header + "\n");

	output_module->Output(header + tmp_string + "Reading status_state...\n");
	
	//This buffer will be used to store the data received by the driver.	
	uint16_t data[STANDARDBUFFERLIMIT];
	
	//collect success/error status.		
	int rc;

	//clearing data.
	bzero(data, STANDARDBUFFERLIMIT);
	data[0] = 0;
	
	//reading status_state register and put the data read in "data" array.		
	rc = modbus_read_registers(ctx, STATUS_STATE, 1, &data[0]);
	
	//collecting the status.		
	*rc_arg = rc;
	
	//sleeping a while in order to give the programmer the time to send the data.		
	usleep(SLEEPMODBUS);
	
	//If an error occurred.		
	if (rc == -1) 
	{
		//recording the error.			
		tmp_errno = modbus_strerror(errno);
		output_module->Output(header + "READING READBACK REMOTE STATE FAILED " + tmp_string + "\t" + tmp_errno + "\n");
		
		return 0;
	}
	else
	{
		//If success, printing and returning the value read.				
		output_module->Output(header + tmp_string + ": Reading status_state DONE rc: " + to_string(rc) + " \t status_state content is: " + to_string((int)data[0]) + "\n");
		return data[0];
	}	
	
}

//Function used to read the request_state register of the driver.
//rc_arg will contain the status of the operation. 
uint16_t ReadRequestState(modbus_t *ctx, int* rc_arg, string header)
{
	//Singleton to manage the output of the program.		
	OutputModule *output_module;
	output_module = OutputModule::Instance ();
	
	//Recording the function name.			
	string tmp_string(__func__);
	string tmp_errno;	
	
	output_module->Output(header + "\n");

	output_module->Output(header + tmp_string + "Reading request_state...\n");
	
	//This buffer will be used to store the data received by the driver.	
	uint16_t data[STANDARDBUFFERLIMIT];
	
	//collect success/error status.		
	int rc;

	//clearing data.
	bzero(data, STANDARDBUFFERLIMIT);
	data[0] = 0;
	
	//reading request_state register and put the data read in "data" array.		
	rc = modbus_read_registers(ctx, REQUEST_STATE, 1, &data[0]);
	
	//collecting the status.		
	*rc_arg = rc;
	
	//sleeping a while in order to give the programmer the time to send the data.		
	usleep(SLEEPMODBUS);
	
	//If an error occurred.		
	if (rc == -1) 
	{
		//recording the error.			
		tmp_errno = modbus_strerror(errno);
		output_module->Output(header + "READING READBACK REMOTE STATE FAILED " + tmp_string + "\t" + tmp_errno + "\n");
		
		return 0;
	}
	else
	{
		//If success, printing and returning the value read.				
		output_module->Output(header + tmp_string + ": Reading request_state DONE rc: " + to_string(rc) + " \t request_state content is: " + to_string((int)data[0]) + "\n");
		return data[0];
	}	
	
}

//Function used to read the analog_input0 of the driver.
//rc_arg will contain the status of the operation. 
uint16_t ReadAnalogInput0(modbus_t *ctx, int* rc_arg, string header)
{
	//This buffer will be used to store the data received by the driver.		
	uint16_t data[STANDARDBUFFERLIMIT];
	
	//collect success/error status.		
	int rc;

	//clearing data.
	bzero(data, STANDARDBUFFERLIMIT);
	data[0] = 0;
	
	//reading analog_input0 register and put the data read in "data" array.		
	rc = modbus_read_registers(ctx, ANALOG_INPUT_0, 1, &data[0]);
	
	//collecting the status.		
	*rc_arg = rc;
	
	//sleeping a while in order to give the programmer the time to send the data.		
	usleep(SLEEPMODBUS);
	
	//If an error occurred.		
	if (rc == -1) 
	{
		return 0;
	}
	else
	{
		//If success, returning the value read.				
		return data[0];
	}	
	
}

//Function used to read the current_position of the driver.
//rc_arg will contain the status of the operation. 
int ReadCurrentPosition(modbus_t *ctx, int* rc_arg, string header)
{
	int CurrentPosition = 0;
	
	//This buffer will be used to store the data received by the driver.		
	uint16_t data[STANDARDBUFFERLIMIT];
	
	//collect success/error status.		
	int rc;

	//clearing data.
	bzero(data, STANDARDBUFFERLIMIT);
	
	//reading current_position register and put the data read in "data" array.	
	rc = modbus_read_registers(ctx, CURRENT_POSITION, 2, &data[0]);
	
	//collecting the status.		
	*rc_arg = rc;
	
	//sleeping a while in order to give the programmer the time to send the data.		
	usleep(SLEEPMODBUS);
	
	//If an error occurred.		
	if (rc == -1) 
	{
		return 0;
	}
	else
	{
		//If success, composing and returning the value read.						
		
		CurrentPosition = CurrentPosition | data[1];
		CurrentPosition = CurrentPosition | (data[0] << 16);
		
		return CurrentPosition;
	}
	
}


//Function used to read the phase_current variable of the driver.
//rc_arg will contain the status of the operation. 
uint16_t ReadHomeDone(modbus_t *ctx, int* rc_arg, string header)
{
	
	//Singleton to manage the output of the program.		
	OutputModule *output_module;
	output_module = OutputModule::Instance ();
	
	//Recording the function name.			
	string tmp_string(__func__);
	string tmp_errno;	
	
	output_module->Output(header + "\n");

	output_module->Output(header + tmp_string + "Reading home_done...\n");
	
	//This buffer will be used to store the data received by the driver.	
	uint16_t data[STANDARDBUFFERLIMIT];
	
	//collect success/error status.		
	int rc;

	//clearing data.
	bzero(data, STANDARDBUFFERLIMIT);
	data[0] = 0;
	
	//reading home_done register and put the data read in "data" array.		
	rc = modbus_read_registers(ctx, HOME_DONE, 1, &data[0]);
	
	//collecting the status.		
	*rc_arg = rc;
	
	//sleeping a while in order to give the programmer the time to send the data.		
	usleep(SLEEPMODBUS);
	
	//If an error occurred.		
	if (rc == -1) 
	{
		//recording the error.			
		tmp_errno = modbus_strerror(errno);
		output_module->Output(header + "READING READBACK REMOTE STATE FAILED " + tmp_string + "\t" + tmp_errno + "\n");
		
		return 0;
	}
	else
	{
		//If success, printing and returning the value read.				
		output_module->Output(header + tmp_string + ": Reading home_done DONE rc: " + to_string(rc) + " \t home_done content is: " + to_string((int)data[0]) + "\n");
		return data[0];
	}	
	
}

//Function used to read the Max_TargetPos variable of the driver.
//rc_arg will contain the status of the operation. 
int ReadMaxTargetPosition(modbus_t *ctx, int* rc_arg, string header)
{
	int MaxTargetPosition = 0;
	
	//This buffer will be used to store the data received by the driver.		
	uint16_t data[STANDARDBUFFERLIMIT];
	
	//collect success/error status.		
	int rc;

	//clearing data.
	bzero(data, STANDARDBUFFERLIMIT);
	
	//reading Max_TargetPos variable and put the data read in "data" array.	
	rc = modbus_read_registers(ctx, MAX_TARGET_POS, 2, &data[0]);
	
	//collecting the status.		
	*rc_arg = rc;
	
	//sleeping a while in order to give the programmer the time to send the data.		
	usleep(SLEEPMODBUS);
	
	//If an error occurred.		
	if (rc == -1) 
	{
		return 0;
	}
	else
	{
		//If success, composing and returning the value read.						
		
		MaxTargetPosition = MaxTargetPosition | data[1];
		MaxTargetPosition = MaxTargetPosition | (data[0] << 16);
		
		return MaxTargetPosition;
	}
	
}


//Function used to read the encoder_min variable of the driver.
//rc_arg will contain the status of the operation. 
uint16_t ReadEncoderMin(modbus_t *ctx, int* rc_arg, string header)
{
	
	//Singleton to manage the output of the program.		
	OutputModule *output_module;
	output_module = OutputModule::Instance ();
	
	//Recording the function name.			
	string tmp_string(__func__);
	string tmp_errno;	
	
	output_module->Output(header + "\n");

	output_module->Output(header + tmp_string + "Reading encoder_min...\n");
	
	//This buffer will be used to store the data received by the driver.	
	uint16_t data[STANDARDBUFFERLIMIT];
	
	//collect success/error status.		
	int rc;

	//clearing data.
	bzero(data, STANDARDBUFFERLIMIT);
	data[0] = 0;
	
	//reading encoder_min register and put the data read in "data" array.		
	rc = modbus_read_registers(ctx, ENCODER_MIN, 1, &data[0]);
	
	//collecting the status.		
	*rc_arg = rc;
	
	//sleeping a while in order to give the programmer the time to send the data.		
	usleep(SLEEPMODBUS);
	
	//If an error occurred.		
	if (rc == -1) 
	{
		//recording the error.			
		tmp_errno = modbus_strerror(errno);
		output_module->Output(header + "READING READBACK REMOTE STATE FAILED " + tmp_string + "\t" + tmp_errno + "\n");
		
		return 0;
	}
	else
	{
		//If success, printing and returning the value read.				
		output_module->Output(header + tmp_string + ": Reading encoder_min DONE rc: " + to_string(rc) + " \t encoder_min content is: " + to_string((int)data[0]) + "\n");
		return data[0];
	}	
	
}

//Function used to read the encoder_max variable of the driver.
//rc_arg will contain the status of the operation. 
uint16_t ReadEncoderMax(modbus_t *ctx, int* rc_arg, string header)
{
	
	//Singleton to manage the output of the program.		
	OutputModule *output_module;
	output_module = OutputModule::Instance ();
	
	//Recording the function name.			
	string tmp_string(__func__);
	string tmp_errno;	
	
	output_module->Output(header + "\n");

	output_module->Output(header + tmp_string + "Reading encoder_max...\n");
	
	//This buffer will be used to store the data received by the driver.	
	uint16_t data[STANDARDBUFFERLIMIT];
	
	//collect success/error status.		
	int rc;

	//clearing data.
	bzero(data, STANDARDBUFFERLIMIT);
	data[0] = 0;
	
	//reading encoder_max register and put the data read in "data" array.		
	rc = modbus_read_registers(ctx, ENCODER_MAX, 1, &data[0]);
	
	//collecting the status.		
	*rc_arg = rc;
	
	//sleeping a while in order to give the programmer the time to send the data.		
	usleep(SLEEPMODBUS);
	
	//If an error occurred.		
	if (rc == -1) 
	{
		//recording the error.			
		tmp_errno = modbus_strerror(errno);
		output_module->Output(header + "READING READBACK REMOTE STATE FAILED " + tmp_string + "\t" + tmp_errno + "\n");
		
		return 0;
	}
	else
	{
		//If success, printing and returning the value read.				
		output_module->Output(header + tmp_string + ": Reading encoder_max DONE rc: " + to_string(rc) + " \t encoder_max content is: " + to_string((int)data[0]) + "\n");
		return data[0];
	}	
	
}

//Function used to read the phase_current_user variable of the driver.
//rc_arg will contain the status of the operation. 
uint16_t ReadPhaseCurrentUser(modbus_t *ctx, int* rc_arg, string header)
{
	
	//Singleton to manage the output of the program.		
	OutputModule *output_module;
	output_module = OutputModule::Instance ();
	
	//Recording the function name.			
	string tmp_string(__func__);
	string tmp_errno;	
	
	output_module->Output(header + "\n");

	output_module->Output(header + tmp_string + "Reading phase_current_user...\n");
	
	//This buffer will be used to store the data received by the driver.	
	uint16_t data[STANDARDBUFFERLIMIT];
	
	//collect success/error status.		
	int rc;

	//clearing data.
	bzero(data, STANDARDBUFFERLIMIT);
	data[0] = 0;
	
	//reading phase_current_user register and put the data read in "data" array.		
	rc = modbus_read_registers(ctx, PHASE_CURRENT_USER, 1, &data[0]);
	
	//collecting the status.		
	*rc_arg = rc;
	
	//sleeping a while in order to give the programmer the time to send the data.		
	usleep(SLEEPMODBUS);
	
	//If an error occurred.		
	if (rc == -1) 
	{
		//recording the error.			
		tmp_errno = modbus_strerror(errno);
		output_module->Output(header + "READING READBACK REMOTE STATE FAILED " + tmp_string + "\t" + tmp_errno + "\n");
		
		return 0;
	}
	else
	{
		//If success, printing and returning the value read.				
		output_module->Output(header + tmp_string + ": Reading phase_current_user DONE rc: " + to_string(rc) + " \t phase_current_user content is: " + to_string((int)data[0]) + "\n");
		return data[0];
	}	
	
}


//Function used to read the delta_analog_pos variable of the driver.
//rc_arg will contain the status of the operation. 
uint16_t ReadDeltaAnalogPos(modbus_t *ctx, int* rc_arg, string header)
{
	
	//Singleton to manage the output of the program.		
	OutputModule *output_module;
	output_module = OutputModule::Instance ();
	
	//Recording the function name.			
	string tmp_string(__func__);
	string tmp_errno;	
	
	output_module->Output(header + "\n");

	output_module->Output(header + tmp_string + "Reading delta_analog_pos...\n");
	
	//This buffer will be used to store the data received by the driver.	
	uint16_t data[STANDARDBUFFERLIMIT];
	
	//collect success/error status.		
	int rc;

	//clearing data.
	bzero(data, STANDARDBUFFERLIMIT);
	data[0] = 0;
	
	//reading delta_analog_pos register and put the data read in "data" array.		
	rc = modbus_read_registers(ctx, DELTA_ANALOG_POS, 1, &data[0]);
	
	//collecting the status.		
	*rc_arg = rc;
	
	//sleeping a while in order to give the programmer the time to send the data.		
	usleep(SLEEPMODBUS);
	
	//If an error occurred.		
	if (rc == -1) 
	{
		//recording the error.			
		tmp_errno = modbus_strerror(errno);
		output_module->Output(header + "READING READBACK REMOTE STATE FAILED " + tmp_string + "\t" + tmp_errno + "\n");
		
		return 0;
	}
	else
	{
		//If success, printing and returning the value read.				
		output_module->Output(header + tmp_string + ": Reading delta_analog_pos DONE rc: " + to_string(rc) + " \t delta_analog_pos content is: " + to_string((int)data[0]) + "\n");
		return data[0];
	}	
	
}

//Function used to read the delta_analog_neg variable of the driver.
//rc_arg will contain the status of the operation. 
int16_t ReadDeltaAnalogNeg(modbus_t *ctx, int* rc_arg, string header)
{
	
	//Singleton to manage the output of the program.		
	OutputModule *output_module;
	output_module = OutputModule::Instance ();
	
	//Recording the function name.			
	string tmp_string(__func__);
	string tmp_errno;	
	
	output_module->Output(header + "\n");

	output_module->Output(header + tmp_string + "Reading delta_analog_neg...\n");
	
	//This buffer will be used to store the data received by the driver.	
	uint16_t data[STANDARDBUFFERLIMIT];
	
	//collect success/error status.		
	int rc;

	//clearing data.
	bzero(data, STANDARDBUFFERLIMIT);
	data[0] = 0;
	
	//reading delta_analog_neg register and put the data read in "data" array.		
	rc = modbus_read_registers(ctx, DELTA_ANALOG_NEG, 1, &data[0]);
	
	//collecting the status.		
	*rc_arg = rc;
	
	//sleeping a while in order to give the programmer the time to send the data.		
	usleep(SLEEPMODBUS);
	
	//If an error occurred.		
	if (rc == -1) 
	{
		//recording the error.			
		tmp_errno = modbus_strerror(errno);
		output_module->Output(header + "READING READBACK REMOTE STATE FAILED " + tmp_string + "\t" + tmp_errno + "\n");
		
		return 0;
	}
	else
	{
		//If success, printing and returning the value read.				
		output_module->Output(header + tmp_string + ": Reading delta_analog_neg DONE rc: " + to_string(rc) + " \t delta_analog_neg content is: " + to_string((int)data[0]) + "\n");
		return data[0];
	}	
	
}


//Function used to read the delay_check_rot variable of the driver.
//rc_arg will contain the status of the operation. 
uint16_t ReadDelayCheckRot(modbus_t *ctx, int* rc_arg, string header)
{
	
	//Singleton to manage the output of the program.		
	OutputModule *output_module;
	output_module = OutputModule::Instance ();
	
	//Recording the function name.			
	string tmp_string(__func__);
	string tmp_errno;	
	
	output_module->Output(header + "\n");

	output_module->Output(header + tmp_string + "Reading delay_check_rot...\n");
	
	//This buffer will be used to store the data received by the driver.	
	uint16_t data[STANDARDBUFFERLIMIT];
	
	//collect success/error status.		
	int rc;

	//clearing data.
	bzero(data, STANDARDBUFFERLIMIT);
	data[0] = 0;
	
	//reading delay_check_rot register and put the data read in "data" array.		
	rc = modbus_read_registers(ctx, DELAY_CHECKROT, 1, &data[0]);
	
	//collecting the status.		
	*rc_arg = rc;
	
	//sleeping a while in order to give the programmer the time to send the data.		
	usleep(SLEEPMODBUS);
	
	//If an error occurred.		
	if (rc == -1) 
	{
		//recording the error.			
		tmp_errno = modbus_strerror(errno);
		output_module->Output(header + "READING READBACK REMOTE STATE FAILED " + tmp_string + "\t" + tmp_errno + "\n");
		
		return 0;
	}
	else
	{
		//If success, printing and returning the value read.				
		output_module->Output(header + tmp_string + ": Reading delay_check_rot DONE rc: " + to_string(rc) + " \t delay_check_rot content is: " + to_string((int)data[0]) + "\n");
		return data[0];
	}	
	
}

//Function used to read the fault variable of the driver.
//rc_arg will contain the status of the operation. 
uint16_t ReadFault(modbus_t *ctx, int* rc_arg, string header)
{
	
	//Singleton to manage the output of the program.		
	OutputModule *output_module;
	output_module = OutputModule::Instance ();
	
	//Recording the function name.			
	string tmp_string(__func__);
	string tmp_errno;	
	
	output_module->Output(header + "\n");

	output_module->Output(header + tmp_string + "Reading fault...\n");
	
	//This buffer will be used to store the data received by the driver.	
	uint16_t data[STANDARDBUFFERLIMIT];
	
	//collect success/error status.		
	int rc;

	//clearing data.
	bzero(data, STANDARDBUFFERLIMIT);
	data[0] = 0;
	
	//reading Fault register and put the data read in "data" array.		
	rc = modbus_read_registers(ctx, FAULT, 1, &data[0]);
	
	//collecting the status.		
	*rc_arg = rc;
	
	//sleeping a while in order to give the programmer the time to send the data.		
	usleep(SLEEPMODBUS);
	
	//If an error occurred.		
	if (rc == -1) 
	{
		//recording the error.			
		tmp_errno = modbus_strerror(errno);
		output_module->Output(header + "READING READBACK REMOTE STATE FAILED " + tmp_string + "\t" + tmp_errno + "\n");
		
		return 0;
	}
	else
	{
		//If success, printing and returning the value read.				
		output_module->Output(header + tmp_string + ": Reading fault DONE rc: " + to_string(rc) + " \t fault content is: " + to_string((int)data[0]) + "\n");
		return data[0];
	}	
	
}
