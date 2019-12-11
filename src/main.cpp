#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <DFRobotDFPlayerMini.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <Wire.h>
#include <OneButton.h>

#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>

#if defined(ESP8266)
#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#else
#include <WiFi.h>          //https://github.com/esp8266/Arduino
#endif
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

//needed for library
#include <DNSServer.h>
#if defined(ESP8266)
#include <ESP8266WebServer.h>
#else
#include <WebServer.h>
#endif
#include <WiFiManager.h>  

#include "Equipe.h"

/*******************************************************
/ Pinout
/ TX -> 
/ RX -> 
/ D1 -> Wire SCL ADS1115 / afficheur 7 segments I2C
/ D2 -> Wire SDA ADS1115 / afficheur 7 segments I2C
/ D3 -> 
/ D4 -> DFPlayer RX via softwareserial
/ D0 -> Led Strip
/ D5 -> Bouton Rouge / Pull up resistor 10k
/ D6 -> Bouton Bleu / Pull up resistor 10k
/ D7 -> DFPlayer TX via softwareserial
/ D8 -> 
/
********************************************************/

#define BUTRED_PIN	D5
#define BUTBLUE_PIN 	D6
#define LED_PIN    	D0
#define SSRX_PIN   	D4
#define SSTX_PIN  	D7

const int SCOREVICTOIRE = 5;
const int SCOREECART = 2;
bool pause = false;


/*******************************************************
/ Afficheur 7 segments I2C												
*******************************************************/
Adafruit_7segment scoreboard = Adafruit_7segment();

void afficherScore(int score1, int score2){
	scoreboard.writeDigitNum(0, (score1 / 10) % 10, false);
	scoreboard.writeDigitNum(1, score1 %10, false);
	scoreboard.drawColon(false);
	scoreboard.writeDigitNum(3, (score2 / 10) % 10, false);
	scoreboard.writeDigitNum(4, score2 %10, false);
	scoreboard.writeDisplay();
}


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


// *****************************************************
// DFPlayer Mini
// *****************************************************
SoftwareSerial mySoftwareSerial(SSRX_PIN,SSTX_PIN);
DFRobotDFPlayerMini myDFPlayer;

// *****************************************************
// ADS1115
// *****************************************************/

Adafruit_ADS1115 ads1115;
const uint16_t GOALDETECT = 5000; //sensibilité de detection

// *****************************************************
// Equipes, effets sonores et visuels
// *****************************************************/
EffetSonore effetson(&myDFPlayer);
EffetVisuel effetvis(&strip);

Equipe equipeBleu(0, &ads1115, &effetson, &effetvis); // test du constructeur complet
Equipe equipeRouge(1, &ads1115, &effetson, &effetvis);


/*******************************************************
/ Definition Boutons												
/ Define the number of devices we have in the chain and
/ the hardware interface
*******************************************************/


// define pin numbers for individual switches
OneButton butRed(BUTRED_PIN, true);
OneButton butBlue(BUTBLUE_PIN, true);


/*******************************************************
/ Methodes declenchees par Smartpins
/ boutons et encouragements
********************************************************/
//
void printscores(){
	  Serial.print(F("Score Rouge :"));
 	  Serial.println(equipeRouge.getScore());
 	  Serial.print(F("Score Bleu :"));
 	  Serial.println(equipeBleu.getScore());
}



// boutton rouge
void redClick()
{
pause = false; // action sur bouton enleve la pause
Serial.println();
Serial.println(F("Bouton Rouge - plus 1 rouge"));
equipeRouge.increaseScore(); // appui court score +1
printscores();
}

// boutton bleu
void blueClick()
{
pause = false;// action sur bouton enleve la pause
Serial.println();
Serial.println(F("Bouton bleu - plus 1 bleu"));
equipeBleu.increaseScore(); // appui court score +1
printscores();
}



// Encouragements Rouges
void redcheer()
{
	Serial.println();
	Serial.println(F("Allez les rouges"));
	if (pause == false) equipeRouge.cheer(); //si pas en pause
}

// Encouragements Bleus
void bluecheer()
{
	Serial.println();
	Serial.println(F("Allez les bleus"));
	if (pause == false) equipeBleu.cheer(); //si pas en pause
}


// *****************************************************
// setup
// *****************************************************/
void setup() {
	mySoftwareSerial.begin(9600);
	Serial.begin(115200);

	WiFiManager wifiManager;
	wifiManager.autoConnect("AutoConnectAP");

	while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  	}
  
	// on affiche l'adresse IP attribuée pour le serveur DSN
	Serial.println("");
	Serial.print("IP address: ");
	Serial.println(WiFi.localIP());
  
	//******* OTA ***************
	// Hostname defaults to esp8266-[ChipID]
	ArduinoOTA.setHostname("ESPTEST");
	ArduinoOTA.begin();
	//********* Fin OTA ***************


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
	myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD); // lecture sur carte SD
	myDFPlayer.volume(25);  //Set volume value. From 0 to 30
	
	delay(1000);
	
	Serial.println(F("DFPlayer Mini En ligne."));
	
	// Initialize Afficheur 7 segments " 0: 0" faudra creer une fonction afficher (score1, score2)
	scoreboard.begin(0x70);
    afficherScore(equipeBleu.getScore(),equipeRouge.getScore());


// initialize Buttons

	butRed.attachClick(redClick);
	butBlue.attachClick(blueClick);

	// Convertisseur analogiques pour buts
	ads1115.begin();
	
	Serial.println();
	Serial.println("ADS1115 demarre");
	
		
	// Initialize Ledstrip
  	strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
	strip.show();            // Turn OFF all pixels ASAP
	strip.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)
	
	Serial.println();
	Serial.println("strip demarre");
  	//effetvis.theaterChaseRainbow(10000);
	
	// Initialize les equipes
	equipeRouge.setFolderCheer(1);
	equipeRouge.setFolderGoal(2);
	equipeRouge.setFolderWin(3);
	equipeRouge.setEffetSonore(&effetson);
	equipeRouge.setEffetVisuel(&effetvis);

	equipeBleu.setFolderCheer(4);
	equipeBleu.setFolderGoal(5);
	equipeBleu.setFolderWin(6);
	equipeBleu.setEffetSonore(&effetson);
	equipeBleu.setEffetVisuel(&effetvis);
	
	myDFPlayer.play(1);
	
}

void loop() {
  
 /*
 Routine pour introduire et modifier la frequence des encouragements
 spRed.everyRandom(bluermin, bluermax, redcheer); // encouragements aleatoires
 spBlue.everyRandom(redrmin, redrmax, bluecheer); // encouragements aleatoires
 */
butRed.tick();
butBlue.tick();
/*
spRed.loop();// teste les boutons
spBlue.loop();
*/

afficherScore(equipeBleu.getScore(),equipeRouge.getScore());

// Surveillance des demandes de mise à jour en OTA
  ArduinoOTA.handle();

if (pause == false) {
	//printscores(); // debug
	if(equipeBleu.testgoal(GOALDETECT) == true){
		Serial.println();
	    Serial.println(F("But bleu !"));
		if ((equipeBleu.getScore() + 1 >= SCOREVICTOIRE) && (equipeBleu.getScore()+ 1 - SCOREECART >= equipeRouge.getScore())){
			Serial.println();
			Serial.println("Victoire Bleus");
			equipeBleu.win();
			pause = true; // met en pause action sur un bouton pour repartir
		}
		else equipeBleu.goal();
	}
	if(equipeRouge.testgoal(GOALDETECT) == true){
		Serial.println();
	    Serial.println(F("But Rouge !"));
		if ((equipeRouge.getScore() + 1 >= SCOREVICTOIRE) && (equipeRouge.getScore()+ 1 - SCOREECART >= equipeBleu.getScore())){
			Serial.println();
			Serial.println("Victoire Rouge");
			equipeRouge.win();
			pause = true; // met en pause action sur un bouton pour repartir
		}
		else equipeRouge.goal();
	}
}

}
