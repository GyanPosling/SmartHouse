#include "header.hpp"

int main() {
    setlocale(LC_ALL, "Russian");
    int restart;
    do{
        run();
        restart = safeInputNumeric<int>(cin, 0, 1, "Do you want to restart project(0 - no / 1 - yes ): ");
        
    }while(restart != 0);
    return 0;
}
