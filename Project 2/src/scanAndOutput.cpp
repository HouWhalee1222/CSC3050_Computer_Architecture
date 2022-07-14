#include "scanAndOutput.h"
using namespace std;
// The number of instructions of each format
extern const int numberOfR = 32;
extern const int numberOfI = 36;
extern const int numberOfJ = 2;

// Formats with their instructions included
extern const string R_operationName[numberOfR] = {"add","addu","and","div","divu","mult","multu",
                                   "nor","or","sll","sllv","sra","srav",
                                   "srl","srlv","sub","subu","xor","slt","sltu",
                                   "jalr","jr","teq","tne","tge","tgeu","tlt","tltu",
                                   "mfhi","mflo","mthi","mtlo"};

extern const string I_operationName[numberOfI] = {"addi","addiu","andi","ori","xori","lui","slti",
                                   "sltiu","beq","bgez","bgezal","bgtz","blez","bltzal",
                                   "bltz","bne","teqi","tnei","tgei","tgeiu","tlti","tltiu",
                                   "lb","lbu","lh","lhu","lw","lwl","lwr","ll","sb","sh",
                                   "sw","swl","swr","sc"};

extern const string J_operationName[numberOfJ] = {"j","jal"};




// Scan the input file for the first time, output a file with only instruction
// Filter the comments and labels and blank
// store the label and address and return a map from label name to its address(line number)
map<string, int> FirstScan(string& Ifile, string& Ofile, string& Ofile_data){
    // define a map from label name to its address(line number)
    map<string, int> labelAndAddress;
    map<string, int>::iterator labit;

    ifstream testfile(Ifile);
    ofstream outputfile(Ofile);
    ofstream outputfile_data(Ofile_data);

    string asmCode; // each line
    int lineNumber = 0;
    if (!testfile.is_open()) cout << "Fail to open the file...";
    bool data_flag = false;

    while (getline(testfile,asmCode)) {
        // scan the symbol
        if (asmCode.find(".data")!=asmCode.npos){
            data_flag = true;
            continue;
        }
        if (asmCode.find(".text")!=asmCode.npos){
            data_flag = false;
            continue;
        }

        // delete the comment
        if (asmCode.find("#")!=asmCode.npos) asmCode.erase(asmCode.find("#"));
        DeleteBlank(asmCode);

        if(!asmCode.empty() && asmCode!="\t" && data_flag){ // data
            asmCode.erase(0,asmCode.find_first_of(':')+1);
            DeleteBlank(asmCode);
            outputfile_data << asmCode << endl;
        }


        if (!asmCode.empty() && asmCode!="\t" && !data_flag){ // text segment
            lineNumber++;
            vector<string> judgeLabel = SplitString(asmCode,":");
            if (judgeLabel.size()>1){
                string label = judgeLabel[0];
                labelAndAddress[label] = lineNumber;
                asmCode = SplitString(asmCode,":")[1];
            }
            else asmCode = judgeLabel[0];
            DeleteBlank(asmCode);
            outputfile << asmCode << endl;
        }
    }
    testfile.close();
    outputfile.close();
    outputfile_data.close();

    return labelAndAddress;
}

// Scan the output file of the first scan and return a new file with machine codes
void SecondScan(string& Ifilex, string& Ofilex, map<string, int>& labelAndAddress,map<string, string>& regToBin){
    ifstream middlefile(Ifilex);
    ofstream finaloutput(Ofilex);

    // Iterator for regToBin and labelAndAddress
    map<string, string>::iterator regit;
    map<string, int>::iterator labit;

    string ins; // instruction after filtration e.g add $s0, $s1, $s2
    int lineNum = 0;
    if (!middlefile.is_open()) cout << "Fail to open the file...";

    while (getline(middlefile,ins)){
        lineNum++;
        vector<string> insPart; // store the part of the instruction e.g. add $s0 $s1 $s2
        insPart = SplitString(ins," ,()\t"); // split the instruction
        int vecLength = insPart.size(); // number of elements in an instruction e.g. 4

        vector<string> eleBin;  // store the binary string of the register/constant

        if (insPart[0]=="syscall"){
            finaloutput << "00000000000000000000000000001100" << endl;
            continue;
        }

        int insNumber = JudgeInsNumber(insPart[0]); // get serial number of the instruction e.g. 0



        if (insNumber<numberOfR){ // R format situation
            for (int i=1;i<vecLength;i++){ //
                regit = regToBin.find(insPart[i]);//find the mapping of the register (iterator)
                if (regit != regToBin.end()){ // find the register name
                    string bin = regit->second; // put the corresponding binary string of the register to bin;
                    eleBin.push_back(bin);
                }
                else{ // if it is integer, then it is shift instrcution
                    int shamtbit = 5; // length of the shamt code
                    string shamt = insPart[i];
                    string binString = ChangeDecToBinStr(shamt,shamtbit);
                    eleBin.push_back(binString);
                }
            }
            string rCode = ReturnRcode(insNumber, eleBin);
            finaloutput << rCode << endl;
        }

        else if (insNumber<numberOfR+numberOfI){ // I format situation
            for (int i=1;i<vecLength;i++){ //
                regit = regToBin.find(insPart[i]);//find the mapping of the register (iterator)
                if (regit != regToBin.end()){ // find the register name
                    string bin = regit->second; // put the corresponding binary string of the register to bin;
                    eleBin.push_back(bin);
                }
                else{
                    labit = labelAndAddress.find(insPart[i]);
                    int iBit = 16;
                    if (labit != labelAndAddress.end()){// if it is a label
                        int offset = labit->second - lineNum - 1; // calculate the offset
                        string offsetStr = to_string(offset);
                        string offsetBin = ChangeDecToBinStr(offsetStr,iBit);
                        eleBin.push_back(offsetBin);// put the label inside
                    }
                    else{//if it is integer (immediate or offset)
                        string immString = ChangeDecToBinStr(insPart[i],iBit);
                        eleBin.push_back(immString);
                    }
                }
            } // the location of the } :a bug which costs me an hour
            string iCode = ReturnIcode(insNumber, eleBin);
            finaloutput << iCode << endl;
        }

        else if (insNumber<numberOfR+numberOfI+numberOfJ){ // J format situation
            int targetbit = 26;
            labit = labelAndAddress.find(insPart[1]);
            if (labit != labelAndAddress.end()){ // if the target is a label
                int labAddr = labit->second-1; // address of the label
                string labAddrStr = to_string(labAddr);// string type
                string targetString = ChangeDecToBinStr(labAddrStr,targetbit);
                eleBin.push_back(targetString);
            }
            else{ // if the target is the absolute address
                string targetString = ChangeDecToBinStr(insPart[1],targetbit);
                eleBin.push_back(targetString);
            }
            string jCode = ReturnJcode(insNumber,eleBin);
            finaloutput << jCode << endl;
        }

        else cout << "Invalid instructions"; // if no corresponding instruction name
    }
}

// Store the data
void DataScan(int * &static_data_pointer,string& Ifilex_data){
    // Open the data file
    ifstream data_file(Ifilex_data);
    string data_line;
    while (getline(data_file,data_line)){
        if (data_line.substr(0,6) == ".ascii"){ // .ascii & .asciiz

            // judge .asciiz
            bool asciiz_flag = false;
            if (data_line.substr(0,7) == ".asciiz") asciiz_flag = true;

            int posi_start = data_line.find_first_of('\"');
            int posi_end = data_line.find_last_of('\"');
            // extract the string
            string asciiz = data_line.substr(posi_start+1,posi_end-posi_start-1);

            // write in the data
            char * asciiz_ptr = (char *) static_data_pointer;
            for (int i=0;i<int(asciiz.size());i++){
                *asciiz_ptr = asciiz[i];
                asciiz_ptr += 1;
            }

            if (asciiz_flag) *asciiz_ptr = '\0'; // add the null-terminated
            else asciiz_ptr -= 1; // .ascii

            // decides the position of static_data_pointer
            int asciiz_length = asciiz.length();
            if (asciiz_flag) asciiz_length += 1; // additional '\0'

            if (asciiz_length % 4 != 0){
                asciiz_length = asciiz_length - asciiz_length % 4 + 4;
            }

            static_data_pointer += asciiz_length/4;
        }
        else{
            string type = data_line.substr(0,5);
            if (type == ".word"){ //.word
                vector<string> word_data = SplitString(data_line," ,");
                for (int i=1;i<int(word_data.size());i++){
                    stringstream word_data_stream;
                    word_data_stream << word_data[i];
                    // get the int (word) data
                    int32_t word_data_int;
                    word_data_stream >> word_data_int;

                    int32_t * word_ptr = static_data_pointer;
                    * word_ptr = word_data_int;
                    static_data_pointer += 1;
                }
            }
            else if (type == ".half"){
                vector<string> half_data = SplitString(data_line," ,");
                for (int i=1;i<int(half_data.size());i++){
                    stringstream half_data_stream;
                    half_data_stream << half_data[i];
                    int16_t half_data_int;
                    half_data_stream >> half_data_int;

                    int16_t * half_ptr = (int16_t *) static_data_pointer;
                    * half_ptr = half_data_int;
                    static_data_pointer = (int *)(((int16_t *) static_data_pointer) + 1);
//                    cout << (long long)static_data_pointer << endl;
                }
            }
            else if (type == ".byte"){
                vector<string> byte_data = SplitString(data_line," ,");
                for (int i=1;i<int(byte_data.size());i++){
                    stringstream byte_data_stream;
                    byte_data_stream << byte_data[i];
                    int byte_data_int;
                    byte_data_stream >> byte_data_int;
                    int8_t * byte_ptr = (int8_t *) static_data_pointer;
                    * byte_ptr = byte_data_int;
                    static_data_pointer = (int *)(((int8_t *) static_data_pointer) + 1);
//                    cout << (long long)static_data_pointer << endl;
                }
            }
        }


    }
}


// Return the R-format code according to the instruction number and other instruction elements of binary form
string ReturnRcode(int& insNumber, vector<string>& regis){
    string opcode = "000000";
    string rd,rs,rt;
    string shamt = "00000";
    string funct;

    switch (insNumber){
        case 0://add
            rd = regis[0];
            rs = regis[1];
            rt = regis[2];
            funct = "100000";
            break;
        case 1://addu
            rd = regis[0];
            rs = regis[1];
            rt = regis[2];
            funct = "100001";
            break;
        case 2://and
            rd = regis[0];
            rs = regis[1];
            rt = regis[2];
            funct = "100100";
            break;
        case 3://div
            rd = "00000";
            rs = regis[0];
            rt = regis[1];
            funct = "011010";
            break;
        case 4://divu
            rd = "00000";
            rs = regis[0];
            rt = regis[1];
            funct = "011011";
            break;
        case 5://mult
            rd = "00000";
            rs = regis[0];
            rt = regis[1];
            funct = "011000";
            break;
        case 6://multu
            rd = "00000";
            rs = regis[0];
            rt = regis[1];
            funct = "011001";
            break;
        case 7://nor
            rd = regis[0];
            rs = regis[1];
            rt = regis[2];
            funct = "100111";
            break;
        case 8://or
            rd = regis[0];
            rs = regis[1];
            rt = regis[2];
            funct = "100101";
            break;
        case 9://sll
            rd = regis[0];
            rs = "00000";
            rt = regis[1];
            shamt = regis[2];
            funct = "000000";
            break;
        case 10://sllv
            rd = regis[0];
            rt = regis[1];
            rs = regis[2];
            funct = "000100";
            break;
        case 11://sra
            rd = regis[0];
            rs = "00000";
            rt = regis[1];
            shamt = regis[2];
            funct = "000011";
            break;
        case 12://srav
            rd = regis[0];
            rt = regis[1];
            rs = regis[2];
            funct = "000111";
            break;
        case 13://srl
            rd = regis[0];
            rs = "00000";
            rt = regis[1];
            shamt = regis[2];
            funct = "000010";
            break;
        case 14://srlv
            rd = regis[0];
            rt = regis[1];
            rs = regis[2];
            funct = "000110";
            break;
        case 15://sub
            rd = regis[0];
            rs = regis[1];
            rt = regis[2];
            funct = "100010";
            break;
        case 16://subu
            rd = regis[0];
            rs = regis[1];
            rt = regis[2];
            funct = "100011";
            break;
        case 17://xor
            rd = regis[0];
            rs = regis[1];
            rt = regis[2];
            funct = "100110";
            break;
        case 18://slt
            rd = regis[0];
            rs = regis[1];
            rt = regis[2];
            funct = "101010";
            break;
        case 19://sltu
            rd = regis[0];
            rs = regis[1];
            rt = regis[2];
            funct = "101011";
            break;
        case 20://jalr
            rd = regis[1];
            rs = regis[0];
            rt = "00000";
            funct = "001001";
            break;
        case 21://jr
            rd = "00000";
            rs = regis[0];
            rt = "00000";
            funct = "001000";
            break;
        case 22://teq
            rd = "00000";
            rs = regis[0];
            rt = regis[1];
            funct = "110100";
            break;
        case 23://tne
            rd = "00000";
            rs = regis[0];
            rt = regis[1];
            funct = "110110";
            break;
        case 24://tge
            rd = "00000";
            rs = regis[0];
            rt = regis[1];
            funct = "110000";
            break;
        case 25://tgeu
            rd = "00000";
            rs = regis[0];
            rt = regis[1];
            funct = "110001";
            break;
        case 26://tlt
            rd = "00000";
            rs = regis[0];
            rt = regis[1];
            funct = "110010";
            break;
        case 27://tltu
            rd = "00000";
            rs = regis[0];
            rt = regis[1];
            funct = "110011";
            break;
        case 28://mfhi
            rd = regis[0];
            rs = "00000";
            rt = "00000";
            funct = "010000";
            break;
        case 29://mflo
            rd = regis[0];
            rs = "00000";
            rt = "00000";
            funct = "010010";
            break;
        case 30://mthi
            rd = "00000";
            rs = regis[0];
            rt = "00000";
            funct = "010001";
            break;
        case 31://mtlo
            rd = "00000";
            rs = regis[0];
            rt = "00000";
            funct = "010011";
            break;
    }
    return opcode + rs + rt + rd +shamt + funct;
}

// Return the I-format code according to the instruction number and other instruction elements of binary form
string ReturnIcode(int& insNumber, vector<string>& regis){
    string opcode,rs,rt,imm;
    switch (insNumber) {
        case numberOfR+0://addi
            opcode = "001000";
            rt = regis[0];
            rs = regis[1];
            imm = regis[2];
            break;
        case numberOfR+1://addiu
            opcode = "001001";
            rt = regis[0];
            rs = regis[1];
            imm = regis[2];
            break;
        case numberOfR+2://andi
            opcode = "001100";
            rt = regis[0];
            rs = regis[1];
            imm = regis[2];
            break;
        case numberOfR+3://ori
            opcode = "001101";
            rt = regis[0];
            rs = regis[1];
            imm = regis[2];
            break;
        case numberOfR+4://xori
            opcode = "001110";
            rt = regis[0];
            rs = regis[1];
            imm = regis[2];
            break;
        case numberOfR+5://lui
            opcode = "001111";
            rt = regis[0];
            rs = "00000";
            imm = regis[1];
            break;
        case numberOfR+6://slti
            opcode = "001010";
            rt = regis[0];
            rs = regis[1];
            imm = regis[2];
            break;
        case numberOfR+7://sltiu
            opcode = "001011";
            rt = regis[0];
            rs = regis[1];
            imm = regis[2];
            break;
        case numberOfR+8://beq
            opcode = "000100";
            rt = regis[1];
            rs = regis[0];
            imm = regis[2];
            break;
        case numberOfR+9://bgez
            opcode = "000001";
            rt = "00001";
            rs = regis[0];
            imm = regis[1];
            break;
        case numberOfR+10://bgezal
            opcode = "000001";
            rt = "10001";
            rs = regis[0];
            imm = regis[1];
            break;
        case numberOfR+11://bgtz
            opcode = "000111";
            rt = "00000";
            rs = regis[0];
            imm = regis[1];
            break;
        case numberOfR+12://blez
            opcode = "000110";
            rt = "00000";
            rs = regis[0];
            imm = regis[1];
            break;
        case numberOfR+13://bltzal
            opcode = "000001";
            rt = "10000";
            rs = regis[0];
            imm = regis[1];
            break;
        case numberOfR+14://bltz
            opcode = "000001";
            rt = "00000";
            rs = regis[0];
            imm = regis[1];
            break;
        case numberOfR+15://bne
            opcode = "000101";
            rt = regis[1];
            rs = regis[0];
            imm = regis[2];
            break;
        case numberOfR+16://teqi
            opcode = "000001";
            rt = "01100";
            rs = regis[0];
            imm = regis[1];
            break;
        case numberOfR+17://tnei
            opcode = "000001";
            rt = "01110"; // a bug here
            rs = regis[0];
            imm = regis[1];
            break;
        case numberOfR+18://tgei
            opcode = "000001";
            rt = "01000";
            rs = regis[0];
            imm = regis[1];
            break;
        case numberOfR+19://tgeiu
            opcode = "000001";
            rt = "01001";
            rs = regis[0];
            imm = regis[1];
            break;
        case numberOfR+20://tlti
            opcode = "000001";
            rt = "01010";
            rs = regis[0];
            imm = regis[1];
            break;
        case numberOfR+21://tltiu
            opcode = "000001";
            rt = "01011";
            rs = regis[0];
            imm = regis[1];
            break;
        case numberOfR+22://lb
            opcode = "100000";
            rt = regis[0];
            rs = regis[2];
            imm = regis[1];
            break;
        case numberOfR+23://lbu
            opcode = "100100";
            rt = regis[0];
            rs = regis[2];
            imm = regis[1];
            break;
        case numberOfR+24://lh
            opcode = "100001";
            rt = regis[0];
            rs = regis[2];
            imm = regis[1];
            break;
        case numberOfR+25://lhu
            opcode = "100101";
            rt = regis[0];
            rs = regis[2];
            imm = regis[1];
            break;
        case numberOfR+26://lw
            opcode = "100011";
            rt = regis[0];
            rs = regis[2];
            imm = regis[1];
            break;
        case numberOfR+27://lwl
            opcode = "100010";
            rt = regis[0];
            rs = regis[2];
            imm = regis[1];
            break;
        case numberOfR+28://lwr
            opcode = "100110";
            rt = regis[0];
            rs = regis[2];
            imm = regis[1];
            break;
        case numberOfR+29://ll
            opcode = "110000";
            rt = regis[0];
            rs = regis[2];
            imm = regis[1];
            break;
        case numberOfR+30://sb
            opcode = "101000";
            rt = regis[0];
            rs = regis[2];
            imm = regis[1];
            break;
        case numberOfR+31://sh
            opcode = "101001";
            rt = regis[0];
            rs = regis[2];
            imm = regis[1];
            break;
        case numberOfR+32://sw
            opcode = "101011";
            rt = regis[0];
            rs = regis[2];
            imm = regis[1];
            break;
        case numberOfR+33://swl
            opcode = "101010";
            rt = regis[0];
            rs = regis[2];
            imm = regis[1];
            break;
        case numberOfR+34://swr
            opcode = "101110";
            rt = regis[0];
            rs = regis[2];
            imm = regis[1];
            break;
        case numberOfR+35://sc
            opcode = "111000";
            rt = regis[0];
            rs = regis[2];
            imm = regis[1];
            break;
    }
    return opcode + rs + rt + imm;

}

// Return the J-format code according to the instruction number and other instruction elements of binary form
string ReturnJcode(int& insNumber, vector<string>& regis){
    string opcode,target;

    switch (insNumber) {
        case numberOfR+numberOfI:
            opcode = "000010";
            target = regis[0];
            break;
        case numberOfR+numberOfI+1:
            opcode = "000011";
            target = regis[0];
            break;
    }

    return opcode + target;
}
