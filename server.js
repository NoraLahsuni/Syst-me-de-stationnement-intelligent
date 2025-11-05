import express from "express";
import cors from "cors";
import admin from "firebase-admin";
import fs from "fs";
import dotenv from "dotenv";
dotenv.config();


// ===== READ FIREBASE KEY =====
const serviceAccount = JSON.parse(process.env.FIREBASE_KEY);


// ===== INIT FIREBASE =====
admin.initializeApp({
  credential: admin.credential.cert(serviceAccount),
  databaseURL: "https://smartparking-842d1-default-rtdb.firebaseio.com/"
});

const db = admin.database();

// ===== INIT EXPRESS =====
const app = express();
app.use(cors());
app.use(express.json());

// ===== TEST ROUTE =====
app.get("/", (req, res) => {
  res.send("Backend is running");
});

// ===== CREATE RESERVATION ROUTE (simplifié) =====
app.post("/create-reservation", async (req, res) => {
  try {
    const { parkingSpot } = req.body;

    if (!parkingSpot) {
      return res.status(400).json({ success: false, message: "Parking spot is required" });
    }

    // generate random 6-digit reservation code
    const reservationCode = Math.floor(100000 + Math.random() * 900000);

    // save only parkingSpot + reservationCode to Firebase
    const ref = db.ref("reservations");
    await ref.push({
      parkingSpot,
      reservationCode
    });

    // return reservation info
    res.json({
      success: true,
      reservation: {
        parkingSpot,
        reservationCode
      }
    });

  } catch (error) {
    console.error(error);
    res.status(500).json({ success: false, message: "Error creating reservation" });
  }
});

// ===== START SERVER =====
const PORT = process.env.PORT || 3000;
app.listen(PORT, () => {
  console.log(`Server running on port ${PORT}`);
});
