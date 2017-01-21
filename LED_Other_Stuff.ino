double brightness = MAX_BRIGHTNESS;
double inc = -1.0/20.0;

double driveBrightness = MAX_BRIGHTNESS;
double driveInc = -1.0/30.0;

unsigned long lastTime = millis();
bool isOn = true;

double maxVal = 0.0;
double minVal = 100.0;

void resetMain(){
  brightness = MAX_BRIGHTNESS;
  inc = -1.0/20.0;
}

void resetDrive(){
  driveBrightness = MAX_BRIGHTNESS;
  driveInc = -1.0/30.0;
}

void autoRedRun(int* pins,int numPins){
    red(brightness,pins,numPins);
    //cycle through bright and dim
    if(brightness + inc < 0.0 || brightness + inc > MAX_BRIGHTNESS){
      inc = inc*-1.0;
    }
    brightness += inc;
}

void autoBlueRun(int* pins,int numPins){
    blue(brightness,pins,numPins);
    //cycle through bright and dim
    if(brightness + inc < 0.0 || brightness + inc > MAX_BRIGHTNESS){
      inc = inc*-1.0;
    }
    brightness += inc;
}

void teleopNoBallShooterOffRun(int* pins,int numPins){
  yellow(MAX_BRIGHTNESS,pins,numPins);
}

void teleopNoBallShooterOnRun(int* pins,int numPins){
    yellow(brightness,pins,numPins);
    //cycle through bright and dim
    if(brightness + inc < 0.0 || brightness + inc > MAX_BRIGHTNESS){
      inc = inc*-1.0;
    }
    brightness += inc;
}

void shooterOnCanSeeSwagRun(int* pins,int numPins){
    green(brightness,pins,numPins);
    //cycle through bright and dim
    if(brightness + inc < 0.0 || brightness + inc > MAX_BRIGHTNESS){
      inc = inc*-1.0;
    }
    brightness += inc;
}

void shooterOnAimingRun(int* pins,int numPins){
  double wait = 150;
  //FLASH lights
    if(millis() - lastTime > wait){
      if(isOn){
        turnOff(pins,numPins);
        isOn = false;
      }else{
        green(MAX_BRIGHTNESS,pins,numPins);
        isOn = true;
      }
      lastTime = millis();
    }
}

void afterShootingRun(int* pins,int numPins){
  rainbow(MAX_BRIGHTNESS,pins,numPins,false);
}

void drivetrainUp(int* pins,int numPins){
    red(driveBrightness,pins,numPins);
    //cycle through bright and dim
    if(driveBrightness + driveInc < 0.0 || driveBrightness + driveInc > MAX_BRIGHTNESS){
      driveInc = driveInc*-1.0;
    }
    driveBrightness += driveInc;
}

void drivetrainDown(int* pins,int numPins){
    purple(driveBrightness,pins,numPins);
    //cycle through bright and dim
    if(driveBrightness + driveInc < 0.0 || driveBrightness + driveInc > MAX_BRIGHTNESS){
      driveInc = driveInc*-1.0;
    }
    driveBrightness += driveInc;
}

void aiming(int* pins, int numPins, byte angleIn){
  boolean right = false;
  int angle = angleIn - 5;
  if(angle > 0){
    right = true;
  }
  unsigned int red = Color(0,0,(int)(MAX_BRIGHTNESS*31.0));
  unsigned int greenColor = Color(0,(int)(MAX_BRIGHTNESS*31.0),0);
  if(angle == 0){
    green(MAX_BRIGHTNESS,pins,numPins);
  }else if(right){
    for(int i = angle;i < numPins;i++){
      strip.setPixelColor(pins[i],greenColor);
    }
    for(int i = angle - 1;i >= 0;i--){
      strip.setPixelColor(pins[i], red);
    }
    strip.show();
  }else{
    byte negAngle = angle + numPins;
    for(int i = negAngle;i < numPins;i++){
      strip.setPixelColor(pins[i],red);
    }
    for(int i = negAngle - 1;i >= 0;i--){
      strip.setPixelColor(pins[i], greenColor);
    }
    strip.show();
  }
}

void wheelMove(int* pins,int numPins, byte pos){
  pos -= 100;
  unsigned int red = Color(0,0,(int)(MAX_BRIGHTNESS*31.0));
  unsigned int purple = Color((int)(MAX_BRIGHTNESS*31.0),0,(int)(MAX_BRIGHTNESS*31.0));
  if(pos == 0){
    drivetrainDown(pins,numPins);
  }else if(pos == numPins / 2){
    drivetrainUp(pins,numPins);
  }else{
    for(int i = pos;i < numPins/2;i++){
      strip.setPixelColor(pins[i], purple);
      strip.setPixelColor(pins[numPins - i - 1], purple);
    }
    for(int i = pos - 1;i >= 0;i--){
      strip.setPixelColor(pins[i], red);
      strip.setPixelColor(pins[numPins - i - 1], red);
    }
    strip.show();
  }
}

void PARTYSWAGHWEHWEHWE_NOAM_IS_WRONG(int* pins,int numPins,bool usePremadeRainbow){
  const int sampleWindow = 40; // Sample window width in mS (50 mS = 20Hz)
  unsigned int sample;
   unsigned long startMillis= millis();  // Start of sample window
   unsigned int peakToPeak = 0;   // peak-to-peak level
 
   unsigned int signalMax = 0;
   unsigned int signalMin = 1024;
 
   // collect data for 50 mS
   while (millis() - startMillis < sampleWindow)
   {
      sample = analogRead(0);
      if (sample < 1024)  // toss out spurious readings
      {
         if (sample > signalMax)
         {
            signalMax = sample;  // save just the max levels
         }
         else if (sample < signalMin)
         {
            signalMin = sample;  // save just the min levels
         }
      }
   }
   peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude
   double volts = (peakToPeak * 3.3) / 1024;  // convert to volts
   if(volts < minVal){
    minVal = volts;
   }
   if(volts > maxVal){
    maxVal = volts;
   }
   if(maxVal > 1.0){
    maxVal = 1.0;
   }
   volts = newMap(volts,minVal,maxVal,MAX_BRIGHTNESS / 4.0,MAX_BRIGHTNESS);
   if(volts < 0.1){
    volts = 0.0;
   }else if(volts > 0.7){
    volts = 1.0;
   }
   /*Serial.println(volts);
   Serial.print("min: ");
   Serial.print(minVal);
   Serial.print(", max: ");
   Serial.println(maxVal);*/
   if(usePremadeRainbow){
    rainbow(volts,pins,numPins,usePremadeRainbow);
   }else{
    green(volts,pins,numPins);
   }
   //green(volts,pins,numPins);
}

double newMap(double x, double in_min, double in_max, double out_min, double out_max)
{
 return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
