#include <Arduino.h>
#include <unity.h>

String STR_TO_TEST;

void setUp(void)
{
    //setup stuff here
    STR_TO_TEST = "Hello World";
}

// void tearDown(void)
// {
//     //Clean stuff up here
// }

void testLedBuiltinPinNumber(void)
{
    TEST_ASSERT_EQUAL(13, LED_BUILTIN);
}

void testLedStateHigh(void)
{
    digitalWrite(LED_BUILTIN, HIGH);
    TEST_ASSERT_EQUAL(HIGH, digitalRead(LED_BUILTIN));
}

void testLedStateLow(void)
{
    digitalWrite(LED_BUILTIN, LOW);
    TEST_ASSERT_EQUAL(LOW, digitalRead(LED_BUILTIN));
}

void setup()
{
    //Wait for > 2 seconds
    //if board does not support reset via Serial.DTR/RTS
    delay(1000 * 10);

    UNITY_BEGIN();
    RUN_TEST(testLedBuiltinPinNumber);

    pinMode(LED_BUILTIN, OUTPUT);
 
    
}

u_int8_t i = 0;
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