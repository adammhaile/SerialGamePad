#include <FastLED.h>

uint8_t _pins[] = {3,1,0,2,4,5,6,7,8,9};
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

CRGB * _fastLEDs;

uint16_t numLEDs = 5; //Change this if you have more

#define DATA_PIN 16
#define CLOCK_PIN 15

inline void setupFastLED()
{
	_fastLEDs = (CRGB*)malloc(sizeof(CRGB)*numLEDs);
	memset(_fastLEDs, 0, sizeof(CRGB)*numLEDs);
	//
	FastLED.addLeds<WS2812B, DATA_PIN, GRB>(_fastLEDs, numLEDs);
	//
	FastLED.clear();
	FastLED.show();
}

void setup() {
  Serial.begin(115200);
  for(uint8_t i=0; i<sizeof(_pins); i++)
  {
  	pinMode(_pins[i], INPUT_PULLUP);
  }

  setupFastLED();
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
	static uint16_t packSize = numLEDs * 3;

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
		else if (cmd == CMDTYPE::SET_LEDS)
		{
			count = 0;
			emptyCount = 0;

			if (size == packSize)
			{
				while (count < packSize - 1)
				{
					c = Serial.readBytes(((char*)_fastLEDs) + count, packSize - count);
					if (c == 0)
					{
						emptyCount++;
						if(emptyCount > EMPTYMAX) break;
					}
					else
					{
						emptyCount = 0;
					}

					count += c;
				}
			}

			uint8_t resp = RETURN_CODES::SUCCESS;

			if (count == packSize)
			{
				FastLED.show();
			}
			else
				resp = RETURN_CODES::ERROR_SIZE;

			Serial.write(resp);
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
