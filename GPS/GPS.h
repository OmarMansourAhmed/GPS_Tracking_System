#ifndef GPS
#define GPS

#include <stdbool.h>

#define PI 3.141592653589793238462643383279502884197
#define Earth_Radius  	6371000

#include "tm4c123gh6pm.h"
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#define logname_length 7
#define pi   3.14159265359
#define Earth_Radius    6371000

#define finalDestinationLat 30.2460663
#define finalDestinationLong 31.4666137




//void GPS_read();

//void GPS_initialize();
//bool GPS_form(float *Lat ,float *Long , float *Speed , char *GPS[] );

float ToDegree (float angle);
float toRad(float angle);
float getDistance(float currentLat, float currentLong,float desiredLat, float desiredLong);

//void Calc_Coord (char GPS_arr[],float coordinates[]);

float* GPS_OUTPUT();
//float gettotalDistance(float totalDisplacement);
void printFloat(float a);
float getDisplacement(float currentLat, float currentLong,float desiredLat, float desiredLong);
float str_to_float(const char *str);

#endif	

