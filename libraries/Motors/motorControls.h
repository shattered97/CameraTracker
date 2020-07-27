 /*
 *Direction 
 *0 - Clockwise - Negative
 *1 - Counter-Clockwise - Positive
 */
#ifndef motorControls_h
#define motorControls_h
#include <Arduino.h>

class MotorControls
{

private:
    int ms1;
    int stp;
    int dir;
    int ms2;
    int enable;
    int rst;
    int slp;
    int positionDifference;

    int moveMotor(int moveSteps);
    
public:
    motorControls(int MS1, int STEP, int DIR, int MS2, int ENABLE, int RST, int SLP);
    void pinSetup();
    void fullStep();
    void motorWait();
    void turnMotorOn();
    int changePosition(int currentPosition, int desiredPosition);
};

#endif