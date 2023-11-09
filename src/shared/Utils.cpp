#include <iostream>
#include <fstream>
#include <sstream>
#include <istream>
#include <algorithm>
#include "Utils.h"

void uti::print(const std::string &msg){
    std::cout << msg << std::endl;
}


void uti::replaceRoundBracketsWithCurlyBrackets(std::string &str){
    std::replace(str.begin(), str.end(), '(', '{');
    std::replace(str.begin(), str.end(), ')', '}');
}

int uti::mlTransformTwoInt(int x, int y){
        const int shiftPositions = 8;
        y <<= shiftPositions;
        y |= x;
        return y;
}
