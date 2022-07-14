#ifndef REGISTERANDINSTRUCTIONS_H
#define REGISTERANDINSTRUCTIONS_H
#include <string>
#include <map>
using namespace std;


// define a map from register name to its binary string
map<string, string> RegBinMap();

// Judge the sequential number of the instruction e.g. add -> 0; addi -> 32;
int JudgeInsNumber(string& Operation);


#endif // REGISTERANDINSTRUCTIONS_H
