export default class WeatherUI {
  hideWelcome() {
    const welcomeHeader = document.getElementById("welcome-header");
    if (welcomeHeader) welcomeHeader.classList.add("hidden");
  }

  showLoader() {
    document.getElementById("loader").classList.remove("hidden");
    document.getElementById("weather-output").classList.add("hidden");
  }

  hideLoader() {
    document.getElementById("loader").classList.add("hidden");
  }

  showWeatherOutput() {
    document.getElementById("weather-output").classList.remove("hidden");
  }

  clearWeatherSections() {
    document.getElementById("current-weather").innerHTML = "";
    document.getElementById("forecast").innerHTML = "";
  }

  displayCurrentWeather(data) {
    // CREATE CURRENT WEATHER SECTION HEADING
    const currentWeatherHeading = document.getElementById(
      "current-weather-heading"
    );
    currentWeatherHeading.textContent = "Current Weather";

    const container = document.getElementById("current-weather");
    container.innerHTML = "";

    // === CREATE HEADING ===
    const heading = document.createElement("h3");
    heading.classList.add("location-heading");
    heading.textContent = data.resolvedAddress;

    // == CREATE CURRENT CONDITION CONTAINER
    const conditionContainer = document.createElement("div");
    conditionContainer.classList.add("current-weather-condition");

    const iconWrapper = document.createElement("div");
    iconWrapper.classList.add("condition-wrapper");

    const icon = document.createElement("img");
    icon.src = `https://raw.githubusercontent.com/visualcrossing/WeatherIcons/main/SVG/2nd%20Set%20-%20Color/${data.currentConditions.icon}.svg`;
    icon.alt = data.currentConditions.conditions;
    icon.classList.add("condition-icon", "weather-icon");

    const text = document.createElement("p");
    text.textContent = data.currentConditions.conditions;
    text.classList.add("condition-text");

    iconWrapper.appendChild(icon);
    iconWrapper.appendChild(text);

    const tempWrapper = document.createElement("div");
    tempWrapper.classList.add("current-weather-temp");

    const tempText = document.createElement("p");
    tempText.classList.add("temp-text");
    tempText.textContent = `${Math.round(data.currentConditions.temp)}°F`;

    tempWrapper.appendChild(tempText);
    conditionContainer.appendChild(iconWrapper);
    conditionContainer.appendChild(tempWrapper);

    container.appendChild(heading);
    container.appendChild(conditionContainer);
  }

  displayHourlyWeather(data) {
    const hourlyWeatherGrid = document.createElement("div");
    hourlyWeatherGrid.classList.add("hourly-weather-grid");
    hourlyWeatherGrid.innerHTML = "";

    const targetHours = ["08:00:00", "14:00:00", "18:00:00"];
    const todayHours = data.days[0].hours.filter((hour) =>
      targetHours.includes(hour.datetime)
    );

    todayHours.forEach((hour) => {
      const hourCard = document.createElement("div");
      hourCard.classList.add("hour-card");

      const time = document.createElement("span");
      time.classList.add("hour-time");
      time.textContent = hour.datetime.slice(0, 5);

      const icon = document.createElement("img");
      icon.classList.add("hour-icon", "weather-icon");
      icon.src = `https://raw.githubusercontent.com/visualcrossing/WeatherIcons/main/SVG/2nd%20Set%20-%20Color/${hour.icon}.svg`;
      icon.alt = hour.conditions;

      const temp = document.createElement("span");
      temp.classList.add("hour-temp");
      temp.textContent = `${Math.round(hour.temp)}°F`;

      hourCard.appendChild(time);
      hourCard.appendChild(icon);
      hourCard.appendChild(temp);
      hourlyWeatherGrid.appendChild(hourCard);
    });
    const container = document.getElementById("current-weather");
    container.appendChild(hourlyWeatherGrid);
  }

  displayForecast(data) {
    const forecastContainer = document.getElementById("forecast");
    forecastContainer.innerHTML = "";

    // CREATE FORECAST SECTION HEADING
    const forecastHeading = document.createElement("h2");
    forecastHeading.classList.add("forecast-heading");
    forecastHeading.textContent = "5 Day Forecast";
    forecastContainer.appendChild(forecastHeading);

    const forecastGrid = document.createElement("div");
    forecastGrid.classList.add("forecast-grid");

    data.days.slice(1, 6).forEach((day) => {
      const dayCard = document.createElement("div");
      dayCard.classList.add("forecast-day");

      // === LEFT COLUMN ===
      const left = document.createElement("div");
      left.classList.add("forecast-left");

      const dateObj = new Date(day.datetime);
      const weekday = dateObj.toLocaleDateString("en-US", { weekday: "short" });
      const dayNum = dateObj.toLocaleDateString("en-US", { day: "numeric" });

      const daySpan = document.createElement("span");
      daySpan.classList.add("forecast-daynum");
      daySpan.textContent = dayNum;

      const weekdaySpan = document.createElement("span");
      weekdaySpan.classList.add("forecast-weekday");
      weekdaySpan.textContent = weekday;

      left.appendChild(daySpan);
      left.appendChild(weekdaySpan);

      // === CENTER COLUMN ===
      const center = document.createElement("div");
      center.classList.add("forecast-center");

      const icon = document.createElement("img");
      icon.classList.add("forecast-icon", "weather-icon");
      icon.src = `https://raw.githubusercontent.com/visualcrossing/WeatherIcons/main/SVG/2nd%20Set%20-%20Color/${day.icon}.svg`;
      icon.alt = day.conditions;

      center.appendChild(icon);

      // === RIGHT COLUMN ===
      const right = document.createElement("div");
      right.classList.add("forecast-right");

      const temps = document.createElement("div");
      temps.classList.add("forecast-temp");
      temps.innerHTML = `<span class="high">${Math.round(
        day.tempmax
      )}°</span> | <span class="low">${Math.round(day.tempmin)}°</span>`;

      const desc = document.createElement("span");
      desc.classList.add("forecast-desc");
      desc.textContent = day.conditions;

      right.appendChild(temps);
      right.appendChild(desc);

      // === ASSEMBLE FULL CARD ===
      dayCard.appendChild(left);
      dayCard.appendChild(center);
      dayCard.appendChild(right);
      forecastGrid.appendChild(dayCard);
    });
    forecastContainer.appendChild(forecastGrid);
  }

  displayError(message) {
    const errorBox = document.getElementById("error-box");
    if (errorBox) {
      errorBox.textContent = message;
      errorBox.classList.remove("hidden");
    }
  }

  hideError() {
    const errorBox = document.getElementById("error-box");
    if (errorBox) {
      errorBox.textContent = "";
      errorBox.classList.add("hidden");
    }
  }
}
