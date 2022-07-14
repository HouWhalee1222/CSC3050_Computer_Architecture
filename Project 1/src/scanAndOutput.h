#ifndef SCANANDOUTPUT_H
#define SCANANDOUTPUT_H
#include <iostream>
#include <fstream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include "stringOperation.h"
#include "RegisterAndInstructions.h"
using namespace std;


// Scan the input file for the first time, output a file with only instruction
// Filter the comments and labels and blank
// store the label and address and return a map from label name to its address(line number)
map<string, int> FirstScan(string& Ifile, string& Ofile);


// Scan the output file of the first scan and return a new file with machine codes
// Will use the following three functions to transform
void SecondScan(string& Ifilex, string& Ofilex, map<string, int>& labelAndAddress,map<string, string>& regToBin);

// Return the R-format code according to the instruction number and other instruction elements of binary form
string ReturnRcode(int& insNumber, vector<string>& regis);

// Return the I-format code according to the instruction number and other instruction elements of binary form
string ReturnIcode(int& insNumber, vector<string>& regis);

// Return the J-format code according to the instruction number and other instruction elements of binary form
string ReturnJcode(int& insNumber, vector<string>& regis);


#endif // SCANANDOUTPUT_H
