/*
 *Direction 
 *0 - Clockwise - Negative
 *1 - Counter-Clockwise - Positive
 */
#ifndef motorControls_h
#define motorControls_h
#include <Arduino.h>
#include "MotorControllerConstants.h"

class MotorControls
{

private:
	int positionDifference;

	// Step length functions. Eighth step default
	void fullStep();
	void halfStep();
	void quarterStep();
	void eighthStep();
	
	// Pass in number of steps to desired position and the direction
	// param [in] moveSteps 		  - Number of steps between current position and desired new position
	// param [in] direction 		  - Enum indicates direction (reverse: 0/forward: 1/unknown: 2/total: 3) of travel
	// param [in] stepSize (Optional) - Number defining size of step (default: 0)
    void moveMotor(int moveSteps, enum direction, enum stepSize = 0);
	void pinSetup(); //Setup the pins per the MotorControllerConstants.h file
    void turnMotorOn();
	
public:
	// Class declaration
    motorControls();
	
	// Remove low power condition and allow operation
    initalize();
	
	// Takes the input of where the focus currently is and where it must move to in repect to the range of travel. Units are NOT degrees
	// 1 step = 33.8°
	// param [in] currentPosition  - Numeric value indicating where the focus of the motor is in a 360° range of travel. NOT unit of degrees
	// param [in] desiredPosition  - Numeric value indicating where the focus needs to be in a 360° range of travel. NOT unit of degrees
	// param [out] currentPosition - Numerio value indication where the focus of the motor is in a 360° range of travel. NOT unit of degrees
	int calculateNewPosition(int currentPosition, int desiredPosition);
	
};

#endif