#include "WeatherData.h"
#include <iostream>
#include <iomanip>  // 🛠️ Needed for std::setprecision and std::fixed


// Constructor (initialize all fields)
WeatherData::WeatherData()
    : temperature(0), windSpeed(0), humidity(0),
      airQualityPM25(-1.0), airQualityO3(-1.0),
      airQualityIndex(-1), airQualityCategory("Unknown") {}

// Display function
void WeatherData::displayWeather() const {
    std::cout << "\n🌍 Weather in \033[1;36m" << location << ", " << country << "\033[0m:\n"
              << "🌡️ Temperature: \033[1;32m" << temperature << "°C\033[0m\n"
              << "🌤️ Condition: \033[1;34m" << condition << "\033[0m\n"
              << "💨 Wind Speed: \033[1;35m" << windSpeed << " km/h\033[0m\n"
              << "💧 Humidity: \033[1;36m" << humidity << "%\033[0m\n";

    // Air Quality
    if (airQualityPM25 >= 0 || airQualityO3 >= 0 || airQualityIndex >= 0) {
        std::cout << "🏭 \033[1;31mAir Quality:\033[0m\n";
        if (airQualityPM25 >= 0)
            std::cout << "   • PM2.5: \033[1;33m" << std::fixed << std::setprecision(1) << airQualityPM25 << " µg/m³\033[0m\n";
        else
            std::cout << "   • PM2.5: N/A\n";

        if (airQualityO3 >= 0)
            std::cout << "   • O3: \033[1;33m" << std::fixed << std::setprecision(1) << airQualityO3 << " ppb\033[0m\n";
        else
            std::cout << "   • O3: N/A\n";

        if (airQualityIndex >= 0)
            std::cout << "   • AQI Index: \033[1;33m" << airQualityIndex << " (" << airQualityCategory << ")\033[0m\n";
        else
            std::cout << "   • AQI Index: N/A\n";

    } else {
        std::cout << "🏭 \033[1;31mAir Quality: Data not available\033[0m\n";
    }

    std::cout << std::endl;
}
