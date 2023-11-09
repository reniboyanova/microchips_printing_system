#include "Board.h"
#include <vector>
Board::Board(int height)
{   
    if( Board::checkIfValidBoardHeight()){
        m_boardHeight = height;
        m_boardWidht = height;
    }
}

Board::Board()
{
}

void Board::setBoardSize(int size)
{   
    m_boardSize = size;
}

bool Board::checkIfValidBoardHeight()
{
    return false;
}

int Board::getDefaultSize()
{   //we can write a function to load the default board sizes from a config file
    const std::vector<int> defaultSizes = {8, 16, 24, 32, 64, 128, 256, 512};
    int minSize{};
    if( m_boardHeight > m_boardWidht ){
       minSize = m_boardHeight;
    }
    else{
        minSize = m_boardWidht;
    }
    int vecSize = defaultSizes.size();
    for(int i{}; i < vecSize; ++i){
        if(minSize < defaultSizes[i]){
            minSize = defaultSizes[i];
            break;
        }    
    }
    return minSize;
}
