#include "PrintManager.h"
#include "../shared/sparser.h"
#include "../shared/Utils.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <chrono>
#include <thread>
#include <unordered_map>
#include <iomanip>

PrintManager::PrintManager(FILENAME_PATH PATH) 
{   
    if(!PATH.empty()){
        m_PRINTJOBS_PATH = PATH + m_PRINT_FILE;
    }
    else{
        m_PRINTJOBS_PATH += m_PRINT_FILE;
    }
    loadCfg();
}

PrintManager::PrintManager()
{   
    m_PRINTJOBS_PATH += m_PRINT_FILE;
}

void PrintManager::readPrintJobs()
{   
    std::ifstream infile{m_PRINTJOBS_PATH, std::ios::in};
    if(!infile){ 
        std::cerr << "PrintManager::readPrintJob(): File print_jobs.txt could not be opened! Exiting program."<<std::endl;
        exit(0);
    }
    std::string line;
    bool isAlreadyPrinting = false;
    
    while(std::getline(infile, line)){
        std::string id, name;
        int quantity{};
        Parser::parseLineFromPrintJob(line, id, name, quantity);
        std::shared_ptr<PrintJob> currentJob = std::make_unique<PrintJob>(quantity, id, name);
        for(size_t i{}; i < m_printJobs.size(); ++i){
            if (m_printJobs[i]->m_printJobId == currentJob->m_printJobId){
                isAlreadyPrinting = true;
            }
        }
        if(isAlreadyPrinting) {
            continue;
        }
        m_printJobs.push_back(currentJob);         
    }
    if(m_printJobs.empty()){
        std::cout << "Waiting for a print job.." << std::endl;   
        return;
    }

    const char* PATH_TEMP = TEMP_FILE.c_str();
    std::ofstream tempFile;
    tempFile.open(PATH_TEMP, std::ofstream::out);
    if (!tempFile) {
        std::cerr << "Error creating temporary file." << std::endl;
        return;
    }
    infile.close();
    tempFile.close();
    if(remove(m_PRINTJOBS_PATH.c_str()) != 0) {
            std::cerr << "readPrintJob(): Error deleting the file: " << m_PRINTJOBS_PATH;
        };
    
    rename(PATH_TEMP, m_PRINTJOBS_PATH.c_str());
      
}

void PrintManager::addReadyProductToFile(const PrintJob &printJob, std::ofstream& output, const HardwareComponent& hc, const MATRIX& matrix)
{   
    output << printJob.m_printJobId << ':';
    output << hc.toStringDiscriptionFormat() << std::endl;
    output << hc.toStringMachineFormat();
    output << hc.connectionsMachineFormat<< std::endl;
    output << std::endl;  
    output << hc.m_id << std::endl;
    for(int i{}; i < hc.m_height; ++i){
        for(int j{}; j < hc.m_widht; ++j){
            output << std::setw(3) << matrix[i][j];
        }
        output << std::endl;
    }
    output << std::endl;  
}

void PrintManager::printSteps()
{   
    std::ofstream output{m_DATA_PATH + m_PRODUCTION_FILE, std::ios::out|std::ios::app};
    if(!output){
        std::cerr << "PrintManager::printSteps():" << m_DATA_PATH + m_PRODUCTION_FILE << " could not be opened.";
        return;
    }
      
    while(!m_printJobs.empty()){   
        std::unique_ptr<HardwareComponent> pHC; 
        try{
            pHC = std::make_unique<HardwareComponent>(m_printJobs.front()->m_hardWarecompId);
        }
        catch(const std::exception& ex){
            std::cerr << ex.what() << std::endl;
            logError(*m_printJobs.front(), ex);
            m_printJobs.pop_front();
            continue;
        }

        std::cout << pHC->m_id << "\n" <<std::endl;
        CutBoard(*pHC);
        std::unique_ptr<MATRIX> matrix = std::make_unique<MATRIX>(pHC->m_board->m_boardSize ,  std::vector<std::string>(pHC->m_board->m_boardSize , "*"));
        printMatrix(*matrix, pHC->m_board->m_boardSize, pHC->m_board->m_boardSize);

        std::this_thread::sleep_for(std::chrono::seconds(m_secondsToSleep));
        
        int stepCounter{}, connectionCounter{};
        std::cout << "\n----------------- STEP " << stepCounter << ": Cut board\n" << std::endl;
        (*matrix).resize(pHC->m_board->m_boardHeight, std::vector<std::string>(pHC->m_board->m_boardWidht , "*"));
        pHC->setSizeToMachineFormat();
        pHC->printIdAndSizeToMachineFormat();
        printMatrix(*matrix, pHC->m_board->m_boardWidht, pHC->m_board->m_boardHeight );
        ++stepCounter;

        std::this_thread::sleep_for(std::chrono::seconds(m_secondsToSleep));

        pHC->rotateAllElComponents();
        printConnection(*pHC, *matrix, stepCounter, connectionCounter);
        printElComponents(*matrix, *pHC, stepCounter);
        
        std::cout << "\n----------------- STEP " << stepCounter << ": COMPONENTS\n\n";
        std::cout<< pHC->toStringMachineFormat();
        std::cout << pHC->connectionsMachineFormat << '\n' << std::endl;
        for(const auto& ec: pHC->m_electronicComponents){
            maskElCompOnBoard(*matrix, *ec, '0');
        }
        printMatrix(*matrix, pHC->m_board->m_boardWidht, pHC->m_board->m_boardHeight );

        std::this_thread::sleep_for(std::chrono::seconds(m_secondsToSleep));

        std::cout << "\n----------------- STEP " << stepCounter << ": FINAL RESULT FROM BIT IN ALL 3 FORMATS" << std::endl;
        pHC->printDiscriptionFormat();
        std::cout << std::endl;
        std::cout << pHC->toStringMachineFormat();
        std::cout << pHC->connectionsMachineFormat << std::endl;
        std::cout << std::endl;
        placeElectronicComponentsOnBoard(*matrix, *pHC);
        printMatrix(*matrix, pHC->m_board->m_boardWidht, pHC->m_board->m_boardHeight);

        std::this_thread::sleep_for(std::chrono::seconds(m_secondsToSleep));
            
        addReadyProductToFile(*m_printJobs.front(), output, *pHC, *matrix);
        m_printJobs.pop_front();
    }
    output.close();

    std::this_thread::sleep_for(std::chrono::seconds(m_secondsToSleep));    
}



void PrintManager::CutBoard(HardwareComponent &component)
{
    component.m_board->m_boardHeight = component.m_height;
    component.m_board->m_boardWidht = component.m_widht;
    component.m_board->m_boardSize = component.m_board->getDefaultSize();
}

void PrintManager::placeElectronicComponentsOnBoard(MATRIX& matrix,HardwareComponent &hardComp)
{
    
    for(auto& item: hardComp.m_electronicComponents){
        char counter = 1;
        for(const auto& pin: item->m_pinsVec){
            matrix[pin.m_YonBoard][pin.m_XonBoard] = std::to_string(counter);
            ++counter;
        }
    }   
}

void PrintManager::printConnection(HardwareComponent &hc, MATRIX &matrix, int& stepCounter, int& connectionCounter)
{   
    
    for(const auto& connection: hc.m_connections){
        std::cout << "\n----------------- "<< "STEP "<< stepCounter << ": Connection " <<  connectionCounter << "\n"<<std::endl;
        hc.printIdAndSizeToMachineFormat();
        
        Pin pin1 = hc.m_electronicComponents[connection->m_firstECid-1]->m_pinsVec[connection->m_firstPIN-1];
        Pin pin2 = hc.m_electronicComponents[connection->m_secondECid-1]->m_pinsVec[connection->m_secondPIN-1];

        matrix[pin1.m_YonBoard][pin1.m_XonBoard] = '1';
        matrix[pin2.m_YonBoard][pin2.m_XonBoard] = '1';
        
        hc.connectionsMachineFormat += std::to_string(uti::mlTransformTwoInt(pin1.m_XonBoard, pin1.m_YonBoard)) + ' ' + std::to_string(uti::mlTransformTwoInt(pin2.m_XonBoard, pin2.m_YonBoard)) + ' ';
        std::cout << hc.connectionsMachineFormat << std::endl;
        std::cout << std::endl;
        printMatrix(matrix, hc.m_widht, hc.m_height);

        matrix[pin1.m_YonBoard][pin1.m_XonBoard] = '*';
        matrix[pin2.m_YonBoard][pin2.m_XonBoard] = '*';

        stepCounter++;
        connectionCounter++;
        std::this_thread::sleep_for(std::chrono::seconds(m_secondsToSleep));
        
    }

}

void PrintManager::printElComponents(MATRIX& matrix, const HardwareComponent &hc, int& stepCounter)
{   
    std::unordered_map<std::string, int> names;

    for(const auto& el: hc.m_electronicComponents){
       names[el->m_id] += 1;
    }
    for (const auto& name : names) {
        std::cout << "\n----------------- "<< "STEP " << stepCounter << ": COMPONENTS MASK - ";
        std::cout << name.first << "\n" <<std::endl;
        std::cout << hc.m_id << " " <<  std::to_string(hc.m_sizeMachineFormat) << "\n" <<std::endl;
        std::cout << hc.connectionsMachineFormat << '\n' << std::endl;
        for(const auto& ec: hc.m_electronicComponents){
            if(name.first == ec->m_id){
                maskElCompOnBoard(matrix, *ec, '1');
            }         
        }
        printMatrix(matrix, hc.m_widht, hc.m_height);

        //prints mask for every component
        printMaskInMachineFormat(matrix);

        for(const auto& ec: hc.m_electronicComponents){
            if(name.first == ec->m_id){
                maskElCompOnBoard(matrix, *ec, '*');
            }         
        }
        ++stepCounter;
        std::this_thread::sleep_for(std::chrono::seconds(m_secondsToSleep));

    }
}

void PrintManager::printMatrix(const MATRIX &matrix, size_t width, size_t height)
{   
    for(size_t row{}; row < height; ++row){
        for(size_t column{}; column < width; ++column){
            std::cout << std::setw(3) <<matrix[row][column];
        }
        std::cout<< std::endl;
    }
    
}

void PrintManager::printMaskInMachineFormat(const MATRIX &matrix)
{
    size_t rows = matrix.size();
    size_t columns = matrix[0].size();
    int remainderBits{0}; 
    int number{};

    for(size_t i{}; i < rows; ++i){
        int bitsCounter{1};
        if(remainderBits){
            number = number << ( 8 - remainderBits);
            std::cout << number << " ";
            number = 0;
        }
        std::cout << std::endl;
        for(size_t j{}; j < columns; ++j){
            if(bitsCounter % 9 == 0){
                std::cout << number << " ";
                number = 0;
                remainderBits = 0;
            }
            if(matrix[i][j] == "*"){
                number <<= 1;
            }
            else if(matrix[i][j] == "1"){
                number = (number << 1) | 1;
            }

            ++bitsCounter;
            ++remainderBits;
        }
    }
    std::cout << number << std::endl;

}

void PrintManager::logError(const PrintJob &printjob, const std::exception& ex)
{
    std::ofstream outfile{ERROR_FILE,std::ios::out | std::ios::app};
    if(outfile){
        outfile << ex.what() << ":" <<printjob.m_printJobId << ';' <<printjob.m_hardWarecompId << ';' << printjob.m_quantity<< ';' << std::endl;
        outfile.close();
    }
    else{
        std::cerr << "PrintManager::LogError: Cannot open file " << PRINT_ERROR_FILE << std::endl;
    }
}   

void PrintManager::loadCfg()
{
    std::ifstream infile{CFG_FILE, std::ios::in};
    if(!infile){ 
        std::cerr << "PrintManager::loadCfg(): File cfg_bit.txt could not be opened!";
        return;
    }
    std::string line;
    std::getline(infile, line);
    std::string word;
    std::istringstream ss(line);
    std::getline(ss, word, ' ');
    if(word == SLEEP_TIME){
        std::getline(ss, word, ';');
        m_secondsToSleep = std::stoi(word);
    }
    else{
        std::cout << "PrintManager::loadCfg(): Cannot Load Sleep Time from cfg.\n" << std::endl;
    }
    infile.close();
}

void PrintManager::maskElCompOnBoard(MATRIX &matrix, const ElectronicComponent &ec, char symbol)
{
    for(int row{ec.m_yRotated}; row < ec.m_yRotated + ec.m_height; ++row){
        for(int column{ec.m_xRotated}; column < ec.m_xRotated + ec.m_widht; ++column){
            matrix[row][column] = symbol;
        }
    }
}

