#ifndef MotorControllerConstanst_h
#define MotorControllerConstanst_h

/**************************************************************************************************/
// RMF69 Adafruit board

// MS1/MS2 : These digital inputs control the microstepping mode. Possible settings are 
// (MS1/MS2) : full step (0,0), half step (1,0), 1/4 step (0,1), and 1/8 step (1,1 : default).
#define RMF69_MS1 = 12 //PIN 12  //Determine Step Size Valid Values Pin H or L
#define RMF69_MS2 = 9  //PIN 9

#define RMF69_STEP   = 11 //PIN 11  //Commands movement by amount determined by MS1/MS2 Valid Values Pin H or L

// RST/SLP/ENABLE : These are enablement parameters to allow operation
#define RMF69_ENABLE = 6  //PIN 6   //Allow step when set H Valid Values Pin H or L

// RST: When set LOW, all STEP commands are ignored and all FET functionality is turned off. 
#define RMF69_RST = 5  //PIN 5  //pulled HIGH to enable STEP control Valid Values Pin H or L

// SLP: Must be pulled HIGH to exit low power mode
#define	RMF69_SLP = 19 //PIN A5 //Do not use pinmode for DAC // Valid Values Pin H or L
/*******************************************************************************************************/

//Live Code
#define MS1    = RMF69_MS1
#define	MS2    = RMF69_MS2
#define STEP   = RMF69_STEP
#define ENABLE = RMF69_ENABLE
#define RST	   = RMF69_RST
#define SLP	   = RMF69_SLP

namespace stepper
{
	enum direction
	{
		reverse = 0,
		forward = 1,
		unknown = 2,
		total   = 3
		
	};
	enum stepSize
	{
		full 	= 0,
		half 	= 1,
		quarter = 2,
		eighth  = 3,
		total 	= 6
	};
}

#endif

