#pragma once
#include <iostream>
#include <string>
using namespace std;

struct Date {
    int day;
    int month;
    int year;
    
    Date();
    Date(int d, int m, int y);
    
    bool isValid() const;
    string toString() const;
    
    friend ostream& operator<<(ostream& os, const Date& date);
    friend istream& operator>>(istream& is, Date& date);
    
    bool operator==(const Date& other) const;
    bool operator<(const Date& other) const;
};


