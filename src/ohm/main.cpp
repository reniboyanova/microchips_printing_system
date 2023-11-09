#include <iostream>
#include <ctime>
#include <fstream>
#include "OrderManagement.h"
//g++ .\main.cpp .\OrderManagement.cpp .\Order.cpp ..\shared\Utils.cpp -o .\ohm.exe

int main(int argc, char* argv[]) {
    srand(time(0));

    OrderManagement ordMang;
    std::string incomeOrdersFile;

    if (argc >= 2) {
        incomeOrdersFile = argv[1];
    } else {
        std::cin >> incomeOrdersFile;
        return 1;
    }

    ordMang.processOrdersFromFstream(incomeOrdersFile);
    ordMang.menu();
    
    return 0;
}



