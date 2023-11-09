#ifndef HARDWARE_COMPONENT_H
#define HARDWARE_COMPONENT_H
#include "Board.h"
#include "ElectronicComponent.h"
#include "Connection.h"
#include <vector>
#include <string>
#include <memory>


class HardwareComponent{
public:
    HardwareComponent(const std::string& name);
    int m_widht{};
    int m_height{};
    int m_sizeMachineFormat{};
    std::string m_id;
    std::string connectionsMachineFormat;
    std::shared_ptr<Board> m_board;
    std::vector<std::shared_ptr<Connection>> m_connections;
    std::vector<std::shared_ptr<ElectronicComponent>> m_electronicComponents;
    
    void setHardwareComponentSize(int widht, int height);
    void printDiscriptionFormat() const;
    void printName() const;
    void printConnectionsToMachineLevel() const;
    void printIdAndSizeToMachineFormat() const;
    std::string toStringMachineFormat() const;
    std::string toStringDiscriptionFormat() const;
    void setSizeToMachineFormat();
    void rotateAllElComponents();

};


#endif // HARDWARE_COMPONENT_H