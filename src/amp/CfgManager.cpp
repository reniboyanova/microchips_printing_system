#include "CfgManager.h"
#include <fstream>
#include <vector>

int CfgManager::getNumberOfLinesOfTextFile(std::string& fileName)
{
    std::ifstream fileIn;
    std::string line;
    int numberOfLines = 0;

    fileIn.open(fileName);

    if (!fileIn.is_open())
    {
        std::cerr << "Cannot open cfg.txt for reading." << std::endl;
    }
    else
    {
        while (getline(fileIn, line))
        {
            ++ numberOfLines;
        }

        fileIn.close();
    }

    return numberOfLines;
}

void CfgManager::addNewComponentsToCfg()
{
    int numberOfLinesInInventory = getNumberOfLinesOfTextFile(m_accessToInventory.m_inventoryFile);
    int numberOfLinesInCfg = getNumberOfLinesOfTextFile(m_cfgFile);
    std::vector<std::string> storeComponents;
    std::string line;

    int difference = numberOfLinesInInventory - numberOfLinesInCfg;

    if (difference != 0)
    {
        std::ifstream fileIn;
        std::ofstream temp;

        fileIn.open(m_accessToInventory.m_inventoryFile);
        temp.open("temp.txt");
        if (!fileIn.is_open())
        {
            std::cerr << "Cannot open cfg.txt for reading." << std::endl;
        }
        else
        {
            while (getline(fileIn, line))
            {
                storeComponents.push_back(line);
            }

            for (int i = 0; i < storeComponents.size(); ++i)
            {
                temp << storeComponents[i] << std:: endl;
            }

        }

        fileIn.close();
        temp.close();
        std::remove(m_cfgFile.c_str());
        std::rename("temp.txt", m_cfgFile.c_str());
        std::cout << "Cfg is updated." << std:: endl;
    }
}



void CfgManager::fillVectorWithCfgLines()
{
    std::ifstream fileIn;
    std::string line;

    fileIn.open(m_accessToInventory.m_inventoryFile);

    if (!fileIn.is_open())
    {
        std::cerr << "Cannot open cfg.txt for reading." << std::endl;
    }
    else
    {
        while (getline(fileIn, line))
        {
            m_storeCfgLines.push_back(line);
        }
    }

    fileIn.close();
}

void CfgManager::setMinimumQuantityCfg()
{

    fillVectorWithCfgLines();

    std::ofstream temp;

    for (int i = 0; i< m_storeCfgLines.size(); ++i)
    {
        int currentQuantityOfComponent = m_accessToInventory.getQuantity(m_storeCfgLines[i]);
        std::string name = m_accessToInventory.getName(m_storeCfgLines[i]);
        std::cout << name << std::endl;

        if (currentQuantityOfComponent != m_accessToInventory.m_minimalQuantity)
        {
            for (int j = 0; j < m_storeCfgLines[i].size(); ++j)
            {
                if(m_storeCfgLines[i][j] == ' ') //Board 90
                {
                    m_storeCfgLines[i] = name + " " + std::to_string(m_accessToInventory.m_minimalQuantity); //20
                    break;
                }
            }
        }
    }

    temp.open("temp.txt");

    if (!temp.is_open())
    {
        std::cerr << "Cannot open cfg.txt for writting." << std::endl;
    }
    else
    {
        for (int i = 0; i< m_storeCfgLines.size(); ++i)
        {
            temp << m_storeCfgLines[i] << std:: endl;
        }

        temp.close();
        std::remove(m_cfgFile.c_str());
        std::rename("temp.txt", m_cfgFile.c_str());
        
    } 
    std:: cout << "Minimal quantity of 8 added to new components." << std:: endl;
}

void CfgManager::runProgram()
{
    addNewComponentsToCfg();
    setMinimumQuantityCfg();
    m_accessToInventory.updateInventoryWithMinimalQuantity();
   // m_accessToInventory.printInventory();
}