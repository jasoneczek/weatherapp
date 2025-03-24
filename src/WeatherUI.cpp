#include "WeatherUI.h"
#include <iostream>
#include <cstdlib>
#include <limits>
#include <termios.h>
#include <unistd.h>
#include "DatabaseManager.h"

void WeatherUI::run() {
  const char* envKey = std::getenv("WEATHER_API_KEY");
  if (!envKey) {
    std::cerr << "WEATHER_API_KEY not set\n";
    return;
  }

  std::string apiKey = std::string(envKey);
  WeatherAPI api(apiKey);
  DatabaseManager db("weatherapp.db");

  if (!db.initialize()) {
    std::cerr << "Database setup failed\n";
    return;
  }

  std::string loggedInUser;
  if (!handleLogin(db, loggedInUser)) {
    return;
  }

  weatherLoop(loggedInUser, api);
}

// Read password from input without echoing
std::string getHiddenInput(const std::string& prompt) {
  std::cout << prompt;
  std::cout.flush();

  termios oldt, newt;
  std::string password;

  // Turn off terminal echo
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~ECHO;
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);

  std::getline(std::cin, password);

  // Restore terminal
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  std::cout << std::endl;
  return password;
}

// Handles user registration or login
bool WeatherUI::handleLogin(DatabaseManager& db, std::string& loggedInUser) {
  std::string username, password;
  int choice;

  std::cout << "\n=== Welcome to WeatherApp ===\n";

  // Loop until valid input choice
  while (true) {
    std::cout << "[1] Register\n[2] Login\nChoose an option: ";
    if (!(std::cin >> choice)) {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      std::cerr << "Invalid choice. Please enter 1 or 2.\n";
      continue;
    }
    std::cin.ignore();
    if (choice == 1 || choice == 2) break;
    std::cout << "Invalid choice. Please enter 1 or 2.\n";
  }

  std::cout << "Username: ";
  std::getline(std::cin, username);
  password = getHiddenInput("Password: ");


  // Prevent empty username or password
  if (username.empty() || password.empty()) {
    std::cerr << "Username or password is empty.\n";
    return false;
  }

  bool authenticated = false;

  if (choice == 1) {
    authenticated = db.registerUser(username, password);
    if (authenticated) {
      std::cout << "Successfully registered! You are now logged in.\n";
    } else {
      return false;
    }
  } else if (choice == 2) {
    authenticated = db.loginUser(username, password);
    if (authenticated) {
      std::cout << "Successfully logged in.\n";
    } else {
      std::cerr << "Login failed. Username or password may be incorrect.\n";
      return false;
    }
  }

  loggedInUser = username;
  return true;
}

// Weather loop
void WeatherUI::weatherLoop(const std::string& loggedInUser, WeatherAPI& api) {
  WeatherData weather;
  std::string city;

  while (true) {
    std::cout << "\nLogged in as: " << loggedInUser << "\n";
    std::cout << "🌎 Enter a city (or type 'exit'): ";
    std::getline(std::cin, city);

    // Trim whitespace
    city.erase(0, city.find_first_not_of(" \t\n\r\f\v"));
    city.erase(city.find_last_not_of(" \t\n\r\f\v") + 1);

    // Exit check
    if (city == "exit") {
      std::cout << "Goodbye!\n";
      break;
    }

    if (city.empty()) {
      std::cerr << "City name cannot be empty.\n";
      continue;
    }

    // Fetch weather
    if (api.fetchWeather(city, weather)) {
      weather.displayWeather();
    } else {
      std::cerr << "Please try again with a valid city name.\n";
    }
  }
}
