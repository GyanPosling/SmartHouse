#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <limits>
#include "InputException.hpp"
#include "Date.hpp"

enum class Language {
    ENGLISH,
    RUSSIAN
};

void safeGetLine(std::istream& is, std::string& value, Language lang, const std::string& prompt = "");

void safeInputInt(std::istream& is, int& value, int min = std::numeric_limits<int>::min(), int max = std::numeric_limits<int>::max(), const std::string& prompt = "");

void safeInputDate(std::istream& is, Date& value, const std::string& format = "", const std::string& prompt = "");

void safeInputText(std::istream& is, std::string& value, const std::string& prompt = "");

void safeInputDouble(std::istream& is, double& value, double min = std::numeric_limits<double>::min(), double max = std::numeric_limits<double>::max(), const std::string& prompt = "");

bool safeInputBool(std::istream& is, const std::string& prompt = "");

bool isEnglishOnly(const std::string& str);

bool isRussianOnly(const std::string& str);

