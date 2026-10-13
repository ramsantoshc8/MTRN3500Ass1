#include "Galil.h"

static const char* DEFAULT_ADDRESS = "192.168.0.120 -d";

Galil::Galil() {
    Functions = new EmbeddedFunctions();
    constructed = true;

    g = 0;
    setPoint = 0;
    ControlParameters[0] = 0.0;   // Kp
    ControlParameters[1] = 0.0;   // Ki
    ControlParameters[2] = 0.0;   // Kd
    ReadBuffer[0] = '\0';
    bytesRead = 0;

    Functions->GOpen(DEFAULT_ADDRESS, &g);
}

Galil::Galil(EmbeddedFunctions* Funcs, GCStringIn address) {
    Functions = Funcs;
    constructed = false;

    g = 0;
    setPoint = 0;
    ControlParameters[0] = 0.0;   // Kp
    ControlParameters[1] = 0.0;   // Ki
    ControlParameters[2] = 0.0;   // Kd
    ReadBuffer[0] = '\0';
    bytesRead = 0;

    Functions->GOpen(address, &g);
}

Galil::Galil(const Galil& other) {
    Functions = new EmbeddedFunctions();
    constructed = true;

    g = 0;
    setPoint = other.setPoint;
    ControlParameters[0] = other.ControlParameters[0];   // Kp
    ControlParameters[1] = other.ControlParameters[1];   // Ki
    ControlParameters[2] = other.ControlParameters[2];   // Kd
    ReadBuffer[0] = '\0';
    bytesRead = 0;
    
    Functions->GOpen(DEFAULT_ADDRESS, &g);    
}

Galil::~Galil() {
    if (g != 0) {
        Functions->GClose(g);
    }
    if (constructed == true) {
        delete Functions;
    }
}

void Galil::DigitalOutput(uint16_t value) {
    uint8_t mask = 0xFF;
    uint8_t highBytes = mask & (value >> 8); 
    uint8_t lowBytes = mask & value;

    std::string highBytesStr = std::to_string(highBytes);
    std::string lowBytesStr = std::to_string(lowBytes);

    std::string msg = "OP " + lowBytesStr + "," + highBytesStr + ";";
    SendCommand(msg);
}

void Galil::DigitalByteOutput(bool bank, uint8_t value) {   
    std::string msg = "OP ";
    if (bank == true) {
        msg += "," + std::to_string(value) + ";";
    } else {
        msg += std::to_string(value) + ";";
    }
    SendCommand(msg);
}

void Galil::DigitalBitOutput(bool val, uint8_t bit) {
    std::string msg = "";
    if (val) {
        msg += "SB " + std::to_string(bit) + ";";
    } else {
        msg += "CB " + std::to_string(bit) + ";";
    }
    SendCommand(msg);
}

uint16_t Galil::DigitalInput() {
    uint16_t inpData = 0x0000;

    for (int i = 0; i < 16; i++) {
        uint16_t currBitMask = DigitalBitInput(i);
        currBitMask = (currBitMask << i);
        inpData = inpData | currBitMask;
    }  
    return inpData;
}

uint8_t Galil::DigitalByteInput(bool bank) {
    uint8_t inpData = 0x00;
    uint16_t allBits = DigitalInput();

    if (!bank) { //low
        inpData = allBits & 0x00FF;
    } else { //high
        inpData = allBits >> 8;
    }
    return inpData;
}

bool Galil::DigitalBitInput(uint8_t bit) {
    std::string msg = "MG @IN[" + std::to_string(bit) + "];";
    SendCommand(msg);
    return std::stoi(ReadBuffer);
}

bool Galil::CheckSuccessfulWrite() {
    std::string conv = ReadBuffer;
    if (conv.find("?") != std::string::npos) {
        return false;
    } else if (conv.find(":") != std::string::npos) {
        return true;
    } else {
        return false;
    }
}

float Galil::AnalogInput(uint8_t channel) {
    std::string msg = "MG @AN[" + std::to_string(channel) + "];";
    SendCommand(msg);    
    return std::stof(ReadBuffer);
}

void Galil::AnalogOutput(uint8_t channel, double voltage) {
    std::ostringstream volts;
    volts << std::fixed << std::setprecision(2) << voltage;
    std::string msg = "AO " + std::to_string(channel) + "," + volts.str() + ";";
    SendCommand(msg);
}

void Galil::AnalogInputRange(uint8_t channel, uint8_t range) {
    std::string msg = "AQ " + std::to_string(channel) + "," + std::to_string(range) + ";";
    SendCommand(msg);
}

void Galil::WriteEncoder() {
    std::string msg = "WE 0;";
    SendCommand(msg);
}

int Galil::ReadEncoder() {
    std::string msg = "QE 0;";
    SendCommand(msg);
    return std::stoi(ReadBuffer);
}   

void Galil::setSetPoint(int s) {
    setPoint = s;
}

double Galil::getSetPoint() {
    return setPoint;
}

void Galil::setKp(double gain) {
    ControlParameters[0] = gain;
}

double Galil::getKp() {
    return ControlParameters[0];
}

void Galil::setKi(double gain) {
    ControlParameters[1] = gain;
}

double Galil::getKi() {
    return ControlParameters[1];
}

void Galil::setKd(double gain) {
    ControlParameters[2] = gain;
}

double Galil::getKd() {
    return ControlParameters[2];
}

std::ostream& operator<<(std::ostream& output, Galil& galil) {

    char infoOutput[1024];
    char versOutput[1024];
    galil.Functions->GInfo(galil.g, infoOutput, sizeof(infoOutput));
    galil.Functions->GVersion(versOutput, sizeof(versOutput));

    output << infoOutput << "\n\n";
    output << versOutput << "\n\n";

    return output;
}

Galil& Galil::operator=(const Galil & other) {
    if (this == &other) {
        return *this;
    }

    if (this->g != 0) {
        Functions->GClose(this->g);
    }
    
    if (this->constructed == true) {
        delete this->Functions;
    }    

    this->Functions = new EmbeddedFunctions();
    this->constructed = true;
    this->g = 0;

    this->setPoint = other.setPoint;
    this->ControlParameters[0] = other.ControlParameters[0];   // Kp
    this->ControlParameters[1] = other.ControlParameters[1];   // Ki
    this->ControlParameters[2] = other.ControlParameters[2];   // Kd

    Functions->GOpen(DEFAULT_ADDRESS, &this->g);
    return *this;
}

// HELPER FUNCTION
void Galil::SendCommand(const std::string& command) {
    Functions->GCommand(g, command.c_str(), ReadBuffer, sizeof(ReadBuffer), &bytesRead);
}