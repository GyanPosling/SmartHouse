#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include "../../exceptions/include/InputException.hpp"
using namespace std;

class InputUtils {
public:
    // Функция для безопасного ввода числа (int) с проверкой на смешанный ввод
    static int readInt(istream& input, const string& prompt);
    
    // Функция для безопасного ввода числа (int) с диапазоном
    static int readIntInRange(istream& input, const string& prompt, int min, int max);
    
    // Функция для безопасного ввода числа (double) с проверкой на смешанный ввод
    static double readDouble(istream& input, const string& prompt);
    
    // Функция для безопасного ввода строки (только английские буквы)
    static string readEnglishString(istream& input, const string& prompt);
    
    // Функция для безопасного ввода строки (только русские буквы)
    static string readRussianString(istream& input, const string& prompt);
    
    // Функция для безопасного ввода строки (смешанная)
    static string readString(istream& input, const string& prompt);
    
    // Функция для безопасного ввода даты (формат: DD.MM.YYYY)
    static string readDate(istream& input, const string& prompt);
    
    // Функция для безопасного ввода булевого значения
    static bool readBool(istream& input, const string& prompt);
    
    // Функция для безопасного ввода строки с ограничением длины
    static string readStringWithLimit(istream& input, const string& prompt, int maxLength);

private:
    static bool isValidEnglishString(const string& str);
    static bool isValidRussianString(const string& str);
    static bool isValidDate(const string& date);
    static void clearInputBuffer(istream& input);
};

