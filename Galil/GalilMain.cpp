#include "Galil.h"

int main(void) {

	//Galil a;

	EmbeddedFunctions funcs(true); 
	Galil myGalil(&funcs, "192.168.0.120 -d");
	
	myGalil.DigitalOutput(0xFFFF);
	//myGalil.DigitalByteOutput(0, 0x0F);
	myGalil.DigitalBitOutput(false, 1);

	//myGalil.CheckSuccessfulWrite();


	std::cout << std::endl;

	//myGalil.DigitalInput();
	//myGalil.DigitalByteInput(0);
	//myGalil.DigitalBitInput(5);

	myGalil.AnalogInput(0);
	myGalil.AnalogOutput(0, 10.00);
	myGalil.AnalogInputRange(0, 1);
	//myGalil.WriteEncoder();
	std::cout << myGalil.ReadEncoder() << std::endl;

	myGalil.PositionControl(1, 0);
	
	return 0;
}