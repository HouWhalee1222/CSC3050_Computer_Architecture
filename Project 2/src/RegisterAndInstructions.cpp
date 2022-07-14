#include "RegisterAndInstructions.h"
using namespace std;
// The number of instructions of each format
const int numberOfR = 32;
const int numberOfI = 36;
const int numberOfJ = 2;

// Formats with their instructions included
const string R_operationName[numberOfR] = {"add","addu","and","div","divu","mult","multu",
                                   "nor","or","sll","sllv","sra","srav",
                                   "srl","srlv","sub","subu","xor","slt","sltu",
                                   "jalr","jr","teq","tne","tge","tgeu","tlt","tltu",
                                   "mfhi","mflo","mthi","mtlo"};

const string I_operationName[numberOfI] = {"addi","addiu","andi","ori","xori","lui","slti",
                                   "sltiu","beq","bgez","bgezal","bgtz","blez","bltzal",
                                   "bltz","bne","teqi","tnei","tgei","tgeiu","tlti","tltiu",
                                   "lb","lbu","lh","lhu","lw","lwl","lwr","ll","sb","sh",
                                   "sw","swl","swr","sc"};

const string J_operationName[numberOfJ] = {"j","jal"};

// define a map from register name to its binary string
map<string, string> RegBinMap(){
    map<string, string> regToBin;
    regToBin["$zero"] = "00000";
    regToBin["$at"] = "00001";
    regToBin["$v0"] = "00010";
    regToBin["$v1"] = "00011";
    regToBin["$a0"] = "00100";
    regToBin["$a1"] = "00101";
    regToBin["$a2"] = "00110";
    regToBin["$a3"] = "00111";
    regToBin["$t0"] = "01000";
    regToBin["$t1"] = "01001";
    regToBin["$t2"] = "01010";
    regToBin["$t3"] = "01011";
    regToBin["$t4"] = "01100";
    regToBin["$t5"] = "01101";
    regToBin["$t6"] = "01110";
    regToBin["$t7"] = "01111";
    regToBin["$s0"] = "10000";
    regToBin["$s1"] = "10001";
    regToBin["$s2"] = "10010";
    regToBin["$s3"] = "10011";
    regToBin["$s4"] = "10100";
    regToBin["$s5"] = "10101";
    regToBin["$s6"] = "10110";
    regToBin["$s7"] = "10111";
    regToBin["$t8"] = "11000";
    regToBin["$t9"] = "11001";
    regToBin["$k0"] = "11010";
    regToBin["$k1"] = "11011";
    regToBin["$gp"] = "11100";
    regToBin["$sp"] = "11101";
    regToBin["$fp"] = "11110";
    regToBin["$ra"] = "11111";
    // end of the map
    return regToBin;
}


// Judge the sequential number of the instruction e.g. add -> 0; addi -> 32;
int JudgeInsNumber(string& Operation){
    for (int i=0;i<numberOfR;i++){
        if (Operation == R_operationName[i]){
            return i;
        }
    }

    for (int i=0;i<numberOfI;i++){
        if (Operation == I_operationName[i]){
            return i+numberOfR;
        }
    }

    for (int i=0;i<numberOfJ;i++){
        if (Operation == J_operationName[i]){
            return i+numberOfR+numberOfI;
        }
    }

    return -1;
}
