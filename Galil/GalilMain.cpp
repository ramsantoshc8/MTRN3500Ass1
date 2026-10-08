#include "Galil.h"


int main(void) {

	EmbeddedFunctions Functions;
	Galil myGalil = Galil(&Functions, "192.168.0.120 -d");

	myGalil.DigitalOutput(0xFFFF);
	Sleep(2000);
	myGalil.DigitalOutput(0x0000);

	
	
	return 0;
}
/*

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



	a.setKp(10);
	a.setKi(9);
	a.setKd(8);

	Galil b(a);
	Galil c(b);



	b.DigitalOutput(0xFF00);
	b.DigitalOutput(0x00FF);

	std::cout << "object a " << std::endl;
	std::cout << a.getKp() << std::endl;
	std::cout << a.getKi() << std::endl;
	std::cout << a.getKd() << std::endl;

	std::cout << "object b " << std::endl;
	std::cout << b.getKp() << std::endl;
	std::cout << b.getKi() << std::endl;
	std::cout << b.getKd() << std::endl;

	std::cout << "object c " << std::endl;
	std::cout << c.getKp() << std::endl;
	std::cout << c.getKi() << std::endl;
	std::cout << c.getKd() << std::endl;


*/