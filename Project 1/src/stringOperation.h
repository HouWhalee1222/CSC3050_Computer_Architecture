#ifndef STRINGOPERATION_H
#define STRINGOPERATION_H
#include <string>
#include <vector>
#include <sstream>
#include <string.h>
using namespace std;

// Delete the space or \t of the string at the front and at the back
void DeleteBlank(string& text);

// Change the decimal string to binary string with certain bit
string ChangeDecToBinStr(string& num,int& bit);

// Split the string by the token (can have multiple tokens)
vector<string> SplitString(const string& str, const string& token);

#endif // STRINGOPERATION_H
