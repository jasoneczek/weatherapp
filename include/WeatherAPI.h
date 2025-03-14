#ifndef WEATHERAPI_H
#define WEATHERAPI_H

#include <string>
#include "WeatherData.h"

class WeatherAPI {
public:
  explicit WeatherAPI(const std::string& key);  // Constructor

  bool fetchWeather(const std::string& city, WeatherData& weather);
  bool fetchAirQuality(const std::string& city, WeatherData& weather);

private:
  std::string apiKey;
  static size_t writeCallback(void* contents, size_t size, size_t nmemb, std::string* output);
};

#endif // WEATHERAPI_H

