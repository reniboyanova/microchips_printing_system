#include <iostream>
#include <vector>

class InventoryManager
{
    private:

        int m_quantity = 0;
        std::string m_name;
        
        void printVec(const std::vector<std::string>& inventory);
        std::string getNameOfLine(const std::string& line);



    public:

        std::string m_inventoryFile = "../../data/inventory.txt";
        int m_minimalQuantity = 20;
        void printInventory();
        void updateInventoryWithMinimalQuantity();
        int getQuantity(const std::string& line);
        std::string getName(const std::string& line);
        
};