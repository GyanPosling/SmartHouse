#include "../include/InputHandler.hpp"
#include <cctype>
#include <algorithm>
#include <limits>
#include <stdexcept>
#include <sstream>

std::string safeGetLine(std::istream& is, Language lang, const std::string& prompt) {
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
            throw InputException(6, "Invalid input. String must contain only English letters, spaces, or hyphens. Got: " + input);
        }
    } else if (lang == Language::RUSSIAN) {
        isValid = isRussianOnly(input);
        if (!isValid) {
            throw InputException(7, "Invalid input. String must contain only Russian letters, spaces, or hyphens. Got: " + input);
        }
    }
    
    return input;
}

Date safeInputDate(std::istream& is, const std::string& format, const std::string& prompt) {
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
    
    return tempDate;
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
    for (unsigned char c : str) {
        if (c < 128) {
            if (std::isalpha(c) && ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))) {
                return false;
            }
            if (!std::isspace(c) && c != '-') {
            }
        } else {
        }
    }
    return true;
}