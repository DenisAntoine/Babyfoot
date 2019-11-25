#ifndef DEF_EFFETVIS
#define DEF_EFFETVIS

#include <Adafruit_NeoPixel.h>

class EffetVisuel
{
    public:
    	
		EffetVisuel(Adafruit_NeoPixel* strip); //
		void flash(uint32_t color, int wait);
		void cheer(int team);
		void goal(int team);
		void win(int team);
	
	
    private:
	
		// pointeur pour le strip
		Adafruit_NeoPixel* _strip;
};

#endif
