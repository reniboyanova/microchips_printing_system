#ifndef PRINT_MANAGER_H
#define PRINT_MANAGER_H
#include "PrintJob.h"
#include <deque>
#include "../shared/HardwareComponent.h"
#include "defaults.h"

class PrintManager {
public:
    PrintManager();
    PrintManager(FILENAME_PATH PATH);

    void readPrintJobs();
    void addReadyProductToFile(const PrintJob& printJob, std::ofstream& output,const HardwareComponent& hc, const MATRIX& matrix);
    void printSteps();

    //gets X and Y position of pins on board based on rotation and starting position 
    void placeElectronicComponentsOnBoard(MATRIX& matrix, HardwareComponent& hardComp);
    void printConnection(HardwareComponent& hardC, MATRIX& matrix, int& stepCounter, int& connectionCounter);
    void printElComponents(MATRIX& matrix, const HardwareComponent& hardC, int& stepCounter);
    void printMatrix(const MATRIX& matrix, size_t width, size_t height);

private:
    std::deque<std::shared_ptr<PrintJob>> m_printJobs;
    FILENAME_PATH m_PRINTJOBS_PATH = DEFAULT_DATA_PATH; 
    FILENAME_PATH m_PRINT_FILE = DEFAULT_PRINT_FILE;
    FILENAME_PATH m_DATA_PATH = DEFAULT_DATA_PATH;
    FILENAME_PATH m_PRODUCTION_FILE = DEFAULT_PRODUCTION_FILE;
    const FILENAME_PATH ERROR_FILE = m_PRINTJOBS_PATH + PRINT_ERROR_FILE;
    const FILENAME_PATH TEMP_FILE = m_PRINTJOBS_PATH + TEMP_PRINT_FILE;
    int m_secondsToSleep{1};

    void loadCfg();
    void maskElCompOnBoard(MATRIX& matrix, const ElectronicComponent& elC,char symbol);
    void CutBoard(HardwareComponent &component);
    void printMaskInMachineFormat(const MATRIX& matrix);
    void logError(const PrintJob& printjob, const std::exception& ex);

};

#endif // PRINT_MANAGER_H