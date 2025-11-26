#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <limits>
#include <stdexcept>
#include "InputException.hpp"
#include "Date.hpp"

enum class Language {
    ENGLISH,
    RUSSIAN
};

template <typename T>
T safeInputNumeric(istream& is, T min, T max, const string& prompt = "") {
    T result;
    bool success = false;

    do {
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
                throw InputException(1, "Empty input. Please enter a number.");
            }

            istringstream iss(input);
            T temp;

            if (!(iss >> temp)) {
                throw InputException(2, "Invalid input. Expected a number, got: " + input);
            }

            char remaining;
            if (iss >> remaining) {
                throw InputException(3, "Invalid input. Number contains non-numeric characters: " + input);
            }

            if (temp < min || temp > max) {
                stringstream ss;
                ss << "Number out of range. Expected value between " << min << " and " << max << ", got: " << temp;
                throw InputException(4, ss.str());
            }

            result = temp;
            success = true;

        } catch (const InputException& e) {
            cout << e.what() << "\n";
            success = false;
        }

    } while (!success);

    return result;
}

string safeGetLine(istream& is, Language lang, const string& prompt = "");

Date safeInputDate(istream& is, const string& format = "", const string& prompt = "");

bool isEnglishOnly(const string& str);

bool isRussianOnly(const string& str);