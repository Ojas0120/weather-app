#include <iostream>
#include <string>
#include <curl/curl.h>
#include <json/json.h>  // Requires jsoncpp library

// Callback function to handle API response data
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t totalSize = size * nmemb;
    output->append((char*)contents, totalSize);
    return totalSize;
}

// Function to fetch weather data
std::string fetchWeatherData(const std::string& city, const std::string& apiKey) {
    CURL* curl;
    CURLcode res;
    std::string response;

    // OpenWeatherMap API URL
    std::string url = "http://api.openweathermap.org/data/2.5/weather?q=" + city + "&appid=" + apiKey + "&units=metric";

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }

    return response;
}

// Function to parse and display weather data
void parseWeatherData(const std::string& jsonData) {
    Json::CharReaderBuilder reader;
    Json::Value root;
    std::string errs;
    
    std::istringstream s(jsonData);
    if (!Json::parseFromStream(reader, s, &root, &errs)) {
        std::cerr << "Failed to parse weather data: " << errs << std::endl;
        return;
    }

    std::string city = root["name"].asString();
    std::string description = root["weather"][0]["description"].asString();
    double temperature = root["main"]["temp"].asDouble();
    int humidity = root["main"]["humidity"].asInt();

    std::cout << "\nWeather in " << city << ":\n";
    std::cout << "Temperature: " << temperature << "°C\n";
    std::cout << "Humidity: " << humidity << "%\n";
    std::cout << "Condition: " << description << "\n";
}

int main() {
    std::string city;
    std::string apiKey = "YOUR_API_KEY";  // Replace with your API key

    std::cout << "Enter city name: ";
    std::getline(std::cin, city);

    std::string weatherData = fetchWeatherData(city, apiKey);

    if (!weatherData.empty()) {
        parseWeatherData(weatherData);
    } else {
        std::cerr << "Failed to fetch weather data. Check your internet connection or API key.\n";
    }

    return 0;
}
