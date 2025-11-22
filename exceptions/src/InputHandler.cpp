#include "../include/InputHandler.hpp"
#include <cctype>
#include <algorithm>
#include <limits>

void safeGetLine(std::istream& is, std::string& value, Language lang, const std::string& prompt) {
    if (!prompt.empty()) {
        std::cout << prompt;
    }
    
    std::string input;
    std::getline(is, input);
    
    if (input.empty()) {
        throw InputException(5, "Empty input. Please enter a non-empty string.");
    }
    
    bool isValid = false;
    if (lang == Language::ENGLISH) {
        isValid = isEnglishOnly(input);
        if (!isValid) {
            throw InputException(6, "Invalid input. String must contain only English letters. Got: " + input);
        }
    } else if (lang == Language::RUSSIAN) {
        isValid = isRussianOnly(input);
        if (!isValid) {
            throw InputException(7, "Invalid input. String must contain only Russian letters. Got: " + input);
        }
    }
    
    value = input;
}

void safeInputInt(std::istream& is, int& value, int min, int max, const std::string& prompt) {
    if (!prompt.empty()) {
        std::cout << prompt;
    }
    
    std::string input;
    std::getline(is, input);
    
    if (input.empty()) {
        throw InputException(1, "Empty input. Please enter a number.");
    }
    
    std::istringstream iss(input);
    int temp;
    
    if (!(iss >> temp)) {
        throw InputException(2, "Invalid input. Expected a number, got: " + input);
    }
    
    char remaining;
    if (iss >> remaining) {
        throw InputException(3, "Invalid input. Number contains non-numeric characters: " + input);
    }
    
    if (temp < min || temp > max) {
        throw InputException(4, "Number out of range. Expected value between " + std::to_string(min) + " and " + std::to_string(max) + ", got: " + std::to_string(temp));
    }
    
    value = temp;
}

void safeInputDate(std::istream& is, Date& value, const std::string& format, const std::string& prompt) {
    if (!format.empty()) {
        std::cout << "Format: " << format << std::endl;
    }
    if (!prompt.empty()) {
        std::cout << prompt;
    }
    
    std::string input;
    std::getline(is, input);
    
    if (input.empty()) {
        throw InputException(8, "Empty input. Please enter a date.");
    }
    
    std::istringstream iss(input);
    char separator1, separator2;
    int day, month, year;
    
    if (!(iss >> day >> separator1 >> month >> separator2 >> year)) {
        throw InputException(9, "Invalid date format. Expected format: DD/MM/YYYY. Got: " + input);
    }
    
    if (separator1 != '/' || separator2 != '/') {
        throw InputException(10, "Invalid date format. Expected separators '/'. Got: " + input);
    }
    
    char remaining;
    if (iss >> remaining) {
        throw InputException(11, "Invalid date format. Extra characters after date: " + input);
    }
    
    Date tempDate(day, month, year);
    if (!tempDate.isValid()) {
        throw InputException(12, "Invalid date. Date is not valid: " + tempDate.toString());
    }
    
    value = tempDate;
}

void safeInputText(std::istream& is, std::string& value, const std::string& prompt) {
    safeGetLine(is, value, Language::ENGLISH, prompt);
}

void safeInputDouble(std::istream& is, double& value, double min, double max, const std::string& prompt) {
    if (!prompt.empty()) {
        std::cout << prompt;
    }
    
    std::string input;
    std::getline(is, input);
    
    if (input.empty()) {
        throw InputException(13, "Empty input. Please enter a number.");
    }
    
    std::istringstream iss(input);
    double temp;
    
    if (!(iss >> temp)) {
        throw InputException(14, "Invalid input. Expected a number, got: " + input);
    }
    
    char remaining;
    if (iss >> remaining) {
        throw InputException(15, "Invalid input. Number contains non-numeric characters: " + input);
    }
    
    if (temp < min || temp > max) {
        throw InputException(16, "Number out of range. Expected value between " + std::to_string(min) + " and " + std::to_string(max) + ", got: " + std::to_string(temp));
    }
    
    value = temp;
}

bool safeInputBool(std::istream& is, const std::string& prompt) {
    if (!prompt.empty()) {
        std::cout << prompt;
    }
    
    std::string input;
    std::getline(is, input);
    
    if (input == "yes" || input == "Yes" || input == "YES" || input == "1" || input == "true" || input == "True") {
        return true;
    } else if (input == "no" || input == "No" || input == "NO" || input == "0" || input == "false" || input == "False") {
        return false;
    } else {
        throw InputException(17, "Invalid value. Enter 'yes' or 'no' (or 1/0). Got: " + input);
    }
}

bool isEnglishOnly(const std::string& str) {
    if (str.empty()) return false;
    for (char c : str) {
        if (!std::isalpha(c) && c != ' ' && c != '-') {
            return false;
        }
        if (std::isalpha(c) && !((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))) {
            return false;
        }
    }
    return true;
}

bool isRussianOnly(const std::string& str) {
    if (str.empty()) return false;
    for (char c : str) {
        if (std::isalpha(c)) {
            if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
                return false;
            }
        } else if (c != ' ' && c != '-' && (unsigned char)c < 128) {
            if (!std::isspace(c)) {
                return false;
            }
        }
    }
    return true;
}

