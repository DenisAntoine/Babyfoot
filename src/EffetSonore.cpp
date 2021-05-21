#include "EffetSonore.h"

EffetSonore::EffetSonore(DFRobotDFPlayerMini* dfp)
{
	_DFPlayer = dfp;
	m_lastSound =millis();
	m_lastplay=1;
	m_volume=25;
	
}
void EffetSonore::increaseVolume(int increment){
	m_volume =m_volume+increment;
	if (m_volume>30){
		m_volume =0;
	}
	_DFPlayer->volume(m_volume);
}

void EffetSonore::setVolume(int volume){
	
	m_volume=volume;

	if (m_volume>30){
		m_volume =0;
	}
	_DFPlayer->volume(m_volume);
}

//test de la methode play
void EffetSonore::play(int folder)
{
//Serial.println();
//Serial.print("folder : ");
//Serial.println(folder);
//Serial.print("nbre fichiers : ");
//Serial.println(_DFPlayer->readFileCountsInFolder(folder));
//long m = _DFPlayer->readFileCountsInFolder(folder)+1;

long filecount = _DFPlayer->readFileCountsInFolder(folder);
delay(100);
//Serial.print("Joue : ");
//Serial.println(n);
m_lastplay += random(1,3);
m_lastplay = m_lastplay % 5;
m_lastSound =millis(); // last play
_DFPlayer->playFolder(folder, m_lastplay); //play aleatoire dans folder
//delay(1000);
//Serial.print("Etat : ");
//Serial.println(_DFPlayer->readState());
}

unsigned long EffetSonore::getLastSound(){
	return m_lastSound;
}