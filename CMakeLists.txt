cmake_minimum_required(VERSION 3.30)
project(WeatherApp)

set(CMAKE_CXX_STANDARD 20)

add_definitions(-DDEBUG)

# Find and link libcurl
find_package(CURL REQUIRED)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(/opt/homebrew/opt/curl/include)
include_directories(/opt/homebrew/opt/sqlite/include)
include_directories(/opt/homebrew/include)

# Add executable and include all source files
add_executable(WeatherApp
        src/main.cpp
        src/WeatherAPI.cpp
        src/WeatherData.cpp
        src/WeatherUI.cpp
        src/DatabaseManager.cpp
        include/DatabaseManager.h
        src/DatabaseManager.cpp)

# Link libraries
target_link_libraries(WeatherApp curl sqlite3)
