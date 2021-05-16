#include "Equipe.h"

Equipe::Equipe(int goalPin)
{
	m_goalpin = goalPin;
	m_score=0; //score initial à 0
	m_lastGoal = millis();
	m_lastCheer = millis();
}

Equipe::Equipe(int goalPin, uint32_t color)
{
	m_goalpin = goalPin;
	m_color = color;
	m_score=0; //score initial à 0
	m_lastGoal = millis();
	m_lastCheer = millis();
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

uint32_t Equipe::getColor(){
	return m_color;
}

int Equipe::getFolderCheer(){
	return m_folderCheer;
}
int Equipe::getFolderGoal(){
	return m_folderGoal;
	}
int Equipe::getFolderWin(){
	return m_folderWin;
	}



void Equipe::setColor(uint32_t color){
	m_color = color;
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
}

void Equipe::goal()
{
	m_lastGoal = millis();
}
void Equipe::win()
{

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


unsigned long Equipe::getNextCheer(unsigned long tmin, unsigned long tmax){
	unsigned long ctime = millis();
	ctime += random(tmin, tmax);
	return ctime;
}