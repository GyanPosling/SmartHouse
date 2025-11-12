#include "../include/InputUtils.hpp"
#include "../exceptions/include/InputException.hpp"
#include <regex>
#include <limits>

void InputUtils::clearInputBuffer(istream& input) {
    input.clear();
    input.ignore(numeric_limits<streamsize>::max(), '\n');
}

int InputUtils::readInt(istream& input, const string& prompt) {
    string line;
    cout << prompt;
    
    while (true) {
        getline(input, line);
        
        if (line.empty()) {
            throw InputException(1, "Error: empty input. Please enter an integer.");
        }
        
        // Проверяем, что строка содержит только число (возможно с пробелами)
        size_t firstNonSpace = line.find_first_not_of(" \t");
        size_t lastNonSpace = line.find_last_not_of(" \t");
        
        if (firstNonSpace == string::npos) {
            throw InputException(1, "Error: empty input. Please enter an integer.");
        }
        
        string trimmed = line.substr(firstNonSpace, lastNonSpace - firstNonSpace + 1);
        
        // Проверяем, что все символы - цифры или знак минуса в начале
        bool isValid = true;
        for (size_t i = 0; i < trimmed.length(); ++i) {
            if (i == 0 && trimmed[i] == '-') {
                continue; // Минус в начале допустим
            }
            if (trimmed[i] < '0' || trimmed[i] > '9') {
                isValid = false;
                break;
            }
        }
        
        if (!isValid) {
            throw InputException(1, "Error: invalid input. Please enter only an integer (no letters or special characters).");
        }
        
        try {
            int value = stoi(trimmed);
            return value;
        } catch (const invalid_argument&) {
            throw InputException(1, "Error: not a valid integer. Please enter an integer.");
        } catch (const out_of_range&) {
            throw InputException(1, "Error: number is too large. Please enter a smaller integer.");
        }
    }
}

double InputUtils::readDouble(istream& input, const string& prompt) {
    string line;
    cout << prompt;
    
    while (true) {
        getline(input, line);
        
        if (line.empty()) {
            throw InputException(2, "Error: empty input. Please enter a number.");
        }
        
        // Проверяем, что строка содержит только число (возможно с пробелами)
        size_t firstNonSpace = line.find_first_not_of(" \t");
        size_t lastNonSpace = line.find_last_not_of(" \t");
        
        if (firstNonSpace == string::npos) {
            throw InputException(2, "Error: empty input. Please enter a number.");
        }
        
        string trimmed = line.substr(firstNonSpace, lastNonSpace - firstNonSpace + 1);
        
        // Проверяем, что все символы - цифры, точка или знак минуса
        bool isValid = true;
        bool hasDot = false;
        for (size_t i = 0; i < trimmed.length(); ++i) {
            if (i == 0 && trimmed[i] == '-') {
                continue; // Минус в начале допустим
            }
            if (trimmed[i] == '.' || trimmed[i] == ',') {
                if (hasDot) {
                    isValid = false; // Две точки недопустимы
                    break;
                }
                hasDot = true;
                if (trimmed[i] == ',') {
                    trimmed[i] = '.'; // Заменяем запятую на точку
                }
            } else if (trimmed[i] < '0' || trimmed[i] > '9') {
                isValid = false;
                break;
            }
        }
        
        if (!isValid) {
            throw InputException(2, "Error: invalid input. Please enter only a number (no letters or special characters).");
        }
        
        try {
            double value = stod(trimmed);
            return value;
        } catch (const invalid_argument&) {
            throw InputException(2, "Error: not a valid number. Please enter a number.");
        } catch (const out_of_range&) {
            throw InputException(2, "Error: number is too large. Please enter a smaller number.");
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

int InputUtils::readIntInRange(istream& input, const string& prompt, int min, int max) {
    int value = readInt(input, prompt);
    
    if (value < min || value > max) {
        throw InputException(12, "Error: number out of range. Please enter a number between " + 
                            to_string(min) + " and " + to_string(max) + ".");
    }
    
    return value;
}

