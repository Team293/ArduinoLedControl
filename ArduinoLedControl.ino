#include <Wire.h>
#include <TimerOne.h>
#include <SPI.h>
#include "Neophob_LPD6803.h"

/***********************CODES*******************************/
//Main Codes
#define AutoRed 255
#define AutoBlue 254
#define TeleopNoBallShooterOff 253
#define TeleopNoBallShooterOn 252
#define ShooterOnCanSeeSwag 251
#define AfterShooting 250

//Drivetrain Codes
#define DrivetrainUp 231
#define DrivetrainDown 230

// All LEDs
#define PARTY 240

//pins 11 and 13
/**********************END CODES****************************/

#define MAX_BRIGHTNESS 1.0    //float from 0 to 1
#define LED_MODULES 6        //number of chips on led strip

//initialize LED codes
byte mainCode = PARTY;
byte driveCode = PARTY;
byte code = PARTY;

Neophob_LPD6803 strip = Neophob_LPD6803(LED_MODULES);

#define numMain 6
#define numDrive 0
#define numAll 6
#define AFTER_RUN_TIME 2500

int mainLeds[numMain];
int driveLeds[numDrive];

int allLeds[LED_MODULES];

unsigned long afterShootingTime = -AFTER_RUN_TIME;
bool partying = true;

bool partyDrive = true;
bool partyMain = true;

void setup() {
  Serial.begin(9600);
  //initialize LED arrays
  for(int i = 0;i < LED_MODULES;i++){
    allLeds[i] = i;
  }
  for(int i = numMain;i < LED_MODULES;i++){
    driveLeds[i-numMain] = i;
  }
  for(int i = 0;i < numMain;i++){
    mainLeds[i] = i;
  }
  createRainbow(1);
  Wire.begin(8);
  Wire.onReceive(recieveEvent);
  strip.setCPUmax(50);
  strip.begin(SPI_CLOCK_DIV64);
  strip.show();
  Serial.println("starting");
}

void loop() {
  /*if(millis() - afterShootingTime < AFTER_RUN_TIME){
    afterShootingRun(mainLeds,numMain);
  }else{
    selectMainCode();
  }
  selectDriveCode();*/
  purple(1.0,allLeds,6);
  strip.show();
  delay(1);
}

void selectDriveCode(){
  if(wheelIsMoving(driveCode)){
    wheelMove(driveLeds,numDrive,driveCode);
  }else{
    partyDrive = true;
  }
}

void selectMainCode(){
  if(isAiming(mainCode)){
    aiming(mainLeds,numMain,mainCode);
  }else{
  switch(mainCode){
    case AutoRed:
      autoRedRun(mainLeds,numMain);
      break;
    case AutoBlue:
      autoBlueRun(mainLeds,numMain);
      break;
    case TeleopNoBallShooterOff:
      teleopNoBallShooterOffRun(mainLeds,numMain);
      break;
    case TeleopNoBallShooterOn:
      teleopNoBallShooterOnRun(mainLeds,numMain);
      break;
    case ShooterOnCanSeeSwag:
      shooterOnCanSeeSwagRun(mainLeds,numMain);
      break;
    case AfterShooting:
      afterShootingRun(mainLeds,numMain);
      break;
    default:
      partyMain = true;
      break;
  }
  }
  if(partyDrive && partyMain){
    Serial.println("both");
    PARTYSWAGHWEHWEHWE_NOAM_IS_WRONG(allLeds,LED_MODULES,true);
  }else if(partyDrive){
    Serial.println("drive");
    PARTYSWAGHWEHWEHWE_NOAM_IS_WRONG(driveLeds,numDrive,false);
  }else if(partyMain){
    Serial.println("main");
    PARTYSWAGHWEHWEHWE_NOAM_IS_WRONG(mainLeds,numMain,false);
  }
}

bool isAiming( byte num){
  return (-1 < num && num < 11);
}

bool wheelIsMoving(byte num){
  return (99 < num && num < 106);
}

void recieveEvent(int howMany){
  while(Wire.available()){
    code = Wire.read();
    if(code != PARTY){
      partying = false;
    }
    if(code == AfterShooting){
      afterShootingTime = millis();
    }
    if(code > PARTY || code < 100){
      partyMain = false;
      if(mainCode != code){
        mainCode = code;
        resetMain();
      }
      selectMainCode();
    }else if(code < PARTY){
      partyDrive = false;
      if(driveCode != code){
        driveCode = code;
        resetDrive();
      }
      selectDriveCode();
    }else{
      if(!partying){
        partying = true;
        resetRainbow();
        PARTYSWAGHWEHWEHWE_NOAM_IS_WRONG(allLeds,LED_MODULES,true);
      }
    }
  }
}

void serialEvent() {
  Serial.println("dlskgfasd");
  while (Serial.available() > 0) {
    // get the new byte:
    int inChar = (int)Serial.parseInt();
    Serial.println(inChar);
    if(inChar != PARTY){
      partying = false;
    }
    if(inChar == AfterShooting){
      afterShootingTime = millis();
    }
    if(inChar > PARTY || inChar < 100){
      if(mainCode != inChar){
        partyMain = false;
        mainCode = inChar;
        resetMain();
      }
      selectMainCode();
    }else if(inChar < PARTY){
      if(driveCode != inChar){
        partyDrive = false;
        driveCode = inChar;
        resetDrive();
      }
      selectDriveCode();
    }else{
      if(!partying){
        partying = true;
        resetRainbow();
        PARTYSWAGHWEHWEHWE_NOAM_IS_WRONG(allLeds,LED_MODULES,true);
      }
    }
  }
}

