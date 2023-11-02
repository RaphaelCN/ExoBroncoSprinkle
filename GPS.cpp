#include "GPS.h"

#define RX_PIN 44
#define TX_PIN 43

// Initialize Serial:
HardwareSerial gpsSerial(2);
Adafruit_GPS GPS(&gpsSerial);

// Variable Values
int hour;
int minute;
int second;

int month;
int day;
int year;

float latitude;
float longitude;

float speed;
float angle;
float altitude;

uint32_t timer = millis();

void setupGPS()
{
  gpsSerial.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);
  // 9600 NMEA is the default baud rate for Adafruit MTK GPS's- some use 4800
  GPS.begin(9600);
  // Set to send recommended minimum (RMC) and fix data (GGA) including altitude
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  // Set 1Hz update rate
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
  // Request updates on antenna status, comment out to keep quiet
  GPS.sendCommand(PGCMD_ANTENNA);
}

void readGPS()
{
  char c = GPS.read();
  // Sentence received, check checksum
  if (GPS.newNMEAreceived()) 
  {
    // Checksum passed, try parsing
    if (!GPS.parse(GPS.lastNMEA()))   
      return;  // If parsing fails, wait for new sentence
  }

  if (millis() - timer > 2000)
  {
    timer = millis();
  
    hour = (int) GPS.hour;
    minute = (int) GPS.minute;
    second = (int) GPS.seconds;

    month = (int) GPS.month;
    day = (int) GPS.day;
    year = (int) GPS.year;

    if (GPS.fix) 
    {
      latitude = (float) GPS.latitudeDegrees;
      longitude = (float) GPS.longitudeDegrees;
      speed = (float) GPS.speed;
      angle = (float) GPS.angle;
      altitude = (float) GPS.altitude;
    }
  }
}

int getHour() {return hour;}
int getMinute() {return minute;}
int getSecond() {return second;}
int getMonth() {return month;}
int getDay() {return day;}
int getYear() {return year;}
float getLat() {return latitude;}
float getLong() {return longitude;}
float getSpeed() {return speed;}
float getAngle() {return angle;}
float getAltitude() {return altitude;}