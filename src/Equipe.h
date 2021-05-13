#ifndef DEF_EQUIPE
#define DEF_EQUIPE
#include <Adafruit_ADS1X15.h>
#include "EffetSonore.h"
#include "EffetVisuel.h"

#define MAXSCORE 11

class Equipe
{
    public:
    	
		Equipe(int goalPin); //Constructeur
		Equipe(int goalPin, uint32_t color, Adafruit_ADS1115* _ads, EffetSonore* _son, EffetVisuel* _visu);
		int getScore() const;
		int getPin() const;
		void increaseScore();
		void decreaseScore();
		void resetScore();
		void cheer(); // encourage l equipe
		void goal(); // but de l equipe
		void win(); // victoire de l equipe
		bool testgoal(uint16_t seuildetect);// teste sur ADS
		void setPin(int pin);
		void setFolderCheer(int folder);
		void setFolderGoal(int folder);
		void setFolderWin(int folder);
		void setEffetSonore(EffetSonore *son);
		void setEffetVisuel(EffetVisuel *vis);
		unsigned long getLastGoal();
		unsigned long getLastCheer();
		unsigned long getNextCheer(unsigned long tmin, unsigned long tmax); //renvoi le timestamp du prochain encouragement tmin/tmax  = periodes mini /maxi
    private:
		int m_score;
		int m_goalpin; //pin du dectecteur de but
		
		int m_folderCheer; //folder encouragements
		int m_folderGoal; //folder but
		int m_folderWin; //folder victoire
		uint32_t m_color;//couleur associee
		unsigned long m_lastGoal;
		unsigned long m_lastCheer;
		Adafruit_ADS1115 *_ads1115;
		EffetSonore *_EffetSon;
		EffetVisuel *_EffetVis;
};

#endif
