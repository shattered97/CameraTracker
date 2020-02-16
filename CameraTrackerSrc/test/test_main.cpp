#include <unity.h>
#include "../lib/unitTests/source/tstBasicComm.cpp"

void setup()
{
    //Wait for > 2 seconds
    //if board does not support reset via Serial.DTR/RTS
    delay(1000 * 5);

    UNITY_BEGIN();
    RUN_TEST(testBasicBuiltinPin);

    pinMode(LED_BUILTIN, OUTPUT);   
     
}

u_int8_t max_blinks = 5;

void loop()
{
    for(int i = 0; i < max_blinks; ++i)
    {
        RUN_TEST(testLedStateHigh);
        delay(500);
        RUN_TEST(testLedStateLow);
        delay(500);
    }  
    UNITY_END();   
}