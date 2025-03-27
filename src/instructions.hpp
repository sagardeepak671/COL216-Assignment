#ifndef INSTRUCTIONS
#define INSTRUCTIONS
#include <bits/stdc++.h>
using namespace std;

struct instruction{
    char type;
    string opcode; 
    uint8_t rd = 32;
    uint8_t rs1 = 32;
    uint8_t rs2 = 32;
    int imm; 
    uint8_t funct3;
    uint8_t funct7;
    int result;
    int rs1_value;
    int rs2_value;
};


void update_register_value(instruction &ins);
string func3_func7_check(int pc,int funct3, int funct7);
instruction process_instruction(string input_format);
string get_expression(instruction &ins);
bool manage_branch(instruction &ins);
int get_jump(instruction &ins);
int execute(instruction &ins);
void memory_access(instruction &ins);
bool load_opcode(string opcode);

#endif
