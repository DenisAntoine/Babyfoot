#include "EffetSonore.h"

EffetSonore::EffetSonore(DFRobotDFPlayerMini* dfp)
{
	_DFPlayer = dfp;
}

//test de la methode play
void EffetSonore::play(int folder)
{
long n = random(_DFPlayer->readFileCountsInFolder(folder));
_DFPlayer->playFolder(folder, n); //play aleatoire dans folder
}
