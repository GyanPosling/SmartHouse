#include "../include/Date.hpp"
#include <sstream>
#include <iomanip>
using namespace std;

Date::Date() : day(1), month(1), year(1900) {}

Date::Date(int d, int m, int y) : day(d), month(m), year(y) {}

bool Date::isValid() const {
    if (year < 1900 || year > 2100) return false;
    if (month < 1 || month > 12) return false;
    if (day < 1) return false;
    
    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)) {
        daysInMonth[1] = 29;
    }
    
    return day <= daysInMonth[month - 1];
}

string Date::toString() const {
    ostringstream oss;
    oss << setfill('0') << setw(2) << day << "/"
        << setw(2) << month << "/" << year;
    return oss.str();
}

ostream& operator<<(ostream& os, const Date& date) {
    os << date.toString();
    return os;
}

istream& operator>>(istream& is, Date& date) {
    char separator1, separator2;
    is >> date.day >> separator1 >> date.month >> separator2 >> date.year;
    return is;
}

bool Date::operator==(const Date& other) const {
    return day == other.day && month == other.month && year == other.year;
}

bool Date::operator<(const Date& other) const {
    if (year != other.year) return year < other.year;
    if (month != other.month) return month < other.month;
    return day < other.day;
}


