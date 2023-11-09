#ifndef BOARD_H
#define BOARD_H



class Board{
public:
    Board(int n);
    Board();
    int m_boardHeight{};
    int m_boardWidht{};
    int m_boardSize{};
    
    void setBoardSize(int size);
    bool checkIfValidBoardHeight();
    int getDefaultSize();
};


#endif // BOARD_H