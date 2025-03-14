#ifndef WEATHERDATA_H
#define WEATHERDATA_H

#include <string>

class WeatherData {
public:
    std::string location;
    std::string country;
    double temperature;
    std::string condition;
    double windSpeed;
    int humidity;

    // Air Quality Fields
    double airQualityPM25;  // Fine particulate matter
    double airQualityO3;    // Ozone level
    int airQualityIndex;    // Overall Air Quality Index (AQI)
    std::string airQualityCategory;  // AQI category (e.g., "Moderate", "Unhealthy")

    WeatherData();  // Constructor
    void displayWeather() const;
};

#endif // WEATHERDATA_H

