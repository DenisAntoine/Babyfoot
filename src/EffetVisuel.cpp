#include "EffetVisuel.h"



EffetVisuel::EffetVisuel(Adafruit_NeoPixel* strip)
{
	_strip = strip;
  
}


void EffetVisuel::flash(uint32_t color, int wait, uint16_t pstart, uint16_t pend) {
  
  for(int i=pstart; i<pend; i++) { // For each pixel in strip...
    _strip->setPixelColor(i, color);         //  Set pixel's color (in RAM)
    _strip->show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}

void EffetVisuel::strobe(uint32_t color, int strobecount, int flashdelay, uint16_t pstart, uint16_t pend) {
  
  for(int i=0; i<strobecount; i++) { // For each pixel in strip...
    this->flash(color, 0, pstart, pend);         //  Set pixel's color (in RAM)
    
    delay(flashdelay);                           //  Pause for a moment
    this ->flash(0, 0, pstart, pend);
    
    delay(flashdelay);
  }
}

void EffetVisuel::cheer(uint32_t color)
{
	//à definir
}

void EffetVisuel::goal(uint32_t color)
{
  this-> strobe(color, 15, 1, 0, _strip->numPixels());
} //à definir

void EffetVisuel::win(uint32_t color)
{} //à definir



// Theater-marquee-style chasing lights. Pass in a color (32-bit value,
// a la strip.Color(r,g,b) as mentioned above), and a delay time (in ms)
// between frames.
void EffetVisuel::theaterChase(uint32_t color, int wait) {
  for(int a=0; a<10; a++) {  // Repeat 10 times...
    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
      _strip->clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in steps of 3...
      for(int c=b; c<_strip->numPixels(); c += 3) {
        _strip->setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      _strip->show(); // Update strip with new contents
      delay(wait);  // Pause for a moment
    }
  }
}

// Rainbow cycle along whole strip. Pass delay time (in ms) between frames.
void EffetVisuel::rainbow(int wait) {
  // Hue of first pixel runs 5 complete loops through the color wheel.
  // Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to 5*65536. Adding 256 to firstPixelHue each time
  // means we'll make 5*65536/256 = 1280 passes through this outer loop:
  for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
    for(int i=0; i<_strip->numPixels(); i++) { // For each pixel in strip...
      // Offset pixel hue by an amount to make one full revolution of the
      // color wheel (range of 65536) along the length of the strip
      // (strip.numPixels() steps):
      int pixelHue = firstPixelHue + (i * 65536L / _strip->numPixels());
      // strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
      // optionally add saturation and value (brightness) (each 0 to 255).
      // Here we're using just the single-argument hue variant. The result
      // is passed through strip.gamma32() to provide 'truer' colors
      // before assigning to each pixel:
      _strip->setPixelColor(i, _strip->gamma32(_strip->ColorHSV(pixelHue)));
    }
    _strip->show(); // Update strip with new contents
    delay(wait);  // Pause for a moment
  }
}

// Rainbow-enhanced theater marquee. Pass delay time (in ms) between frames.
void EffetVisuel::theaterChaseRainbow(int wait) {
  int firstPixelHue = 0;     // First pixel starts at red (hue 0)
  for(int a=0; a<30; a++) {  // Repeat 30 times...
    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
      _strip->clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in increments of 3...
      for(int c=b; c<_strip->numPixels(); c += 3) {
        // hue of pixel 'c' is offset by an amount to make one full
        // revolution of the color wheel (range 65536) along the length
        // of the strip (strip.numPixels() steps):
        int      hue   = firstPixelHue + c * 65536L / _strip->numPixels();
        uint32_t color = _strip->gamma32(_strip->ColorHSV(hue)); // hue -> RGB
        _strip->setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      _strip->show();                // Update strip with new contents
      delay(wait);                 // Pause for a moment
      firstPixelHue += 65536 / 90; // One cycle of color wheel over 90 frames
    }
  }
}

void EffetVisuel::whiteOverRainbow(int whiteSpeed, int whiteLength) {

  if(whiteLength >= _strip ->numPixels()) whiteLength = _strip ->numPixels() - 1;

  int      head          = whiteLength - 1;
  int      tail          = 0;
  int      loops         = 3;
  int      loopNum       = 0;
  uint32_t lastTime      = millis();
  uint32_t firstPixelHue = 0;

  for(;;) { // Repeat forever (or until a 'break' or 'return')
    for(int i=0; i<_strip ->numPixels(); i++) {  // For each pixel in strip...
      if(((i >= tail) && (i <= head)) ||      //  If between head & tail...
         ((tail > head) && ((i >= tail) || (i <= head)))) {
        _strip ->setPixelColor(i, _strip ->Color(0, 0, 0, 255)); // Set white
      } else {                                             // else set rainbow
        int pixelHue = firstPixelHue + (i * 65536L / _strip ->numPixels());
        _strip ->setPixelColor(i, _strip ->gamma32(_strip ->ColorHSV(pixelHue)));
      }
    }

    _strip ->show(); // Update strip with new contents
    // There's no delay here, it just runs full-tilt until the timer and
    // counter combination below runs out.

    firstPixelHue += 40; // Advance just a little along the color wheel

    if((millis() - lastTime) > whiteSpeed) { // Time to update head/tail?
      if(++head >= _strip ->numPixels()) {      // Advance head, wrap around
        head = 0;
        if(++loopNum >= loops) return;
      }
      if(++tail >= _strip ->numPixels()) {      // Advance tail, wrap around
        tail = 0;
      }
      lastTime = millis();                   // Save time of last movement
    }
  }
}

void EffetVisuel::pulseWhite(uint8_t wait) {
  for(int j=0; j<256; j++) { // Ramp up from 0 to 255
    // Fill entire strip with white at gamma-corrected brightness level 'j':
    _strip ->fill(_strip ->Color(0, 0, 0, _strip ->gamma8(j)));
    _strip ->show();
    delay(wait);
  }

  for(int j=255; j>=0; j--) { // Ramp down from 255 to 0
    _strip ->fill(_strip ->Color(0, 0, 0, _strip ->gamma8(j)));
    _strip ->show();
    delay(wait);
  }
}

void EffetVisuel::rainbowFade2White(int wait, int rainbowLoops, int whiteLoops) {
  int fadeVal=0, fadeMax=100;

  // Hue of first pixel runs 'rainbowLoops' complete loops through the color
  // wheel. Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to rainbowLoops*65536, using steps of 256 so we
  // advance around the wheel at a decent clip.
  for(uint32_t firstPixelHue = 0; firstPixelHue < rainbowLoops*65536;
    firstPixelHue += 256) {

    for(int i=0; i<_strip ->numPixels(); i++) { // For each pixel in strip...

      // Offset pixel hue by an amount to make one full revolution of the
      // color wheel (range of 65536) along the length of the strip
      // (strip.numPixels() steps):
      uint32_t pixelHue = firstPixelHue + (i * 65536L / _strip ->numPixels());

      // strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
      // optionally add saturation and value (brightness) (each 0 to 255).
      // Here we're using just the three-argument variant, though the
      // second value (saturation) is a constant 255.
      _strip ->setPixelColor(i, _strip ->gamma32(_strip ->ColorHSV(pixelHue, 255,
        255 * fadeVal / fadeMax)));
    }

    _strip ->show();
    delay(wait);

    if(firstPixelHue < 65536) {                              // First loop,
      if(fadeVal < fadeMax) fadeVal++;                       // fade in
    } else if(firstPixelHue >= ((rainbowLoops-1) * 65536)) { // Last loop,
      if(fadeVal > 0) fadeVal--;                             // fade out
    } else {
      fadeVal = fadeMax; // Interim loop, make sure fade is at max
    }
  }

  for(int k=0; k<whiteLoops; k++) {
    for(int j=0; j<256; j++) { // Ramp up 0 to 255
      // Fill entire strip with white at gamma-corrected brightness level 'j':
      _strip ->fill(_strip ->Color(0, 0, 0, _strip ->gamma8(j)));
      _strip ->show();
    }
    delay(1000); // Pause 1 second
    for(int j=255; j>=0; j--) { // Ramp down 255 to 0
      _strip ->fill(_strip ->Color(0, 0, 0, _strip ->gamma8(j)));
      _strip ->show();
    }
  }

  delay(500); // Pause 1/2 second
}

