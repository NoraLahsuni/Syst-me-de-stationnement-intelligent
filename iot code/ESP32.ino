#include <ESP32Servo.h>
#include <Keypad.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

// ------------------- WIFI -------------------
#define WIFI_SSID "Gal"
#define WIFI_PASSWORD "12345678"

// ------------------- ULTRASON -------------------
#define TRIG 19
#define ECHO 18

// ------------------- SERVO -------------------
#define SERVO_PIN 23
Servo myServo;

// ------------------- KEYPAD -------------------
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {13,12,14,27};
byte colPins[COLS] = {26,25,33,32};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// ------------------- FIREBASE -------------------
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
Firebase_ESP_Client fbClient;

// ------------------- VARIABLES -------------------
long duration;
float distance;
const int totalPlaces = 5;

// ------------------- FONCTIONS -------------------
void reconnectWiFiFirebase() {
  if(WiFi.status() != WL_CONNECTED){
    WiFi.reconnect();
    Serial.println("Reconnexion WiFi...");
  }
  if(!fbClient.ready()){
    fbClient.begin(&config,&auth);
    Serial.println("Reconnexion Firebase...");
  }
}

int getFreePlaces() {
  int freeCount = 0;
  for(int i=1; i<=totalPlaces; i++){
    String path = "/ParkingLot/Place" + String(i);
    if(Firebase.RTDB.getString(&fbdo, path)){
      if(fbdo.stringData() == "Libre") freeCount++;
    }
  }
  return freeCount;
}

bool checkCodeInFirebase(const String &inputCode, String &spotId) {
  FirebaseJson json;
  FirebaseJson* jsonPtr = &json;
  FirebaseJsonData jsonData;

  if(fbClient.RTDB.getJSON(&fbdo, "/reservations", jsonPtr)){
    size_t len = json.iteratorBegin();
    for(size_t i=0; i<len; i++){
      int type;
      String key, value;
      if(json.iteratorGet(i, type, key, value) == 0){
        FirebaseJson reservation;
        reservation.setJsonData(value);

        reservation.get(jsonData, "reservationCode");
        String code = String(jsonData.intValue); 

        reservation.get(jsonData, "parkingSpot");
        String spot = jsonData.stringValue;

        if(code == inputCode){
          spotId = spot;
          json.iteratorEnd();
          return true;
        }
      }
    }
    json.iteratorEnd();
  }
  return false;
}

bool removeCodeFromFirebase(const String &inputCode){
  FirebaseJson json;
  FirebaseJson* jsonPtr = &json;
  FirebaseJsonData jsonData;

  if(fbClient.RTDB.getJSON(&fbdo, "/reservations", jsonPtr)){
    size_t len = json.iteratorBegin();
    for(size_t i=0; i<len; i++){
      int type;
      String key, value;
      if(json.iteratorGet(i, type, key, value) == 0){
        FirebaseJson reservation;
        reservation.setJsonData(value);

        reservation.get(jsonData, "reservationCode");
        String code = String(jsonData.intValue); 

        if(code == inputCode){
          json.iteratorEnd();
          return Firebase.RTDB.deleteNode(&fbdo, "/reservations/" + key);
        }
      }
    }
    json.iteratorEnd();
  }
  return false;
}

String saisirCode() {
  String code = "";
  Serial.println("Veuillez saisir votre code puis # pour valider :");

  while(true){
    char key = keypad.getKey();
    if(key){
      if(key == '#'){  
        Serial.println();
        break;
      }
      if(key >= '0' && key <= '9'){ 
        code += key;
        Serial.print(key);
      }
    }
    delay(10);
  }

  return code;
}

// ------------------- SETUP -------------------
void setup() {
  Serial.begin(115200);

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  myServo.attach(SERVO_PIN);
  myServo.write(0);

  Serial.print("Connexion au WiFi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print("."); }
  Serial.println(" ✅ Connecté");

  config.database_url = "https://smartparking-842d1-default-rtdb.firebaseio.com/";
  config.signer.tokens.legacy_token = "CxalmTmiQR5uFlTOmOEk2rfUajYX15l7Ht3yu1SX";
  fbClient.begin(&config,&auth);
  fbClient.reconnectWiFi(true);
  Serial.println("✅ Connecté à Firebase");
}

// ------------------- LOOP -------------------
void loop() {
  reconnectWiFiFirebase();

  // Mesure distance
  digitalWrite(TRIG, LOW); delayMicroseconds(2);
  digitalWrite(TRIG, HIGH); delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  duration = pulseIn(ECHO, HIGH, 30000);
  distance = (duration == 0) ? 1000 : duration * 0.034 / 2;

  Serial.print("Distance détectée : "); Serial.println(distance);

  if(distance < 8){
    int freeCount = getFreePlaces();
    Serial.print("Places libres : "); Serial.println(freeCount);

    if(freeCount == 0){
      Serial.println("Parking plein ! Accès refusé.");
    } else {
      // Afficher choix A/B
      Serial.println("Avez-vous déjà une réservation ?");
     

      char choix = '\0';
      while(true){
        choix = keypad.getKey();
        if(choix == 'A' || choix == 'B') break;
        delay(50);
      }

      if(choix == 'A'){ // utilisateur a déjà une réservation
        String code = saisirCode();
        String spotId;
        if(checkCodeInFirebase(code, spotId)){
          Serial.println("Code correct ! Place réservée : " + spotId);
          Serial.println("Ouverture de la porte...");
          myServo.write(90);
          delay(4000);
          myServo.write(0);

          if(removeCodeFromFirebase(code)){
            Serial.println("Code supprimé de Firebase après utilisation.");
          } else {
            Serial.println("Erreur suppression code de Firebase.");
          }
        } else {
          Serial.println("Code incorrect ou inexistant ! Accès refusé.");
        }
      } 
      else if(choix == 'B'){ // utilisateur n'a pas de réservation
        Serial.println("Désolé, vous devez d'abord réserver avant d'accéder au parking.");
      }
    }
  }

  // Mesure toutes les 3 secondes
  delay(3000);
}
