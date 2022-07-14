#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <string.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include "stringOperation.h"
#include "RegisterAndInstructions.h"
#include "scanAndOutput.h"

using namespace std;


int main(int argc, char* argv[]){

    //a map from register name to its binary string e.g. $s0 -> 10000
    map<string, string>regToBin = RegBinMap();

    // name of the I/O file for first scan
    string Ifile = argv[1];
    string Ofile = "whale.txt";

    // Scan First time, store the label and address
    map<string, int> labelAndAddress = FirstScan(Ifile,Ofile); //a map from label name to its address(line number)

    // name of the I/O file for second scan
    string Ifilex = "whale.txt";
    string Ofilex=  argv[2];

    // Scan the second time, output the new file
    SecondScan(Ifilex, Ofilex, labelAndAddress, regToBin);

    cout << "Program finished..."; // end of the main function
    return 0;
}





