#pragma once
#include <string>
#include <iostream>
using namespace std;

class Menu {
private:
    const string* options;
    int count;
    const string header;

public:
    Menu(const string* opts, int cnt, const string& hdr)
        : options(opts), count(cnt), header(hdr) {}

    static void draw(const string& title, const string opts[], int cnt);
};