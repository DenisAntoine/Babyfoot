#ifndef DEF_EFFETVIS
#define DEF_EFFETVIS

#include <Adafruit_NeoPixel.h>

class EffetVisuel
{
    public:
    	
		EffetVisuel(Adafruit_NeoPixel* strip); //
		void flash(uint32_t color, uint16_t pstart, uint16_t pend);
		void cheer(uint32_t color);
		void goal(uint32_t color);
		void win(uint32_t color);
	    void theaterChase(uint32_t color, int wait);
		void rainbow(int wait);
		void theaterChaseRainbow(int wait);
		void whiteOverRainbow(int whiteSpeed, int whiteLength);
		void pulseWhite(uint8_t wait);
		void rainbowFade2White(int wait, int rainbowLoops, int whiteLoops);
		void strobe(uint32_t color, int strobecount, int flashdelay, uint16_t pstart, uint16_t pend);
		

    private:
	
		// pointeur pour le strip
		Adafruit_NeoPixel* _strip;
		
};

#endif
