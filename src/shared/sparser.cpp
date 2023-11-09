#include "sparser.h"
#include <sstream>
#include <fstream>
#include <iostream>
#include <memory>

//parses Pins to Electronic Component from a  string: 1,1,3,1,5,1
void Parser::PinsToElectronicComponentFromString(std::string& line, ElectronicComponent& elComp){
        std::string num;
        int x, y;
        for(size_t i{1}; i < line.size()-1; ++i){
            if(line[i] == ',') continue;
            if(line[i] == '('){
                for(size_t j{i+1}; j < line.size(); ++j){
                    if(line[j] == ')'){
                        break;
                    }
                    num+=line[j];
                    ++i;
                } 
                std::istringstream temp(num);
                std::getline(temp, num, ',');  
                x = stoi(num);
                std::getline(temp, num, ')');        
                y = stoi(num);
                elComp.m_pinsVec.push_back(Pin{x, y});
                num.erase();
            }
        ++i;     
    }
}
//parses Electronic Component Design from a signle line string:  2N3904; 7; 3;1,1,3,1,5,1; 775 257 263 261;
void Parser::ElectronicComponentFromString(std::string line, ElectronicComponent& elComp){
        std::istringstream ss(line);
        std::string id;
        const char del = ';';
        std::getline(ss, id, del);
        elComp.setId(id);
        std::getline(ss, id, del);
        elComp.m_widht = stoi(id);
        std::getline(ss, id, del);
        elComp.m_height = stoi(id);
        std::getline(ss, id, del);
        PinsToElectronicComponentFromString(id, elComp);
    }

//points at the begining of 4th ';' in file ElectonicComponentLib
#define CURSOR 4


//given a string, separates it in id, name and quantity. Used in printJob 
void Parser::parseLineFromPrintJob(const std::string& line, std::string& id, std::string& name, int& quantity)
    {   
        const char del = ';';
        std::string numString{};
        std::istringstream ss(line);
        std::getline(ss, id, del);
        std::getline(ss, name, del);
        std::getline(ss, numString, del);
        quantity = stoi(numString);
        
    }
 //deserializes hardwarecomponent from a string   
 //Darlington_Pair_0; 11, 7;{2N3904: 0, 0, 0}{2N3904: 10, 0, 1};{1.3,2.2}{1.1,2.1};
 void Parser::parseHardwareComponent(const std::string& inputString, HardwareComponent& hardComp){
        std::istringstream ss(inputString);
        std::string line;
        const char del = ';';
        std::getline(ss, line, del);
        hardComp.m_id = line;
        std::getline(ss, line, del);
        hardComp.m_widht = stoi(line); 
        std::getline(ss, line, del);
        hardComp.m_height = stoi(line);
        std::getline(ss, line, del); 
        size_t lineSize = line.size();
        for(size_t i{1}; i < lineSize-1; ++i){
            std::string currentEl;
            if(line[i] == '{'){
                for(size_t j{i+1}; j < lineSize; ++j){
                    if(line[j] == '}'){
                        break;
                    }
                    currentEl+=line[j];
                    ++i;
                }
            }
            std::istringstream ss_el(currentEl);
            std::getline(ss_el, currentEl, ':');
            std::shared_ptr<ElectronicComponent> elComp = std::make_shared<ElectronicComponent>();
            loadElectronicComponentByName(*elComp, currentEl);
            std::getline(ss_el, currentEl, ',');
            elComp->m_x = stoi(currentEl);
            std::getline(ss_el, currentEl, ',');
            elComp->m_y = stoi(currentEl);
            std::getline(ss_el, currentEl);
            elComp->m_rotation = stoi(currentEl);
            hardComp.m_electronicComponents.push_back(elComp);
            currentEl = "";
            ++i;
        }
        
        //now lets parse connections 
         std::getline(ss, line);
         for(size_t i{1}; i < line.size()-1; ++i){
            if(line[i] == ';'){
                break;
            }
            std::string currentEl;
            if(line[i] == ',') continue;
            if(line[i] == '{'){
                for(size_t j{i+1}; j < line.size(); ++j){
                    if(line[j] == '}'){
                        break;
                    }
                    currentEl+=line[j];
                    ++i;
                }
            }
            
            std::shared_ptr<Connection> currentConnection = std::make_shared<Connection>();
            std::istringstream ss_el(currentEl);
            {
            std::getline(ss_el, currentEl, '.');
            currentConnection->m_firstECid = stoi(currentEl);
            std::getline(ss_el, currentEl, ',');
            currentConnection->m_firstPIN = stoi(currentEl);
            std::getline(ss_el, currentEl, '.');
            currentConnection->m_secondECid = stoi(currentEl);
            std::getline(ss_el, currentEl, '}');
            currentConnection->m_secondPIN = stoi(currentEl);
            hardComp.m_connections.push_back(currentConnection);
            }
            currentEl = "";
            ++i;       
         }

    }

    void Parser::deleteRowInFile(const std::string& PATH, const std::string& PATH_TEMPFILE, int row)
    {   

        const char* PATH_TEMP = PATH_TEMPFILE.c_str();
        std::ifstream infile{PATH, std::ios::in};
        if(!infile){ 
            std::cerr << "PrintManager::readPrintJob(): File print_jobs.txt could not be opened!";
            return;
        }
        std::ofstream tempFile;

        tempFile.open(PATH_TEMP, std::ofstream::out);
        if (!tempFile) {
            std::cerr << "Error creating temporary file." << std::endl;
        return;
        }
        std::string line;
        int currentRow = 1;
    

        while (std::getline(infile, line)) {   
            if (currentRow != row) {
                tempFile << line << std::endl;
            }
            currentRow++;
        }
        
        infile.close();
        tempFile.close();

        if(remove(PATH.c_str()) != 0) {
            std::cerr << "prs::deleteRowInFile(): Error deleting the file: " << PATH;
        };
        rename(PATH_TEMP, PATH.c_str());
    }

//searches every row in HardwareComponentsLibrary.txt for the corresponding design by the name of the design, and loads it to the passed refference  
void Parser::loadHardwareComponent(HardwareComponent &hardComp, const std::string hardCname)
{   
    std::ifstream infile{HARDCOMP_LIB_PATH, std::ios::in};
    if(!infile){ 
        std::cerr << "prs::loadHardwareComponent: File could not be opened!";
        return;
    }
    std::string line; //string that holds the row with the design
    if(findName(hardCname, infile, line)){
        parseHardwareComponent(line, hardComp);
    }
    else{
        throw std::runtime_error( "Hardware Component " + hardCname + " not found in library, continuing with next print job");
    }
}

bool Parser::findName(const std::string &name, std::ifstream &infile, std::string& line)
{
        while(std::getline(infile, line)){
            std::string lineElCName = line;
            std::istringstream ss(lineElCName);
            std::getline(ss, lineElCName, ';');
            if( name == lineElCName ){
                return true;
                break;
            }
            
        }
        return false;
    
}


// searches every row of the ElectronicComponentLib.txt for the corresponding name and loads it to the class
void Parser::loadElectronicComponentByName(ElectronicComponent& elcomp,const std::string& name){
        std::ifstream infile{ELECTRONIC_COMPONENT_LIB__PATH, std::ios::in};
        if(!infile){ 
            std::cerr << "File could not be opened!";
            return;
        }
        std::string line;
        if(findName(name, infile, line)){
            ElectronicComponentFromString(line, elcomp);
        }
        else{
            throw std::runtime_error( "Electronic Component " + name + " not found in library, continuing with next print job");
            
        }
       
}

    