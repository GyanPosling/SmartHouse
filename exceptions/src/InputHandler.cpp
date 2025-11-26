#include "../include/InputHandler.hpp"
#include <cctype>
#include <algorithm>
#include <limits>
#include <stdexcept>
#include <sstream>
#include <iostream>

using namespace std;

string safeGetLine(istream& is, Language lang, const string& prompt) {
    string input;
    bool valid = false;

    do {
        if (!prompt.empty()) {
            cout << prompt;
        }

        if (!getline(is, input)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Input error. Try again.\n";
            continue;
        }

        try {
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

            valid = true;

        } catch (const InputException& e) {
            cout << "Error: " << e.what() << "\n";
            valid = false;
        }

    } while (!valid);

    return input;
}

Date safeInputDate(istream& is, const string& format, const string& prompt) {
    Date result;
    bool valid = false;

    do {
        if (!format.empty()) {
            cout << "Format: " << format << endl;
        }
        if (!prompt.empty()) {
            cout << prompt;
        }

        string input;
        if (!getline(is, input)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Input error. Try again.\n";
            continue;
        }

        try {
            if (input.empty()) {
                throw InputException(8, "Empty input. Please enter a date.");
            }

            istringstream iss(input);
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

            result = tempDate;
            valid = true;

        } catch (const InputException& e) {
            cout << "Error: " << e.what() << "\n";
            valid = false;
        }

    } while (!valid);

    return result;
}


bool isEnglishOnly(const string& str) {
    if (str.empty()) return false;
    for (char c : str) {
        if (!isalpha(c) && c != ' ' && c != '-') {
            return false;
        }
        if (isalpha(c) && !((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))) {
            return false;
        }
    }
    return true;
}

bool isRussianOnly(const string& str) {
    if (str.empty()) return false;
    for (unsigned char c : str) {
        if (c < 128) {
            if (isalpha(c) && ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))) {
                return false;
            }
            if (!isspace(c) && c != '-') {
            }
        } else {
        }
    }
    return true;
}