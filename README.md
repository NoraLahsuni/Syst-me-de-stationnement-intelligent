# 🚗 Système de Gestion Intelligente de Stationnement

### 🏫 Université : Ibn Zohr  
### 📅 Année : 2025 / 2026 
### ✅ Statut : Développement en cours  

---

## 🧠 Description du Projet

Ce projet vise à développer un **système intelligent de gestion de stationnement** basé sur l’IoT et le cloud.  
L’objectif est d’optimiser l’utilisation des places de parking en temps réel grâce à la détection automatique des véhicules et à la synchronisation avec une base de données distante.

---

## 🏗️ Architecture du Système

- **Capteurs (IoT / ESP32)** : détectent la présence des véhicules.  
- **Firebase Realtime Database** : envoie et reçoit les données des capteurs.  
- **Serveur Node.js (Express)** : synchronise Firebase ↔ PostgreSQL (Neon).  
- **Base de Données (Neon PostgreSQL)** : stocke les états des places.  
- **Interface Web (prochaine étape)** : affichage en temps réel des places libres/occupées.

---

## ⚙️ Technologies Utilisées

| Composant | Technologie |
|------------|--------------|
| 💡 Microcontrôleur | ESP32 / Arduino |
| ☁️ Cloud | Firebase Realtime Database |
| 🖥️ Backend | Node.js + Express |
| 🗄️ Base de Données | PostgreSQL (Neon) |
| 🔐 Sécurité | Variables d'environnement (.env) |
| 🧰 Outils | VS Code, Git, Render (déploiement futur) |

---

---

## 🧩 Tests & Validation

- ✅ Test de synchronisation Firebase → Neon.  
- ✅ Vérification de l’insertion en base PostgreSQL.  
- 🔄 Simulation de changement d’état (place occupée / libre).

---

## 🚀 Perspectives d’Amélioration

- Déploiement sur **Render** ou **Vercel**.  
- Ajout d’une **interface web interactive**.  
- Notifications (SMS / Email) en cas d’occupation complète.  
- Sécurisation HTTPS + authentification.

---

## 🏁 Conclusion

Ce projet démontre une application pratique des systèmes connectés (IoT) pour la **gestion intelligente des ressources** en milieu urbain.  
Il constitue une base solide pour le développement d’un système de stationnement en temps réel fiable et extensible.
