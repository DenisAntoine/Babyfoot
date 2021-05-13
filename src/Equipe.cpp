#include "Equipe.h"

Equipe::Equipe(int goalPin)
{
	m_goalpin = goalPin;
	m_score=0; //score initial à 0
}

Equipe::Equipe(int goalPin, uint32_t color, Adafruit_ADS1115* _ads, EffetSonore* _son)
{
	m_goalpin = goalPin;
	m_color = color;
	_ads1115 = _ads;
	_EffetSon = _son;
	m_score=0; //score initial à 0
	m_lastGoal = millis();
	m_lastCheer = millis();
}

bool Equipe::testgoal(uint16_t seuildetect){
	bool goal = false;
	//Serial.println(_ads1115 -> readADC_SingleEnded(m_goalpin));
	if (_ads1115 -> readADC_SingleEnded(m_goalpin) < seuildetect){
		goal = true;
		
		this -> increaseScore(); // comptabilise le but
		//Serial.println("but detecte");
		}
	return goal;
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
unsigned long Equipe::getLastGoal(){
	return m_lastGoal;
}

unsigned long Equipe::getLastCheer(){
	return m_lastCheer;
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
	//Serial.println("cheer");
	m_lastCheer= millis();
	_EffetSon -> play(m_folderCheer); // passe le repertoire à jouer
}

void Equipe::goal()
{
	m_lastGoal = millis();
	_EffetSon -> play(m_folderGoal); // passe le repertoire à jouer
}
void Equipe::win()
{
	_EffetSon -> play(m_folderWin); // passe le repertoire à jouer
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


unsigned long Equipe::getNextCheer(unsigned long tmin, unsigned long tmax){
	unsigned long ctime = millis();
	ctime += random(tmin, tmax);
	return ctime;
}