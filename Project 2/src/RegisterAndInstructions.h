#ifndef REGISTERANDINSTRUCTIONS_H
#define REGISTERANDINSTRUCTIONS_H
#include <string>
#include <map>



// define a map from register name to its binary string
std::map<std::string, std::string> RegBinMap();

// Judge the sequential number of the instruction e.g. add -> 0; addi -> 32;
int JudgeInsNumber(std::string& Operation);


#endif // REGISTERANDINSTRUCTIONS_H
