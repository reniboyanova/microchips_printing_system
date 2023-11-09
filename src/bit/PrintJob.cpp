#include "PrintJob.h"

PrintJob::PrintJob()
{
}

PrintJob::PrintJob(int quantity, std::string printJobId, std::string hardwareCompId) :
    m_quantity{quantity}, m_printJobId{printJobId}, m_hardWarecompId{hardwareCompId}
{

}
