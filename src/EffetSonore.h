#ifndef DEF_EFFETSON
#define DEF_EFFETSON
#include <ESP8266TrueRandom.h>
#include <DFRobotDFPlayerMini.h>

class EffetSonore
{
    public:
    	
		EffetSonore(DFRobotDFPlayerMini* dfp); //
		void play(int folder);
	
	
    private:
			
		// pointeur vers linstance DFPLAYER
		DFRobotDFPlayerMini* _DFPlayer;
};

#endif
