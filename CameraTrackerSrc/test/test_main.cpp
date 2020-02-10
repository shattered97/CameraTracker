#include <Arduino.h>
#include <unity.h>

// void setUp(void)
// {
//     //setup stuff here
// }

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
    delay(4000);

    UNITY_BEGIN();
    RUN_TEST(testLedBuiltinPinNumber);

    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(115200);
    Serial.print("Hello");
}

u_int8_t i = 0;
u_int8_t max_blinks = 5;

void loop()
{
    if(i < max_blinks)
    {
        RUN_TEST(testLedStateHigh);
        delay(500);
        RUN_TEST(testLedStateLow);
        delay(500);
        i++;
    }

    UNITY_END();   
  
}