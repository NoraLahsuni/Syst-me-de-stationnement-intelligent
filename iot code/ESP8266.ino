#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

// --- Identifiants WiFi ---
#define WIFI_SSID "Gal"
#define WIFI_PASSWORD "12345678"

// --- Broches des capteurs IR ---
#define IR1 D5
#define IR2 D6
#define IR3 D7
#define IR4 D2
#define IR5 D1

// --- Objets Firebase ---
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
Firebase_ESP_Client fbClient;

void setup() {
  Serial.begin(115200);

  // Configuration des broches IR
  pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);
  pinMode(IR3, INPUT);
  pinMode(IR4, INPUT);
  pinMode(IR5, INPUT);

  // Connexion WiFi
  Serial.println();
  Serial.print("Connexion au WiFi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\n✅ Connecté au WiFi !");

  // Configuration Firebase
  config.database_url = "https://smartparking-842d1-default-rtdb.firebaseio.com/";
  config.signer.tokens.legacy_token = "CxalmTmiQR5uFlTOmOEk2rfUajYX15l7Ht3yu1SX";

  fbClient.begin(&config, &auth);
  fbClient.reconnectWiFi(true);
  Serial.println("✅ Connecté à Firebase !");
}

void loop() {
  // Lecture des capteurs
  int ir1 = digitalRead(IR1);
  int ir2 = digitalRead(IR2);
  int ir3 = digitalRead(IR3);
  int ir4 = digitalRead(IR4);
  int ir5 = digitalRead(IR5);

  // Détermination de l'état de chaque place
  // HIGH = occupée, LOW = libre
  String state1 = (ir1 == HIGH) ? "Occupée" : "Libre";
  String state2 = (ir2 == HIGH) ? "Occupée" : "Libre";
  String state3 = (ir3 == HIGH) ? "Occupée" : "Libre";
  String state4 = (ir4 == HIGH) ? "Occupée" : "Libre";
  String state5 = (ir5 == HIGH) ? "Occupée" : "Libre";

  // Affichage dans le moniteur série
  Serial.println("------ État des places ------");
  Serial.println("Place 1: " + state1);
  Serial.println("Place 2: " + state2);
  Serial.println("Place 3: " + state3);
  Serial.println("Place 4: " + state4);
  Serial.println("Place 5: " + state5);
  Serial.println("-----------------------------");

  // Envoi des données à Firebase
  fbClient.RTDB.setString(&fbdo, "/ParkingLot/Place1", state1);
  fbClient.RTDB.setString(&fbdo, "/ParkingLot/Place2", state2);
  fbClient.RTDB.setString(&fbdo, "/ParkingLot/Place3", state3);
  fbClient.RTDB.setString(&fbdo, "/ParkingLot/Place4", state4);
  fbClient.RTDB.setString(&fbdo, "/ParkingLot/Place5", state5);

  delay(2000); // Attendre 2 secondes avant la prochaine mise à jourrr
}
