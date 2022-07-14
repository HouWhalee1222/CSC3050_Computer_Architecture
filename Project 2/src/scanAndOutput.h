#ifndef SCANANDOUTPUT_H
#define SCANANDOUTPUT_H
#include <iostream>
#include <fstream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include "stringOperation.h"
#include "RegisterAndInstructions.h"



// Scan the input file for the first time, output a file with only instruction
// Filter the comments and labels and blank
// store the label and address and return a map from label name to its address(line number)
std::map<std::string, int> FirstScan(std::string& Ifile, std::string& Ofile,std::string& Ofile_data);


// Scan the output file of the first scan and return a new file with machine codes
// Will use the following three functions to transform
void SecondScan(std::string& Ifilex, std::string& Ofilex, std::map<std::string, int>& labelAndAddress,std::map<std::string, std::string>& regToBin);

// Store the data
void DataScan(int * &static_data_pointer, std::string& Ifilex_data);

// Return the R-format code according to the instruction number and other instruction elements of binary form
std::string ReturnRcode(int& insNumber, std::vector<std::string>& regis);

// Return the I-format code according to the instruction number and other instruction elements of binary form
std::string ReturnIcode(int& insNumber, std::vector<std::string>& regis);

// Return the J-format code according to the instruction number and other instruction elements of binary form
std::string ReturnJcode(int& insNumber, std::vector<std::string>& regis);


#endif // SCANANDOUTPUT_H
