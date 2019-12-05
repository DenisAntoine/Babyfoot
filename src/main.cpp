#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <DFRobotDFPlayerMini.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_ADS1015.h>

#include "SmartPins.h"

#include "Equipe.h"

/*******************************************************
/ Pinout
/ TX -> DFPlayer TX via serial (pour economiser pin)
/ RX -> DFPlayer RX via serial (pour economiser pin)
/ D1 -> Wire SCL ADS1115 / OledShield / afficheur
/ D2 -> Wire SDA ADS1115 / OledShield / afficheur
/ D3 -> OLED_RESET
/ D4 -> Led Strip
/ D0 -> 
/ D5 -> Bouton Rouge / Pull up resistor 10k
/ D6 -> Bouton Bleu / Pull up resistor 10k
/ D7 -> Software serial RX
/ D8 -> Software serial TX
/
********************************************************/
// OLED
#define OLED_RESET D3  // GPIO0

#define BUTRED_PIN	D5
#define BUTBLUE_PIN D6
#define LED_PIN    	D0
#define SSRX_PIN   	D4
#define SSTX_PIN  	D7






/*******************************************************
 Ledstrip												
*******************************************************/

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


/*****************************************************
OLED Display Variables and constants
****************************************************/
//Adafruit_SSD1306 display(OLED_RESET);

// *****************************************************
// DFPlayer Mini and constants
// *****************************************************
SoftwareSerial mySoftwareSerial(SSRX_PIN,SSTX_PIN);
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
*******************************************************/

// define pin numbers for individual switches
SmartPins spRed;
SmartPins spBlue;
#define DEBOUNCE_TIME 15

void redChange(int hilo, int value)
{
  // boutton rouge
  if(value > 10000) { // appui de 10s reset des 2 scores
	  
	  Serial.println();
	  Serial.println(F("Bouton rouge >10s - reset scores"));
	  equipeRouge.resetScore();
	  equipeBleu.resetScore();
  }
  else if (value > 5000) {
	  Serial.println();
	  Serial.println(F("Bouton rouge >5s - reset rouge"));
	  equipeRouge.resetScore(); // appui de 10s reset du score
  }
	
  else {
	  Serial.println();
	  Serial.println(F("Bouton rouge - plus 1rouge"));
	  equipeRouge.increaseScore(); // appui court score +1
  }
}

void blueChange(int hilo, int value)
{
  // boutton bleu
  if(value > 10000) { // appui de 10s reset des 2 scores
	  Serial.println();
	  Serial.println(F("Bouton bleu >10s - reset scores"));
	  equipeRouge.resetScore();
	  equipeBleu.resetScore();
	}
  else if (value > 5000) {
	  Serial.println();
	  Serial.println(F("Bouton bleu >5s - reset bleu"));
	  equipeBleu.resetScore(); // appui de 10s reset du score
  	}
  else {
	  Serial.println();
	  Serial.println(F("Bouton bleu - plus 1 bleu"));
	  equipeBleu.increaseScore(); // appui court score +1
        }
}

void redcheer()
{
	equipeRouge.cheer();
}
void bluecheer()
{
	equipeBleu.cheer();
}

// *****************************************************
// test les buts sur ADS1115
// *****************************************************/

int testgoal()
{
int goal = 0;
if (ads1115.readADC_SingleEnded(0) < GOALDETECT){
	goal = 1;
	}
if (ads1115.readADC_SingleEnded(1) < GOALDETECT){
	goal = 2;
	}
return goal;
}

// *****************************************************
// setup
// *****************************************************/
void setup() {
mySoftwareSerial.begin(9600);
Serial.begin(115200);




// Initialize DFPlayer on softwareserial
	
	Serial.println();
	Serial.println(F("DFRobot DFPlayer Mini Demo"));
	Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));

if (!myDFPlayer.begin(mySoftwareSerial)) {  //Utilisation de  softwareSerial pour communiquer
    Serial.println(F("Pb communication:"));
    Serial.println(F("1.SVP verifier connexion serie!"));
    Serial.println(F("2.SVP verifier SDcard !"));
    while(true);
  }
    Serial.println(F("DFPlayer Mini En ligne."));
  
 	myDFPlayer.setTimeOut(500); // Définit un temps de time out sur la communication série à 500 ms
	myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);
	myDFPlayer.volume(25);  //Set volume value. From 0 to 30
	
	delay(1000);
	

	Serial.println(F("DFPlayer Mini En ligne."));
	//effetson.play(1); // joue l intro


// initialize Buttons
spRed.Timed(BUTRED_PIN,INPUT,DEBOUNCE_TIME,redChange);       // external pullup resistor 10k
spBlue.Timed(BUTBLUE_PIN,INPUT,DEBOUNCE_TIME,blueChange);    // external pullup resistor 10k
spRed.everyRandom(120000, 580000, redcheer); // encouragements aleatoires
spBlue.everyRandom(120000, 580000, bluecheer); // encouragements aleatoires

// Convertisseur analogiques pour buts
ads1115.begin();
Serial.println();
Serial.println("ADS1115 demarre");


/* Initialize LCD DiSplay
	display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 64x48)
	display.clearDisplay();
	display.setRotation(2);
	display.setTextSize(1);
	display.setTextColor(WHITE);
	display.setCursor(0,0);
	display.println("init...");
	display.display();
*/



 
 // Initialize Ledstrip
  
	strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
	strip.show();            // Turn OFF all pixels ASAP
	strip.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)
	//effetvis.theaterChaseRainbow(10000);
  Serial.println();
  Serial.println("strip demarre");
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
	equipeBleu.setEffetVisuel(&effetvis);
	
	Serial.println();
  	Serial.println("Allez les bleus");
	equipeBleu.cheer();
	delay(10000);
	
	Serial.println();
  	Serial.println("But bleus");
	equipeBleu.goal();
	Serial.print("Score Bleu :");
	Serial.println(equipeBleu.getScore());
	delay(10000);
	
	Serial.print("But annule :");
	equipeBleu.decreaseScore();
	Serial.println(equipeBleu.getScore());
  
	Serial.println();
  	Serial.println("But Rouges");
	equipeRouge.goal();
	Serial.print("Score Rouges :");
	Serial.println(equipeRouge.getScore());
	
	Serial.println();
	Serial.println("Victoire Rouges");
	equipeRouge.win();
    
	Serial.println();
	Serial.println("Fin setup");
	delay(60000);
}

void loop() {
  
  /*
  Routine pour introduire et modifier la frequence des encouragements
  spRed.everyRandom(bluermin, bluermax, redcheer); // encouragements aleatoires
  spBlue.everyRandom(redrmin, redrmax, bluecheer); // encouragements aleatoires
  */
  Serial.print(F("Score Rouge :"));
  Serial.println(equipeRouge.getScore());
  Serial.print(F("Score Bleu :"));
  Serial.println(equipeBleu.getScore());
  
  switch (testgoal()) {
  case 1:// but bleu
    Serial.println();
    Serial.println(F("But bleu !"));
    equipeBleu.goal();
    
  break;
  case 2:// but rouge
    Serial.println();
    Serial.println(F("But Rouge !"))
    equipeRouge.goal();

  break;
  default:// pas but
    spRed.loop();// teste les boutons
    spBlue.loop();
    break;
  }
 
}
