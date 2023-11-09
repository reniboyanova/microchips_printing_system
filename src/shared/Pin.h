#ifndef PIN_H
#define PIN_H

class Pin{
public:
    Pin(int x, int y) : m_pinX{x}, m_pinY{y} {}
    int m_pinX{};
    int m_pinY{};
    int m_XonBoard{-1};
    int m_YonBoard{-1};
};


#endif // PIN_H