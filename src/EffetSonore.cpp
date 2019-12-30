#include "EffetSonore.h"

EffetSonore::EffetSonore(DFRobotDFPlayerMini* dfp)
{
	_DFPlayer = dfp;
	m_lastSound =0;
}

//test de la methode play
void EffetSonore::play(int folder)
{
Serial.println();
Serial.print("folder : ");
Serial.println(folder);
Serial.print("nbre fichiers : ");
Serial.println(_DFPlayer->readFileCountsInFolder(folder));

long n = ESP8266TrueRandom.random(1,_DFPlayer->readFileCountsInFolder(folder));
Serial.print("Joue : ");
Serial.println(n);

_DFPlayer->playFolder(folder, n); //play aleatoire dans folder
delay(1000);
Serial.print("Etat : ");
Serial.println(_DFPlayer->readState());
}

unsigned long EffetSonore::getLastSound(){
	return m_lastSound;
}