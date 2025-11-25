# Système de Gestion Intelligente de Stationnement

## Université : Ibn Zohr  
## Année : 2025 / 2026  
---
# 🅿️ Smart Parking IoT System – ESP32 / ESP8266 / Firebase / React / Node.js

## 📌 Description du Projet

Ce projet présente un **système intelligent de gestion de parking** basé sur l’IoT, permettant à la fois :

* la détection automatisée de l’occupation des places de stationnement
* la gestion de l’accès par code unique
* la réservation et consultation en temps réel via interface web
* la supervision par un panneau d’administration

Le système combine IoT (ESP32/ESP8266), Cloud (Firebase), Backend API (Node.js) et Frontend (React).

---

## 🧩 Architecture Générale

Le système repose sur deux modules matériels principaux :

### 🔐 Module d’Accès (ESP32)

* Détection de présence à l’entrée par capteur ultrason
* Interaction conducteur via LCD
* Vérification d’un code de réservation
* Contrôle de la barrière par servomoteur

### 📡 Module de Surveillance (ESP8266/ESP32)

* Capteurs IR positionnés sur chaque place
* Mise à jour de l’état des places (Libre/Occupée/Réservée)
* Envoi des données vers Firebase RTDB

### 🗄 Infrastructure Logicielle

* Firebase Realtime Database → synchronisation immédiate
* Neon/PostgreSQL → historique et archivage
* API Node.js → logique métier et sécurité
* Interface React → utilisateur et administrateur

---

## 🛠 Technologies Utilisées

### Matériel

* ESP32
* ESP8266
* Capteurs IR
* Capteur Ultrason
* Servomoteur
* LCD I2C
* Clavier matriciel 4×4

### Logiciel & Cloud

* Arduino IDE
* VSCode
* Firebase Realtime Database
* Neon PostgreSQL
* Node.js Express API
* React Web App

---

## 🔥 Fonctionnalités principales

✔ Réservation de place en ligne
✔ Attribution d’un code d’accès unique
✔ Détection automatique des véhicules
✔ Ouverture intelligente de la barrière
✔ Supervision du parking en temps réel
✔ Tableau de bord administrateur

---

## 🚗 Scénario de fonctionnement

1. Le conducteur arrive à l’entrée
2. Le capteur ultrason détecte la voiture
3. Le LCD demande le code de réservation
4. L’ESP32 interroge Firebase
5. Si le code est valide → ouverture de barrière
6. Sinon → accès refusé

Parallèlement :

* les capteurs IR détectent l’état de chaque place
* Firebase envoie les états en temps réel à React
* l’interface affiche les informations aux utilisateurs

---

## 🔥 Performance & Résultats

* Mise à jour temps réel
* Vérification du code d’accès instantanée
* Fiabilité > 98%


---

## 👥 Équipe du projet
Filière IISE:
*Khaoula EL HARRAZ
*Maryem EL-BOUCHTI
*Nora LAHSUNI
*Oussama GOUSSA

Filière ADIA:
*Imane BARAKAT
*Naoual ELHILLALI


Encadré par :
*Pr. Amine RGHIOUI
*Pr. Monsef BOUGHROUS

---

## 🔮 Améliorations Futures

* Paiement automatique QR Code
* Navigation guidée vers la place réservée
* Prédiction d’occupation par IA



