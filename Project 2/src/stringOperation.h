#ifndef STRINGOPERATION_H
#define STRINGOPERATION_H
#include <string>
#include <vector>
#include <sstream>
#include <string.h>


// Delete the space or \t of the string at the front and at the back
void DeleteBlank(std::string& text);

// Change the decimal string to binary string with certain bit
std::string ChangeDecToBinStr(std::string& num,int& bit);

// Split the string by the token (can have multiple tokens)
std::vector<std::string> SplitString(const std::string& str, const std::string& token);

// change a binary string to integer, signed / unsigned
int binary_to_int(std::string machine_instruction, int bit, bool flag);
#endif // STRINGOPERATION_H
