#include "UpdateDesignLibrary.h"
#include <sstream>

void UpdateDesignLibrary:: fillHardwareComponentDesign(std::string& IdName)
{
    for (int i = 0; i < m_storesHardwareAndElectronicComponentDesigns.size(); ++i)
    {
        if (m_storesHardwareAndElectronicComponentDesigns[i] == '{')
        {
            i += 2; //to avoid printing the second {
            bool run = true;
            while (run)
            {
                m_storesHardwareComponentDesign += m_storesHardwareAndElectronicComponentDesigns[i];
                ++i;
                if (m_storesHardwareAndElectronicComponentDesigns[i] == '}' && m_storesHardwareAndElectronicComponentDesigns[i+1] == '}')
                {
                    run = false;
                }
            }

            std::istringstream ss(m_storesHardwareComponentDesign);
            std::getline(ss,IdName,';');

            break;
        }
    }
}

std::string UpdateDesignLibrary:: takeHardwareComponentID()
{
    std:: string ID;
    for (int i = 0; i< m_storesHardwareComponentDesign.size(); ++i)
    {
        if (m_storesHardwareComponentDesign[i] != ';')
        {
            ID += m_storesHardwareComponentDesign[i];
        }
        else
        {
            break;
        }
    }
    return ID;
}

bool UpdateDesignLibrary:: checkIfHardwareDesignExists()
{
    std::string hardwareDesignID = takeHardwareComponentID();
    std::string line;
    std::ifstream fileIn(m_hardwareLibrary);
    bool ifFound = false;

    if (fileIn.is_open()) 
    {
        std::string currentHardwareComponentID;

        while (std::getline(fileIn, line))  // takes HW comp ID
        {
            for (int i = 0; i<= line.size(); ++i)
            {
                if (line[i] != ';')
                {
                    currentHardwareComponentID += line[i];
                }
                else 
                {
                    break;
                }
            }

            if (currentHardwareComponentID == hardwareDesignID) 
            {
                ifFound = true;
                break;
            }
            currentHardwareComponentID = "";
        }
    }
    else
    {
        std::cerr << "Unable to open the library file for reading." << std::endl;
        return 0;   
    }

    fileIn.close();

    return ifFound;     
}

void UpdateDesignLibrary:: addNewHardwareDesignToLibrary()
{
    bool checkExistance = checkIfHardwareDesignExists();
    
    if (!checkExistance)
    {
        std::ofstream fileOut(m_hardwareLibrary, std::ios::app);

        if (fileOut.is_open()) 
        {
            fileOut << m_storesHardwareComponentDesign << std:: endl;
            std:: cout << "New hardware design added in the library!" << std:: endl;
            fileOut.close();
        }
        else
        {
            std::cerr << "Unable to open the library file for writing." << std::endl;
            return;
        }
    }
} 

void UpdateDesignLibrary:: fillVectorWithElectronicComponents() // store all the el components from the orderLine in a vector
{
    int sizeOfHWDesign = m_storesHardwareComponentDesign.size();
    int countSemiColons = 0;
    std::string lineDuplicate;
    std::string component;

    lineDuplicate = m_storesHardwareAndElectronicComponentDesigns.substr(sizeOfHWDesign + 5);       //   m_storesHardwareAndElectronicComponentDesigns[i];

    for (int i = 0; i < lineDuplicate.size()-1; ++i) //};}}; 2N3904; 7; 3; (1,1),(3,1),(5,1); 8D9775; 9; 9; (1,1),(3,1),(5,1),(7,1),(1,7),(3,7),(5,7),(7,7); 
    {
        if (lineDuplicate[i] == ' ') //&& m_storesHardwareAndElectronicComponentDesigns[i+1] == '{'
        {
            ++i;
            bool run = true;
            while (run)
            {
                component += lineDuplicate[i];

                ++i;
                if ( lineDuplicate[i] == ';') 
                {
                    ++countSemiColons;
                }
                if ( countSemiColons == 4) //m_storesHardwareAndElectronicComponentDesigns[i] == ';' &&
                {
                    component += lineDuplicate[i];
                    run = false;
                    countSemiColons = 0;
                    m_electronicComponents.push_back(component);
                }
                
            }
            component.clear();
        }
    }
}

void UpdateDesignLibrary:: keepOnlyNewElectronicDesignsInVector()
{
    std::ifstream fileIn(m_elLibrary);

    if (fileIn.is_open()) 
    {
        std::string line;
        std::string storeWord;

        while (std::getline(fileIn, line))  // takes el. component ID
        {
            for (int i = 0; i<= line.size(); ++i)
            {
                if (line[i] != ';')
                {
                    storeWord += line[i];
                }
                else 
                {
                    break;
                }
            }
            for (int j = 0; j < m_electronicComponents.size(); ++j)
            {
                std::string elCompID;

                for (int y = 0; y<= m_electronicComponents[j].size(); ++y)
                {
                    if (m_electronicComponents[j][y] != ';')
                    {
                        elCompID += m_electronicComponents[j][y];
                    }
                    else 
                    {
                        break;
                    }
                }

                if (storeWord == elCompID) 
                {
                    if(j != m_electronicComponents.size() - 1 )
                    {
                        int sizeElectronicComponents = m_electronicComponents.size() - 1;
                        std::string temp = m_electronicComponents[j];  // optimize with swap
                        m_electronicComponents[j] = m_electronicComponents[sizeElectronicComponents];
                        m_electronicComponents[sizeElectronicComponents] = temp;
                    }
                    else
                    {
                        m_electronicComponents.pop_back();
                    }
                }
            }
            storeWord = "";
        }
    }
    else
    {
        std::cerr << "Unable to open the library file for reading." << std::endl;
        return;   
    }

    fileIn.close();  
}

void UpdateDesignLibrary:: addNewElectronicDesignToLibrary()
{
    std::ofstream fileOut(m_elLibrary, std::ios::app);

        if (fileOut.is_open()) 
        {
            for (int i = 0; i < m_electronicComponents.size(); ++i)
            {
                fileOut << m_electronicComponents[i] << std:: endl;
                std:: cout << "New electronic design added in the library!" << std:: endl;
            }

            fileOut.close();
        }
        else
        {
            std::cerr << "Unable to open the library file for writing." << std::endl;
            return;
        }
} 

void UpdateDesignLibrary:: printVec ()
{
    for (int i = 0; i< m_electronicComponents.size(); ++i)
    {
        std::cout <<  "El comp: " << m_electronicComponents[i] << std::endl;
    }
}

void UpdateDesignLibrary:: storesReadyProductsInVector()
    {
        std::ifstream fileIn;
        std::ofstream temp;
        std::string line;

        fileIn.open(m_productionFile);
        temp.open("../../data/temp.txt");

        if (fileIn.is_open()) 
        {
            while(getline(fileIn, line))
            {
                m_storesReadyOrdersFromProduction.push_back(line);
            }

            fileIn.close();
            temp.close();
            std::remove(m_productionFile.c_str());
            std::rename("../../data/temp.txt", m_productionFile.c_str());
            
        }
        else
        {
            std::cerr << "Unable to open the production file." << std:: endl;
            return;
        }
    }

void UpdateDesignLibrary:: updateHistoryWithCompletedOrdes()
{   
    if(!m_storesReadyOrdersFromProduction.empty())
    {
    std::ofstream fileOut(m_historyFile, std::ios::app);

        if (fileOut.is_open()) 
        {
            for (int i = 0; i< m_storesReadyOrdersFromProduction.size(); ++i)
            {
                fileOut << m_storesReadyOrdersFromProduction[i] << std::endl;
            }
            std:: cout << "History updated!" << std:: endl;

            fileOut.close();
        }
        else
        {
            std::cerr << "Unable to open History.txt for writting." << std::endl;
        }
    }
}