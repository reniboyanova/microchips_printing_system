#include "PrintManager.h"
#include <iostream>
#include <chrono>
#include <thread>

#define PAUSE 3

int main(int argc, char* argv[]) {

    std::string path;
    for (int i = 1; i < argc; i++) {
        path += argv[i];
    }
    
    PrintManager manager(path);
    while(true){
        manager.readPrintJobs();
        manager.printSteps();
        std::this_thread::sleep_for(std::chrono::seconds(PAUSE));   
         
    }  
}