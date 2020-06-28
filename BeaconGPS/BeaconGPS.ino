// Visual Micro is in vMicro>General>Tutorial Mode
//
/*
Name:       BeaconGPS.ino
Created:	6/26/2020 6:11:12 PM
Author:     shattered97
*/

#include <RH_RF69.h>

#define LED_PIN       13       //used to show when transmitting
#define RF69_FREQ     915.0    //Freq of board
#define RFM69_CS      8        //pins for Adafruit Feather M0 board
#define RFM69_INT     3        //pins for Adafruit Feather M0 board
#define RFM69_RST     4        //pins for Adafruit Feather M0 board
#define LED           13       //pins for Adafruit Feather M0 board

#define GPSSerial     Serial1  //Hardware Serial Adafruit Feather M0 board

// Singleton instance of the radio driver
RH_RF69 rf69(RFM69_CS, RFM69_INT);

unsigned long startMillis;
char buffer[1024];



// The setup() function runs once each time the micro-controller starts
void setup()
{
	setUpSerial();
	setupRFM69();  //configure Adafruit feather board send data
	startMillis = millis();
}

// Add the main program code into the continuous loop() function
void loop()
{
	
	if(GPSSerial.available())
	{
		char c = GPSSerial.read();
		if(c == '\n')
		{
			Serial.println();
		}
		else
		{
			Serial.print(c);
		}
	}

 
}



void debug(String txtToDisplay)
{

		Serial.println(txtToDisplay);
	
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

void setupRFM69()
{
	resetRFM69();
	
	debug("Debug Mode is on, all output will be displayed on the serial port");

	
	pinMode(LED_PIN, OUTPUT);      //configures the LED pin to show when transmitting
	pinMode(RFM69_RST, OUTPUT);    //configures the RMF69 reset pin
	digitalWrite(RFM69_RST, LOW);  //sets pin low (High will reset)
	
	if(rf69.init())
	{
		debug("Successfully initialized the RFM69 module!");
		if(!rf69.setFrequency(RF69_FREQ))
		{
			debug("Failed to set the RFM69 module frequency");
			while(1);
		}

		rf69.setTxPower(20);  // range from 14-20 for power
		
		// The encryption key has to be the same as the one in the server
		uint8_t key[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
		0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
		rf69.setEncryptionKey(key);
	}
	else
	{
		debug("Failed to initialize the RFM69 module!");
		while(1);
	}
	
}

void setUpSerial()
{
	while(!Serial);
    Serial.begin(115200);   //used to display data to serial port to aid in debugging
	GPSSerial.begin(9600);  //used to receive GPS information from the GPS board  
}
	                       

