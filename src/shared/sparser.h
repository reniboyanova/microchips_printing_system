#ifndef S_PARSER_H
#define S_PARSER_H

#include "ElectronicComponent.h"
#include "HardwareComponent.h"



namespace Parser{
   
    const std::string HARDCOMP_LIB_PATH = "../../lib/HardwareComponentsLib.txt";
    const std::string ELECTRONIC_COMPONENT_LIB__PATH = "../../lib/ElectronicComponentsLib.txt";

    void loadElectronicComponentByName(ElectronicComponent& elcomp,const std::string& name);
    
    void loadHardwareComponent(HardwareComponent &hardComp, const std::string hardCname);

    void PinsToElectronicComponentFromString(std::string& line, ElectronicComponent& elComp);

    void ElectronicComponentFromString(std::string line, ElectronicComponent& elComp);

    void parseLineFromPrintJob(const std::string& line, std::string& id, std::string& name, int& quantity);
    
    void parseHardwareComponent(const std::string& inputString, HardwareComponent& hardComp);
    
    void deleteRowInFile(const std::string& PATH, const std::string& PATH_TEMPFILE, int row);    
    
    void loadHardwareComponent(HardwareComponent &hardComp, const std::string hardCname);

    bool findName(const std::string& name,std::ifstream& infile, std::string& line);

    void loadElectronicComponentByName(ElectronicComponent& elcomp,const std::string& name);
    
}

#endif // S_PARSER_H