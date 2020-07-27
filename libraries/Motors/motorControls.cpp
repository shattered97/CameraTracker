/*
*Direction
*0 - Clockwise - Negative
*1 - Counter-Clockwise - Positive
*/
#include <Arduino.h>
#include "motorControl.h"

MotorControls::motorControls(int MS1, int STEP, int DIR, int MS2, int ENABLE, int RST, int SLP)
{
    ms1 = MS1;
    stp = STEP;
    dir = DIR;
    ms2 = MS2;
    enable = ENABLE;
    rst = RST;
    slp = SLP;
}
void MotorControls::pinSetup()
{
    pinMode(ms1, OUTPUT);
    pinMode(stp, OUTPUT);
    pinMode(dir, OUTPUT);
    pinMode(ms2, OUTPUT);
    pinMode(enable, OUTPUT);
    pinMode(rst, OUTPUT);
    // slp is set to a DAC and does not need to be set as output
}
void MotorControls::fullStep()
{
    digitalWrite(ms1, LOW);
    digitalWrite(ms2, LOW);
    digitalWrite(stp, HIGH);
}
void MotorControls::motorWait()
{
    digitalWrite(stp, LOW);
}
void MotorControls::turnMotorOn()
{
    digitalWrite(slp, HIGH);
    digitalWrite(rst, HIGH);
}
int MotorControls::moveMotor(int moveSteps)
{
    int steps = 1;
    int delayTime = 50; // milliseconds

    if (moveSteps > 0)
    {
        digitalWrite(DIR, HIGH);
        currentPosition = currentPosition + steps;
    }
    else
    {
        digitalWrite(DIR, LOW);
        currentPosition = currentPosition - steps;
    }
    fullStep();
    motorWait();
    delay(delayTime);

    return currentPosition;
}
int MotorControls::changePosition(int currentPosition, int desiredPosition)
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
            currentPosition = moveMotor(positionDifference);
            break;
        }
    }
    return currentPosition;
}