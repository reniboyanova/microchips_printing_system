#ifndef PRINT_JOB_H
#define PRINT_JOB_H
#include <string>
#include <memory>

class PrintJob{
public:
    PrintJob();
    PrintJob(int quantity, std::string printJobId, std::string hardwareCompId);
    int m_quantity;
    std::string m_printJobId;
    std::string m_hardWarecompId;
};

#endif //PRINT_JOB_H