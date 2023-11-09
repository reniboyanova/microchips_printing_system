#include <iostream>
#include <string>
#include <fstream>

#include "Order.h"

std::string PRIORITY_MSG = "Please, set a priority for ORDER:\n(0 for NORMAL; 1 for HIGH):\n";
std::string STATUS_MSG = "Please, set a status for ORDER:\n(0 for UNPROCESSED (default); 1 for PENDING):\n";

const std::string Order::generateOrderId()
{
    const unsigned int idBase = 1000;
    const unsigned int currentId = idBase + (rand() % idBase);


    const unsigned int ASCIIBaseLow = 65; //(A)
    const unsigned int ASCIIBaseHigh = 70; //(F)

    char firstLetter = ASCIIBaseLow + rand() % (ASCIIBaseHigh - ASCIIBaseLow + 1);
    char secondLetter = ASCIIBaseLow + rand() % (ASCIIBaseHigh - ASCIIBaseLow + 1);
    char thirddLetter = ASCIIBaseLow + rand() % (ASCIIBaseHigh - ASCIIBaseLow + 1);

    std::string uniqueOrderId;  
    uniqueOrderId += firstLetter;
    
    uniqueOrderId += std::to_string(currentId);
    uniqueOrderId += secondLetter;
    uniqueOrderId += thirddLetter;
    return uniqueOrderId;
}

Order::Order(std::string clientName, ORDERPRIORITY priority)
    : m_clientName(clientName), m_priority(priority)
{
    m_orderId = generateOrderId();
}

const std::string& Order::getOrderId() const {
    return m_orderId;
}


const void Order::setPriority(int priority) {
    this->m_priority = static_cast<ORDERPRIORITY>(priority);
}

void Order::setStatus() {
    std::cout << STATUS_MSG << std::endl;

    int answer = 0;
    std::cin >> answer;

    this->m_status = static_cast<STATUS>(answer);
}

std::string Order::castOrderPriorityToSTr() const{
    std::string prioString;
        switch (m_priority) {
            case ORDERPRIORITY::HIGH:
                prioString = "HIGH";
                break;
            case ORDERPRIORITY::NORMAL:
                prioString = "NORMAL";
                break;
        }
    return prioString;
}

std::string Order::castStatusToStr() const{
    std::string statusString;
        switch (m_status) {
            case STATUS::UNPROCESSED:
                statusString = "UNPROCESSED";
                break;
            case STATUS::PENDING:
                statusString = "PENDING";
                break;
        }
    return statusString;
}


ORDERPRIORITY Order::getPriority() const {
    return m_priority;
}

STATUS Order::getStatus() const {
    return m_status;
}


