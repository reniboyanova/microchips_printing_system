#ifndef ORDERMANAGEMENT_H
#define ORDERMANAGEMENT_H

#include "Order.h"
#include <vector>
#include <map>

class OrderManagement
{
private:
    enum class OHM_OPTIONS{
        ADD_BY_FILE = 0,
        ADD_BY_CIN = 1,
        SHOW_ORDERS_IN_ORDER_THEY_WARE_MADE = 2,
        SHOW_ORDERS_BY_HIGH_PRIORITY = 3, 
        SHOW_ORDERS_BY_NORMAL_PRIORITY = 4,
        CANCEL_ORDER = 5,
        EXIT = 6,
    };

    using mapWithPreOrderComponents = std::map<int, std::vector<std::string>>;
    std::vector<Order> m_orders;
    static const std::string m_cfgFileName;

    mapWithPreOrderComponents preOrderComponents;
    mapWithPreOrderComponents parseInformation(std::vector<std::string>& components);
    void splitPreOrderInfo(const std::string& str, std::vector<std::string> &components);
    bool fullPreOrderInfo();
    void serializationOrders(Order currentOrder);
    bool searchForComponents(std::string &componentID,  const std::string &fileName);

public:
    void processOrdersFromFstream(const std::string &cfgFileName); 
    void showAllOrders(void (OrderManagement::*sortOrderFunction)());
    void showHighPriorityOrdersFirst();
    void showNormalPriorityOrdersFirst();
    void cancelOrder();
    std::string takeClientInfoForHardWareComponents(); //const?
    void deserializeFromOrders();
    void orderFromCin();
    void menu();    
};
#endif

