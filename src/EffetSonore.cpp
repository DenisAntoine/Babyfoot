#include "EffetSonore.h"

EffetSonore::EffetSonore(DFRobotDFPlayerMini* dfp)
{
	_DFPlayer = dfp;
}

//test de la methode play
void EffetSonore::play(int folder)
{
	_DFPlayer->play(folder); //play folder
}
