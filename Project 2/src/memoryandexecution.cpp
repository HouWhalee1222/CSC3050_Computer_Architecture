#include "memoryandexecution.h"
#include <math.h>
using namespace std;

// Simulate register using int(32-bit)
int reg[34]={0}; // lo-32  hi-33


void simulate_memory_and_execution(string& Ofilex,string& Ifilex_data){
    // Allocate 6MB
    void * memory_base_address = malloc(6*1024*1024); // it should be 64-bit  long long

    // Map 0x00400000(4194304)  and real address
    long long real_to_fake_memory_diff = (long long) 0x00400000 - (long long)(memory_base_address);

    // Point couter
    int * PC = (int *) memory_base_address;

    // Text segment counter
    int * text_segment_pointer = (int *) memory_base_address;

    // Static data pointer 1MB = 1024KB = 1024*1024 B   int * +1 = address + 4
    int * static_data_pointer = (int *) memory_base_address + 1024 * 1024 / 4; // int pointer!!! remember to divide by 4!!!

    // Stack-29 & Frame-30 & Global pointer-28    store the fake address
    reg[29] = int((long long)(memory_base_address) + 6 * 1024 * 1024 + real_to_fake_memory_diff); // 10485760 = 0x00A00000
    reg[30] = reg[29];  // Stack Bottom
    reg[28] = int((long long)(memory_base_address) + 1024 * 1024 + real_to_fake_memory_diff); // 5242880 = 0x00500000

    // Open the machine code file
    ifstream exe_file(Ofilex);

    // 32-bit
    int code_length = 32;

    // Store in text segment
    string machine_instruction;
    while (getline(exe_file, machine_instruction)){// store the code into the text segment
        int int_code = binary_to_int(machine_instruction,code_length,false); // signed
        *text_segment_pointer = int_code;
        text_segment_pointer += 1;
    } // Again the } !!!

    // Store the .data
    DataScan(static_data_pointer, Ifilex_data);

    // Maintain a dynamic_data_pointer
    int * dynamic_data_pointer = static_data_pointer; // It should change

    // About the file operation: syscall
    string file_name_syscall;
    fstream file_syscall;

    // Execute the code
    while (*PC!=0){
        int current_code_int = *PC;
        int * current_address = (int *)((long long)PC + real_to_fake_memory_diff);
        string current_code_str = to_string(current_code_int);
        // Point counter + 1
        PC += 1;

        // Convert back to string form
        string current_code_string = ChangeDecToBinStr(current_code_str, code_length);

        // Decode
        int exe_opcode = binary_to_int(current_code_string.substr(0,6),6,true); // opcode

        // Parameters
        int reg_rs; reg_rs = binary_to_int(current_code_string.substr(6,5),5,true); // register number: unsigned
        int reg_rt; reg_rt = binary_to_int(current_code_string.substr(11,5),5,true);

        int reg_rd; reg_rd = binary_to_int(current_code_string.substr(16,5),5,true);
        int exe_shamt; exe_shamt = binary_to_int(current_code_string.substr(21,5),5,true); // unsigned
        int exe_funct_case; exe_funct_case = binary_to_int(current_code_string.substr(26,6),6,true);

        int immediate; immediate = binary_to_int(current_code_string.substr(16,16),16,false); // signed
        // name should not be too similar ...   str & string

        int target; target = binary_to_int(current_code_string.substr(6,26),26,true); // address unsigned

        // Judge R/I/J instructions
        switch (exe_opcode) {

        case 0: // R format or syscall

            // Decide specific instructions
            switch (exe_funct_case) {

            case 12:{ // syscall
                int reg_v0 = reg[2];    // $v0 ~ reg[2]  $a0 ~ reg[4]  $a1 ~ reg[5]

                switch (reg_v0) {

                case 1:{ // print_int
                    cout << reg[4] << endl;
                    break;
                }

                case 4:{ // print_string
                    long long string_address = (long long)reg[4] - real_to_fake_memory_diff;
                    char * str_ptr = (char *) string_address;
                    string print_str = "";
                    while (* str_ptr != '\0'){
                        char ele = *str_ptr;
                        print_str += ele;
                        str_ptr += 1;
                    }
                    cout << print_str << endl;
                    break;
                }

                case 5:{ // read_int
                    cout << "Enter an integer: ";
                    int input_int;
                    cin >> input_int;
                    reg[2] = input_int; // store in $v0
                    break;
                }

                case 8:{ // read_string
                    long long string_address = (long long)reg[4] - real_to_fake_memory_diff; // $a0
                    char * string_ptr = (char *) string_address;
                    int string_length = reg[5] - 1; // maximum: string_length -1  null-terminated
                    cout << "Enter a string of length " << string_length << ":";
                    string input_str;
                    getline(cin, input_str); // there may be a space
                    for (int i=0;i<string_length;i++){
                        *string_ptr = input_str[i];
                        string_ptr += 1;
                    }
                    *string_ptr = '\0';
                    break;
                }

                case 9:{ // sbrk
                    int sbrk_amount = reg[4];
                    int sbrk_address = (long long)(dynamic_data_pointer) + real_to_fake_memory_diff;
                    dynamic_data_pointer += sbrk_amount/4; // int *
                    reg[2] = sbrk_address;
//                    cout << reg[2] << endl; cout << (long long)(dynamic_data_pointer) + real_to_fake_memory_diff << endl;
//                    cout << 0x00500000 + 1024*1024*2 << endl;
                    break;
                }

                case 10:{ // exit
                   cout << "Current address: " << hex << current_address << endl;
                   cout << "Program exits with status 0, goodbye!" << endl;
                   exit(0);
                   break;
               }

                case 11:{ // print_char
                   char char_o;
                   char_o = reg[4];
                   cout << char_o << endl;
                   break;
               }

                case 12:{ // read_char
                   cout << "Enter a character: ";
                   char input_char;
                   cin >> input_char;
                   reg[2] = input_char;
                   break;
               }

                case 13:{ // open
                    long long filename_address = (long long)reg[4] - real_to_fake_memory_diff; // $a0
                    string file_name="";
                    char * file_name_ptr = (char *) filename_address;
                    while (*file_name_ptr!='\0'){
                        file_name += *file_name_ptr;
                        file_name_ptr += 1;
                    }

                    int flags = reg[5];
                    if (flags == 0){ // read only
                        file_syscall.open(file_name,ios::in);
                    }
                    else if (flags == 1){ // write only
                        file_syscall.open(file_name,ios::out);
                    }
                    else if (flags == 2){ // if exists:read/write  if not exists:create and read/write
                        file_syscall.open(file_name,ios::in | ios::out);//already exists
                        if (!file_syscall.is_open()){
                            file_syscall.open(file_name,ios::in | ios::out | ios::trunc); // if not exists, create
                        }
                    }
                    file_name_syscall = file_name;
                    reg[4] = 3; // file descriptor
                    break;
                }

                case 14:{ // read
                    if (reg[4] == 3){ // fd
                        long long read_address = (long long)reg[5] - real_to_fake_memory_diff;
                        char * buffer = (char *) read_address;
                        int char_length = reg[6];
                        for (int i=0;i<char_length;i++){
                            *buffer = file_syscall.get();
                            buffer++;
                        }
                        reg[4] = char_length;
                    }
                    break;
                }

                case 15:{ // write
                    if (reg[4] == 3){
                        long long write_address = (long long)reg[5] - real_to_fake_memory_diff;
                        char * buffer = (char *) write_address;
                        int char_length = reg[6];
                        for (int i=0;i<char_length;i++){
                            file_syscall << *buffer;
                            buffer++;
                        }
                        reg[4] = char_length;
                    }
                    break;
                }

                case 16:{ // close
                    file_syscall.close();
                    break;
                }

                case 17:{ // exit2
                    int result;
                    result = reg[4];
                    cout << "Current address: " << hex << current_address << endl;
                    cout << "Program exits with status " << result <<", goodbye!" << endl;
                    exit(result);
                    break;
                }
                }  // switch $v0 end
                break; // very implicit error!!!
            }  // syscall end

            case 32:{ // add
                long long result = reg[reg_rs] + reg[reg_rt];
                if (fabs(result) >= pow(2.0,31)){
                    cout << "Current address: " << hex << current_address << endl;
                    cout << "Overflow! Goodbye." << endl;
                    exit(0);
                }
                else reg[reg_rd] = int(result);
                break;
            }

            case 33:{ // addu
                reg[reg_rd] = reg[reg_rs] + reg[reg_rt];
                break;
            }

            case 36:{ // and
                string reg_rs_str = to_string(reg[reg_rs]);
                string reg_rt_str = to_string(reg[reg_rt]); // and!!!
                reg_rs_str = ChangeDecToBinStr(reg_rs_str, code_length);
                reg_rt_str = ChangeDecToBinStr(reg_rt_str, code_length);// int --> 32 "bit" string
                string reg_rd_str = "";
                for (int i=0;i<code_length;i++){
                    if (reg_rs_str[i] == reg_rt_str[i]){ // should be rt instead of rd!!!
                        reg_rd_str += reg_rs_str[i];
                    }
                    else reg_rd_str += "0";
                }
                reg[reg_rd] = binary_to_int(reg_rd_str,code_length,false); // signed into register
                break; // if there is initialization, use {}
            }

            case 26:{ // div
                // lo = quotient hi = remainder
//                reg[reg_rs] = 100; reg[reg_rt] = -7;
                reg[32] = reg[reg_rs] / reg[reg_rt];
                reg[33] = reg[reg_rs] % reg[reg_rt];
//                cout << reg[32] << endl; cout << reg[33] << endl;
                break;
            }

            case 27:{ // divu
                reg[32] = reg[reg_rs] / reg[reg_rt];
                reg[33] = reg[reg_rs] % reg[reg_rt];
                break;
            }

            case 24:{ // mult
                // lo = low order hi = high order
//                reg[reg_rs] = 100; reg[reg_rt] = 3534;
                long long product = (long long) reg[reg_rs] * reg[reg_rt]; // rt!!!!!!
                string product_str = to_string(product);
                int product_bit = 64;
                product_str = ChangeDecToBinStr(product_str, product_bit);
                reg[32] = binary_to_int(product_str.substr(32,32),32,false); // store: signed
                reg[33] = binary_to_int(product_str.substr(0,32),32,false);
//                cout << reg[32] << endl; cout << reg[33] << endl;
                break;
            }

            case 25:{ // multu
                long long product = reg[reg_rs] * reg[reg_rt]; // rt!!!!!!
                string product_str = to_string(product);
                int product_bit = 64;
                product_str = ChangeDecToBinStr(product_str, product_bit);
                reg[32] = binary_to_int(product_str.substr(32,32),32,false); // store: signed
                reg[33] = binary_to_int(product_str.substr(0,32),32,false);
                break;
            }

            case 39:{ // nor
//                                reg[reg_rs] = 10342432; reg[reg_rt] = 100234234;
                string reg_rs_str = to_string(reg[reg_rs]);
                string reg_rt_str = to_string(reg[reg_rt]); // one mistake, all mistakes
                reg_rs_str = ChangeDecToBinStr(reg_rs_str, code_length);
                reg_rt_str = ChangeDecToBinStr(reg_rt_str, code_length);
                string reg_rd_str = "";
                for (int i=0;i<code_length;i++){
                    if (reg_rs_str[i] == '0' && reg_rt_str[i]=='0'){
                        reg_rd_str += "1";
                    }
                    else reg_rd_str += "0";
                }
                reg[reg_rd] = binary_to_int(reg_rd_str,code_length,false);
//                cout << reg_rs_str << endl;
//                cout << reg_rt_str << endl;
//                cout << reg_rd_str << endl;
//                cout << reg[reg_rd] << endl;
                break;
            }

            case 37:{ // or
//                reg[reg_rs] = 34323; reg[reg_rt] = 2313123;
                // difference between rd and rt!!!
                string reg_rs_str = to_string(reg[reg_rs]);
                string reg_rt_str = to_string(reg[reg_rt]);
                reg_rs_str = ChangeDecToBinStr(reg_rs_str, code_length);
                reg_rt_str = ChangeDecToBinStr(reg_rt_str, code_length);
                string reg_rd_str = "";
                for (int i=0;i<code_length;i++){
                    if (reg_rs_str[i] == '0' && reg_rt_str[i]=='0'){
                        reg_rd_str += "0";
                    }
                    else reg_rd_str += "1";
                }
                reg[reg_rd] = binary_to_int(reg_rd_str,code_length,false);
//                cout << reg_rs_str << endl;cout << reg_rt_str << endl; cout << reg_rd_str << endl; cout << reg[reg_rd] << endl;
                break;
            }

            case 0:{ // sll
//               reg[reg_rt] = 1234;

                string rt_str = to_string(reg[reg_rt]);
                rt_str = ChangeDecToBinStr(rt_str,code_length);
                string remain_str = rt_str.substr(exe_shamt,code_length-exe_shamt);
                string new_str = "";
                for (int i=0;i<exe_shamt;i++){
                    new_str += "0";
                }
                string rd_str = remain_str + new_str;
                reg[reg_rd] = binary_to_int(rd_str,32,false); // signed

//                cout << rt_str << endl;
//                cout << rd_str << endl;
//                cout << reg[reg_rd] << endl;
                break;
            }

            case 4:{ // sllv
                exe_shamt = reg[reg_rs];
                string rt_str = to_string(reg[reg_rt]);
                rt_str = ChangeDecToBinStr(rt_str,code_length);
                string remain_str = rt_str.substr(exe_shamt,code_length-exe_shamt);
                string new_str = "";
                for (int i=0;i<exe_shamt;i++){
                    new_str += "0";
                }
                string rd_str = remain_str + new_str;
                reg[reg_rd] = binary_to_int(rd_str,32,false); // signed
                break;
            }

            case 3:{ // sra
//                reg[reg_rt] = -1234;
                bool negative = false;
                if (reg[reg_rt] < 0) negative = true;
                string rt_str = to_string(reg[reg_rt]);
                rt_str = ChangeDecToBinStr(rt_str,code_length);
                string remain_str = rt_str.substr(0,code_length-exe_shamt);
                string new_str = "";

                if (negative){ // negative shift: add 1
                    for (int i=0;i<exe_shamt;i++){
                        new_str += "1";
                    }
                }
                else {
                    for (int i=0;i<exe_shamt;i++){
                        new_str += "0";
                    }
                }

                string rd_str = new_str + remain_str;
                reg[reg_rd] = binary_to_int(rd_str,32,false); // signed

//                cout << rt_str << endl;
//                cout << rd_str << endl;
//                cout << reg[reg_rd] << endl;
                break;
            }

            case 7:{ // srav
                exe_shamt = reg[reg_rs];
                string rt_str = to_string(reg[reg_rt]);
                rt_str = ChangeDecToBinStr(rt_str,code_length);
                string remain_str = rt_str.substr(0,code_length-exe_shamt);
                string new_str = "";
                for (int i=0;i<exe_shamt;i++){
                    new_str += "0";
                }
                string rd_str = new_str + remain_str;
                reg[reg_rd] = binary_to_int(rd_str,32,false); // signed
                break;
            }

            case 2:{ // srl
                string rt_str = to_string(reg[reg_rt]);
                rt_str = ChangeDecToBinStr(rt_str,code_length);
                string remain_str = rt_str.substr(0,code_length-exe_shamt);
                string new_str = "";
                for (int i=0;i<exe_shamt;i++){
                    new_str += "0";
                }
                string rd_str = new_str + remain_str;
                reg[reg_rd] = binary_to_int(rd_str,32,false); // signed
                break;
            }

            case 6:{ // srlv
                exe_shamt = reg[reg_rs];
                string rt_str = to_string(reg[reg_rt]);
                rt_str = ChangeDecToBinStr(rt_str,code_length);
                string remain_str = rt_str.substr(0,code_length-exe_shamt);
                string new_str = "";
                for (int i=0;i<exe_shamt;i++){
                    new_str += "0";
                }
                string rd_str = new_str + remain_str;
                reg[reg_rd] = binary_to_int(rd_str,32,false); // signed
                break;
            }

            case 34:{ // sub
                long long result = reg[reg_rs] - reg[reg_rt];
                if (fabs(result) >= pow(2.0,31)){
                    cout << "Current address: " << hex << current_address << endl;
                    cout << "Overflow! Goodbye." << endl;
                    exit(0);
                }
                else reg[reg_rd] = int(result);
                break;
            }

            case 35:{ // subu
                reg[reg_rd] = reg[reg_rs] - reg[reg_rt];
                break;
            }

            case 38:{ // xor
                string reg_rs_str = to_string(reg[reg_rs]);
                string reg_rt_str = to_string(reg[reg_rt]);
                reg_rs_str = ChangeDecToBinStr(reg_rs_str, code_length);
                reg_rt_str = ChangeDecToBinStr(reg_rt_str, code_length);
                string reg_rd_str = "";
                for (int i=0;i<code_length;i++){
                    if (reg_rs_str[i] == reg_rt_str[i]){
                        reg_rd_str += "0";
                    }
                    else reg_rd_str += "1";
                }
                reg[reg_rd] = binary_to_int(reg_rd_str,code_length,false);
                break;
            }

            case 42:{ // slt
                if (reg[reg_rs] < reg[reg_rt]){// rt!!!!!!
                    reg[reg_rd] = 1;
                }
                else reg[reg_rd] = 0;
                break;
            }

            case 43:{ // sltu
//                reg[reg_rs] = -3; reg[reg_rt] = -2;
                if (reg[reg_rs] < reg[reg_rt]){
                    reg[reg_rd] = 1;
                }
                else reg[reg_rd] = 0;
//                cout << reg[reg_rd] << endl;
                break;
            }

            case 9:{ // jalr
//                reg[reg_rs] = 4194304;
                long long jump_address = reg[reg_rs]; // fake address
                reg[reg_rd] = int((long long)(PC) + real_to_fake_memory_diff); // also fake address
                //transfer between fake and real address
                PC = (int *) (jump_address - real_to_fake_memory_diff); // real address
//                cout << (long long)(PC) << "==" << (long long)(0x00400000)-real_to_fake_memory_diff << endl;
//                cout << reg[reg_rd] << "==" << (long long)(PC+14)+real_to_fake_memory_diff << endl;
                break;

            }

            case 8:{ // jr
                long long jump_address = reg[reg_rs]; // false
                PC = (int *) (jump_address - real_to_fake_memory_diff); // real
                break;
            }

            case 52:{ // teq
                if(reg[reg_rs] == reg[reg_rt]){
                    cout << "Trap exception occurs, goodbye!" << endl;
                    exit(0);
                }
                break;
            }

            case 54:{ // tne
                if(reg[reg_rs] != reg[reg_rt]){
                    cout << "Trap exception occurs, goodbye!" << endl;
                    exit(0);
                }
                break;
            }

            case 48:{ // tge
                if(reg[reg_rs] >= reg[reg_rt]){
                    cout << "Trap exception occurs, goodbye!" << endl;
                    exit(0);
                }
                break;
            }

            case 49:{ // tgeu
                if(reg[reg_rs] >= reg[reg_rt]){
                    cout << "Trap exception occurs, goodbye!" << endl;
                    exit(0);
                }
                break;
            }

            case 50:{ // tlt
                if(reg[reg_rs] < reg[reg_rt]){
                    cout << "Trap exception occurs, goodbye!" << endl;
                    exit(0);
                }
                break;
            }

            case 51:{ // tltu
                if(reg[reg_rs] < reg[reg_rt]){
                    cout << "Trap exception occurs, goodbye!" << endl;
                    exit(0);
                }
                break;
            }

            case 16:{ // mfhi
                reg[reg_rd] = reg[33];
                break;
            }

            case 18:{ // mflo
                reg[reg_rd] = reg[32];
                break;
            }

            case 17:{ // mthi
                reg[33] = reg[reg_rs];
                break;
            }

            case 19:{ // mtlo
                reg[32] = reg[reg_rs];
                break;
            }

            } // R switch ends
            break;

// ************************************************************** //  The followings are I and J

        case 8:{ // addi
            long long result = reg[reg_rs] + immediate;
            if (fabs(result) >= pow(2.0,31)){
                cout << "Current address: " << hex << current_address << endl;
                cout << "Overflow! Goodbye." << endl;
                exit(0);
            }
            else reg[reg_rt] = int(result);
            break;
        }

        case 9:{ // addiu
            immediate = binary_to_int(current_code_string.substr(16,16),16,true); // unsigned
            reg[reg_rt] = reg[reg_rs] + immediate;
            break;
        }

        case 12:{ // andi
//            reg[reg_rs] = 5201314;

            string reg_rs_str = to_string(reg[reg_rs]);
            reg_rs_str = ChangeDecToBinStr(reg_rs_str, code_length);
            string immediate_str = to_string(immediate);
            immediate_str = ChangeDecToBinStr(immediate_str, code_length);
            string reg_rt_str = "";
            for (int i=0;i<code_length;i++){
                if (reg_rs_str[i] == immediate_str[i]){ // should be rt instead of rd!!!
                    reg_rt_str += reg_rs_str[i];
                }
                else reg_rt_str += "0";
            }
            reg[reg_rt] = binary_to_int(reg_rt_str,code_length,true); // unsigned rt!!!!!!

//            cout << reg_rs_str << endl;cout << immediate_str << endl; cout << reg_rt_str << endl; cout << reg[reg_rt] << endl;
            break;
        }

        case 13:{ // ori
//            reg[reg_rs] = 5201314;

            string reg_rs_str = to_string(reg[reg_rs]);
            reg_rs_str = ChangeDecToBinStr(reg_rs_str, code_length);
            string immediate_str = to_string(immediate);
            immediate_str = ChangeDecToBinStr(immediate_str, code_length);
            string reg_rt_str = "";
            for (int i=0;i<code_length;i++){
                if (reg_rs_str[i] == '0' && immediate_str[i] == '0'){ // should be rt instead of rd!!!
                    reg_rt_str += "0";
                }
                else reg_rt_str += "1";
            }
            reg[reg_rt] = binary_to_int(reg_rt_str,code_length,true); // unsigned rt!!!!!!

//            cout << reg_rs_str << endl;cout << immediate_str << endl; cout << reg_rt_str << endl; cout << reg[reg_rt] << endl;
            break;
        }

        case 14:{ // xori
//            reg[reg_rs] = 5201314;
            string reg_rs_str = to_string(reg[reg_rs]);
            reg_rs_str = ChangeDecToBinStr(reg_rs_str, code_length);
            string immediate_str = to_string(immediate);
            immediate_str = ChangeDecToBinStr(immediate_str, code_length);
            string reg_rt_str = "";
            for (int i=0;i<code_length;i++){
                if (reg_rs_str[i] == immediate_str[i]){
                    reg_rt_str += "0";
                }
                else reg_rt_str += "1";
            }
            reg[reg_rt] = binary_to_int(reg_rt_str,code_length,true);
//            cout << reg_rs_str << endl;cout << immediate_str << endl; cout << reg_rt_str << endl; cout << reg[reg_rt] << endl;
            break;
        }

        case 15:{ // lui
            string imm_str = to_string(immediate); // no debug = die
            imm_str = ChangeDecToBinStr(imm_str,code_length);
            string imm_str_lower = imm_str.substr(16,16);
            imm_str = imm_str_lower + "0000000000000000";
            reg[reg_rt] = binary_to_int(imm_str,32,false); // signed stored
            break;
        }

        case 10:{ // slti
            //rt!!!!!!!!!!!!!!!!
            if (reg[reg_rs] < immediate){// signed immediate
                reg[reg_rt] = 1;
            }
            else reg[reg_rt] = 0;
            break;
        }

        case 11:{ // sltiu
            immediate = binary_to_int(current_code_string.substr(16,16),16,true); // unsigned
            if (reg[reg_rs] < immediate){// unsigned immediate
                reg[reg_rt] = 1;
            }
            else reg[reg_rt] = 0;
            break;
        }

        case 4:{ // beq
            int offset = immediate;
            if (reg[reg_rs] == reg[reg_rt]){
                PC += offset;
            }
            break;
        }

        case 1:{ // bgez & bgezal & bltzal & bltz & teqi & tnei & tgei & tgeiu & tlti & tltiu
//            int current_address = (long long)PC
            int offset = immediate;
            long long next_instruction_address = (long long)(PC) + real_to_fake_memory_diff;
            switch (reg_rt) { // There should be a break after every case!!!
            case 1:case 17:{ // bgez & bgezal
                if (reg[reg_rs] >= 0){
                    PC += offset;
                }
                break; // BREAK!!!
            }
            case 16:case 0:{ // bltzal & bltz
                if (reg[reg_rs] < 0){
                    PC += offset;
                }
                break;
            }
            case 12:{ // teqi
                if (reg[reg_rs] == immediate){
                    cout << "Current address: " << hex << current_address << endl;
                    cout << "Trap exception occurs, goodbye!" << endl;
                    exit(0);
                }
                break; // BREAK!!!!
            }

            case 14:{ // tnei
                if (reg[reg_rs] != immediate){
                    cout << "Current address: " << hex << current_address << endl;
                    cout << "Trap exception occurs, goodbye!" << endl;
                    exit(0);
                }
                break;
            }

            case 8:{ // tgei
                if (reg[reg_rs] >= immediate){
                    cout << "Current address: " << hex << current_address << endl;
                    cout << "Trap exception occurs, goodbye!" << endl;
                    exit(0);
                }
                break;
            }

            case 9:{ // tgeiu
                immediate = binary_to_int(current_code_string.substr(16,16),16,true); // unsigned
                if (reg[reg_rs] >= immediate){
                    cout << "Current address: " << hex << current_address << endl;
                    cout << "Trap exception occurs, goodbye!" << endl;
                    exit(0);
                }
                break;
            }

            case 10:{ // tlti
                if (reg[reg_rs] < immediate){
                    cout << "Current address: " << hex << current_address << endl;
                    cout << "Trap exception occurs, goodbye!" << endl;
                    exit(0);
                }
                break;
            }

            case 11:{ // tltiu
                immediate = binary_to_int(current_code_string.substr(16,16),16,true); // unsigned
                if (reg[reg_rs] < immediate){
                    cout << "Current address: " << hex << current_address << endl;
                    cout << "Trap exception occurs, goodbye!" << endl;
                    exit(0);
                }
                break;
            }

            }

            if (reg_rt == 17 || reg_rt == 16){ // bgezal / bltzal : save the address into $ra
                reg[31] = next_instruction_address; // save the fake address

//                cout << reg[31] << endl;
//                cout << reg[29] << endl; //sp
//                cout << reg[29] - reg[31] << endl;
//                cout << 6 * 1024 * 1024 << endl; //for test!!!
            }
            break;
        }

        case 7:{ // bgtz
            int offset = immediate;
            if (reg[reg_rs] > 0){
                PC += offset;
            }
            break;
        }

        case 6:{ // blez
            int offset = immediate;
            if (reg[reg_rs] <= 0){
                PC += offset;
            }
            break;
        }

        case 5:{ // bne
            int offset = immediate;
            if (reg[reg_rs] != reg[reg_rt]){
                PC += offset;
            }
            break;
        }

        case 2:{ // j
            PC = (int *) memory_base_address + target;
            break;
        }

        case 3:{ // jal
            reg[31] = int((long long)(PC) + real_to_fake_memory_diff);
            PC = (int *) memory_base_address + target;
            break;
        }

        case 32:{ // lb
            // lb $rt, imm($rs)
            long long load_address = (long long)reg[reg_rs] - real_to_fake_memory_diff + immediate;
            int8_t * lb_ptr = (int8_t *) load_address;
            reg[reg_rt] = *lb_ptr;
            break;
        }

        case 36:{ // lbu
            // lbu $rt, imm($rs)
            long long load_address = (long long)reg[reg_rs] - real_to_fake_memory_diff + immediate;
            uint8_t * lbu_ptr = (uint8_t *) load_address;
            reg[reg_rt] = *lbu_ptr;
            break;
        }

        case 33:{ // lh
            // lh $rt, imm($rs)
            long long load_address = (long long)reg[reg_rs] - real_to_fake_memory_diff + immediate;
            int16_t * lh_ptr = (int16_t *) load_address;
            reg[reg_rt] = *lh_ptr;
            break;
        }

        case 37:{ // lhu
            // lhu $rt, imm($rs)
            long long load_address = (long long)reg[reg_rs] - real_to_fake_memory_diff + immediate;
            uint16_t * lhu_ptr = (uint16_t *) load_address;
            reg[reg_rt] = *lhu_ptr;
            break;
        }

        case 35:{ // lw
            // lw $rt, imm($rs)
            // fake address to real address: minus!!!
            long long load_address = (long long)reg[reg_rs] - real_to_fake_memory_diff + immediate;
            int32_t * lw_ptr = (int32_t *) load_address;
            reg[reg_rt] = *lw_ptr;
            break;
        }

        case 34:{ // lwl
            // lwl $rt, imm($rs)
            long long load_address = (long long)reg[reg_rs] - real_to_fake_memory_diff + immediate;
            int16_t * lwl_ptr = (int16_t *) load_address; // get the half-word int16
            int half_word_length = 16;
            string lwl_str = to_string(*lwl_ptr);
            lwl_str = ChangeDecToBinStr(lwl_str,half_word_length); // transfer to 16-bit string
            string reg_rt_str = to_string(reg[reg_rt]);
            reg_rt_str = ChangeDecToBinStr(reg_rt_str,half_word_length).substr(16,16); // right 16-bit in register
            string new_str = lwl_str + reg_rt_str;
            int new_int = binary_to_int(new_str,32,false);
            reg[reg_rt] = new_int;
            break;
        }

        case 38:{ // lwr
            // lwr $rt, imm($rs)
            long long load_address = (long long)reg[reg_rs] - real_to_fake_memory_diff + immediate + 2;
            int16_t * lwr_ptr = (int16_t *) load_address; // get the half-word int16
            int half_word_length = 16;
            string lwr_str = to_string(*lwr_ptr);
            lwr_str = ChangeDecToBinStr(lwr_str,half_word_length); // transfer to 16-bit string
            string reg_rt_str = to_string(reg[reg_rt]);
            reg_rt_str = ChangeDecToBinStr(reg_rt_str,half_word_length).substr(0,16); // right 16-bit in register
            string new_str = reg_rt_str + lwr_str;
            int new_int = binary_to_int(new_str,32,false);
            reg[reg_rt] = new_int;
            break;
        }

        case 48:{ // ll
            // ll $rt, imm($rs)
            long long load_address = (long long)reg[reg_rs] - real_to_fake_memory_diff + immediate;
            int32_t * ll_ptr = (int32_t *) load_address;
            reg[reg_rt] = *ll_ptr;
            break;
        }

        case 40:{ // sb
            // sb $rt, imm($rs)
            long long store_address = (long long)reg[reg_rs] - real_to_fake_memory_diff + immediate;
            int8_t * sb_ptr = (int8_t *) store_address;
            * sb_ptr = reg[reg_rt];
            break;
        }

        case 41:{ // sh
            // sh $rt, imm($rs)
            long long store_address = (long long)reg[reg_rs] - real_to_fake_memory_diff + immediate;
            int16_t * sh_ptr = (int16_t *) store_address;
            * sh_ptr = reg[reg_rt];
            break;
        }

        case 43:{ // sw
            // sw $rt, imm($rs)
            long long store_address = (long long)reg[reg_rs] - real_to_fake_memory_diff + immediate;
            int32_t * sw_ptr = (int32_t *) store_address;
            * sw_ptr = reg[reg_rt];
            break;
        }

        case 42:{ // swl
            // swl $rt, imm($rs)
            long long load_address = (long long)reg[reg_rs] - real_to_fake_memory_diff + immediate;
            int16_t * swl_ptr = (int16_t *) load_address; // target  half-word
            int half_word_length = 16;
            string reg_rt_str = to_string(reg[reg_rt]);
            reg_rt_str = ChangeDecToBinStr(reg_rt_str,code_length).substr(0,16); // transfer to 16-bit string
            string swl_str = to_string(*swl_ptr);
            swl_str = ChangeDecToBinStr(swl_str,half_word_length); // right 16-bit in register
            string new_str = reg_rt_str + swl_str;
            int new_int = binary_to_int(new_str,32,false); // signed
            *swl_ptr = new_int;
            break;
        }

        case 46:{ // swr
            // swr $rt, imm($rs)
            long long load_address = (long long)reg[reg_rs] - real_to_fake_memory_diff + immediate + 2;
            int16_t * swr_ptr = (int16_t *) load_address; // target  half-word
            int half_word_length = 16;
            string reg_rt_str = to_string(reg[reg_rt]);
            reg_rt_str = ChangeDecToBinStr(reg_rt_str,code_length).substr(16,16); // transfer to 16-bit string
            string swr_str = to_string(*swr_ptr);
            swr_str = ChangeDecToBinStr(swr_str,half_word_length); // right 16-bit in register
            string new_str = swr_str + reg_rt_str;
            int new_int = binary_to_int(new_str,32,false); // signed
            *swr_ptr = new_int;
            break;
        }

        case 56:{ // sc
            // sc $rt, imm($rs)
            long long store_address = (long long)reg[reg_rs] - real_to_fake_memory_diff + immediate;
            int32_t * sc_ptr = (int32_t *) store_address;
            * sc_ptr = reg[reg_rt];
            reg[reg_rt] = 1;
            break;
        }

        } // switch end
        // remember to write the break
    } // while end

    free(memory_base_address); // free the space


    exe_file.close();
}
