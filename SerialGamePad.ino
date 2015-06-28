uint8_t _pins[] = {0,1,2,3,4,5,6,7,8,9};
namespace CMDTYPE
{
	enum CMDTYPE
	{
        INIT = 0,
		GET_BTNS = 1,
		SET_LEDS = 2,
        SET_MODE = 4,
	};
}

namespace RETURN_CODES
{
	enum RETURN_CODES
	{
		SUCCESS = 255,
		ERROR = 0,
		ERROR_SIZE = 1,
		ERROR_UNSUPPORTED = 2,
		ERROR_BAD_CMD = 4,
	};
}

void setup() {
  Serial.begin(115200);
  for(uint8_t i=0; i<sizeof(_pins); i++)
  {
  	pinMode(_pins[i], INPUT_PULLUP);
  }
}

uint16_t readState(){
    uint16_t result = 0;
    for(uint8_t i=0; i<sizeof(_pins); i++)
    {
        if(!digitalRead(_pins[i]))
        {
            bitSet(result, i);
        }
    }
    return result;
}

#define EMPTYMAX 100
inline void doSerial()
{
	static char cmd = 0;
	static uint16_t size = 0;
	static uint16_t count = 0;
	static uint8_t emptyCount = 0;
	static size_t c = 0;

	if (Serial.available())
	{
		cmd = Serial.read();
		size = 0;
		Serial.readBytes((char*)&size, 2);

        if(cmd == CMDTYPE::INIT)
        {
            Serial.write(RETURN_CODES::SUCCESS);
        }
        else if (cmd == CMDTYPE::GET_BTNS)
        {
            uint16_t result = readState();
            Serial.write(RETURN_CODES::SUCCESS);
			Serial.write(result & 0xFF);
            Serial.write(result >> 8);
        }
		else
		{
			Serial.write(RETURN_CODES::ERROR_BAD_CMD);
		}

		Serial.flush();
	}
}


void loop() {
  doSerial();
}
