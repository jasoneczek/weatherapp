import express from "express";
import cors from "cors";
import dotenv from "dotenv";
import fetch from "node-fetch";

dotenv.config();

const app = express();
const PORT = 3000;

app.use(cors());

app.get("/weather", async (req, res) => {
  const city = req.query.city;
  if (!city) return res.status(400).json({ error: "City is required" });

  const apiKey = process.env.VISUALCROSSING_API_KEY;
  const url = `https://weather.visualcrossing.com/VisualCrossingWebServices/rest/services/timeline/${encodeURIComponent(
    city
  )}?key=${apiKey}`;

  try {
    const response = await fetch(url);
    if (!response.ok) throw new Error("Failed to fetch weather");

    const data = await response.json();
    res.json(data);
  } catch (err) {
    console.error("Error during fetch:", err);
    res.status(500).json({ error: err.message || "Server error" });
  }
});

app.listen(PORT, () => {
  console.log(`Server running at http://localhost:${PORT}`);
});
