#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>  // ⚠️ nouvelle bibliothèque
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

// --- Identifiants WiFi ---
#define WIFI_SSID "La_Fibre_dOrange_DD0B"
#define WIFI_PASSWORD "G454NRA32A3K9EHQTE"

// --- Broche du capteur IR ---
#define IR1 D5   // exemple D5 sur ESP8266

// --- Objets Firebase ---
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
Firebase_ESP_Client fbClient;  // ⚠️ nouvel objet principal

void setup() {
  Serial.begin(115200);
  pinMode(IR1, INPUT);

  // Connexion WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connexion au WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\n✅ Connecté au WiFi !");

  // ---- Configuration Firebase ----
  config.database_url = "https://smart-parking-ef8b7-default-rtdb.firebaseio.com/";
  config.signer.tokens.legacy_token = "8fAaNf6ecD5pHZgcuZOaSxtB6XReFBfgdXeBtoOX";

  // Initialisation Firebase
  fbClient.begin(&config, &auth);
  fbClient.reconnectWiFi(true);

  Serial.println("✅ Connecté à Firebase !");
}

void loop() {
  int ir1 = digitalRead(IR1);
  String state1 = (ir1 == LOW) ? "Occupée" : "Libre"; // LOW si capteur détecte voiture

  // --- Envoi à Firebase ---
  if (fbClient.RTDB.setString(&fbdo, "/ParkingLot/Place1", state1)) {
    Serial.println("✅ Mise à jour Firebase réussie");
  } else {
    Serial.println("❌ Erreur Firebase : " + fbdo.errorReason());
  }

  Serial.println("Place1: " + state1);
  delay(2000);
}
