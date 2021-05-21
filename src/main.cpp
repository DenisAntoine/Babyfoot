#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <DFRobotDFPlayerMini.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <Wire.h>
#include <OneButton.h>
#include <Adafruit_ADS1X15.h>
#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>

#include "EffetSonore.h"
#include "EffetVisuel.h"
#include "Equipe.h"

/*******************************************************
/ Pinout
/ TX -> 
/ RX -> 
/ D1 -> Wire SCL ADS1115 / afficheur 7 segments I2C
/ D2 -> Wire SDA ADS1115 / afficheur 7 segments I2C
/ D3 -> DFPlayer TX via softwareserial
/ D4 -> DFPlayer RX via softwareserial
/ D0 -> 
/ D5 -> Bouton Rouge / Pull up resistor 10k
/ D6 -> Bouton Bleu / Pull up resistor 10k
/ D7 -> Bouton Select / Pull up resistor 10k
/ D8 -> Led Strip
/
********************************************************/

#define BUTRED_PIN	D6
#define BUTBLUE_PIN 	D5
#define LED_PIN    	D8
#define SOUND_PIN  	D7
#define SSRX_PIN   	D4
#define SSTX_PIN  	D3

const int SCOREVICTOIRE = 5;
const int SCOREECART = 2;
bool pause = false;

unsigned long timepause = 0;
unsigned long periodeSon = 30000;

/*******************************************************
/ Ledstrip												
*******************************************************/
#define LED_COUNT 60 // How many NeoPixels are attached to the wemos

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
const uint32_t COL_RED = strip.Color(255,   0,   0);
const uint32_t COL_GREEN = strip.Color(0,   255,   0);
const uint32_t COL_BLUE = strip.Color(0,   0,   255);
const uint32_t COL_WHITE = strip.Color(127, 127, 127);

// *****************************************************
// DFPlayer Mini
// *****************************************************
SoftwareSerial mySoftwareSerial(SSRX_PIN,SSTX_PIN);
DFRobotDFPlayerMini myDFPlayer;

// *****************************************************
// ADS1115
// *****************************************************/

Adafruit_ADS1115 ads1115;
const uint16_t GOALDETECT = 9500; //sensibilité de detection




/*******************************************************
/ Afficheur 7 segments I2C												
*******************************************************/
Adafruit_7segment scoreboard = Adafruit_7segment();

void afficherScore(int score1, int score2){
	scoreboard.clear();
	if ((score1 / 10) % 10 > 0 ) scoreboard.writeDigitNum(0, (score1 / 10) % 10, false);
	scoreboard.writeDigitNum(1, score1 %10, false);
	scoreboard.drawColon(false);
	if ((score2 / 10) % 10 > 0 ) scoreboard.writeDigitNum(3, (score2 / 10) % 10, false);
	scoreboard.writeDigitNum(4, score2 %10, false);
	scoreboard.writeDisplay();
}

// *****************************************************
// Equipes, effets sonores et visuels
// *****************************************************/
EffetSonore effetson(&myDFPlayer);
EffetVisuel effetvis(&strip);
Equipe equipeBleu(1,COL_BLUE);
Equipe equipeRouge(0,COL_RED);

/*******************************************************
/ Definition Boutons												
/ Define the number of devices we have in the chain and
/ the hardware interface
*******************************************************/

OneButton butRed(BUTRED_PIN, true);
OneButton butBlue(BUTBLUE_PIN, true);
OneButton butSound(SOUND_PIN, true);
// boutton rouge
void redClick()
{
pause = false; // action sur bouton enleve la pause
//Serial.println();
//Serial.println(F("Bouton Rouge - plus 1 rouge"));
equipeRouge.increaseScore(); // appui court score +1
//printscores();
afficherScore(equipeRouge.getScore(),equipeBleu.getScore());
}

// boutton bleu
void blueClick()
{
pause = false;// action sur bouton enleve la pause
//Serial.println();
//Serial.println(F("Bouton bleu - plus 1 bleu"));
equipeBleu.increaseScore(); // appui court score +1
//printscores();
afficherScore(equipeRouge.getScore(),equipeBleu.getScore());
}
// boutton sound
void soundClick() // plus 5 sur volume
{
pause = false;// action sur bouton enleve la pause
effetson.increaseVolume(5);  //Set volume value. From 0 to 30

}
// boutton sound
void soundLong() // plus 5 sur volume
{
pause = false;// action sur bouton enleve la pause
myDFPlayer.volume(0);  //Set volume value. From 0 to 30

}
void redLong(){
	pause = false;// action sur bouton enleve la pause
	equipeRouge.resetScore();
	afficherScore(equipeRouge.getScore(),equipeBleu.getScore());
}
void blueLong(){
	pause = false;// action sur bouton enleve la pause
	equipeBleu.resetScore();
	afficherScore(equipeRouge.getScore(),equipeBleu.getScore());
}

/*******************************************************
/ SETUP
*******************************************************/
void setup() {
  /*Serial*/
    mySoftwareSerial.begin(9600);
	Serial.begin(115200);
	delay(1000);
  
  // Initialize Afficheur 7 segments " 0: 0" 
	scoreboard.begin(0x70);
	afficherScore(0,0);
	Serial.println();
	Serial.println(F("Afficheur demarre"));
	delay(1000);

  // initialize Buttons

	butRed.attachClick(redClick);
	butBlue.attachClick(blueClick);
	butRed.attachLongPressStop(redLong);
	butBlue.attachLongPressStop(blueLong);
	butSound.attachClick(soundClick);
	butSound.attachDoubleClick(soundLong);

  /*Strip*/
  	effetvis.begin();
	Serial.println();
	Serial.println("strip demarre");
  
  //Effets Sonore
  Serial.println();
	Serial.println(F("DFRobot DFPlayer Mini Demo"));
	Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));
	
	int attempt = 0;
	while (!myDFPlayer.begin(mySoftwareSerial) && attempt<5)//5 attempts
	{
		attempt++;
		Serial.println(F("Pb communication:"));
	    Serial.println(F("1.SVP verifier connexion serie!"));
	    Serial.println(F("2.SVP verifier SDcard !"));
	    delay(10000);
	}
	if (attempt<5) {
		Serial.println(F("Du gros son!!!"));
		myDFPlayer.setTimeOut(500); // Définit un temps de time out sur la communication série à 500 ms
  		myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD); // lecture sur carte SD
		myDFPlayer.volume(25);  //Set volume value. From 0 to 30
		delay(1000);
	}
		
	 

  // Convertisseur analogiques pour buts
  	ads1115.begin();
  	Serial.println();
	Serial.println("ADS1115 demarre");
	//pinMode(SSRX_PIN,INPUT);
	//pinMode(SSTX_PIN,INPUT);

  // Initialize les equipes
	//equipeRouge.setFolderCheer(1);
	equipeRouge.setFolderGoal(2);
	equipeRouge.setFolderWin(4);

	//equipeBleu.setFolderCheer(4);
	equipeBleu.setFolderGoal(3);
	equipeBleu.setFolderWin(4);
	Serial.println();
	Serial.println("equipes initialisees");

 // Champions League + Rainbow
	
    effetvis.setStripState(RAIN);
//	effetvis.tick();
	myDFPlayer.play(1);
	
	Serial.println("fin setup");
}

void loop() {
  unsigned long cTime = millis(); //current time
  //Serial.println("loop");
  int16_t adsRed = ads1115.readADC_SingleEnded(equipeRouge.getPin());
  int16_t adsBlue = ads1115.readADC_SingleEnded(equipeBleu.getPin());
  /*Serial.print("Rouge :");
  Serial.println(adsRed);
  Serial.print("Bleu :");
  Serial.println(adsBlue);
  delay(1000);*/
  switch (pause){
    case true:
		if (cTime > timepause+5000) { //fin pause
			pause = false;
			timepause = 0;
			equipeRouge.resetScore();
			equipeBleu.resetScore();
			afficherScore(equipeRouge.getScore(),equipeBleu.getScore());
		}
    break;
    
	case false:
		if (adsBlue < GOALDETECT){
			
			equipeBleu.increaseScore(); // comptabilise le but
			Serial.print("but bleu");
			Serial.println(equipeBleu.getScore());
			afficherScore(equipeRouge.getScore(),equipeBleu.getScore());
			delay(500);
			if ((equipeBleu.getScore() >= SCOREVICTOIRE) && (equipeBleu.getScore() - SCOREECART >= equipeRouge.getScore())){
				Serial.println();
				Serial.println("Victoire Bleus");
				effetvis.win(COL_BLUE);
				effetson.play(equipeBleu.getFolderWin());
				pause = true; // met en pause action sur un bouton pour repartir
				timepause = millis();
			}
			else {
				effetvis.goal(COL_BLUE);
				effetson.play(equipeBleu.getFolderGoal());
			}
		}
		if (adsRed < GOALDETECT){
			
			equipeRouge.increaseScore(); // comptabilise le but
			Serial.print("but Rouge");
			Serial.println(equipeRouge.getScore());
			afficherScore(equipeRouge.getScore(),equipeBleu.getScore());
			delay(500);
			if ((equipeRouge.getScore() >= SCOREVICTOIRE) && (equipeRouge.getScore() - SCOREECART >= equipeBleu.getScore())){
				Serial.println();
				Serial.println("Victoire Rouges");
				effetvis.win(COL_RED);
				effetson.play(equipeRouge.getFolderWin());
				pause = true; // met en pause action sur un bouton pour repartir
				timepause = millis();
			}
			else {
				effetvis.goal(COL_RED);
				effetson.play(equipeRouge.getFolderGoal());
			}
		}
		periodeSon = random(30000, 80000);
		if (cTime > effetson.getLastSound() + periodeSon) {
			if (cTime > max(equipeBleu.getLastGoal(), equipeRouge.getLastGoal()) + periodeSon) // pas de but depuis x sec
				{
				//equipeBlanche.cheer();//on motive le groupe equipe neutre
				effetvis.cheer();
				effetson.play(1);
				Serial.println();
				Serial.println("cheer");
				}	
		}		
    break;
  }
  
  
  effetvis.tick();
  butRed.tick();
  butBlue.tick();
  butSound.tick();

  
}