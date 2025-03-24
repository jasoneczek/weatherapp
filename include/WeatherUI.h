#ifndef WEATHERUI_H
#define WEATHERUI_H

#include "WeatherAPI.h"
#include "DatabaseManager.h"

class WeatherUI {
    public:
        static void run();

    private:
        static bool handleLogin(DatabaseManager& db, std::string& loggedInUser);
        static void weatherLoop(std::string const& loggedInUser, WeatherAPI& api);
};

#endif //WEATHERUI_H
