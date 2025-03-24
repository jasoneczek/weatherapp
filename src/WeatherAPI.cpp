#include "WeatherAPI.h"
#include <curl/curl.h>
#include <iostream>
#include <nlohmann/json.hpp>
#include <algorithm>

using json = nlohmann::json;

// Helper function to encode spaces in city names
std::string encodeCityName(const std::string& city) {
    std::string encoded = city;
    size_t pos = 0;
    while ((pos = encoded.find(" ")) != std::string::npos) {
        encoded.replace(pos, 1, "%20");
        pos += 3;
    }
    return encoded;
}

// Callback function for libcurl
size_t WeatherAPI::writeCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t realsize = size * nmemb;
    output->append((char*)contents, realsize);
    return realsize;
}

// Constructor
WeatherAPI::WeatherAPI(const std::string& key) : apiKey(key) {}

// Fetch weather data
bool WeatherAPI::fetchWeather(const std::string& city, WeatherData& weather) {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if (curl) {
        std::string encodedCity = encodeCityName(city);
        std::string url = "https://api.weatherapi.com/v1/current.json?key=" + apiKey + "&q=" + encodedCity + "&aqi=yes";
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        if (res != CURLE_OK) {
            std::cerr << "Failed to fetch weather data: " << curl_easy_strerror(res) << std::endl;
            return false;
        }

        try {
            json jsonResponse = json::parse(readBuffer);

            if (jsonResponse.contains("error")) {
                std::cerr << "API Error: " << jsonResponse["error"]["message"] << "\n";
                return false;
            }

            weather.location = jsonResponse["location"]["name"].get<std::string>();
            weather.country = jsonResponse["location"]["country"].get<std::string>();
            weather.temperature = jsonResponse["current"]["temp_c"].is_number() ? jsonResponse["current"]["temp_c"].get<double>() : 0.0;
            weather.condition = jsonResponse["current"]["condition"]["text"].is_string() ? jsonResponse["current"]["condition"]["text"].get<std::string>() : "Unknown";
            weather.windSpeed = jsonResponse["current"]["wind_kph"].is_number() ? jsonResponse["current"]["wind_kph"].get<double>() : 0.0;
            weather.humidity = jsonResponse["current"]["humidity"].is_number() ? jsonResponse["current"]["humidity"].get<int>() : 0;

            // Air quality data
            if (jsonResponse["current"].contains("air_quality")) {
                const auto& air = jsonResponse["current"]["air_quality"];

                weather.airQualityPM25 = air.contains("pm2_5") && air["pm2_5"].is_number()
                    ? air["pm2_5"].get<double>()
                    : -1.0;

                weather.airQualityO3 = air.contains("o3") && air["o3"].is_number()
                    ? air["o3"].get<double>()
                    : -1.0;

                weather.airQualityIndex = air.contains("us-epa-index") && air["us-epa-index"].is_number_integer()
                    ? air["us-epa-index"].get<int>()
                    : -1;

                weather.airQualityCategory =
                    (weather.airQualityIndex == 1) ? "Good" :
                    (weather.airQualityIndex == 2) ? "Moderate" :
                    (weather.airQualityIndex == 3) ? "Unhealthy for Sensitive Groups" :
                    (weather.airQualityIndex == 4) ? "Unhealthy" :
                    (weather.airQualityIndex == 5) ? "Very Unhealthy" :
                    (weather.airQualityIndex == 6) ? "Hazardous" :
                    "Unknown";
            } else {
                weather.airQualityPM25 = -1.0;
                weather.airQualityO3 = -1.0;
                weather.airQualityIndex = -1;
                weather.airQualityCategory = "Unknown";
            }

            return true;

        } catch (const json::exception& e) {
            std::cerr << "JSON Parsing Error: " << e.what() << std::endl;
            return false;
        }
    }
    return false;
}





