#include "stringOperation.h"


// Delete the space or \t at the front and at the back
void DeleteBlank(string& text){
    text.erase(0,text.find_first_not_of("\t"));
    text.erase(0,text.find_first_not_of(" "));
    text.erase(text.find_last_not_of(" ")+1);
    text.erase(text.find_last_not_of("\t")+1);
}


// Change the decimal string to binary string with certain bit
string ChangeDecToBinStr(string& num,int& bit){// bit<31
    stringstream s(num);
    bool flag = true;
    int temp;
    s >> temp;
    s.clear();s.str("");
    if (temp<0){
        flag = false;
        temp = -temp-1;
    }
    // Here the range of the number exceeds int type
    long long mul=1;
    long long bin=temp % 2;
    while (temp>1){
        mul *= 10;
        temp /= 2;
        bin += mul*(temp%2);
    }
    string binStr = to_string(bin);
    int length = binStr.length();

    while (length<bit){
        binStr = "0" + binStr;
        length ++;
    }
    if (flag) return binStr;
    else{
        string binStrn="";
        length = binStr.length();
        for (int i=0;i<length;i++){
            if (binStr[i]=='0') binStrn += "1";
            else binStrn += "0";
        }
        return binStrn;
    }
}

// Split the string by the token (can have multiple tokens)
vector<string> SplitString(const string& str, const string& token){
    vector<string> strPart;
    if (str==token || str==""){
        strPart.push_back("");
        return strPart;
    }

    // Change the string type to char type in C;
    char* strs = new char[str.length()+1];
    strcpy(strs, str.c_str());

    char* t = new char[token.length()+1];
    strcpy(t, token.c_str());

    char* p = strtok(strs, t);//Use the strtok function in C to split
    while (p){
        string part = p;
        strPart.push_back(part);
        p = strtok(NULL, t);//empty pointer - NULL
    }
    return strPart;
}
