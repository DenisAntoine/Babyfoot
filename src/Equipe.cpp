#include "Equipe.h"

Equipe::Equipe(int goalPin)
{
	m_goalpin = goalPin;
	m_score=0; //score initial à 0
}

//acesseur 
int Equipe::getScore() const
{
    return m_score;
}

int Equipe::getPin() const
{
	return m_goalpin;
}

//
void Equipe::increaseScore()
{
   m_score++;
   if (m_score > MAXSCORE){
	   m_score = 0; //
   }
}


void Equipe::decreaseScore()
{
   m_score--;
   if (m_score < 0){
	   m_score = MAXSCORE; //
   }
}

void Equipe::resetScore()
{
   m_score=0;
}

void Equipe::cheer()
{
	Serial.println("cheer");

	_EffetSon -> play(m_folderCheer); // passe le repertoire à jouer
	_EffetVis -> cheer(m_goalpin); // lance effet visuel le goal pin determine la couleur et le camp
}
void Equipe::goal()
{
	_EffetSon -> play(m_folderGoal); // passe le repertoire à jouer
	_EffetVis-> goal(m_goalpin); // lance effet visuel le goal pin determine la couleur et le camp
	this -> increaseScore();
}
void Equipe::win()
{
	_EffetSon -> play(m_folderWin); // passe le repertoire à jouer
	_EffetVis -> win(m_goalpin); // lance effet visuel le goal pin determine la couleur et le camp
	this -> increaseScore(); // comptabilise le but victorieux
}

void Equipe::setPin(int pin)
{
	m_goalpin = pin;
}

void Equipe::setFolderCheer(int folder)
{
	m_folderCheer = folder;
}

void Equipe::setFolderGoal(int folder)
{
	m_folderGoal = folder;
}

void Equipe::setFolderWin(int folder)
{
	m_folderWin = folder;
}
void Equipe::setEffetSonore(EffetSonore *son)
{
	_EffetSon = son;
}

void Equipe::setEffetVisuel(EffetVisuel *vis)
{
	_EffetVis = vis;
}
