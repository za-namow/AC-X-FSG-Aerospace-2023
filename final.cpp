//Ac x CG Aerospace Camp 2023 "CubeSAT Operation"

//Download some of the libraries if the code failed to run when connected with components

//Sensors:
//-DS18B20 EXT.
//-DS3231
//-ATGM336H
//-PMS7003
//Micro SD Card
//LoRa

//Board: Arduino MEGA2560

#include <Wire.h>
#include <DS3231.h>
#include <TinyGPSPlus.h> 
#include <SPI.h> 
#include <SD.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 22 //กำหนดขาที่จะเชื่อมต่อ Sensor
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress insideThermometer;
#include "PMS.h"

PMS pms(Serial1);
PMS::DATA data;

TinyGPSPlus gps;
DS3231 rtc(A1,A2); 
char GPST[32];
char dataFile2[100];
float TMP = 0;
float TM = 0;
String T = "";

float GLAT = 0;
float GLON = 0;
float GALT = 0;
float GSATS = 0;
float dust = 0;
char GPSTime[32] = "xx:xx:xx";

String PKT = "";

uint32_t d1 = 0;
uint32_t d2 = 0;

String Tele = "";

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial2.begin(9600);
  Serial3.begin(9600);
  Wire.begin();
  rtc.begin();
  sensors.begin();
if (!SD.begin(53)) {
    Serial.println("Card failed, or not present");            //<-----Micro SD Card
    // don't do anything more:
  }

}
void DS(){
  T = rtc.getTimeStr();
  TMP = rtc.getTemp();
//  Serial.println(TMP);
}
void GPS (){
  while (Serial2.available()){
    gps.encode(Serial2.read());
  }
  if (gps.location.isValid()){
  GLAT = float(gps.location.lat());
  GLON = float(gps.location.lng());
  GALT = (gps.altitude.meters());
  GSATS = (gps.satellites.value());
  sprintf(GPSTime, "%02d:%02d:%02d ", gps.time.hour()+7, gps.time.minute(), gps.time.second());
  }
}

void DSB(){
  sensors.requestTemperatures(); 
  TM = sensors.getTempCByIndex(0);
}
void PMs(){
 if (pms.read(data))
  {
    dust = data.PM_AE_UG_2_5;
  }
}


long FALT = GALT;

unsigned long counter = 0;
void loop() {
  d2 = millis();
  GPS();
  if (d2 - d1 > 999) {
    DS();
    DSB();
    PMs(); 


long firstalti = GALT - FALT;

  String id = String("03");
  String tim = String(GPSTime);
  String pktc = String(counter);
  String alti = String(firstalti);
  String temp = String(TMP); //Sensor 1 DS3231
  String volt = String("5");
  String gpst = String(GPSTime);
  String gpal = String(GALT);
  String lati = String(GLAT); 
  String logi = String(GLON);
  String sats = String(GSATS);
  String pms = String(dust);
  String dsb = String(TM); //Sensor 2 DS18B20 ext.

  Tele = "TeamID: " + id + "," + tim + "," + "pkt: " + pktc + "," + "Ground Alti: " + alti + "m" + "," + "Sensor 1: " + temp + "C" + "," + volt + "," + gpst + "," + "Altitude" + gpal + "m" + "," + lati + "องศาเหนือ" + "," + logi + "องศาตะวันตก" + "," + sats + "SAT" + "," + "Dust: " + pms + "," + "Sensor 2: " + dsb + "C";
  counter++;
  d1 = d2;
  Serial3.println(Tele);
  Serial.println(Tele);
  }
File dataFile2 = SD.open("SGER.txt", FILE_WRITE);
  // if the file is available, write to it:
  if (dataFile2) {
    dataFile2.println(Tele);                                  //<------Micro SD Card
    dataFile2.close();
  }
  else {
    Serial.println("error opening datalog.txt");
  }
  }
