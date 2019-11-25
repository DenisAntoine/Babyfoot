#ifndef DEF_EQUIPE
#define DEF_EQUIPE

#include "EffetSonore.h"
#include "EffetVisuel.h"

#define MAXSCORE 11

class Equipe
{
    public:
    	
		Equipe(int goalPin); //Constructeur
		
		int getScore() const;
		int getPin() const;
		void increaseScore();
		void decreaseScore();
		void resetScore();
		void cheer(); // encourage l equipe
		void goal(); // but de l equipe
		void win(); // victoire de l equipe
		
		void setPin(int pin);
		void setFolderCheer(int folder);
		void setFolderGoal(int folder);
		void setFolderWin(int folder);
		void setEffetSonore(EffetSonore *son);
		void setEffetVisuel(EffetVisuel *vis);

    private:
		int m_score;
		int	m_goalpin; //pin du dectecteur de but
		
		int m_folderCheer; //folder encouragements
		int m_folderGoal; //folder but
		int m_folderWin; //folder victoire
		
		
		
		EffetSonore *_EffetSon;
		EffetVisuel *_EffetVis;
};

#endif
