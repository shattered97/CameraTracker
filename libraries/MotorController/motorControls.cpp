/*
*Direction
*0 - Clockwise - Negative
*1 - Counter-Clockwise - Positive
*/
#include <Arduino.h>
#include "motorControls.h"
#include "MotorControllerConstants.h"

MotorControls::MotorControls()
{
	//Intentionally Left Blank 
}

//Steps necessary to begin operation of the motor
void MotorControls::Initalize()
{
	pinSetup();		//Uses values determined in MotorControllerConstants
	turnMotorOn();	//Removes from low power mode
}

//Sets PINs from MotorControllerConstants as OUTPUT. DAC PINs do not need OUTPUT declaration
void MotorControls::pinSetup()
{
    pinMode(MS1   , OUTPUT);
    pinMode(STEP  , OUTPUT);
    pinMode(DIR	  , OUTPUT);
    pinMode(MS2	  , OUTPUT);
    pinMode(ENABLE, OUTPUT);
    pinMode(RST	  , OUTPUT);
    // slp is set to a DAC and does not need to be set as output
}

//***********************************************************************************************************************************
// Easy Driver - Stepper Motor Driver
// ROB-12779
//***********************************************************************************************************************************
// Commanding the length of a step requires pulling MS1/MS2 to differing states
// (MS1/MS2) : full step (0,0), half step (1,0), 1/4 step (0,1), and 1/8 step (1,1 : default).

// Configuration for taking a full length step
void MotorControls::fullStep()
{
	// MS1 LOW MS2 LOW = full step
    digitalWrite(MS1, LOW);
    digitalWrite(MS2, LOW);
    digitalWrite(STEP, HIGH);  // Actually move the motor
	delay(1);  // Microsecond units Minimum 1uS PIN HIGH
	digitalWrite(STEP, LOW); // We are done moving lets go to the bar
}

// Configuration for taking a half length step
void MotorControls::halfStep()
{
	// MS1 HIGH MS2 LOW = half step
    digitalWrite(MS1, HIGH);
    digitalWrite(MS2, LOW);
    digitalWrite(STEP, HIGH);  // Actually move the motor
	delay(1);  // Microsecond units Minimum 1uS PIN HIGH
	digitalWrite(STEP, LOW); // We are done moving lets go to the bar
}

// Configuration for taking a quarter length step
void MotorControls::quarterStep()
{
	// MS1 LOW MS2 HIGH = quarter step
    digitalWrite(MS1, LOW);
    digitalWrite(MS2, HIGH);
    digitalWrite(STEP, HIGH);  // Actually move the motor
	delay(1);  // Microsecond units Minimum 1uS PIN HIGH
	digitalWrite(STEP, LOW); // We are done moving lets go to the bar
}

// Configuration for taking a quarter length step
void MotorControls::eighthStep()
{
	// MS1 HIGH MS2 HIGH = eighth step
    digitalWrite(MS1, HIGH);
    digitalWrite(MS2, HIGH);
    digitalWrite(STEP, HIGH);  // Actually move the motor
	delay(1);  // Microsecond units Minimum 1uS PIN HIGH
	digitalWrite(STEP, LOW); // We are done moving lets go to the bar
}
//***********************************************************************************************************************************

// Enable controller to command motor operation
void MotorControls::turnMotorOn()
{
    digitalWrite(SLP, HIGH);
    digitalWrite(RST, HIGH);
}

// Pass in number of steps to desired position and the direction
// param [in] moveSteps 		  - Number of steps between current position and desired new position
// param [in] direction 		  - Enum indicates direction (reverse: 0/forward: 1/unknown: 2/total: 3) of travel
// param [in] stepSize (Optional) - Number defining size of step (default: 0)
void MotorControls::moveMotor(int moveSteps, enum direction, int stepSize = 0)
{
	// Prepare the Motor for movement we need direction and number of steps
	digitalWrite(DIR,direction);  // set the direction high is left low is r5ight
		
	// Lets move the specified number of steps	
	for(int i = 0; i < moveSteps; ++i)
	{
		fullStep();				// Simplicity allow only full step option. Half, quarter and eighth also possible
		
		// Decide which way the motor must move to make currentPosition become desiredPosition
		if (direction == 0)
		{
			++currentPosition;	// Update the position count to reflect new location
		}
		elseif (direction == 1)
		{
			--currentPosition;	// Update the position count to reflect new location
		}
		else
		{
			Serial.print("Not a valid direction");
			//TODO: Warning that it did not work
		}
	}
}

// Takes the input of where the focus currently is and where it must move to in repect to the range of travel. Units are NOT degrees
// 1 step = 33.8°
// param [in] currentPosition  - Numeric value indicating where the focus of the motor is in a 360° range of travel. NOT unit of degrees
// param [in] desiredPosition  - Numeric value indicating where the focus needs to be in a 360° range of travel. NOT unit of degrees
// param [out] currentPosition - Numerio value indication where the focus of the motor is in a 360° range of travel. NOT unit of degrees
int MotorControls::calculateNewPosition(int currentPosition, int desiredPosition)
{
    while (currentPosition != desiredPosition)
    {
        positionDifference = desiredPosition - currentPosition;
        switch (positionDifference)
        {
        case 0:
            //Wait for a new desiredPosition
            break;
        default:
            // Move motor an allowed amount and receive new current position
            currentPosition = moveMotor(positionDifference, stepper::direction,0);
            break;
        }
    }
    return currentPosition;
}