#ifndef ELECTRONIC_COMPONENT_H
#define ELECTRONIC_COMPONENT_H
#include <vector>
#include <string>
#include "Pin.h"

enum class Rotation{
    NO_ROTATION = 0,
    ROTATE_90d = 1,
    ROTATE_180d,
    ROTATE_270d,
};

class ElectronicComponent{
public:
    ElectronicComponent();
    ElectronicComponent(std::string id, int height, int widht);
    
    std::string m_id{};
    int m_height{};
    int m_widht{};
    int m_x{}, m_y{}, m_rotation{};
    int m_xRotated{}, m_yRotated{};
    std::vector<Pin> m_pinsVec;

    void setId(std::string id);
    void addPin(Pin& inputPin);
    void printComponent() const;
    std::string toStringMachineFormat();
    void printMachineFormat() const;
    void rotateComponent(); 
    void printPinCoordinatesOnBoard() const;
};




#endif // ELECTRONIC_COMPONENT_H