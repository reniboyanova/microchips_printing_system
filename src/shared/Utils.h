#ifndef UTILS_H
#define UTILS_H
#include <vector>
#include <string>

namespace uti{

    void print(const std::string &msg);
    void replaceRoundBracketsWithCurlyBrackets(std::string &str);
//machine-level
    int mlTransformTwoInt(int x, int y);
}
#endif