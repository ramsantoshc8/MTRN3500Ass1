#include "EmbeddedFunctions.h"

EmbeddedFunctions::EmbeddedFunctions() {
	GalilHndl = nullptr;
	GalilStream = nullptr;
}

EmbeddedFunctions::~EmbeddedFunctions() {
	try { GClose(); }
	catch (Exception^) {}
}

void EmbeddedFunctions::GOpen(String^ address, const int port) {
	if (GalilHndl != nullptr) {
		GClose();
	}

	IPAddress = address->Replace(" -d", "")->Trim();
	Port = port;

	try {
		GalilHndl = gcnew TcpClient(IPAddress, Port);
		GalilHndl->SendBufferSize = 64;
		GalilHndl->ReceiveBufferSize = 2048;
		GalilHndl->SendTimeout = 500;
		GalilHndl->ReceiveTimeout = 500;
		GalilHndl->NoDelay = true;
		GalilStream = GalilHndl->GetStream();
		SendData = gcnew array<uint8_t>(64);
		RecvData = gcnew array<uint8_t>(2048);

		Console::WriteLine("Connected to " + IPAddress);
	}
	catch (Exception^ e) {
		Console::WriteLine("GOpen failed: " + e->Message);
		GalilHndl = nullptr;
		GalilStream = nullptr;
		throw;
	}
}

void EmbeddedFunctions::GClose() {
	if (GalilHndl == nullptr) {
		return;
	}
	if (GalilStream != nullptr) {
		GalilStream->Close();
	}
	GalilHndl->Close();

	GalilStream = nullptr;
	GalilHndl = nullptr;
}

String^ EmbeddedFunctions::GCommand(String^ command) { 
	if (GalilStream == nullptr) {
		throw gcnew Exception("No exisiting stream");
	}

	if (!command->EndsWith(";")) {
		command += ";";
	}
	SendData = Encoding::ASCII->GetBytes(command);
	GalilStream->Write(SendData, 0, SendData->Length);
	Threading::Thread::Sleep(10);

	GalilStream->Read(RecvData, 0, RecvData->Length);
	Threading::Thread::Sleep(25);
	Response = Encoding::ASCII->GetString(RecvData);
	return Response;
}

