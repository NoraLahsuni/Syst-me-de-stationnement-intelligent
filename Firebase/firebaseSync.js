// firebaseSync.js
import { initializeApp } from "firebase/app";
import { getDatabase, ref, onValue } from "firebase/database";
import pkg from 'pg';
import dotenv from "dotenv";

dotenv.config();
const { Pool } = pkg;

// --- Configuration Firebase ---
const firebaseConfig = {
  apiKey: process.env.FIREBASE_API_KEY,
  authDomain: process.env.FIREBASE_AUTH_DOMAIN,
  databaseURL: process.env.FIREBASE_DB_URL,
  projectId: process.env.FIREBASE_PROJECT_ID,
  storageBucket: process.env.FIREBASE_STORAGE_BUCKET,
  messagingSenderId: process.env.FIREBASE_MSG_SENDER_ID,
  appId: process.env.FIREBASE_APP_ID,
  measurementId: process.env.FIREBASE_MEASUREMENT_ID
};

// --- Initialiser Firebase ---
const app = initializeApp(firebaseConfig);
const db = getDatabase(app);

// --- Connexion à Neon PostgreSQL ---
const pool = new Pool({
  connectionString: process.env.DATABASE_URL,
  ssl: { rejectUnauthorized: false },
});

// --- Écoute le changement sur Place1 ---
const placeRef = ref(db, "ParkingLot/Place1");

onValue(placeRef, async (snapshot) => {
  const etat = snapshot.val(); // directement la valeur ("Libre" ou "Occupé")
  if (!etat) return;

  const timestamp = new Date().toISOString();

  try {
    await pool.query(
      "INSERT INTO places (capteur, etat, timestamp) VALUES ($1, $2, $3)",
      ["Place1", etat, timestamp]
    );
    console.log(`✅ Donnée insérée : Place1 → ${etat}`);
  } catch (err) {
    console.error("⚠️ Erreur lors de l’insertion :", err);
  }
});

console.log("🔥 Synchronisation Firebase ↔ Neon démarrée (Place1 seule)...");
