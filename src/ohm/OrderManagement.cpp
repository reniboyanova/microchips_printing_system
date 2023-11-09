#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <sstream>  

#include "OrderManagement.h"
#include "OrderMessages.h"
#include "Order.h"
#include "../shared/Utils.h"
#include "../shared/SharedTxtFiles.h"


const std::string OrderManagement::m_cfgFileName = "cfg_ohm.txt"; 

void OrderManagement::orderFromCin(){
    std::fstream fileWIthOrders; 

    fileWIthOrders.open(ORDERS_TXT, std::ios::in | std::ios::out | std::ios::app);  // orders.txt
 
    if (!fileWIthOrders.is_open()) {
        std::cerr << UNABLE_OPEN_FILE << std::endl;
        return;
    }
    uti::print(GET_NEW_ORDER_MSG);

    uti::print(GET_CLIENT_NAME_MSG);
    std::string clientName;
    std::getline(std::cin, clientName);

    uti::print(GET_PRIORITY_MSG);
    int priority;
    std::cin >> priority;
    std::cin.ignore();

    uti::print(GET_HARDWARE_ID_MSG);
    std::string hardwareID;
    std::getline(std::cin, hardwareID);

    std::string electronicID;

    // start scan the library for same hardwareID as that we take from client input
    bool foundHardware = searchForComponents(hardwareID, HARDWARE_COMPONENTS_LIBRARY_FILE_PATH);

    if (!foundHardware){
        uti::print(NOT_FOUND_HARDWARE_COMPONENT);
        electronicID += SEMICOLON_WITH_SPACE;
        electronicID = takeClientInfoForHardWareComponents();
        if(electronicID.empty()){
            uti::print(NOT_SUCCESS_ORDER);
            fileWIthOrders.close();
            return;
        }
    }else{
        hardwareID += SEMICOLON + DOUBLE_CURLLY_BRACKET_CLOSE;
    }
    
    Order currentOrder(clientName, static_cast<ORDERPRIORITY>(priority));
    fileWIthOrders << currentOrder.getOrderId() << ' ' << clientName << COLON_WITH_SPACE << currentOrder.castOrderPriorityToSTr() << SEMICOLON_WITH_SPACE  << currentOrder.castStatusToStr() << SEMICOLON_WITH_SPACE << DOUBLE_CURLLY_BRACKET_OPEN <<  hardwareID << electronicID << std::endl;
    uti::print(SUCCESS_ORDER);
    m_orders.push_back(currentOrder);
  
    fileWIthOrders.close();
    std::cout << std::endl;

}

bool OrderManagement::searchForComponents(std::string &componentID,  const std::string &fileName){
    std::ifstream fileWithComponents(fileName);
    std::string searchedComponent;
    
    if (!fileWithComponents.is_open()) {
        std::cerr << UNABLE_OPEN_FILE + COLON_WITH_SPACE << fileName << std::endl;
        return false;
    }

    while (std::getline(fileWithComponents, searchedComponent)) {
        size_t delimiterPos = searchedComponent.find(SEMICOLON); 

        if (delimiterPos != std::string::npos) {
            searchedComponent = searchedComponent.substr(0, delimiterPos); 
        }

        if (searchedComponent == componentID){
            uti::print(FOUND_COMPONENT + fileName);
            return true;
        }
    }
    fileWithComponents.close();
    return false;
}

void OrderManagement::showAllOrders(void (OrderManagement::*sortOrderFunction)()) {
    if (sortOrderFunction != nullptr) {
        (this->*sortOrderFunction)();
    }
    for (const auto& order : m_orders) {
        std::cout << "Order#: " << order.getOrderId() << "; Client Name:" << order.getClientName() << "; Priority: " << order.castOrderPriorityToSTr() << "; Status: " << order.castStatusToStr() << std::endl;
    }

    std::cout << std::endl;
}

    
void OrderManagement::showHighPriorityOrdersFirst(){
    std::sort(m_orders.begin(), m_orders.end(), [](const Order& order1, const Order& order2) {
        return order1.getPriority() > order2.getPriority();
    });
}


void OrderManagement::showNormalPriorityOrdersFirst(){
    std::sort(m_orders.begin(), m_orders.end(), [](const Order& order1, const Order& order2) {
        return order1.getPriority() < order2.getPriority();
    });
}

void  OrderManagement::cancelOrder(){
    uti::print(CANSEL_ORDER_MSG);
    std::string orderID;
    std::getline(std::cin, orderID);
    bool findOrder = false;

    for (size_t i = 0; i < m_orders.size(); i++)
    {
        if (m_orders[i].getOrderId() == orderID){
            m_orders.erase(m_orders.begin() + i);
            findOrder = true;
            break;
        }  
    }
    if (!findOrder){
        uti::print(CANT_FIND_ORDER_BY_ID);
    }else{
        uti::print(SUCCESS_CANSEL_ORDER);
    }
}


void OrderManagement::deserializeFromOrders() {
    std::ifstream fileWithCfgOrders(m_cfgFileName); // cfg_ohm.txt
    if (fileWithCfgOrders.is_open()) {
        m_orders.clear();
        std::string line;
        std::vector<std::string> components;
        int ordersCount = 0;

        while (std::getline(fileWithCfgOrders, line)) {
            if (line.empty()) {
                preOrderComponents = parseInformation(components);
                if (!preOrderComponents.empty()){
                    std::string name = preOrderComponents[0][0];
                    int priority = std::stoi(preOrderComponents[1][0]);
                    Order newOrder(name, static_cast<ORDERPRIORITY>(priority)); 
                    serializationOrders(newOrder);
                    preOrderComponents.clear();
                    components.clear();
                    m_orders.push_back(newOrder);
                }else{
                    components.clear();
                    continue;
                }
            }
            splitPreOrderInfo(line, components);  
        }
        preOrderComponents = parseInformation(components);
        if (!preOrderComponents.empty()){
            std::string name = preOrderComponents[0][0];
            int priority = std::stoi(preOrderComponents[1][0]);
            Order newOrder(name, static_cast<ORDERPRIORITY>(priority)); 
            serializationOrders(newOrder);
            preOrderComponents.clear();
            components.clear();
            m_orders.push_back(newOrder);
        }
        fileWithCfgOrders.close();
        uti::print(SUCCESS_DESERIALIZE_ORDERS);
    } else {
        std::cerr << UNABLE_OPEN_FILE << std::endl;
    }
}


bool OrderManagement::fullPreOrderInfo(){
    for (auto &component : preOrderComponents)
    {
        if ((std::stoi(preOrderComponents[PRIORITY_POSITION_IN_MAP][0]) != (int)ORDERPRIORITY::HIGH) || (std::stoi(preOrderComponents[PRIORITY_POSITION_IN_MAP][0])) != (int)ORDERPRIORITY::NORMAL){
            return false;
        }
        if(component.second[0].empty()){
            return false;
        }
    }
    return true;
}

void OrderManagement::splitPreOrderInfo(const std::string& str, std::vector<std::string> &components) {
    const char delimiter = '\n';
    std::istringstream iss(str);
    std::string token;
    while (std::getline(iss, token, delimiter)) {
        components.push_back(token);
    }
}


void OrderManagement::serializationOrders(Order currentOrder){
    std::fstream fileWIthOrders; 
    std::string hardwarecomponent;
    std::string otherInfo;
    int i = 0;
    fileWIthOrders.open(ORDERS_TXT, std::ios::out | std::ios::app); // orders.txt
    for (auto &component : preOrderComponents)
    {   
        if (i == HARDWARE_POSITION_IN_MAP){
            hardwarecomponent += DOUBLE_CURLLY_BRACKET_OPEN;
            hardwarecomponent += component.second[0];
            i++;
            continue;
        }else if (i >= 3) {
            for (auto &part : component.second){
                if (!part.empty()){
                    otherInfo += part;
                }
                
            }
            otherInfo += SEMICOLON_WITH_SPACE;
        }
        i++;
    }
   
    if (otherInfo.empty()){
        hardwarecomponent += SEMICOLON;
        otherInfo += DOUBLE_CURLLY_BRACKET_CLOSE;
    }else{
        hardwarecomponent += SEMICOLON_WITH_SPACE;
    }
    
    fileWIthOrders << currentOrder.getOrderId() << ' ' << currentOrder.getClientName() << COLON_WITH_SPACE << currentOrder.castOrderPriorityToSTr() << SEMICOLON_WITH_SPACE << currentOrder.castStatusToStr() << SEMICOLON_WITH_SPACE <<  hardwarecomponent  << otherInfo << std::endl;
    fileWIthOrders.close();
}

std::string OrderManagement::takeClientInfoForHardWareComponents(){
    std::string hardwareComponentRecepy;
    uti::print(GET_HARDWARE_WIDTH);
    std::string width;
    std::getline(std::cin, width);
    hardwareComponentRecepy += width + SEMICOLON_WITH_SPACE;

    uti::print(GET_HARDWARE_HIGHT);
    std::string height;
    std::getline(std::cin, height);
    hardwareComponentRecepy += height + SEMICOLON_WITH_SPACE;
    
    uti::print(GET_NUMBER_OF_COMPONENTS);
    int nComponents = 0;
    std::cin >> nComponents;
    std::cin.ignore();

    std::string elComponentId;

    std::vector<std::string> elComponentIdSVec;

    for (size_t i = 0; i < nComponents; i++)
    {   
        
        uti::print(GET_ELECTRONIC_COMPONENT_ID);
        std::getline(std::cin, elComponentId);
        elComponentIdSVec.push_back(elComponentId);

        hardwareComponentRecepy += '{';
        hardwareComponentRecepy += elComponentId; 
   
        hardwareComponentRecepy += COLON_WITH_SPACE;

        std::string coordinatesInfo;
        uti::print(GET_COORDINATES_WITH_ROTATION);
        std::getline(std::cin, coordinatesInfo);

        hardwareComponentRecepy += coordinatesInfo;
        
        hardwareComponentRecepy += '}' + SEMICOLON_WITH_SPACE;
    }
    
    uti::print(GET_CONECTIONS);
    std::string connections;
    std::getline(std::cin, connections);
    connections.erase(std::remove_if(connections.begin(), connections.end(), ::isspace), connections.end());
    uti::replaceRoundBracketsWithCurlyBrackets(connections);

    hardwareComponentRecepy += connections + SEMICOLON + DOUBLE_CURLLY_BRACKET_CLOSE;
    
    for (auto &elCompID : elComponentIdSVec)
    {  
        bool foundElComponents = searchForComponents(elCompID, ELECTRONIC_COMPONENTS_LIBRARY_FILE_PATH);
       
        if (!foundElComponents) {
        uti::print(INSERT_INFO_FOR_EL_COMP_WITH_ID + elCompID);

        hardwareComponentRecepy += SEMICOLON_WITH_SPACE + elCompID + SEMICOLON_WITH_SPACE;
        int elCompHigh = 0;
        int elCompWidth = 0;
        std::string pinsPosition;

        uti::print(GET_EL_ELEMENT_W); 
        std::cin >> elCompWidth;
        std::cin.ignore();
        hardwareComponentRecepy += std::to_string(elCompWidth) + SEMICOLON_WITH_SPACE;

        uti::print(GET_EL_ELEMENT_H);
        std::cin >> elCompHigh;
        std::cin.ignore();
        hardwareComponentRecepy += std::to_string(elCompHigh) + SEMICOLON_WITH_SPACE;

        uti::print(GET_PINS_POSITIONS);
        std::getline(std::cin, pinsPosition);
        hardwareComponentRecepy += pinsPosition;
        }else{
            uti::print(FOUND_ELECTRONIC_COMPONENT_IN_LIBRARY + elCompID);
        }

    }
    
    return hardwareComponentRecepy + SEMICOLON;

}


void OrderManagement::processOrdersFromFstream(const std::string &incomeOrdersFile) {
    std::fstream fileWithCfgOrders(incomeOrdersFile);
    if (fileWithCfgOrders.is_open()){
        std::vector<std::string> components;
        std::string line;
        bool skipLines = false;
        int countLines = 0;

        std::ofstream cfgOrdersToProcess(m_cfgFileName);  // cfg_ohm.txt

        while (std::getline(fileWithCfgOrders, line)) {
            if (line == READ_THIS_ORDER) {
                skipLines = false;
                if(countLines > 0){
                    cfgOrdersToProcess << std::endl; 
                }
                continue;
            } else if ((line == SKIP_THIS_ORDER) || line.empty()) {
                skipLines = true;
                continue;
            }

            if (!skipLines) {
                cfgOrdersToProcess << line << std::endl; 
            }
                countLines++;
            }

        cfgOrdersToProcess.close();
        fileWithCfgOrders.close();
    // deserializeFromOrders(); // CAN BE MAKE HERE, BUT BETTER OUTSIDE
    } else {
        std::cerr << UNABLE_OPEN_FILE << std::endl;
    }
}

void OrderManagement::menu(){
    int option = -1;
    do {
        uti::print(WELCOME_MENU_MSG);
        std::cin >> option;
        std::cin.ignore();
        
        switch (static_cast<OHM_OPTIONS>(option)) {
            case OHM_OPTIONS::ADD_BY_FILE:
                deserializeFromOrders();
                break;

            case OHM_OPTIONS::ADD_BY_CIN:
                orderFromCin();
                break;

            case OHM_OPTIONS::SHOW_ORDERS_IN_ORDER_THEY_WARE_MADE:
                showAllOrders(nullptr);
                break;

            case OHM_OPTIONS::SHOW_ORDERS_BY_HIGH_PRIORITY:
                showAllOrders(&showHighPriorityOrdersFirst);
                break;

            case OHM_OPTIONS::SHOW_ORDERS_BY_NORMAL_PRIORITY:
                showAllOrders(&showNormalPriorityOrdersFirst);
                break;

            case OHM_OPTIONS::CANCEL_ORDER:
                cancelOrder();
                break;

            case OHM_OPTIONS::EXIT:
                uti::print(GOOD_BYE_MASSAGE);
                break;

            default:
                uti::print(INVALID_MENU_OPTIONS_MSG);
                break;
        }
    } while (option != EXIT_OHM);
}


OrderManagement::mapWithPreOrderComponents OrderManagement::parseInformation(std::vector<std::string>& components) {
    int key = 0;    
    std::string delimiter = DELIMITER_CONNECTIONS;
    int elkey = 0;
    bool findElComp = false;
    for (const std::string& component : components) {
        size_t colonPos = component.find(COLON);

        if (component == FIND_COMPONENTS){
            elkey = key;
            findElComp = true;
            continue;
        }
        if(findElComp && !(component.find(delimiter) != std::string::npos)){
            std::string valueStr = component;
            valueStr.erase(std::remove(valueStr.begin(), valueStr.end(), '('), valueStr.end());
            valueStr.erase(std::remove(valueStr.begin(), valueStr.end(), ')'), valueStr.end());
            std::string elComp;
            elComp += "{";
            elComp += valueStr;
            elComp += "}";
            
            preOrderComponents[elkey].push_back(elComp);
    

        }else if (component.find(delimiter) != std::string::npos) {
            findElComp = false;
            std::string valueStr = component.substr(colonPos + 1);
            if (valueStr.empty()){
                preOrderComponents.clear();
                uti::print(NOT_FULL_ORDER_INFO);
                return preOrderComponents;
            }

            valueStr.erase(std::remove_if(valueStr.begin(), valueStr.end(), ::isspace), valueStr.end());
            uti::replaceRoundBracketsWithCurlyBrackets(valueStr);

            preOrderComponents[key].push_back(valueStr + SEMICOLON + DOUBLE_CURLLY_BRACKET_CLOSE);

        }else if (colonPos != std::string::npos) {
            std::string valueStr = component.substr(colonPos + 1);
            valueStr.erase(std::remove_if(valueStr.begin(), valueStr.end(), ::isspace), valueStr.end());

            if (valueStr.empty()){
                preOrderComponents.clear();
                uti::print(NOT_FULL_ORDER_INFO);
                return preOrderComponents;
            }
            
            preOrderComponents[key].push_back(valueStr); 
        }
        key++;
    }
    return preOrderComponents;
}
