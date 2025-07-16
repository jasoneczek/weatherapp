export default class WeatherAPI {
  async fetchWeather(city) {
    const url = `http://localhost:3000/weather?city=${encodeURIComponent(
      city
    )}`;

    const response = await fetch(url);
    if (!response.ok) throw new Error(`HTTP error: ${response.status}`);
    return await response.json();
  }
}
