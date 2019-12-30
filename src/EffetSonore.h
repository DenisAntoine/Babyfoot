#ifndef DEF_EFFETSON
#define DEF_EFFETSON
#include <ESP8266TrueRandom.h>
#include <DFRobotDFPlayerMini.h>

class EffetSonore
{
    public:
    	
		EffetSonore(DFRobotDFPlayerMini* dfp); //
		void play(int folder);
		unsigned long getLastSound();
	
	
    private:
			
		// pointeur vers linstance DFPLAYER
		DFRobotDFPlayerMini* _DFPlayer;
		unsigned long m_lastSound;
};

#endif
