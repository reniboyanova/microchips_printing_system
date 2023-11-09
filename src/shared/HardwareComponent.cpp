#include "HardwareComponent.h"
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include "../shared/Utils.h"
#include "sparser.h"

HardwareComponent::HardwareComponent(const std::string& name)
{
    m_board = std::make_unique<Board>();
    Parser::loadHardwareComponent(*this, name);
}


void HardwareComponent::setHardwareComponentSize(int widht, int height)
{
    m_widht = widht;
    m_height = height;
}


void HardwareComponent::printDiscriptionFormat() const
{
    
    std::cout << "\nid: " << m_id << std::endl;
    std::cout << "width: "<< m_widht << std::endl;
    std::cout << "height: " << m_height <<std::endl;
    std::cout << "componenets:\n";
    for(const auto& elComp: m_electronicComponents){
        std::cout << elComp->m_id << ": " << '(' << elComp->m_x << ',' << elComp->m_y <<")," <<elComp->m_rotation << std::endl;
    }
    std::cout << "connections: ";
    for(size_t i{}; i < m_connections.size(); ++i){
        std::cout << '(' << m_connections[i]->m_firstECid << '.' << m_connections[i]->m_firstPIN 
        << ',' << m_connections[i]->m_secondECid << '.' << m_connections[i]->m_secondPIN << ")";
        if(i != m_connections.size()-1){
            std::cout << ", ";
        }
    }
    std::cout << std::endl;
}

void HardwareComponent::printName() const
{
    std::cout << m_id << ": "<< std::endl;
}

void HardwareComponent::printConnectionsToMachineLevel() const
{
    for(const auto& connection: m_connections){
        Pin pin1 = m_electronicComponents[connection->m_firstECid-1]->m_pinsVec[connection->m_firstPIN-1];
        Pin pin2 = m_electronicComponents[connection->m_secondECid-1]->m_pinsVec[connection->m_secondPIN-1];

        std::cout << uti::mlTransformTwoInt(pin1.m_XonBoard, pin1.m_YonBoard) << " ";
        std::cout << uti::mlTransformTwoInt(pin2.m_XonBoard, pin2.m_YonBoard) << " ";
    }
    std::cout << std::endl;
}

void HardwareComponent::printIdAndSizeToMachineFormat() const
{
    std::cout << m_id << ": " <<  m_sizeMachineFormat << "\n" <<std::endl;

}

std::string HardwareComponent::toStringMachineFormat() const
{
    std::unordered_map<std::string, int> names; //push to header
    std::string line;

    for(const auto& el: m_electronicComponents){
        names[el->m_id] += 1;
    }
    line += m_id + ": " + std::to_string(m_sizeMachineFormat) +'\n';
    for (const auto& name : names){
        line += name.first + ": " + std::to_string(name.second) + ' ';
        for(const auto& el: m_electronicComponents){
            if(name.first == el->m_id){
                line += std::to_string(el->m_x) + " " +std::to_string(el->m_rotation) + " ";
            }
        }
        line+= '\n';    
    }
    return line;
}

std::string HardwareComponent::toStringDiscriptionFormat() const
{   
    std::shared_ptr<std::string> line(std::make_unique<std::string>());
    *line += "\nid: " + m_id + '\n';
    *line +=  "width: " + std::to_string(m_widht) + '\n';
    *line += "height: " + std::to_string(m_height) + '\n';
    *line += "componenets:\n";
    for(const auto& elComp: m_electronicComponents){
        *line += elComp->m_id + ": (";
        *line += std::to_string(elComp->m_x) + "," + std::to_string(elComp->m_y) + "),"  + std::to_string(elComp->m_rotation) + '\n';
    }
    *line += "connections: ";
    for(size_t i{}; i < m_connections.size(); ++i){
        *line += '(' + std::to_string(m_connections[i]->m_firstECid) + '.' + std::to_string(m_connections[i]->m_firstPIN) 
        + ',' + std::to_string(m_connections[i]->m_secondECid) + '.' + std::to_string(m_connections[i]->m_secondPIN) + ")";
        if(i != m_connections.size()-1){
            *line += ", ";
        }
    }
    *line += '\n';
    return *line;
}

void HardwareComponent::setSizeToMachineFormat() 
{
    m_sizeMachineFormat = uti::mlTransformTwoInt(m_widht, m_height);
}

void HardwareComponent::rotateAllElComponents()
{
    for(auto& item: m_electronicComponents){
        item->rotateComponent();       
    }
}
