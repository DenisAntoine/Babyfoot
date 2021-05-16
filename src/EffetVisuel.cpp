#include "EffetVisuel.h"



EffetVisuel::EffetVisuel(Adafruit_NeoPixel* strip)
{
	_strip = strip;
  _NumPixel = strip->numPixels();
  _stripState = OFF;
  _NbCycle=0;
  _color = strip->Color(127, 127, 127);
  
}
void EffetVisuel::goal(uint32_t color)
{
  _stripState = STROB;
  this->setPixelInterval(10);
  this->setColor(color);

} //à definir

void EffetVisuel::win(uint32_t color)
{
  _stripState = THEAC;
  this->setPixelInterval(100);
  this->setColor(color);

} //à definir
void EffetVisuel::cheer()
{
  this->setPixelInterval(10);
  _stripState = RAIN;

} //à definir



OpState EffetVisuel::getStripState()
{
  return _stripState;
}

void EffetVisuel::setStripState(OpState state){
  _stripState = state;
}

uint16_t EffetVisuel::getStripCycle()
{
  return _NbCycle;
}
void EffetVisuel::setPixelInterval(unsigned long pixelsInterval){
  _pixelsInterval=pixelsInterval;
}

void EffetVisuel::setColor(uint32_t color){
  _color=color;
}


void EffetVisuel::tick(){
  if ((unsigned long)(millis() - _previousMillis) >= _pixelsInterval) {
    _previousMillis=millis();
    
    switch (_stripState){
        case OFF:
          this->off();
        break;
        case WIPE:
          this->colorWipe(_color, 1);
        break;
        case THEAC:
          this->theaterChase(_color, 50);
        break;
        case FLASH:
          this->flash(_color, 1);
        break;
        case STROB:
          this->strobe(_color, 50);
        break;
        case RAIN:
          this->rainbow(10);
        break;
    
    }
  }
}

void EffetVisuel::off(){
//_strip->clear();
_strip->show();
_currentPixel = 0;
_stripState = OFF;
}

void EffetVisuel::colorWipe(uint32_t c, uint16_t cycle){
  _strip->setPixelColor(_currentPixel,c);
  _strip->show();
  _stripState = WIPE;
  _currentPixel++;
  if(_currentPixel == _NumPixel){
    _currentPixel = 0;
    _NbCycle++;
  }
  if (_NbCycle == cycle)
  {
    _NbCycle=0;
    _currentPixel = 0;
    _stripState = OFF;
  }
}

void EffetVisuel::theaterChase(uint32_t c, uint16_t cycle){
  
  for (int i=0; i < _NumPixel; i=i+3) {
      _strip->setPixelColor(i+_currentPixel, c);    //turn every third pixel on
    }
  
  _strip->show();
  for (int i=0; i < _NumPixel; i=i+3) {
      _strip->setPixelColor(i+_currentPixel, 0);        //turn every third pixel off
    _stripState = THEAC;
    }
  _currentPixel++;   
  if(_currentPixel >= 3){
    _currentPixel = 0;
    _NbCycle++;
  }
  if (_NbCycle == cycle)
  {
    _NbCycle=0;
    _currentPixel = 0;
    _strip->clear();
    _stripState = OFF;
  }
}


void EffetVisuel::flash(uint32_t c, uint16_t cycle){
  _strip->setPixelColor(_currentPixel,c);
  _currentPixel++;
  _stripState = FLASH;
  if(_currentPixel == _NumPixel){
    _strip->show();
    _currentPixel = 0;
    _NbCycle++;
    
  }
  if (_NbCycle == cycle)
  {
    _NbCycle=0;
    _currentPixel = 0;
    _strip->clear();
    _stripState = OFF;
  }
}
void EffetVisuel::rainbow(uint16_t cycle) {
  
  _stripState = RAIN;
  uint16_t i;
  for(i=0; i< _NumPixel; i++) {
      _strip->setPixelColor(i, wheel(((i * 256 / _NumPixel) + _currentPixel) & 255));
    }
  _strip->show();
  _currentPixel++;
    
  _NbCycle++;
  if (_NbCycle == cycle)
    {
    _NbCycle=0;
    _currentPixel = 0;
    _stripState = OFF;
    }
}

void EffetVisuel::strobe(uint32_t c, uint16_t cycle) {
  if (_NbCycle%2 == 0){ //cycle pair
    _strip->setPixelColor(_currentPixel,c);
  }
  else {
    _strip->setPixelColor(_currentPixel,0);
  }
  _strip->show();
  _stripState = STROB;
  _currentPixel++;
  
  if(_currentPixel == _NumPixel){
    _currentPixel = 0;
    _NbCycle++;
  }
  if (_NbCycle == cycle)
  {
    _NbCycle=0;
    _currentPixel = 0;
    _strip->clear();
    _stripState = OFF;
  }
}

uint32_t EffetVisuel::wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return _strip->Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return _strip->Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return _strip->Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}


