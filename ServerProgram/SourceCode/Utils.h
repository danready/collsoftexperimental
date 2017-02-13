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

#ifndef UTILS_H
#define UTILS_H
#include <vector>
#include <regex.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string>

using std::vector;
using std::string;

//The FindPointer function returns a pointer to the word that follow the initial one.
//Ex. : FindPointer("moveto 1000") returns a pointer to 1 
char * FindPointer (char *yytext);

//The FindIntegerValue function returns the value of the integer that follow the initial word of the string yytext.
//Ex. : FindIntegerValue("moveto 1000") returns 1000
int FindIntegerValue (char *yytext);

//The reg_matches function tries to find "pattern" in the string pointed by str.
//It returns true if the string is found or returns false if the string is not found.
bool reg_matches (const char *str, const char *pattern);

//The function GetTime returns a string that represent the current date.
string GetTime();

//This struct is used to fetch the serial number informations from the file 
//SerialDrvLog.txt.
typedef struct {
int drv_num;
unsigned int serial_number;
} SerialCouple;

//This struct is used to collect the parameters informations either from the FileParLog.txt file or
//from the drivers.
typedef struct {
int drv_num;
int max_vel;
int16_t vel_home;
int acceleration;
int deceleration;
int phase_current;
int analog_output0;
} ParameterStruct;

//This struct is used to collect the encoding information either from the EncoderLog.txt file or
//from the drivers.
typedef struct {
int drv_num;
double slope;
double intercept;
double coefficient;
} EncoderStruct;

//This function is generated by flex. Its purpose is to analyze SerialDrvLog.txt.
void Analizza1(vector<SerialCouple>& serial_list, int* max_log);

//This function is generated by flex. Its purpose is to analyze FileParLog.txt.
void AnalizzaFilePar(vector<ParameterStruct>& parameter_arg, int* max_log);

//This function is generated by flex. Its purpose is to analyze EncoderLog.txt.
void AnalizzaFileEncoder(vector<EncoderStruct>& encoder_arg, int* max_log);

#endif
