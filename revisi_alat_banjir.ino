//------------------------------------------------------------
//LIBRARY WEB SERVER
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <SPI.h>
#include <SD.h>
#include "RTClib.h"

//------------------------------------------------------------
//MEMORY
#include "WebPage.h"
File myFile;

//------------------------------------------------------------
//RTC
RTC_DS3231 rtc;
char daysOfTheWeek[7][12] = {"Minggu", "Senin", "Selasa", "Rabu", "Kamis", "Jumat", "Sabtu"};

//------------------------------------------------------------
//SENSOR ULTRASONIC SETUP
#define trig D1
#define echo D2
float duration, distance;

//------------------------------------------------------------
//SETUP TYPE DATA
String deepTime, modeMcu, tinggiAlat, DataKetinggian;
#define pinButton 3

//------------------------------------------------------------
//WIFI ACCESS POINT SETUP
#ifndef APSSID
#define APSSID "Alat 002"
#define APPSK  "alat0002"
#endif 

/* Set these to your desired credentials. */
const char *ssid = APSSID;
const char *password = APPSK;

ESP8266WebServer server(80);

//------------------------------------------------------------
// XML UNTUK JEMBATAN DATA MESIN DENGAN WEB
void buildXML() {
  DateTime now = rtc.now();
  XML = "<?xml version='1.0' encoding='utf-8'?>";
  XML += "<t>";
  XML += "<dataKetinggian>";
  myFile = SD.open("DataKedalamanAir.txt", FILE_READ);
  if (myFile) {
    while (myFile.available()) {
      XML += myFile.readStringUntil('\n');
    }
  } else {
    XML += "Belum ada Data!";
  }
  XML += "</dataKetinggian>";
  XML += "<tinggiair>";
  XML += distance;
  XML += "</tinggiair>";
  XML += "<deeptime>";
  XML += deepTime;
  XML += "</deeptime>";
  XML += "<rYear>";
  XML += now.year();
  XML += "</rYear>";
  XML += "<rMonth>";
  XML += now.month();
  XML += "</rMonth>";
  XML += "<rDay>";
  XML += now.day();
  XML += "</rDay>";
  XML += "<rHour>";
  if (now.hour() < 10) {
    XML += "0";
    XML += now.hour();
  } else {
    XML += now.hour();
  }
  XML += "</rHour>";
  XML += "<rMinute>";
  if (now.minute() < 10) {
    XML += "0";
    XML += now.minute();
  } else {
    XML += now.minute();
  }
  XML += "</rMinute>";
  XML += "</t>";
}

void handleXML() {
  buildXML();
  server.send(200, "text/xml", XML);
}

void handleRoot() {
  server.send(200, "text/html", dashboard);
  if (server.hasArg("reset")) {
    SD.remove("DataKedalamanAir.txt");
    myFile = SD.open("DataKedalamanAir.txt", FILE_WRITE);
    myFile.close();
  }
}

//------------------------------------------------------------
//SETUP
void setup() {
  Serial.begin(115200);
  Serial.println();
  Wire.begin(D3, D4);
  //SETUP PINMODE
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(pinButton, INPUT_PULLUP);

  Serial.println("Wake Up.");
  //CONFIGURING SD CARD
  if (!SD.begin(15)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
  //CONFIGURING RTC
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }
  delay(50);
  //READ DATA SD CARD
  myFile = SD.open("deepTime.txt", FILE_READ);
  if (myFile) {
    while (myFile.available()) {
      deepTime = myFile.readStringUntil('\n');
      Serial.print("waktu sleep : ");
      Serial.print(deepTime);
      Serial.print(" Menit");
      Serial.println();
    }
    myFile.close();
  }
  delay(1500);

  if (digitalRead(pinButton) == HIGH) {
    deepSleepMode();
    return;
  }
  //WIFI ACCESS POINT
  Serial.print("Configuring access point...");
  WiFi.softAP(ssid, password);
  Serial.println(ssid);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  //HANDLE CLIENT ROOT
  server.on("/", handleRoot);
  server.on("/setup", []() {
    server.send_P(200, "text/html", settingUp);
    if (server.hasArg("deeptime")) {
      const String data = server.arg("deeptime");
      SD.remove("deepTime.txt");
      myFile = SD.open("deepTime.txt", FILE_WRITE);
      myFile.println(data);
      myFile.close();
      return dataReader();
    }
    if (server.hasArg("date")) {
      uint16_t jam;
      uint8_t menit;
      uint8_t detik;
      String sd = server.arg("date");
      String lastSd;
      jam = ((sd[0] - '0') * 1000) + ((sd[1] - '0') * 100) + ((sd[2] - '0') * 10) + (sd[3] - '0');
      menit = ((sd[5] - '0') * 10) + (sd[6] - '0');
      detik = ((sd[8] - '0') * 10) + (sd[9] - '0');
      if (sd != lastSd) {
        DateTime now = rtc.now();
        uint8_t hour = now.hour();
        uint8_t minute = now.minute();
        rtc.adjust(DateTime(jam, menit, detik, hour, minute, 0));
        lastSd = sd;
      }
      server.send ( 404 , "text", message );
    }
    if (server.hasArg("time")) {
      String st = server.arg("time");
      String lastSt;
      uint8_t jam = ((st[0] - '0') * 10) + (st[1] - '0');
      uint8_t menit = ((st[3] - '0') * 10) + (st[4] - '0');
      if (st != lastSt) {
        DateTime now = rtc.now();
        uint16_t year = now.year();
        uint8_t month = now.month();
        uint8_t day = now.day();
        rtc.adjust(DateTime(year, month, day, jam, menit, 0));
        lastSt = st;
      }
      server.send ( 404 , "text", message );
    }
  });
  server.on("/table", []() {
    server.send_P(200, "text/html", datatable);
  });
  server.on("/xml", handleXML);
  server.begin();
  Serial.println("HTTP server started");
  dataReader();
}

//------------------------------------------------------------
//LOOP
void loop() {
  server.handleClient();

  //SENSOR ULTRASONIC
  digitalWrite(trig, LOW); delayMicroseconds(2);
  digitalWrite(trig, HIGH); delayMicroseconds(10);
  digitalWrite(trig, LOW);
  duration = pulseIn(echo, HIGH);
  distance = duration / 58.2;
  delay(50);
  Serial.print(distance);
  Serial.println(" Centimeter");
}

//------------------------------------------------------------
//READ DATA
void dataReader() {
  myFile = SD.open("tinggiAlat.txt", FILE_READ);
  if (myFile) {
    while (myFile.available()) {
      tinggiAlat = myFile.readStringUntil('\n');
    }
    myFile.close();
    delay(50);
  }
  myFile = SD.open("deepTime.txt", FILE_READ);
  if (myFile) {
    while (myFile.available()) {
      deepTime = myFile.readStringUntil('\n');
    }
    myFile.close();
    delay(50);
  }
}

void deepSleepMode() {
  digitalWrite(trig, LOW); delayMicroseconds(2);
  digitalWrite(trig, HIGH); delayMicroseconds(10);
  digitalWrite(trig, LOW);
  duration = pulseIn(echo, HIGH);
  distance = duration / 58.2;
  delay(50);
  Serial.print(distance);
  Serial.println(" Centimeter");


  //PRINT SERIAL MONITOR
  DateTime now = rtc.now();
  Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
  Serial.print(" ~ ");
  Serial.print(now.year());
  Serial.print("/");
  Serial.print(now.month());
  Serial.print("/");
  Serial.print(now.day());
  Serial.print(" - ");
  if (now.hour() < 10) {
    Serial.print("0");
    Serial.print(now.hour());
  } else {
    Serial.print(now.hour());
  }
  Serial.print(":");
  if (now.minute() < 10) {
    Serial.print("0");
    Serial.print(now.minute());
  } else {
    Serial.print(now.minute());
  }
  Serial.print(":");
  if (now.second() < 10) {
    Serial.print("0");
    Serial.print(now.second());
  } else {
    Serial.print(now.second());
  }
  Serial.print(" Ketinggian Air : ");
  Serial.print(distance);
  Serial.println();

  //PRINT DATA SD CARD
  myFile = SD.open("DataKedalamanAir.txt", FILE_WRITE);
  if (myFile) {
    myFile.print("___");
    myFile.print(daysOfTheWeek[now.dayOfTheWeek()]);
    myFile.print(" ~ ");
    myFile.print(now.year());
    myFile.print("/");
    myFile.print(now.month());
    myFile.print("/");
    myFile.print(now.day());
    myFile.print(" - ");
    if (now.hour() < 10) {
      myFile.print("0");
      myFile.print(now.hour());
    } else {
      myFile.print(now.hour());
    }
    myFile.print(":");
    if (now.minute() < 10) {
      myFile.print("0");
      myFile.print(now.minute());
    } else {
      myFile.print(now.minute());
    }
    myFile.print(":");
    if (now.second() < 10) {
      myFile.print("0");
      myFile.print(now.second());
    } else {
      myFile.print(now.second());
    }
    myFile.print(" Ketinggian Air : ");
    myFile.print(distance);
    myFile.print("___");
    myFile.println();
    myFile.close();
    delay(100);
    Serial.println("Data DiTambahkan Ke SD Card");
  }

  delay(200);

  //DEEP SLEEP
  Serial.println("Memasuki Mode Deep Sleep....");
  ESP.deepSleep(deepTime.toInt() * 60 * 1000000);
}
