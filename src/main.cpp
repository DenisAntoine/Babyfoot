#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <DFRobotDFPlayerMini.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <Wire.h>
#include <OneButton.h>

#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>

/*
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
*/

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
#define SSRX_PIN   	D4
#define SSTX_PIN  	D3
#define SOUND_PIN  	D7

const int SCOREVICTOIRE = 5;
const int SCOREECART = 2;
bool pause = false;
unsigned long timepause = 0;
unsigned long periodeSon = 30000;
unsigned long periodeCheerMin = 20000;
unsigned long periodeCheerMax = 100000;
int scompt =1;
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
const uint16_t GOALDETECT = 3000; //sensibilité de detection

// *****************************************************
// Equipes, effets sonores et visuels
// *****************************************************/
EffetSonore effetson(&myDFPlayer);
EffetVisuel effetvis(&strip);

Equipe equipeBleu(1,COL_BLUE, &ads1115, &effetson, &effetvis); // test du constructeur complet
Equipe equipeRouge(0,COL_RED, &ads1115, &effetson, &effetvis);
Equipe equipeBlanche(2,COL_WHITE, &ads1115, &effetson, &effetvis);

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
/*
void printscores(){
	  Serial.print(F("Score Rouge :"));
 	  Serial.println(equipeRouge.getScore());
 	  Serial.print(F("Score Bleu :"));
 	  Serial.println(equipeBleu.getScore());
}
*/


// boutton rouge
void redClick()
{
pause = false; // action sur bouton enleve la pause
//Serial.println();
//Serial.println(F("Bouton Rouge - plus 1 rouge"));
equipeRouge.increaseScore(); // appui court score +1
//printscores();
}

// boutton bleu
void blueClick()
{
pause = false;// action sur bouton enleve la pause
//Serial.println();
//Serial.println(F("Bouton bleu - plus 1 bleu"));
equipeBleu.increaseScore(); // appui court score +1
//printscores();
}
void redLong(){
	pause = false;// action sur bouton enleve la pause
	equipeRouge.resetScore();
}
void blueLong(){
	pause = false;// action sur bouton enleve la pause
	equipeBleu.resetScore();
}

// Encouragements Rouges
void redcheer()
{
	//Serial.println();
	//Serial.println(F("Allez les rouges"));
	if (pause == false) equipeRouge.cheer(); //si pas en pause
}

// Encouragements Bleus
void bluecheer()
{
	//Serial.println();
	//Serial.println(F("Allez les bleus"));
	if (pause == false) equipeBleu.cheer(); //si pas en pause
}


// *****************************************************
// setup
// *****************************************************/
void setup() {
	mySoftwareSerial.begin(9600);
	Serial.begin(115200);

	delay(1000);

// Initialize sound sensor 	
// pinMode(SOUND_PIN,INPUT);


// Initialize Afficheur 7 segments " 0: 0" 
	scoreboard.begin(0x70);
	afficherScore(0,0);
	//Serial.println();
	//Serial.println(F("Afficheur demarre"));
	delay(1000);

// Initialize Ledstrip
  	strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
	strip.show();            // Turn OFF all pixels ASAP
	strip.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)
	Serial.println();
	Serial.println("strip demarre");
	
/*

	WiFiManager wifiManager;
	wifiManager.autoConnect("AutoConnectAP");

	while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    //Serial.print(".");
  	}
  
	// on affiche l'adresse IP attribuée pour le serveur DSN
	//Serial.println("");
	//Serial.print("IP address: ");
	//Serial.println(WiFi.localIP());
  
	//OTA
	// Hostname defaults to esp8266-[ChipID]
	//ArduinoOTA.setHostname("ESPTEST");
	//ArduinoOTA.begin();
	// Fin OTA 
	//Serial.println("");
	//Serial.print("code modifie par OTA");
*/
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
    
 	myDFPlayer.setTimeOut(500); // Définit un temps de time out sur la communication série à 500 ms
	myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD); // lecture sur carte SD
	myDFPlayer.volume(25);  //Set volume value. From 0 to 30
	
	delay(1000);
	
	Serial.println(F("DFPlayer Mini En ligne."));
	
	

// initialize Buttons

	butRed.attachClick(redClick);
	butBlue.attachClick(blueClick);
	butRed.attachLongPressStop(redLong);
	butBlue.attachLongPressStop(blueLong);
	// Convertisseur analogiques pour buts
	ads1115.begin();
	
	Serial.println();
	Serial.println("ADS1115 demarre");
	
		
	
	// Initialize les equipes
	equipeRouge.setFolderCheer(1);
	equipeRouge.setFolderGoal(2);
	equipeRouge.setFolderWin(3);
	

	equipeBleu.setFolderCheer(4);
	equipeBleu.setFolderGoal(5);
	equipeBleu.setFolderWin(6);
	
	equipeBlanche.setFolderCheer(7);
	equipeBlanche.setFolderGoal(8);
	equipeBlanche.setFolderWin(9);

	Serial.println();
	Serial.println("equipes initialisees");

	myDFPlayer.play(1);
	effetvis.flash(COL_RED, 1, 30);
	effetvis.flash(COL_BLUE, 30, LED_COUNT);
	effetvis.rainbow(10);
	delay(1000);

	Serial.println();
	Serial.println("fin setup");
}

void loop() {

unsigned long cTime = millis();

butRed.tick();
butBlue.tick();

afficherScore(equipeRouge.getScore(),equipeBleu.getScore());

// Surveillance des demandes de mise à jour en OTA
//ArduinoOTA.handle();

if (pause == false) 
{
	//printscores(); // debug
	if(equipeBleu.testgoal(GOALDETECT ) == true)
	{
		//Serial.println();
	    //Serial.println(F("But bleu !"));
		afficherScore(equipeRouge.getScore(),equipeBleu.getScore()); //affichage immediat
		if ((equipeBleu.getScore() >= SCOREVICTOIRE) && (equipeBleu.getScore() - SCOREECART >= equipeRouge.getScore())){
			//Serial.println();
			//Serial.println("Victoire Bleus");
			equipeBleu.win();
			pause = true; // met en pause action sur un bouton pour repartir
			timepause = millis();
		}
		else equipeBleu.goal();
	}
	if(equipeRouge.testgoal(GOALDETECT *2.8) == true)
	{
		//Serial.println();
	    //Serial.println(F("But Rouge !"));
		afficherScore(equipeRouge.getScore(),equipeBleu.getScore());//affichage immediat
		if ((equipeRouge.getScore() >= SCOREVICTOIRE) && (equipeRouge.getScore() - SCOREECART >= equipeBleu.getScore()))
		{
			//Serial.println();
			//Serial.println("Victoire Rouge");
			equipeRouge.win();
			pause = true; // met en pause action sur un bouton pour repartir
			timepause = millis();
		}
		else equipeRouge.goal();
	}
	cTime = millis();
	if (cTime > effetson.getLastSound() + periodeSon) {
		if (cTime > max(equipeBleu.getLastGoal(), equipeRouge.getLastGoal()) + periodeSon) // pas de but depuis x sec
		{
			equipeBlanche.cheer();//on motive le groupe equipe neutre
		}
	}
}
else if (cTime > timepause+2000) { //fin pause
	pause = false;
	timepause = 0;
	equipeRouge.resetScore();
	equipeBleu.resetScore();
	}

}
