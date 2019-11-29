#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <DFRobotDFPlayerMini.h>
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_ADS1015.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SmartPins.h>

#include "Equipe.h"

/*******************************************************
/ Pinout
/ TX -> DFPlayer TX via serial (pour economiser pin)
/ RX -> DFPlayer RX via serial (pour economiser pin)
/ D1 -> Wire SCL ADS1115 / OledShield
/ D2 -> Wire SDA ADS1115 / OledShield
/ D3 -> Ledstrip
/ D4 -> 
/ D0 -> Afficheur CS_PIN
/ D5 -> Bouton Rouge / Pull up resistor 10k
/ D6 -> Bouton Bleu / Pull up resistor 10k
/ D7 -> Afficheur CLK_PIN
/ D8 -> Afficheur DATA_PIN
/
********************************************************/

#define BUTRED_PIN	D5
#define BUTBLUE_PIN D6
#define LED_PIN    	D3
#define CLK_PIN   	D7
#define DATA_PIN  	D8
#define CS_PIN    	D0

/*******************************************************
/ Afficheur												
/ Define the number of devices we have in the chain and
/ the hardware interface
/*******************************************************/
#define MAX_DEVICES 4


// Arbitrary output pins
MD_Parola P = MD_Parola(DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);
#define SPEED_TIME  25
#define PAUSE_TIME  1000




/*******************************************************
/ Ledstrip												
/*******************************************************/

// How many NeoPixels are attached to the wemos
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


// *****************************************************
// OLED Display Variables and constants
// *****************************************************/
Adafruit_SSD1306 display(OLED_RESET);

// *****************************************************
// DFPlayer Mini and constants
// *****************************************************/
DFRobotDFPlayerMini myDFPlayer;

// *****************************************************
// ADS1115
// *****************************************************/

Adafruit_ADS1115 ads1115(0x49);
const uint16_t GOALDETECT = 10000; //sensibilité de detection



// *****************************************************
// Equipes, effets sonores et visuels
// *****************************************************/
EffetSonore effetson(&myDFPlayer);
EffetVisuel effetvis(&strip);
Equipe equipeRouge(2);
Equipe equipeBleu(1);

/*******************************************************
/ Boutons												
/ Define the number of devices we have in the chain and
/ the hardware interface
/*******************************************************/

// define pin numbers for individual switches
SmartPins spRed;
SmartPins spBlue;
SmartPins spCheer;
#define DEBOUNCE_TIME 15

void redChange(int hilo, int value))
{
  // boutton rouge
  if(value > 10000) { // appui de 10s reset des 2 scores
	  equipeRouge.reset();
	  equipeBleu.reset();
  }
  else if (value > 5000) equipeRouge.reset(); // appui de 10s reset du score
  else equipeRouge.increaseScore(); // appui court score +1
}

void blueChange(int hilo, int value))
{
  // boutton bleu
    if(value > 10000) { // appui de 10s reset des 2 scores
	  equipeRouge.reset();
	  equipeBleu.reset();
	}
  else if (value > 5000) equipeBleu.reset(); // appui de 10s reset du score
  else equipeBleu.increaseScore(); // appui court score +1
}

void redcheer();
{
	equipeRouge.cheer();
}
void bluecheer();
{
	equipeBleu.cheer();
}

// *****************************************************
// test les buts sur ADS1115
// *****************************************************/

int testgoal(){
int goal =0;
if (ads.readADC_SingleEnded(0) < GOALDETECT){
	goal = 1;
	}
if (ads.readADC_SingleEnded(0) < GOALDETECT){
	goal = 2;
	}
return goal;
}

// *****************************************************
// setup
// *****************************************************/
void setup() {

randomSeed(analogRead(A0)); // aleatoire sur pin non branchée

// initialize Buttons
spRed.Timed(BUTRED_PIN,INPUT,DEBOUNCE_TIME,redChange);       // external pullup resistor 10k
spBlue.Timed(BUTBLUE_PIN,INPUT,DEBOUNCE_TIME,blueChange);    // external pullup resistor 10k
spRed.everyRandom(30000, 240000, redcheer); // encouragements aleatoires
spBlue.everyRandom(30000, 240000, bluecheer); // encouragements aleatoires

// Convertisseur analogiques pour buts
ads1115.begin();



// Afficheur 2 zones
P.begin(2);
P.setZone(0,0,1); // zone 0 = modules 0 et 1
P.setZone(1,2,3); // zone 1 = modules 2 et 3
P.displayZoneText(0, "0", PA_CENTER, SPEED_TIME, PAUSE_TIME, PA_SCROLL_UP, PA_SCROLL_UP); // test zone 0
P.displayZoneText(0, "1", PA_CENTER, SPEED_TIME, PAUSE_TIME, PA_SCROLL_UP, PA_SCROLL_UP);
P.displayZoneText(1, "0", PA_CENTER, SPEED_TIME, PAUSE_TIME, PA_SCROLL_UP, PA_SCROLL_UP);
P.displayZoneText(1, "1", PA_CENTER, SPEED_TIME, PAUSE_TIME, PA_SCROLL_UP, PA_SCROLL_UP); // test zone 1


// Initialize LCD DiSplay
	display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 64x48)
	display.clearDisplay();
	display.setRotation(2);
	display.setTextSize(1);
	display.setTextColor(WHITE);
	display.setCursor(0,0);
	display.println("init...");
	display.display();

// Initialize DFPlayer on serial
	Serial.begin(9600);
	display.println();
	display.println(F("DFRobot DFPlayer Mini Demo"));
	display.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));
	display.display();

	if (!myDFPlayer.begin(Serial)) {  //Use Serial1 to communicate with mp3.
		display.println(F("Unable to begin:"));
		display.println(F("1.Please recheck the connection!"));
		display.println(F("2.Please insert the SD card!"));
		display.display();
		while(true){
		  delay(0); // Code to compatible with ESP8266 watch dog.
		}
	}
	display.clearDisplay();
	display.println(F("DFPlayer Mini online."));
	display.display();
	myDFPlayer.volume(10);  //Set volume value. From 0 to 30
	effetson.play(1); // joue l intro
 
 // Initialize Ledstrip
  
	strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
	strip.show();            // Turn OFF all pixels ASAP
	strip.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)
	effetvis.theaterChaseRainbow(10000);
  
  // Initialize les equipes
	equipeRouge.setFolderCheer(1);
	equipeRouge.setFolderGoal(2);
	equipeRouge.setFolderWin(3);
	equipeRouge.setEffetSonore(&effetson);
	equipeRouge.setEffetVisuel(&effetvis);

	equipeBleu.setFolderCheer(1);
	equipeBleu.setFolderGoal(2);
	equipeBleu.setFolderWin(3);
	equipeBleu.setEffetSonore(&effetson);
	equipeBleu.setEffetVisuel(&effetson);

	equipeBleu.cheer();
	equipeBleu.goal();
	equipeBleu.increaseScore();
	
 // on teste les methodes	
	display.clearDisplay();
	display.print("Score Bleu :");
	display.println(equipeBleu.getScore());
	display.display();
	
	equipeBleu.decreaseScore();
	display.print("Score Bleu :");
	display.println(equipeBleu.getScore());
  
	equipeRouge.increaseScore();
	equipeRouge.goal();
	equipeRouge.win();

}

void loop() {
  char score_buff[2];
  /*
  Routine pour introduire et modifier la frequence des encouragements
  spRed.everyRandom(bluermin, bluermax, redcheer); // encouragements aleatoires
  spBlue.everyRandom(redrmin, redrmax, bluecheer); // encouragements aleatoires
  */
  
  
  switch (testgoal()) {
  case 1:// but bleu
    equipeBleu.goal();
	sprintf(score_buff, "%d", equipeBleu.getScore());
	P.displayZoneText(0, score_buff, PA_CENTER, SPEED_TIME, PAUSE_TIME, PA_SCROLL_UP, PA_SCROLL_UP);
    break;
  case 2:// but rouge
    equipeRouge.goal();
	sprintf(score_buff, "%d", equipeRouge.getScore());
	P.displayZoneText(1, score_buff, PA_CENTER, SPEED_TIME, PAUSE_TIME, PA_SCROLL_UP, PA_SCROLL_UP);
    break;
  default:// pas but
    spRed.loop();// teste les boutons
	spBlue.loop();
    break;
	}
}
