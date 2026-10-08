#include "EmbeddedFunctions.h"

int main(void) {
	EmbeddedFunctions^ a = gcnew EmbeddedFunctions();
	Console::WriteLine("Constructed");

	a->GCommand("SB 2");

	a->GClose();
	Console::WriteLine("GClose done");

	delete a;
	Console::WriteLine("Destructed");

	Console::ReadKey();
	return 0;
}