#include "ProcessManagement.h"
#include "../shared/HardwareComponent.h"
#include <string>
#include <algorithm>
#include <memory>

    void ProcessManagement:: printVec () 
    {
        for (int i = 0; i < m_storesFirstFourDetailsOfOrder.size(); ++i)
        {
            std:: cout << m_storesFirstFourDetailsOfOrder[i];
        }
    }

    std::string ProcessManagement:: readAndDeleteFirstLine()
    {
        std::ifstream fileIn;
        std::ofstream temp;
        std::string line;

        fileIn.open(m_ordersFile);
        temp.open(tempFileInData);

        if (!fileIn.is_open())
        {
            std::cerr << "Cannot open orders.txt for reading." << std:: endl;
        }
        else
        {
            std::getline(fileIn,line);
            m_orderLine = line;

            while (std::getline(fileIn,line))
            {
                temp << line << std::endl;
            }
        }

        temp.close();
        fileIn.close();
        std::remove(m_ordersFile.c_str());
        std::rename(tempFileInData.c_str(), m_ordersFile.c_str());


        return m_orderLine;
    }
    
    void ProcessManagement:: fillVectorWithFirstFourElementsFromOrder()
    {
        int countSpaces = 0;
        int countLimit = 4; // the space after sdtatus ends the loop
        int storeIvalue = 0;
        std:: string storeWord;
        std::string inputStringDublicate;
        for (int i = 0; i< m_orderLine.size(); ++i)
        {
            ++ storeIvalue;

            if (m_orderLine[i] != ' ')
            {
                    storeWord += m_orderLine[i];     
            }
            else
            {
                storeWord += m_orderLine[i];
                m_storesFirstFourDetailsOfOrder.push_back(storeWord);
                storeWord ="";
                ++ countSpaces;
            }

            if (countSpaces == countLimit)
            { 
                clearFirstFourDetailsFromString(storeIvalue, m_orderLine, inputStringDublicate);
                break;
            }
        }
        m_orderLine = inputStringDublicate;
    }

    void ProcessManagement:: clearFirstFourDetailsFromString(const int& storeIvalue, std::string& inputString, std::string& inputStringDublicate)
    {
        for (int j = storeIvalue; j< inputString.size(); ++j)
            {
                inputStringDublicate += inputString[j];
            }
    }

    void ProcessManagement:: putOrderBackInOrdersAsPending()
    {
        std::string pendingOrderLine;
        std::string pendingStatus = "Pending; ";
        int pendingStatusIndex = 3;

        m_storesFirstFourDetailsOfOrder[pendingStatusIndex] = pendingStatus;

        for (int i = 0; i < m_storesFirstFourDetailsOfOrder.size(); ++i)
        {
            pendingOrderLine += m_storesFirstFourDetailsOfOrder[i];
        }

        pendingOrderLine += m_orderLine;
        m_orderLine = pendingOrderLine;

        std::ofstream file(m_ordersFile, std::ios::app);

        if (file.is_open())
        {
            file << m_orderLine << std:: endl;

            file.close();
        }
        else
        {
            std::cerr << "Unable to open orders.txt." << std:: endl;
        }   
    } 

    void ProcessManagement:: checkIfElComponentIsRegisteredInInventory() // if not, adds it with 0 quantity
    {
        int zeroQuantity = 0;
        std::ifstream fileIn(m_inventoryFile);

        if (fileIn.is_open()) 
        {
            bool found = false;
            std::string line;
            std::string storeElCompIDFromLine;

            while (std::getline(fileIn, line)) 
            {
                for (int i = 0; i<= line.size(); ++i)
                {
                    if (line[i] != ' ')
                    {
                        storeElCompIDFromLine += line[i];
                    }
                    else 
                    {
                        break;
                    }
                }
                if (storeElCompIDFromLine == m_ElectronicComponentID) 
                {
                    found = true;
                    break;
                }
                storeElCompIDFromLine = "";
            }

            fileIn.close();

            std::ofstream fileOut(m_inventoryFile, std::ios::app);

            if (fileOut.is_open()) 
            {
                if (!found) 
                {
                    fileOut << m_ElectronicComponentID << " " << zeroQuantity << std::endl;
                    std::cout << "Electronic component not found and added for delivery!" << std::endl;
                }

                fileOut.close();
            } 
            else 
            {
                std::cerr << "Unable to open the inventory file for writing." << std::endl;
                return;
            }
        } 
        else 
        {
            std::cerr << "Unable to open the inventory file for reading." << std::endl;
            return;
        }
    }

    void ProcessManagement:: updateElectronicComponentQuantity(HardwareComponent& hwComp)
    {   
        std::unordered_map<std::string, int> ELCompNames;
        if(&hwComp != nullptr){
            for(const auto& el: hwComp.m_electronicComponents)
            {
                ELCompNames[el->m_id] += 1;
            }
        }
        std::string line;
        std::string storeElCompIDFromLine;
        std::vector<std::string> storeLines;
        int quantity = 0;
         //////
        int storeindexNumberOfQuantity = 0; 
        int countRows = 0;
        int countRowsDublicate = 0;
        std::ifstream fileIn;
        std::ofstream temp;
        
        fileIn.open(m_inventoryFile);
        temp.open(tempFileInAmp);

        while (std::getline(fileIn, line)) 
        {
            storeLines.push_back(line);
            for (int i =0; i<line.size(); ++i) // takes the electronic component ID from inventory.txt  
            {
                if (line[i] != ' ')
                {
                    storeElCompIDFromLine += line[i];
                }
                else
                {
                    storeindexNumberOfQuantity = i; // takes the index of quantity
                    break;
                }
            }
            if (storeElCompIDFromLine == m_ElectronicComponentID)
            {
                countRowsDublicate = countRows;
                quantity = stoi(line.substr(storeindexNumberOfQuantity)); //take the quantity 
            }
            else
            {
                ++countRows;
            }
            storeElCompIDFromLine = "";
        }

        if ((!ELCompNames.empty() && quantity - ELCompNames.at(m_ElectronicComponentID) < 0) || ELCompNames.empty() && quantity == 0 )
        {

            fileIn.close();
            temp.close();
            //std::remove(m_inventoryFile.c_str());
            //std::rename(tempFileInAmp.c_str(), m_inventoryFile.c_str());
            
            addMaterialsBackIfOrderReturned(ELCompNames);
            putOrderBackInOrdersAsPending();
            std::cout << "No available materials. Order back to orders with status pending." << std::endl;
            std:: string addToVec = m_ElectronicComponentID + " " + std::to_string(quantity);  //2n305 0
            storeLines[countRowsDublicate] = addToVec;
            m_checkOrder = false; 
        }
        else
        {   
            if(&hwComp != nullptr)
            {
                if(quantity - ELCompNames.at(m_ElectronicComponentID) >= 0)
                {
                    quantity -= ELCompNames.at(m_ElectronicComponentID);
                }
                
            }
            
            std:: string addToVec = m_ElectronicComponentID + " " + std::to_string(quantity);
            m_storeTakenMatrerialsForOrder.push_back(m_ElectronicComponentID);
            storeLines[countRowsDublicate] = addToVec;

            for (int i  = 0; i< storeLines.size(); ++i)
            {   
                temp << storeLines[i]<< std::endl;
            }
            
            fileIn.close();
            temp.close();
            std::remove(m_inventoryFile.c_str());
            std::rename(tempFileInAmp.c_str(), m_inventoryFile.c_str());
        }
    } 

    void ProcessManagement:: addJobForPrinting()
     {
        int numberOfTimesNeededForPrinting = 1;

        m_libraryManager.m_storesHardwareAndElectronicComponentDesigns = m_orderLine;
        std:: string hardwareComponentID = m_libraryManager.takeHardwareComponentID();
        m_libraryManager.m_storesHardwareComponentDesign = "";

        std::string clientName;

        for (int i = 0; i < m_storesFirstFourDetailsOfOrder[1].size(); ++i)
        {
            if (m_storesFirstFourDetailsOfOrder[1][i] != ':')
            {
                clientName += m_storesFirstFourDetailsOfOrder[1][i];
            }
            else
            {
                break;
            }
        }

        std::ofstream fileOf(m_printJobFile, std::ios::app);
        if (!fileOf.is_open())
        {
            std::cerr << "Cannot open print_jobs for writing" << std::endl;
            return;
        }

        fileOf << clientName <<";" << hardwareComponentID << ";" << numberOfTimesNeededForPrinting << ";" << std::endl;
        std:: cout << "Added order to print_jobs" << std:: endl;

        fileOf.close();
    }

    void ProcessManagement:: showNextSteps()
    {
        std:: cout << "The hardware design along with the electronic designs are in the process of printing over the board..." << std:: endl; 
    }

    void ProcessManagement:: takeBoardInformation() 
    {
        std::string number;
        int countSemiColons = 0;

        for (int i = 0; i < m_orderLine.size(); ++i)
        {
            if (m_orderLine[i] == ';')
            {
                ++countSemiColons;
            }

            if (countSemiColons == 1)
            {
                i += 2; 

                while (m_orderLine[i] != ',')
                {
                    number += m_orderLine[i];
                    ++i;
                }
    
                m_board.m_boardHeight = std::stoi(number); // enters the height of the board
                number = "";

                if (m_orderLine[i] == ',')
                {
                    ++i;
                }

                while (m_orderLine[i] != ';')
                {
                    number += m_orderLine[i];
                    ++i;
                }

                m_board.m_boardWidht = std::stoi(number); // enters the width of the board
                break; 
            }
        }
    }

    void ProcessManagement:: updateBoardQuantity()
    {
        std::string line;
        std::string boardSize;
        std::vector<std::string> storeLines;
        int countRow = 0;
        int quantityPositionIndex = 0;
        int boardSizeNum = 0;
        int boardQuantity = 0;
        
        takeBoardInformation();
        int sizeOfBoard = m_board.getDefaultSize();
        m_board.setBoardSize(sizeOfBoard);

        std::ifstream fileIn;
        fileIn.open(m_inventoryFile);

        if (!fileIn.is_open())
        {
            std::cerr << "Unable to open orders.txt." << std:: endl;
        }   
        else
        {
            std::ofstream temp;
            temp.open(tempFileInAmp);

            while(std::getline(fileIn, line))
            {
                storeLines.push_back(line);
                quantityPositionIndex = line[line.size() - 1]; //last element of the line is the quantity

                for (int i = 0; i<= line.size(); ++i) // line is board8 8
                {
                    if (line[i] == ' ')
                    {
                        break;
                    }
                    else if (line[i] >= '1' && line[i] <= '9')
                    {
                        boardSize += line[i];
                    }
                }
                
                if (line.substr(0,5) == "Board" && (boardSize == "8" || boardSize == "16" || boardSize == "24" ||boardSize == "32" ||boardSize == "64" ||boardSize == "128" ||boardSize == "256" ||boardSize == "512"))
                {
                    boardSizeNum = std::stoi(boardSize);
                }
                //std::cout << "Sizenum: " << boardSizeNum << std::endl;
                if (boardSizeNum == m_board.m_boardSize)
                { 
                    int storeQuantityValueOfCurrentBoard = 0;

                    for (int j = 0; j < line.size(); ++j)
                        {
                            if (line[j] == ' ')
                            {
                                storeQuantityValueOfCurrentBoard = stoi(line.substr(j));      
                                break;
                            }
                        }
                    //std::cout << "Quantity: " << storeQuantityValueOfCurrentBoard << std:: endl;
                    if (storeQuantityValueOfCurrentBoard <= 0)
                    {
                        std::cout << "No available materials. Order back to orders with status pending." << std::endl;
                        m_checkOrder = false;
                        putOrderBackInOrdersAsPending();
                    }
                    else
                    { 
                        storeQuantityValueOfCurrentBoard -= 1;
                        std::string boardName = "Board" + boardSize;
                        storeLines[storeLines.size()-1] = boardName + " " + std::to_string(storeQuantityValueOfCurrentBoard); // the new vector element with the new quantity
                        //std::cout << storeLines[storeLines.size()-1] << std:: endl;
                        m_storeTakenMatrerialsForOrder.push_back(boardName);
                        std::cout << "Board quantity is taken" << std:: endl;
                    }  
                }
                boardSize = ""; 
                boardSizeNum = 0;
            }

            for (int i  = 0; i< storeLines.size(); ++i)
            {
                temp << storeLines[i]<< std::endl;
            }

            temp.close();
            fileIn.close();
            std::remove(m_inventoryFile.c_str());
            std::rename(tempFileInAmp.c_str(), m_inventoryFile.c_str());
        }
    }

    std::string ProcessManagement:: takeElectronicComponentID(const std::string& component)
    {
        std::string storeID;

        for (int i = 0; i < component.size(); ++i)
        {
            if (component[i] == ';')
            {
                break;
            }
            else
            {
                storeID += component[i];
            }
        }
        return storeID;
    }

    int ProcessManagement:: takeNumberOfOrders()
    {
        std::ifstream fileIn;
        std::string line;
        int numberOfOrdersForThePeriod = 0;

        fileIn.open(m_ordersFile);

        if(!fileIn.is_open())
        {
            std::cerr << "Cannot open orders.txt for reading the number of orders" << std:: endl;
        }
        else
        {
            while(getline(fileIn,line))
            {   
               
                ++numberOfOrdersForThePeriod;
            }
        }
        return numberOfOrdersForThePeriod;
    }

    void ProcessManagement:: sortByPriority()
    {
        std::string line;
        std::ifstream fileIn;
        std::ofstream temp;
        std::vector<std::string> keepsPendingStatusOrders;
        std::vector<std::string> keepNormalPrioOrders;
        std::vector<std::string> keepHighPrioOrders;

        fileIn.open(m_ordersFile);
        temp.open(tempFileInData);
        
        if (!fileIn.is_open())
        {
            std::cerr << "Cannot open order.txt for sorting." << std:: endl;
        }
        else
        {
            while(getline(fileIn,line))
            {
                std::string priority;
                std::string status;
                getPriority(line, priority);
                getStatus(line,status);

                if (status == "PENDING")
                {
                    keepsPendingStatusOrders.push_back(line);
                }
                else if (priority[0] == 'h' || priority[0] == 'H') // checks if it is high prio
                {
                    keepHighPrioOrders.push_back(line);
                }
                else if (priority[0] == 'n' || priority[0] == 'N') // checks if it is normal prio
                {
                    keepNormalPrioOrders.push_back(line);
                }
            }

            for (int i = 0; i < keepsPendingStatusOrders.size(); ++i)
            {
                temp << keepsPendingStatusOrders[i] << std::endl;
            }

            for (int i = 0; i < keepHighPrioOrders.size(); ++i)
            {
                temp << keepHighPrioOrders[i] << std::endl;
            }

            for (int i = 0; i < keepNormalPrioOrders.size(); ++i)
            {
                temp << keepNormalPrioOrders[i] << std::endl;
            }

            temp.close();
            fileIn.close();
            std::remove(m_ordersFile.c_str());
            std::rename(tempFileInData.c_str(), m_ordersFile.c_str());
        }
    }

    void ProcessManagement:: getStatus(std::string& line, std::string &status) //C1927BC Samsung: HIGH; UNPROCESSED; 
    {
        for (int i = 0; i < line.size(); ++i)
        {
            if (line[i] == ';')
            {
                i += 2;
                for (int j = i; j < line.size(); ++j)
                {
                    if (line[j] != ';')
                    {
                        status += line[j];
                    }
                    else
                    {
                        break;
                    }
                }
                break;
            }
        }
    }

    void ProcessManagement:: getPriority(std::string& line, std::string& priority)
    {
        
        for (int i = 0; i < line.size(); ++i)
        {
            if (line[i] == ':')
            {
                i += 2;
                for (int j = i; j < line.size(); ++j)
                {
                    if (line[j] != ';')
                    {
                        priority += line[j];
                    }
                    else
                    {
                        break;
                    }
                }
                break;
            }
        }
    }

    void ProcessManagement:: addMaterialsBackIfOrderReturned(const std::unordered_map<std::string, int>& ELCompNames)
    {
        std::ifstream fileIn;
        std::ofstream temp;
        std::string line;
        std::vector<std::string> storeLines;
    
        fileIn.open(m_inventoryFile);
        temp.open(tempFileInAmp);

        if (!fileIn.is_open())
        {
            std:: cerr << "Cannot open inventory for returning back the materials taken as the order cannot be processed." << std:: endl;
        }
        else
        {
            while(getline(fileIn, line)) // Board8 8
            {
                std::string storeCurrentBoardName;
                int storeCurrentQuantity;

                for (int i = 0; i < line.size(); ++i) 
                {
                    if (line[i] != ' ')
                    {
                        storeCurrentBoardName += line[i];
                    }
                    else
                    {
                        storeCurrentQuantity = stoi(line.substr(i));
                        break;
                    }
                }

                for (int i = 0; i < m_storeTakenMatrerialsForOrder.size(); ++i)
                {
                    if (m_storeTakenMatrerialsForOrder[i] == storeCurrentBoardName)
                    {   
                        int newQuantity;
                        std::unordered_map<std::string, int>::const_iterator got = ELCompNames.find(storeCurrentBoardName);
                        if(storeCurrentBoardName.substr(0,5) == "Board"){
                            newQuantity = storeCurrentQuantity + 1; // adding back quantity
                        }
                        else if(got != ELCompNames.end()){
                            newQuantity = storeCurrentQuantity + ELCompNames.at(storeCurrentBoardName); 
                        } // adding back quantity
                        line = storeCurrentBoardName + " " + std::to_string(newQuantity);
                    }
                }

                storeLines.push_back(line);
            }

            for (int i = 0; i < storeLines.size(); ++i)
            {
                temp << storeLines[i] << std:: endl;
            }
        }

        temp.close();
        fileIn.close();
        std::remove(m_inventoryFile.c_str());
        std::rename(tempFileInAmp.c_str(), m_inventoryFile.c_str());
    }

    void ProcessManagement:: printCurrentPrintJobs()
    {
        std::ifstream fileIn;
        std::string line;

        fileIn.open(m_printJobFile);
        
        if (!fileIn.is_open())
        {
            std::cerr << "Cannot open printJobs for printing the current jobs." << std:: endl;
        }
        else
        {
            std:: cout << "Current jobs for printing:\n";
            std:: cout << "----------------------------\n";
            while (getline(fileIn, line))
            {
                std:: cout << line << std:: endl;
            }
        }
            
        std:: cout << "----------------------------\n";
        fileIn.close();

    }

    void ProcessManagement:: runProgram() 
    {
        m_libraryManager.storesReadyProductsInVector();
        
        m_libraryManager.updateHistoryWithCompletedOrdes();

        sortByPriority();

        int numberOfOrders = takeNumberOfOrders();

        while (numberOfOrders != 0)
        {
            readAndDeleteFirstLine(); //аsOrder
            fillVectorWithFirstFourElementsFromOrder();
            m_libraryManager.m_storesHardwareAndElectronicComponentDesigns = m_orderLine;

            std::string IdName;

            m_libraryManager.fillHardwareComponentDesign(IdName);

            m_libraryManager.addNewHardwareDesignToLibrary();
           
            
            m_libraryManager.fillVectorWithElectronicComponents();
            
            takeBoardInformation();
            updateBoardQuantity(); 

            for (int i = 0; i < m_libraryManager.m_electronicComponents.size(); ++i)
            {
                m_ElectronicComponentID = takeElectronicComponentID(m_libraryManager.m_electronicComponents[i]);
                checkIfElComponentIsRegisteredInInventory();
            }

            if (m_checkOrder)
            {
                for (int i = 0; i < m_libraryManager.m_electronicComponents.size(); ++i) // loop for the inventory checks
                {
                    m_ElectronicComponentID = takeElectronicComponentID(m_libraryManager.m_electronicComponents[i]);
                    std::unique_ptr<HardwareComponent> ptrHW;
                    try{
                        ptrHW = std::make_unique<HardwareComponent>(IdName);
                    }
                    catch(const std::exception& e){
                        ptrHW = nullptr;
                    }
                    updateElectronicComponentQuantity(*ptrHW);
                    
                    if (!m_checkOrder)
                    {
                        break;
                    }
                }
                    m_libraryManager.keepOnlyNewElectronicDesignsInVector();
                    m_libraryManager.addNewElectronicDesignToLibrary();
            }

            if (m_checkOrder)
            {
                std::cout << "Quantity taken and new orders added for printing" << std::endl; 
                addJobForPrinting();
            }
            //printCurrentPrintJobs();
            m_storeTakenMatrerialsForOrder.clear();
            m_storesFirstFourDetailsOfOrder.clear();
            m_libraryManager.m_electronicComponents.clear();
            m_libraryManager.m_storesReadyOrdersFromProduction.clear();
            m_libraryManager.m_storesHardwareComponentDesign = "";
            m_checkOrder = true;
            --numberOfOrders;
        }
    }
