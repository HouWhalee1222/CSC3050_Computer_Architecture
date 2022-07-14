#include <iostream>
#include "memoryandexecution.h"

using namespace std;

int main(){
    string argv[3]={"0"};

    cout << "Enter the MIPS input file name: ";
    getline(cin,argv[1]);
    cout << "Enter the machine code output file name: ";
    getline(cin,argv[2]);

    //a map from register name to its binary string e.g. $s0 -> 10000
    map<string, string>regToBin = RegBinMap();

    // name of the I/O file for first scan
    string Ifile = argv[1];
    string Ofile = "whale.txt";
    string Ofile_data = "whale_data.txt";

    // Scan First time, store the label and address
    map<string, int> labelAndAddress = FirstScan(Ifile,Ofile,Ofile_data); //a map from label name to its address(line number)

    // name of the I/O file for second scan and data scan
    string Ifilex = "whale.txt";
    string Ifilex_data = "whale_data.txt";
    string Ofilex=  argv[2];

    // Scan the second time, output the new file
    SecondScan(Ifilex, Ofilex, labelAndAddress, regToBin);

    // Project 2 begins --- memory simulation and execution

    simulate_memory_and_execution(Ofilex,Ifilex_data);

    cout << "Program finished..."; // end of the main function
    return 0;
}





