import WeatherAPI from "./WeatherAPI.js";
import WeatherUI from "./WeatherUI.js";

export default class WeatherApp {
  constructor() {
    this.api = new WeatherAPI();
    this.ui = new WeatherUI();
    this.searchInput = document.getElementById("city-input");
    this.searchBtn = document.getElementById("search-btn");

    this.init();
  }

  init() {
    this.searchBtn.addEventListener("click", (e) => {
      e.preventDefault();
      this.handleSearch();
    });
  }

  async handleSearch() {
    const city = this.searchInput.value.trim();
    if (!city) return;

    this.ui.hideError();
    this.ui.hideWelcome();
    this.ui.clearWeatherSections();
    this.ui.showLoader();

    try {
      const data = await this.api.fetchWeather(city);

      this.ui.displayCurrentWeather(data);
      this.ui.displayHourlyWeather(data);
      this.ui.displayForecast(data);
      this.ui.showWeatherOutput();
    } catch (err) {
      console.error("Failed to fetch weather:", err.message);
      this.ui.displayError(
        `Could not get weather for ${city}.  ${err.message}`
      );
    } finally {
      this.ui.hideLoader();
    }
  }
}
