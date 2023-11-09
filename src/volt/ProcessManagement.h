#pragma once

#include <iostream>
#include <fstream>
#include "UpdateDesignLibrary.h"
#include <sstream>
#include "../shared/Board.h"
#include <unordered_map>

class ProcessManagement
{
    private:  

        UpdateDesignLibrary m_libraryManager;
        Board m_board; 

        std::string m_printJobFile = "../../data/print_jobs.txt";
        std::string m_ordersFile = "../../data/orders.txt";
        std::string m_inventoryFile = "../../data/inventory.txt";
        std::string tempFileInData = "../../data/temp.txt";
        std::string tempFileInAmp = "../amp/temp.txt";
        


        std::string m_ElectronicComponentID;
        std::string m_orderLine;
        
        std::vector<std::string> m_storeTakenMatrerialsForOrder;
        std::vector<std::string> m_storesFirstFourDetailsOfOrder;
        bool m_checkOrder = true;
        
        void clearFirstFourDetailsFromString(const int& storeIvalue, std::string& inputString, std::string& inputStringDublicate);
        std::string takeElectronicComponentID(const std::string& component);
        void getPriority(std::string& line, std::string& priority);
        void printVec(); // test
        void putOrderBackInOrdersAsPending();
        void getStatus(std::string& line, std::string &status);
        
    public: 
        
        std::string readAndDeleteFirstLine();
        void fillVectorWithFirstFourElementsFromOrder();
        void checkIfElComponentIsRegisteredInInventory();
        void updateElectronicComponentQuantity(HardwareComponent& hwComp);
        void addJobForPrinting();
        void showNextSteps(); //not used
        void takeBoardInformation();
        void updateBoardQuantity();
        int takeNumberOfOrders();
        void sortByPriority(); 
        void addMaterialsBackIfOrderReturned(const std::unordered_map<std::string, int>& ELCompNames);
        void runProgram();    
        void printCurrentPrintJobs();
};