#include "Galil.h"

#include <cmath>

#using <System.dll>  
using namespace System;

/**
* Hints for completing this task
*  - Use double std::sin(double) to find the sine value
*  - Use long std::lround(double) to round the value to nearest integer
*  - Use Threading::Thread::Sleep(100) to sleep for 100 ms (CLR Syntax)
*  - Two functions are provided: deg2rad and rad2deg
*    it is up to you whether you find it useful
*/

// convert degrees to radian
template <typename T>
constexpr T deg2rad(T deg) {
	return deg * M_PI / 180.0;
}

// convert radian to degrees
template <typename T>
constexpr T rad2deg(T rad) {
	return rad * 180.0 / M_PI;
}

int main(void) {
	EmbeddedFunctions func;
	Galil g(&func, "192.168.0.120 -d");

	g.AnalogInputRange(0, 4);
	double Vin = g.AnalogInput(0);
	
	double freq = Vin / 2.5 + 1;
	long x = 0;
	
	for (int i = 0; i < 16; i++) {
		double xDecimel = 8 * std::sin((2.0 * 3.14 * freq) / 16.0 * i);
		x = std::lround(xDecimel);

		if (x > 0) {
			g.DigitalOutput(0xFF00);
			std::cout << "works > 0 " << std::endl;
		}
		else if (x < 0) {
			g.DigitalOutput(0xFF00);
			std::cout << "works > 1 " << std::endl;
		}
		else {
			g.DigitalOutput(0x0000);
			std::cout << "works = none " << std::endl;
		}
		Threading::Thread::Sleep(100);
	}

	

	

	


	std::cout << x << std::endl;




	return 0;
}
