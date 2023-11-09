#include "ProcessManagement.h"
#include "UpdateDesignLibrary.h"

int main(int argc, char* argv[])
{
    std::string line;
    for (int i = 1; i <argc; ++i)
    {
        line += argv[i];
    }

    ProcessManagement test;
    
    test.runProgram(); 

    return 0;
}

