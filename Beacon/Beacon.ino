#include <RH_RF69.h>
#include <SPI.h>

/*
    Name:       Beacon.ino
    Created:    3/28/2020 2:01:55 PM
    Author:     shattered97
*/

const bool DEBUG_MODE = true;

#define LED_PIN       13       //used to show when transmitting
#define RF69_FREQ     915.0    //Freq of board
#define RFM69_CS      8        //pins for Adafruit Feather M0 board
#define RFM69_INT     3        //pins for Adafruit Feather M0 board   
#define RFM69_RST     4        //pins for Adafruit Feather M0 board
#define LED           13       //pins for Adafruit Feather M0 board

// Singleton instance of the radio driver
RH_RF69 rf69(RFM69_CS, RFM69_INT);

uint32_t sampleRate_ms = 10;     //Transmit about every 21ms
int16_t packetnum      = 0;      // packet counter, we increment per xmission
bool state             = false;  //just for an example

// The setup() function runs once each time the micro-controller starts
void setup()
{
  resetRFM69();
  
  if(DEBUG_MODE == true)
  {
    //Setup Debug Mode operations here
    Serial.begin(115200);  //used to display data to serial port to aid in debugging
    debug("Debug Mode is on, all output will be displayed on the serial port");
  }
    
  pinMode(LED_PIN, OUTPUT);      //configures the LED pin to show when transmitting
  pinMode(RFM69_RST, OUTPUT);    //configures the RMF69 reset pin
  digitalWrite(RFM69_RST, LOW);  //sets pin low (High will reset) 
  
  if(rf69.init())
  {
    debug("Succefully initialized the RFM69 module!");
    if(!rf69.setFrequency(RF69_FREQ))
    {
      debug("Failed to set the RFM69 module frequency");
      while(1);
    }

    rf69.setTxPower(20, true);  // range from 14-20 for power, 2nd arg must be true for 69HCW

    // The encryption key has to be the same as the one in the server
    uint8_t key[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                      0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
    rf69.setEncryptionKey(key);

    tcConfigure(sampleRate_ms); //configure the timer to run at <sampleRate>Hertz
    delay(1000);                //give time for everything to settle before we go on
    tcStartCounter();           //starts the timer
  }
  else
  {
    debug("Failed to initialize the RFM69 module!");
    while(1);
  }
}

// Add the main program code into the continuous loop() function
void loop()
{

}

//this function gets called by the interrupt at <sampleRate>Hertz
void TC5_Handler (void)
 {

    
    String dataToSend = String(millis()) + "ms #";  //Time since started
    
    const int buffSize = dataToSend.length() + 1 ;
    char radioPacket[50];
    dataToSend.toCharArray(radioPacket, buffSize);
    itoa(packetnum++, radioPacket + 13, 10); 

    debug(radioPacket);
    debug(String(strlen(radioPacket)));
    
    rf69.send((uint8_t *)radioPacket, strlen(radioPacket));
    //rf69.waitPacketSent();

    //No reply - beacon just transmits

    if(state == true)
    {
      digitalWrite(LED_PIN, HIGH);
    }
    else
    {
      digitalWrite(LED_PIN, LOW);
    }
    state = !state;
    
    // END OF YOUR CODE
    TC5->COUNT16.INTFLAG.bit.MC0 = 1; //Writing a 1 to INTFLAG.bit.MC0 clears the interrupt so that it will run again
}

void debug(String txtToDisplay)
{
  if(DEBUG_MODE)
  {
    Serial.println(txtToDisplay);
  }
  
}

//reset the RMF69 board
void resetRFM69()
{
  //Set reset pin High to reset
  digitalWrite(RFM69_RST, HIGH);
  delay(10);

  //Set reset pin Low to return to normal operation
  digitalWrite(RFM69_RST, LOW);
  delay(10);
}


//Timer setup code below 
void tcConfigure(int sampleRate)
{
  // Enable GCLK for TCC2 and TC5 (timer counter input clock)
  //Name:  CLKCTRL
  //Offset:  0x2
  //Reset:  0x0000
  //Property:  Write-Protected
  
  //Bit  14   – CLKEN Clock Enable
  //Bits 11:8 – GEN[3:0] Generic Clock Generator (0x0:GCLKGEN0:Generic clock generator 0)
  //Bits 5:0  – ID[5:0] Generic Clock Selection ID (0x1C GCLK_TC4, GCLK_TC5 TC4,TC5)
   GCLK->CLKCTRL.reg = (uint16_t) (GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN_GCLK0 | GCLK_CLKCTRL_ID(GCM_TC4_TC5)) ;
   
   //Name:  STATUS
   //Offset:  0x1
   //Reset:  0x00
   //Property:  -
  
   while (GCLK->STATUS.bit.SYNCBUSY);

   tcReset(); //reset TC5
   
   //Select 8-, 16- or 32-bit counter mode via the TC Mode bit group in the Control A register (CTRLA.MODE). The
   //default mode is 16-bit
   // Set Timer counter Mode to 16 bits
   TC5->COUNT16.CTRLA.reg |= TC_CTRLA_MODE_COUNT16;
   
   //Select one wave generation operation in the Waveform Generation Operation bit group in the Control A
   //register (CTRLA.WAVEGEN)
   // Set TC5 mode as match frequency
   TC5->COUNT16.CTRLA.reg |= TC_CTRLA_WAVEGEN_MFRQ;
   
   //If desired, the GCLK_TCx clock can be prescaled via the Prescaler bit group in the Control A register
   //(CTRLA.PRESCALER).
   //If the prescaler is used, select a prescaler synchronization operation via the Prescaler and Counter
   //Synchronization bit group in the Control A register (CTRLA.PRESYNC)
   //set prescaler and enable TC5
   TC5->COUNT16.CTRLA.reg |= TC_CTRLA_PRESCALER_DIV64 | TC_CTRLA_ENABLE; //you can use different prescaler divisons here like TC_CTRLA_PRESCALER_DIV1 to get different ranges of frequencies
   
   //set TC5 timer counter based off of the system clock and the user defined sample rate or waveform
   TC5->COUNT16.CC[0].reg = (uint16_t) (SystemCoreClock / sampleRate - 1);
   
   while (tcIsSyncing());
   
    // Configure interrupt request
    NVIC_DisableIRQ(TC5_IRQn);
    NVIC_ClearPendingIRQ(TC5_IRQn);
    NVIC_SetPriority(TC5_IRQn, 0);
    NVIC_EnableIRQ(TC5_IRQn);
    
    // Enable the TC5 interrupt request
    TC5->COUNT16.INTENSET.bit.MC0 = 1;
    
    while (tcIsSyncing()); //wait until TC5 is done syncing
  
}

//Reset TC5
void tcReset()
{
  //Name:  WPCLR
  //Offset:  0x00
  //Reset:  0x00800000
  //Property:  –
    //TC   3   4   5   6   7
  //Bits 11, 12, 13, 14, 15 – TC
  //Writing a zero to these bits has no effect.
  //Writing a one to these bits will clear the Write-Protect bit for the corresponding peripherals.
  TC5->COUNT16.CTRLA.reg = TC_CTRLA_SWRST;
  while (tcIsSyncing());
  while (TC5->COUNT16.CTRLA.bit.SWRST);
}

//Function that is used to check if TC5 is done syncing
//returns true when it is done syncing
bool tcIsSyncing()
{
  //Bit 7 – SYNCBUSY Synchronization Busy Status
  //This bit is cleared when the synchronization of registers between the clock domains is complete.
  //This bit is set when the synchronization of registers between clock domains is started
  return TC5->COUNT16.STATUS.reg & TC_STATUS_SYNCBUSY;
}

//This function enables TC5 and waits for it to be ready
void tcStartCounter()
{
  TC5->COUNT16.CTRLA.reg |= TC_CTRLA_ENABLE; //set the CTRLA register
  while (tcIsSyncing()); //wait until snyc'd
}

//disable TC5
void tcDisable()
{
  TC5->COUNT16.CTRLA.reg &= ~TC_CTRLA_ENABLE;
  while (tcIsSyncing());
}
