#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <DHT.h>
#include <DHT_U.h>
#include <RTClib.h>
#include <Wire.h>

#define LED_PIN LED_BUILTIN
#define CS_PIN 5
#define RTC_SDA 21
#define RTC_SCL 22
#define DHTTYPE DHT22

// Defina os pinos dos sensores DHT22
#define DHT_PIN_COUNT 7
int DHT_PINS[DHT_PIN_COUNT] = {14, 4, 25, 33, 26, 27, 13};

DHT dhts[DHT_PIN_COUNT] = {
  DHT(DHT_PINS[0], DHTTYPE),
  DHT(DHT_PINS[1], DHTTYPE),
  DHT(DHT_PINS[2], DHTTYPE),
  DHT(DHT_PINS[3], DHTTYPE),
  DHT(DHT_PINS[4], DHTTYPE),
  DHT(DHT_PINS[5], DHTTYPE),
  DHT(DHT_PINS[6], DHTTYPE)
};

RTC_DS3231 rtc;

void setup() {
  pinMode(LED_PIN, OUTPUT);

  Serial.begin(9600);
  while (!Serial) { }

  for (int i = 0; i < DHT_PIN_COUNT; i++) {
    dhts[i].begin();
  }

  Wire.begin(RTC_SDA, RTC_SCL);
  rtc.begin();
  rtc.adjust(DateTime((__DATE__), (__TIME__)));

  if (!SD.begin(CS_PIN)) {
    Serial.println("Error, SD Initialization Failed");
    return;
  }

  File testFile = SD.open("/test.txt", FILE_APPEND);
  if (testFile) {
    testFile.println("Ainda que ande pelo vale da morte, não temerei mal algum, pois o Senhor está comigo");
    Serial.println("Success, data written to test.txt");
  } else {
    Serial.println("Error, couldn't not open test.txt");
  }
}

void loop() {
  String linha = "";
  DateTime now = rtc.now();
  String data = String(now.day()) + "/" + String(now.month()) + "/" + String(now.year());
  String hora = String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second());

  for (int i = 0; i < DHT_PIN_COUNT; i++) {
    float humidity = dhts[i].readHumidity();
    float temperature = dhts[i].readTemperature();
    linha += "," + String(temperature) + "," + String(humidity);
    Serial.println(i);
    Serial.println("passei aq");
    delay(10000);
  }

  linha = data + "," + hora + linha;
  File testFile = SD.open("/data.txt", FILE_APPEND);
  if (testFile) {
    testFile.println(linha);
    digitalWrite(LED_PIN, HIGH);
    testFile.close();
    Serial.println("Success, data written to data.txt");
  } else {
    Serial.println("Error, couldn't not open data.txt");
    digitalWrite(LED_PIN, LOW);
  }

  Serial.print(linha);
  delay(5*60*1000);
}
