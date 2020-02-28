#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioPlaySdWav           playSdWav1;     //xy=448,280
AudioOutputAnalog        dac1;           //xy=761,284
AudioConnection          patchCord1(playSdWav1, 0, dac1, 0);
// GUItool: end automatically generated code

int WaterSensorPin = A13;  
int WaterSensorValue = 0;    

int RightLightSensor = A14; //intializing left light sensor pin
int LeftLightSensor = A15; //intializing right light sensor pin
int LeftLightSensorValue = 0;
int RightLightSensorValue = 0;
int count = 0;
int Sneeds = 0; //Plant priortizes water so this int is here to help check if water needs are met.

void setup() {
  pinMode(WaterSensorPin, INPUT);
  Serial.begin(9600);
  while (!Serial && millis() < 2500) ; // wait
  Serial.println("Stereo DAC test");

  AudioMemory(12);
  
  if (!(SD.begin(BUILTIN_SDCARD))) { // stop here, but print a message repetitively
    while (1) {
      Serial.println("Unable to access the SD card");
      delay(500);
    }
  }
}

void SunCheck() {//Checking for light; for the day?
  LeftLightSensorValue = analogRead(LeftLightSensor);
  RightLightSensorValue = analogRead(RightLightSensor);
  if (count == 0){
    while (RightLightSensorValue < 800 && LeftLightSensorValue < 800){ // Checking if light levels are enough
      // print out value read:
      Serial.print(RightLightSensorValue);
      Serial.print("\t");
      Serial.println(LeftLightSensorValue);
      delay(1);
    
      //plays noise to signal needs more light   
      Serial.println("Need light"); 
      playSdWav1.play("WSUN.WAV");
      delay(1000);
      LeftLightSensorValue = analogRead(LeftLightSensor);
      RightLightSensorValue = analogRead(RightLightSensor);
      delay(100);
    } 
    playSdWav1.play("RSUN.WAV");
    delay(2000);
    Serial.println("Getting enough light");
    playSdWav1.stop();
    delay(3000);
    playSdWav1.play("TSUN.WAV");
    delay(8000);
    playSdWav1.stop();
    Serial.println("Done with light");
    count++;
   }
   else if(count != 0 && RightLightSensorValue > 800 && LeftLightSensorValue > 800) {
    playSdWav1.play("TSUN.WAV");
    delay(1000);
    playSdWav1.stop();
    Serial.println("Still done with light");
    Sneeds = 1; //Plant priortizes water so this int is here to help check if water needs are met.    
   }
    else {
   }
 }

void WaterCheck() { // Checking water moisture levels, check to make less finiqy
  WaterSensorValue = analogRead(WaterSensorPin);
  Serial.println(WaterSensorValue);

  //if plant is dehydrated
  if(WaterSensorValue > 500) { 
    playSdWav1.play("WWATER.WAV");
    delay(2000);   
    playSdWav1.stop(); 
  } 

  //if plant is overwatered
  else if(WaterSensorValue < 500) {
    playSdWav1.play("TWATER.WAV");
    delay(2000);
    playSdWav1.stop();
  }
  
  else {
    playSdWav1.stop();
    delay(600);
  }
}

void loop() {
 Serial.println(WaterSensorValue);
  if(Sneeds ==0)
  {
    Serial.println("Sun");
  SunCheck();
  }
  if(Sneeds == 1){
    Serial.println("Water");
     WaterCheck();
    
  }
 
  delay(600);
}

// FILES CURRENTLY ON SD CARD:
//WSUN.WAV
//WWATER.WAV
//TWATER.WAV
//TSUN.WAV
//RWATER.WAV
//RSUN.WAV
