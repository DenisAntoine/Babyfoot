#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "SoftwareSerial.h"
#include <DFRobotDFPlayerMini.h>

#include "Equipe.h"

#define LED_PIN    6
#define TX_PIN    10
#define RX_PIN    11
// How many NeoPixels are attached to the Arduino?
#define LED_COUNT 60

// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)

SoftwareSerial mySoftwareSerial(RX_PIN, TX_PIN); // RX, TX
DFRobotDFPlayerMini myDFPlayer;


EffetSonore monson(&myDFPlayer);
EffetVisuel monvis(&strip);
Equipe equipeRouge(2);
Equipe equipeBleu(1);

void setup() {
  mySoftwareSerial.begin(9600);
  Serial.begin(115200);
  
  if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true){
      delay(0); // Code to compatible with ESP8266 watch dog.
    }
  }
  Serial.println(F("DFPlayer Mini online."));
  myDFPlayer.volume(10);  //Set volume value. From 0 to 30
 
  
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)
  
  equipeRouge.setFolderCheer(1);
	equipeRouge.setFolderGoal(2);
	equipeRouge.setFolderWin(3);
  equipeRouge.setEffetSonore(&monson);
  equipeRouge.setEffetVisuel(&monvis);

  equipeBleu.setFolderCheer(1);
	equipeBleu.setFolderGoal(2);
	equipeBleu.setFolderWin(3);
  equipeBleu.setEffetSonore(&monson);
  equipeBleu.setEffetVisuel(&monvis);




  equipeBleu.cheer();
  equipeBleu.goal();
  equipeBleu.increaseScore();
  Serial.print("Score Bleu :");
  Serial.println(equipeBleu.getScore());
  equipeBleu.decreaseScore();
  Serial.print("Score Bleu :");
  Serial.println(equipeBleu.getScore());
  
  equipeRouge.increaseScore();
  equipeRouge.goal();
  equipeRouge.win();

}

void loop() {
  // put your main code here, to run repeatedly:
}
