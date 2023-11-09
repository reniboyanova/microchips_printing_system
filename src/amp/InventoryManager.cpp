#include "InventoryManager.h"
#include <fstream>


void InventoryManager::printInventory() 
{
    std::cout << "Inventory: " << std::endl;

   std::ifstream fileIn(m_inventoryFile);
   std::string line;
   std::vector<std::string> inventory;
        

        if (!fileIn.is_open())
        {
            std::cerr << "Cannot open orderTest.txt for reading." << std::endl;
            return;
        }
        else
        {
            while(getline(fileIn,line))
            {
                inventory.push_back(line);
            }
        }
        fileIn.close();
        printVec(inventory);
}

void InventoryManager::printVec(const std::vector<std::string>& inventory)
{
    for (int i =0; i< inventory.size(); ++i)
    {
        std::cout << inventory[i] <<std:: endl;
    }
}

void InventoryManager::updateInventoryWithMinimalQuantity()
{
    std::vector<std::string> storeLines;
    std::string line;
    std::ifstream file;
    std::ofstream temp;

    file.open(m_inventoryFile);
    temp.open("temp.txt");
    int indexOfVector = -1;

    while (std::getline(file, line))
    {
        ++indexOfVector;
        storeLines.push_back(line);
        m_name = getNameOfLine(line);
        m_quantity = getQuantity(line);

        if (m_quantity != m_minimalQuantity)
        {
            std::string addToVec = m_name + " " + std::to_string(m_minimalQuantity);
            storeLines[indexOfVector] = addToVec;
        }
        m_name.clear();
    }

    for (int i = 0; i < storeLines.size(); ++i)
    {
        temp << storeLines[i] << std::endl;
    }

    temp.close();
    file.close();
    std::remove(m_inventoryFile.c_str());
    std::rename("temp.txt", m_inventoryFile.c_str());
    std::cout << "Delivered new products to reach the minimum quantity!" << std::endl;
}

std::string InventoryManager::getNameOfLine(const std::string& line)   //Board8 8 
{
    for (int i = 0 ; i < line.size(); ++i)
    {
        if (line[i] != ' ')
        {
            m_name = m_name + line[i];
        }
        else
        {
            break;
        }
        
    }
    return m_name;
}

int InventoryManager::getQuantity(const std::string& line) //Board8 80
{
    int quantity = 0;
    for (int i = 0; i < line.size(); ++i)
    {
        if (line[i] == ' ')
        {
            quantity = std::stoi(line.substr(i));
            break;
        } 
    }
    return quantity;
}

std::string InventoryManager:: getName(const std::string& line)
{
    std::string name;

    for (int i = 0; i< line.size(); ++i)
    {
        if (line[i] == ' ')
        {
            break;
        }
        else
        {
            name += line[i];
        }
    }
    return name;
}

