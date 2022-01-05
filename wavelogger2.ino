//Pressure and temperature logger to run on an adafruit adalogger 32u4 with precision RTC featherwing board
//Set your sampling frequency using "senseInterval" variable. Log writing frequency is independent of sampling frequency and can be set by logInterval
//battery voltage logging can be turned on or off
//To set the rtc, use sketch "ds3231" in the RTC examples

#include <MS5837.h>
#include <Wire.h>
#include <RTClib.h>
#include <SD.h>
#define VBATPIN A9 
MS5837 sensor;
RTC_DS3231 rtc;

//User settings//
int logInterval = 4000; //milliseconds between data saves to the SD card *note max of 32000 allowed with an int data type
int senseInterval = 200; //milliseconds between data read attempts for pressure and temp sensor. 
                        //Data reading +overhead takes about 50ms so a value of 200 here gives ~4Hz,
                        //Fastest achievable is about 18Hz by setting senseInterval low (5ms)
bool recordBatVolts = true; //record or don't record battery voltage


unsigned long lastSenseMils = 0;
unsigned long lastLogMils = 0;
File logFile;
int led = 13;

void setup() {
  Serial.begin(57600);
  //add a light so we can see the file write is happening

  pinMode(led, OUTPUT); 
  Serial.println("Starting");
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    abort();
  } 

  delay(2000);
  openLogFile();
  
  Wire.begin();
  // Initialize pressure sensor
  // Returns true if initialization was successful
  // We can't continue with the rest of the program unless we can initialize the sensor
  while (!sensor.init()) {
    Serial.println("Pressure sensor init failed!");
    delay(1000);
  }
  // .init sets the sensor model for us but we can override it if required.
  // Uncomment the next line to force the sensor model to the MS5837_30BA.
  sensor.setModel(MS5837::MS5837_30BA);
  sensor.setFluidDensity(1029); // kg/m^3 (freshwater, 1029 for seawater)

}

void loop() {
  // put your main code here, to run repeatedly:
  if( (millis()-lastSenseMils) > senseInterval){
    doSensing();
    lastSenseMils = millis();
  }
  if( (millis()-lastLogMils) > logInterval){
    digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
    logFile.flush();
    lastLogMils = millis();
    digitalWrite(led, LOW);   // turn the LED on (HIGH is the voltage level)
  }
  //delay(10);
}

void doSensing() {
  // Update pressure and temperature readings
  sensor.read();
  
  /* //Serial debug
  Serial.print("Pressure: ");
  Serial.print(sensor.pressure());
  Serial.println(" mbar");

  Serial.print("Temperature: ");
  Serial.print(sensor.temperature());
  Serial.println(" deg C");

  Serial.print("Depth: ");
  Serial.print(sensor.depth(),4);
  Serial.println(" m");

  Serial.println(millis()%1000);
  */
  
  DateTime now = rtc.now(); 
  logFile.print(now.year(), DEC);logFile.print(' ');
  logFile.print(now.month(), DEC);logFile.print(' ');
  logFile.print(now.day(), DEC);logFile.print(' ');
  logFile.print(now.hour(), DEC);logFile.print(' ');
  logFile.print(now.minute(), DEC);logFile.print(' ');
  logFile.print(now.second(), DEC);logFile.print(' ');
  logFile.print(millis()%1000);logFile.print(" ");
  
  logFile.print(sensor.pressure());logFile.print(" ");
  logFile.print(sensor.temperature());logFile.print(" ");

  if(recordBatVolts){
    float measuredvbat = analogRead(VBATPIN);
    measuredvbat *= 2;    // we divided by 2, so multiply back
    measuredvbat *= 3.3;  // Multiply by 3.3V, our reference voltage
    measuredvbat /= 1024; // convert to voltage
    logFile.println(measuredvbat);
    //Serial.print("VBat: " ); Serial.println(measuredvbat);
  } else{           
     logFile.println(" ");
  }
}


void openLogFile(){
  Serial.print("Initializing SD card...");
  pinMode(4, OUTPUT);
 
  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("SD initialization done.");
  
  DateTime now = rtc.now();
  char timeStamp[24] = "YYYYMMDD.txt";
  logFile = SD.open(now.toString(timeStamp), FILE_WRITE);
  //logFile = SD.open(timeStamp, FILE_WRITE);
  if(logFile){
    Serial.println("opened logFile");
    //logFile.println("new start");
    logFile.println("YYYY MM DD hh mm ss millisecs Pressure(mB) Temp(C) BatteryVolts");    
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening log file");
  }
  //logfile.close; 
  return logFile; 
}

