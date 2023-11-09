#include "InventoryManager.h"

class CfgManager
{
    private: 

        InventoryManager m_accessToInventory;
        std::vector<std::string> m_storeCfgLines; //elements

        int getNumberOfLinesOfTextFile(std::string& fileName);
    
    public:

        void addNewComponentsToCfg();
        void changeMinimumQuantity(); // needs to be added
        void setMinimumQuantityCfg();
        void fillVectorWithCfgLines();
        std::string m_cfgFile = "cfg.txt";

        void runProgram();
};