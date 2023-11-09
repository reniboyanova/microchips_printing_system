#include "ElectronicComponent.h"
#include "../shared/sparser.h"
#include <iostream>
#include "../shared/Utils.h"

ElectronicComponent::ElectronicComponent()
{
}

ElectronicComponent::ElectronicComponent(std::string id, int height, int widht) : m_id{id}, m_height{height}, m_widht{widht}
{

}

void ElectronicComponent::setId(std::string id)
{
    m_id = id;
}

void ElectronicComponent::addPin(Pin& inputPin)
{   
    m_pinsVec.push_back(inputPin);
}

void ElectronicComponent::printComponent() const
{   
    std::cout << "ID : " << m_id << std::endl;
    std::cout << "Widht: " << m_widht << "; Height: " <<m_height << std::endl;
    for(const auto& pin: m_pinsVec){
        std::cout << "X: " << pin.m_pinX << ";  Y: " << pin.m_pinY << std::endl;
    }
    std::cout << "^^^^^^^" << std::endl;
}

std::string ElectronicComponent::toStringMachineFormat()
{   
    std::string machineFormatString = this->m_id;
    machineFormatString += ": " + std::to_string(uti::mlTransformTwoInt(m_widht, m_height));
    for(const auto& pin: m_pinsVec){
        machineFormatString += ' ' + std::to_string(uti::mlTransformTwoInt(pin.m_pinX, pin.m_pinY));
        
    }

    return machineFormatString;
}
void ElectronicComponent::printMachineFormat() const
{
    std::string line;
    


}
// based on rotation, changes widht, height , and positions of every pin
void ElectronicComponent::rotateComponent()
{   

    m_xRotated = m_x;
    m_yRotated = m_y;
    

    if(m_rotation == static_cast<int>(Rotation::ROTATE_90d)){
        
        
        for(auto& pin: m_pinsVec){
            int temp = pin.m_pinX;
            pin.m_pinX = (m_height - 1) - pin.m_pinY;
            pin.m_pinY = temp;
        }
        std::swap(m_widht, m_height);   
        m_xRotated = m_x - m_widht+1;
        m_yRotated = m_y; 
 
    }
    if(m_rotation == static_cast<int>(Rotation::ROTATE_180d)){
        for(auto& pin: m_pinsVec){
            int tempX = pin.m_pinX;
            int tempY = pin.m_pinY;
            pin.m_pinX = (m_widht- 1) - tempX;
            pin.m_pinY = (m_height-1) - tempY;
        }
        m_xRotated = m_x - m_widht + 1;;
        m_yRotated = m_y - m_height + 1; 
    }
    if(m_rotation == static_cast<int>(Rotation::ROTATE_270d)){
        for(auto& pin: m_pinsVec){
            int tempX = pin.m_pinX;
            pin.m_pinX = pin.m_pinY;
            pin.m_pinY = (m_widht-1)-tempX;
        }  
        std::swap(m_widht, m_height);
        m_xRotated = m_x;
        m_yRotated = m_y - m_height + 1;  
    }
    for(auto& pin: m_pinsVec){
        pin.m_XonBoard = pin.m_pinX + m_xRotated;
        pin.m_YonBoard = pin.m_pinY + m_yRotated;
    }
    
}


void ElectronicComponent::printPinCoordinatesOnBoard() const
{   
    for(auto& pin: m_pinsVec){
        std::cout << "X: " << pin.m_XonBoard << "; Y: " << pin.m_YonBoard << std::endl;
    }
}
