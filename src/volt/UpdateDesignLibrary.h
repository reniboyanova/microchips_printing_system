#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include "../shared/HardwareComponent.h"

class UpdateDesignLibrary
{
    private: 

        std::string m_elLibrary= "../../lib/ElectronicComponentsLib.txt";
        std::string m_hardwareLibrary = "../../lib/HardwareComponentsLib.txt";
        std::string m_productionFile = "../../data/production.txt";
        std::string m_historyFile = "../../data/history.txt";
        //std::string tempFileInLib = "../../lib/temp.txt";
        
        bool checkIfHardwareDesignExists();
        void printVec(); // test
        
    public:
        std::vector<std::string> m_storesReadyOrdersFromProduction;
        std::string m_storesHardwareAndElectronicComponentDesigns; // this will be:{{Darlington_Pair_0; 11, 7;{2N3904: 0, 0, 0}{2N3904: 10, 0, 1};{1.3,2.2}{1.1,2.1};}}{{9P3904; 7; 3;1,1,3,1,5,1;}}
        std::string m_storesHardwareComponentDesign; // this will be Darlington_Pair_0; 11, 7;{2N3904: 0, 0, 0}{2N3904: 10, 0, 1};{1.3,2.2}{1.1,2.1};
        std::vector<std::string> m_electronicComponents;

        std::string takeHardwareComponentID();
        void fillHardwareComponentDesign(std::string& IdName);
        void addNewHardwareDesignToLibrary(); 
        void fillVectorWithElectronicComponents(); 
        void keepOnlyNewElectronicDesignsInVector(); 
        void storesReadyProductsInVector();
        void addNewElectronicDesignToLibrary();
        void updateHistoryWithCompletedOrdes(); 
};