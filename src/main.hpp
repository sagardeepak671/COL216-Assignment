#ifndef MAIN
#define MAIN
#include <bits/stdc++.h>
using namespace std;
#include "instructions.hpp" 

extern bool write_enabled[33];
extern int register_value[33];
extern map<int,int> memory;


const string HEX_TO_BIN[] = {
    "0000", "0001", "0010", "0011", "0100", "0101", "0110", "0111",
    "1000", "1001", "1010", "1011", "1100", "1101", "1110", "1111"
};

void processor(vector<instruction> &pc,int m,bool forwarding);

#endif