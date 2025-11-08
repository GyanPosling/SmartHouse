#include "../include/InputUtils.hpp"
#include "../exceptions/include/InputException.hpp"
#include <regex>
#include <limits>

void InputUtils::clearInputBuffer(istream& input) {
    input.clear();
    input.ignore(numeric_limits<streamsize>::max(), '\n');
}

int InputUtils::readInt(istream& input, const string& prompt) {
    int value;
    cout << prompt;
    
    while (true) {
        if (input >> value) {
            clearInputBuffer(input);
            return value;
        } else {
            clearInputBuffer(input);
            throw InputException(1, "Error: not a number. Please enter an integer.");
        }
    }
}

double InputUtils::readDouble(istream& input, const string& prompt) {
    double value;
    cout << prompt;
    
    while (true) {
        if (input >> value) {
            clearInputBuffer(input);
            return value;
        } else {
            clearInputBuffer(input);
            throw InputException(2, "Error: not a number. Please enter a floating point number.");
        }
    }
}

bool InputUtils::isValidEnglishString(const string& str) {
    for (char c : str) {
        if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == ' ')) {
            return false;
        }
    }
    return !str.empty();
}

bool InputUtils::isValidRussianString(const string& str) {
    for (unsigned char c : str) {
        if (!((c >= 192 && c <= 255) || c == ' ')) {
            return false;
        }
    }
    return !str.empty();
}

string InputUtils::readEnglishString(istream& input, const string& prompt) {
    string value;
    cout << prompt;
    cout << " (English letters only): ";
    
    getline(input, value);
    
    if (value.empty()) {
        throw InputException(3, "Error: empty string. Please enter a string.");
    }
    
    if (!isValidEnglishString(value)) {
        throw InputException(4, "Error: string contains invalid characters. Use English letters only.");
    }
    
    return value;
}

string InputUtils::readRussianString(istream& input, const string& prompt) {
    string value;
    cout << prompt;
    cout << " (Russian letters only): ";
    
    getline(input, value);
    
    if (value.empty()) {
        throw InputException(5, "Error: empty string. Please enter a string.");
    }
    
    if (!isValidRussianString(value)) {
        throw InputException(6, "Error: string contains invalid characters. Use Russian letters only.");
    }
    
    return value;
}

string InputUtils::readString(istream& input, const string& prompt) {
    string value;
    cout << prompt;
    
    getline(input, value);
    
    if (value.empty()) {
        throw InputException(7, "Error: empty string. Please enter a string.");
    }
    
    return value;
}

bool InputUtils::isValidDate(const string& date) {
    regex datePattern(R"(\d{2}\.\d{2}\.\d{4})");
    if (!regex_match(date, datePattern)) {
        return false;
    }
    
    int day = stoi(date.substr(0, 2));
    int month = stoi(date.substr(3, 2));
    int year = stoi(date.substr(6, 4));
    
    if (month < 1 || month > 12) return false;
    if (day < 1) return false;
    
    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)) {
        daysInMonth[1] = 29;
    }
    
    return day <= daysInMonth[month - 1];
}

string InputUtils::readDate(istream& input, const string& prompt) {
    string value;
    cout << prompt;
    cout << " (format: DD.MM.YYYY): ";
    
    getline(input, value);
    
    if (value.empty()) {
        throw InputException(8, "Error: empty string. Please enter a date.");
    }
    
    if (!isValidDate(value)) {
        throw InputException(9, "Error: invalid date format. Use format DD.MM.YYYY (e.g., 25.12.2024).");
    }
    
    return value;
}

bool InputUtils::readBool(istream& input, const string& prompt) {
    string value;
    cout << prompt;
    cout << " (yes/no or 1/0): ";
    
    getline(input, value);
    
    if (value == "yes" || value == "Yes" || value == "YES" || value == "1" || value == "true" || value == "True") {
        return true;
    } else if (value == "no" || value == "No" || value == "NO" || value == "0" || value == "false" || value == "False") {
        return false;
    } else {
        throw InputException(10, "Error: invalid value. Enter 'yes' or 'no' (or 1/0).");
    }
}

string InputUtils::readStringWithLimit(istream& input, const string& prompt, int maxLength) {
    string value = readString(input, prompt);
    
    if (value.length() > maxLength) {
        throw InputException(11, "Error: string too long. Maximum length: " + to_string(maxLength) + " characters.");
    }
    
    return value;
}

