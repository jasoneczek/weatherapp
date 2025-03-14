#include "WeatherUI.h"
#include <iostream>
#include <cstdlib>

void WeatherUI::run() {
  const char* envKey = std::getenv("WEATHER_API_KEY");
  if (!envKey) {
    std::cerr << "WEATHER_API_KEY not set\n";
    return;
  }

  std::string apiKey = std::string(envKey);
  WeatherAPI api(apiKey);
  WeatherData weather;
  std::string city;

  while (true) {
    std::cout << "🌎 Enter a city (or type 'exit'): ";
    std::getline(std::cin, city);

    // Allow user to quit
    if (city == "exit" || city == "exit") {
      std::cout << "Goodbye!\n";
      break;
    }

    // Fetch weather
    if (api.fetchWeather(city, weather)) {
      weather.displayWeather();
    } else {
      std::cerr << "Please try again with a valid city name.\n";
    }
  }
}
