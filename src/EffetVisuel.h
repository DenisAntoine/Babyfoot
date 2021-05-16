#ifndef DEF_EFFETVIS
#define DEF_EFFETVIS
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
enum OpState {
OFF = 0,
WIPE =1,
FLASH =2,
THEAC = 3,
STROB = 4,
RAIN =5,
};

class EffetVisuel
{
    public:
    	
		EffetVisuel(Adafruit_NeoPixel* strip); //
		
		//new
		void begin();
		void tick();
		void colorWipe(uint32_t c, uint16_t cycle);
		void theaterChase(uint32_t c, uint16_t cycle);
		void strobe(uint32_t c, uint16_t cycle);
		void flash(uint32_t c, uint16_t cycle);
		void off();
		void rainbow(uint16_t cycle);
		OpState getStripState();
		void setStripState(OpState state);
		uint16_t getStripCycle();
		uint32_t wheel(byte WheelPos);
		void goal(uint32_t color);
		void win(uint32_t color);
		void cheer();
		void setPixelInterval(unsigned long pixelsInterval);
		void setColor(uint32_t color);

    private:
		Adafruit_NeoPixel* _strip;
		uint16_t _currentPixel = 0;
		uint16_t _NumPixel = 0;
		OpState _stripState;
		uint16_t _NbCycle = 0;
		unsigned long _pixelsInterval=100;
		unsigned long _previousMillis=0;
		uint32_t _color;
		
};

#endif
