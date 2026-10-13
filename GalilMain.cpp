#include "Galil.h"


int main(void) {

	//EmbeddedFunctions Functions;
	//Galil a = Galil(&Functions, "192.168.0.120 -d");

	EmbeddedFunctions Functions(true);
	Galil a(&Functions, "192.168.0.120 -d");

	//a.DigitalOutput(0xFFFF);

	Galil b(a);

	b.DigitalOutput(0xFFFF);

	
	
	return 0;
}