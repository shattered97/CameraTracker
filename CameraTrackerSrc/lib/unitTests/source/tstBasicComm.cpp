#include <unity.h>
#include <Arduino.h>

void setUp(void)
{
    //Setup stuff here
}

void tearDown(void)
{
    //Tear down stuff here
}

void testBasicBuiltinPin(void)
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



