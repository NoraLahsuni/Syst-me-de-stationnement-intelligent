#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <ESP32Servo.h>      // ✅ ESP32 compatible Servo
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

// --- WiFi ---
#define WIFI_SSID "La_Fibre_dOrange_DD0B"
#define WIFI_PASSWORD "G454NRA32A3K9EHQTE"

// --- Firebase ---
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// --- LCD ---
LiquidCrystal_I2C lcd(0x27, 16, 2);

// --- Servo ---
Servo servo;
#define SERVO_PIN 23

// --- Ultrason ---
#define TRIG 19
#define ECHO 18

// --- Variables ---
float distance;
String placeState;

// --- Mesure distance ---
float getDistance() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  long duration = pulseIn(ECHO, HIGH);
  return (duration * 0.0343) / 2; // cm
}

void setup() {
  Serial.begin(115200);

  // --- LCD ---
  Wire.begin(21, 22); // SDA, SCL
  lcd.init();
  lcd.backlight();

  // --- Servo & Ultrason ---
  servo.setPeriodHertz(50);    // fréquence standard servo
  servo.attach(SERVO_PIN, 500, 2400); // min/max pulse ESP32
  servo.write(0);              // porte fermée

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  // --- WiFi ---
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connexion WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\n✅ WiFi connecté !");

  // --- Firebase ---
  config.database_url = "https://smart-parking-ef8b7-default-rtdb.firebaseio.com/";
  config.signer.tokens.legacy_token = "8fAaNf6ecD5pHZgcuZOaSxtB6XReFBfgdXeBtoOX";
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
  Serial.println("✅ Connecté à Firebase !");
}

void loop() {
  if (Firebase.RTDB.getString(&fbdo, "/ParkingLot/Place1")) {
    placeState = fbdo.stringData();
    Serial.println("🚗 Etat Firebase : " + placeState);

    distance = getDistance();
    Serial.print("📏 Distance : ");
    Serial.println(distance);

    if (placeState == "Libre" && distance < 20) {
      servo.write(90); // ouvre porte
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Bienvenue !");
      lcd.setCursor(0,1);
      lcd.print("Place dispo 🅿️");
    } 
    else if (placeState == "Occupée" && distance < 20) {
      servo.write(0); // ferme porte
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Parking complet");
      lcd.setCursor(0,1);
      lcd.print("🚫 Attendez...");
    } 
    else {
      servo.write(0);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Parking :");
      lcd.setCursor(0,1);
      lcd.print(placeState);
    }
  } else {
    Serial.println("❌ Erreur Firebase : " + fbdo.errorReason());
  }

  delay(2000);
}
