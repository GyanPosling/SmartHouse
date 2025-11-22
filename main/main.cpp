#include "Menu.hpp"
#include "../exceptions/include/InputHandler.hpp"
#include <locale>

Language currentLang = Language::ENGLISH;

int main() {
    setlocale(LC_ALL, "Russian");
    Menu menu;
    menu.run();
    
    return 0;
}
