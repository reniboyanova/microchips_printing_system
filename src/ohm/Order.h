#ifndef ORDER_H
#define ORDER_H
#include<string>

enum class ORDERPRIORITY{
    NORMAL,
    HIGH
};

enum class STATUS{
    UNPROCESSED,
    PENDING
};

class Order
{
private:

    std::string m_orderId;
    std::string m_clientName;
    ORDERPRIORITY m_priority;
    const std::string generateOrderId();
    
    STATUS m_status = STATUS();
public:

    Order(std::string clientName, ORDERPRIORITY priority);
    
    const void setPriority(int priority);
    const std::string& getOrderId() const;
    void setStatus();
    std::string castStatusToStr() const;
    std::string castOrderPriorityToSTr() const;
    const std::string& getClientName() const { return m_clientName; }
    ORDERPRIORITY getPriority() const;
    STATUS getStatus() const;

};

#endif

