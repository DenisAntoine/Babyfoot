#include "EffetVisuel.h"

EffetVisuel::EffetVisuel(Adafruit_NeoPixel* strip)
{
	_strip = strip;
}


void EffetVisuel::flash(uint32_t color, int wait) {
  for(int i=0; i<_strip->numPixels(); i++) { // For each pixel in strip...
    _strip->setPixelColor(i, color);         //  Set pixel's color (in RAM)
    _strip->show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}


void EffetVisuel::cheer(int team)
{
	this -> flash(255, 10); //à definir
}

void EffetVisuel::goal(int team)
{} //à definir

void EffetVisuel::win(int team)
{} //à definir
