#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "tm4c123gh6pm.h"
#include "GPIO.h"
#include "UART.h"
#include "GPS.h"
#include "SysTick.h"


///*********************************************** Global Variables ***********************************************///

const int len=80;
char gps[len]={0};
char gps_Check[len]={0};
char GPS_logname[]="$GPRMC,";
int flag=1;
char i=0;



/// ************************************************************************************************************************************* ///



float* GPS_OUTPUT() {
				
        static float coordinates[3]={0,0,0}; // Array to hold latitude and longitude values
        char GPS_arr[80]; // Local array to store GPS data
        char received_char;
        char i=0;
        int log=1;
			  char GPS_logname[]="$GPRMC,";
	    	int j=0;
        // Loop until we receive the required logname
				const int len = 80;
				int m =0;
				//char GPS_Elements[15][15];
				// Tokenization
				char *token;
				char k=0;
				k++;
				
	check:
        do {
						log=1;
					  
						
            for (i = 0; i < 7; i++) {
							
                if (UART1_getChar()!=GPS_logname[i]) {
								
                   log=0;
									break;
                }
							
           }
					
        } while(log==0);
				
			
        strcpy(GPS_arr, "");

        // Read GPS data until '*' is received
						 j=0;
				do{
				
				
			  	 received_char = UART1_getChar();
          
				
					GPS_arr[j++]=received_char;
				
				
				
				}while(received_char != '*');
			
			    UART0_OutString(GPS_arr);
					UART0_OutString("\n");
      
		
				m = -1; //should be zero nevermind hussien
				for(token = strtok(GPS_arr, ",") ; token ; token = strtok(NULL , ",") ){			//"203522.00,A,30.06416667,N,31.28055556,W,0.004,133.4,130522,0.0,E,D*2B"
						
					  m++;																																	

						if(m==0){
						 
						    continue;    
						}
            else if(m==1 && *token!='A'){
							m=-1; //should be -1 nevermind hussien
							j =0;
							strcpy( gps , "" );
              goto check;
						}
                
            else if(m==2){
							
						coordinates[0]= str_to_float(token);
						
						UART0_OutString("ana gwa m=2");
						UART0_OutString(token);
						UART0_OutString("\n");
					//	printFloat(coordinates[0]);

							
						}
						else if(m==3){

							if(*token == 'N')
								coordinates[0] = (coordinates[0]);
							else if(*token != 'N')
								coordinates[0] = -(coordinates[0]);
						}

            else if(m==4){
  
							coordinates[1]= str_to_float(token);

						}
						else if(m==5){

							if(*token == 'E')
								coordinates[1] = (coordinates[1]);
							else if(*token != 'E')
								coordinates[1] = -(coordinates[1]);
						}						
            
						else if(m==6) //speed
                coordinates[2]= str_to_float(token);

            else if(m==7){

							m=-1; // should be -1 nevermind hussien
							j =0;
							strcpy( GPS_arr , "" );
							break;
						}
				
                
			}
			
			
        return coordinates;
			
	
}
/// ************************************************************************************************************************************* ///



	float ToDegree (float angle){
	int degree = (int)angle/100 ; 
	float minutes = angle -(float)degree*100;
		return (degree + (minutes/60) );
}

/// ************************************************************************************************************************************* ///

/////////////////////////displacement function////////////////////////////
		
float getDisplacement(float currentLat, float currentLong,float desiredLat, float desiredLong)
{
    // get radian angle
    float currentLongRad = toRad(ToDegree (currentLong));   /*  convert coordinates to radian   */
    float currentLatRad = toRad(ToDegree (currentLat));
    float desiredLongRad =toRad(desiredLong);
    float desiredLatRad = toRad(desiredLat);

    // get difference
    float longDifference = desiredLongRad - currentLongRad;
    float latDifference = desiredLatRad - currentLatRad;

    // calculate distance
    double a = sin(latDifference/2)*sin(latDifference/2)+(cos(currentLatRad)*cos(desiredLatRad)*sin(longDifference/2)*sin(longDifference/2)); // haversine formula
    double c = 2* atan2(sqrt(a),sqrt(1-a));

    return Earth_Radius * c;
}



/////////////////////////displacement function////////////////////////////

float getDistance(float currentLat, float currentLong,float desiredLat, float desiredLong)
{
    // get radian angle
    float currentLongRad = toRad(ToDegree(currentLong));   /*  convert coordinates to radian   */
    float currentLatRad = toRad(ToDegree(currentLat));
    float desiredLongRad =toRad(ToDegree(desiredLong));
    float desiredLatRad = toRad(ToDegree(desiredLat));

    // get difference
    float longDifference = desiredLongRad - currentLongRad;
    float latDifference = desiredLatRad - currentLatRad;

    // calculate distance
    double a = sin(latDifference/2)*sin(latDifference/2)+(cos(currentLatRad)*cos(desiredLatRad)*sin(longDifference/2)*sin(longDifference/2)); // haversine formula
    double c = 2* atan2(sqrt(a),sqrt(1-a));

    return Earth_Radius * c;
}

//////////////////////////convert angle to radian////////////////////////////
float toRad(float angle){
    return angle * (pi/180);
}


/// ************************************************************************************************************************************* ///
void printFloat(float a){
 char array[10];
	sprintf(array,"%.6f",a);
	UART0_OutString(array);

}

//****************************************************************************************************************************************///


float str_to_float(const char *str) {
    float result = 0.0;
    int sign = 1;
    int integerPart = 0;
    float fractionPart = 0.0;
    int fractionLength = 0;
    int inFraction = 0;

    // Handle sign
    if (*str == '-') {
        sign = -1;
        str++;
    }

    // Parse integer part
    while (*str >= '0' && *str <= '9') {
        integerPart = integerPart * 10 + (*str - '0');
        str++;
    }

    // Parse fraction part
    if (*str == '.') {
        str++;
        inFraction = 1;
        while (*str >= '0' && *str <= '9') {
            fractionPart = fractionPart * 10 + (*str - '0');
            fractionLength++;
            str++;
        }
    }

    // Combine integer and fraction parts
    result = integerPart + fractionPart / pow(10, fractionLength);

    return sign * result;
}

