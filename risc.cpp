#include <bits/stdc++.h>
using namespace std;
#include "prettyPrint.h"

const string HEX_TO_BIN[] = {
    "0000", "0001", "0010", "0011", "0100", "0101", "0110", "0111",
    "1000", "1001", "1010", "1011", "1100", "1101", "1110", "1111"
};
struct instruction{
    char type;
    string opcode; // add, sub,....
    uint8_t rd = 32;
    uint8_t rs1 = 32;
    uint8_t rs2 = 32;
    int imm; 
    uint8_t funct3;
    uint8_t funct7;
};
int register_value[33] = {0}; 
map<int,int> memory;   // RAM
vector<bool> write_enabled(33,false);
// - mean not in any stage 
// I mean in IF stage
// D mean in ID stage
// E mean in EX stage
// M mean in MEM stage
// W mean in WB stage
int binarystring_to_decimalint(string binary_format){
    int an = 0;
    for(int i = 0; i<(int)binary_format.size(); i++){
        an = an*2 + (binary_format[i] - '0');
    }
    return an;
} 
string func3_func7_check(int pc,int funct3, int funct7){
    int opcode = pc & 0x7f;
    if(opcode == 0x33){ // R-type
        if(funct3 == 0x00 && funct7 == 0x00)  return "add"; 
        else if(funct3 == 0x20 && funct7 == 0x00)  return "sub"; 
        else if(funct3 == 0x00 && funct7 == 0x01)  return "sll"; 
        else if(funct3 == 0x00 && funct7 == 0x02)  return "slt";
        else if(funct3 == 0x00 && funct7 == 0x03)  return "sltu";
        else if(funct3 == 0x00 && funct7 == 0x04)  return "xor";
        else if(funct3 == 0x00 && funct7 == 0x05)  return "srl";
        else if(funct3 == 0x20 && funct7 == 0x05)  return "sra";
        else if(funct3 == 0x00 && funct7 == 0x06)  return "or";
        else if(funct3 == 0x00 && funct7 == 0x07)  return "and";
        else{cout << "wrong input" << endl;exit(0);}
    }
    else if(opcode == 0x13){     // I type
       if(funct3 == 0x00 )  return "addi";
       else if(funct3 == 0x02 )  return "slti";
       else if(funct3 == 0x03 )  return "sltiu";
       else if(funct3 == 0x04 )  return "xori";
       else if(funct3 == 0x06 )  return "ori";
       else if(funct3 == 0x07 )  return "andi";
       else if(funct3 == 0x01 )  return "slli";
       else if(funct3 == 0x05 )  return "srli";
       else{cout << "wrong input" << endl;exit(0);}
    }else if(opcode == 0x03){     // I-type
       if(funct3 == 0x00 )  return "lb";
       else if(funct3 == 0x01 )  return "lh";
       else if(funct3 == 0x02 )  return "lw";
       else if(funct3 == 0x03 )  return "lbu";
       else if(funct3 == 0x04 )  return "lhu";
       else{cout << "wrong input" << endl;exit(0);}
    }   
    else if(opcode == 0x23){     // S type  
       if(funct3 == 0x00 )  return "sb";
       else if(funct3 == 0x01 )  return "sh";
       else if(funct3 == 0x02 )  return "sw";
       else if(funct3 == 0x03 )  return "sd";
       else{cout << "wrong input" << endl;exit(0);}
    }
    else if(opcode == 0x63){ //B-type 
       if(funct3 == 0x00 )  return "beq";
       else if(funct3 == 0x01 )  return "bne";
       else if(funct3 == 0x04 )  return "blt";
       else if(funct3 == 0x05 )  return "bge";
       else if(funct3 == 0x06 )  return "bltu";
       else if(funct3 == 0x07 )  return "bgeu";
       else{cout << "wrong input" << endl;exit(0);}
    }
    else if(opcode == 0x37 && funct3 == 0x00 )  return "lui";  
    else if(opcode == 0x17 && funct3 == 0x01 ) return "auipc"; 
    else if(opcode == 0x6f && funct3 == 0x00 ) return "jal";  
    else if(opcode == 0x67 && funct3 == 0x00 )  return "jalr"; 
    return "";
}

instruction process_instruction(string input_format){ 
    // first converting input_format to binary string
    string binary_format;
    for(char c : input_format) {
        if(c >= '0' && c <= '9') { binary_format += HEX_TO_BIN[c - '0'];}
        else if(c >= 'a' && c <= 'f') {binary_format += HEX_TO_BIN[c - 'a' + 10];}
        else if(c >= 'A' && c <= 'F') {binary_format += HEX_TO_BIN[c - 'A' + 10];}
        else {cout << "wrong input" << endl;exit(0);}
    }  
    int pc = binarystring_to_decimalint(binary_format); 
    // extracting opcode rd,rs1,rs2,imm  

    int opcode = pc & 0x7f; 
    instruction ins;
    if(opcode == 0x33) {
        //| funct7 (7 bits) | rs2 (5 bits) | rs1 (5 bits) | funct3 (3 bits) | rd (5 bits) | opcode (7 bits) |
        ins.type = 'R';
        ins.rs1 = (pc >> 15) & 0x1f;
        ins.rs2 = (pc >> 20) & 0x1f;
        ins.rd = (pc >> 7) & 0x1f;
        ins.funct3 = (pc >> 12) & 0x7;
        ins.funct7 = (pc >> 25) & 0x7f;
        ins.opcode = func3_func7_check(pc, ins.funct3, ins.funct7);
    }
    else if(opcode == 0x13) {
        //| imm[11:0] (12 bits) | rs1 (5 bits) | funct3 (3 bits) | rd (5 bits) | opcode (7 bits) |
        ins.type = 'I'; 
        ins.rs1 = (pc >> 15) & 0x1f;
        ins.rd = (pc >> 7) & 0x1f;
        ins.imm = (pc >> 20) & 0xfff;
         // **Sign Extension for negative numbers**
        if (ins.imm & 0x800) {  // If bit 11 (sign bit) is 1
            ins.imm |= 0xfffff000;  // Extend upper bits with 1s
        }
        ins.funct3 = (pc >> 12) & 0x7;
        ins.opcode = func3_func7_check(pc, ins.funct3, ins.funct7);
    }
    else if(opcode == 0x03) {
        //| imm[11:0] (12 bits) | rs1 (5 bits) | funct3 (3 bits) | rd (5 bits) | opcode (7 bits) |
        ins.type = 'I';
        ins.rs1 = (pc >> 15) & 0x1f;
        ins.rd = (pc >> 7) & 0x1f;
        ins.imm = (pc >> 20) & 0xfff;
         // **Sign Extension for negative numbers**
        if (ins.imm & 0x800) {  // If bit 11 (sign bit) is 1
            ins.imm |= 0xfffff000;  // Extend upper bits with 1s
        }
        ins.funct3 = (pc >> 12) & 0x7;
        ins.opcode = func3_func7_check(pc, ins.funct3, ins.funct7);
    }
    
    else if(opcode == 0x23) {
        //| imm[11:5] (7 bits) | rs2 (5 bits) | rs1 (5 bits) | funct3 (3 bits) | imm[4:0] (5 bits) | opcode (7 bits) |
        ins.type = 'S';
        ins.rs1 = (pc >> 15) & 0x1f;
        ins.rs2 = (pc >> 20) & 0x1f;
        ins.imm = ((pc >> 25) & 0x7f) << 5 | ((pc >> 7) & 0x1f);
        ins.funct3 = (pc >> 12) & 0x7;
        // **Sign Extension** for negative values
        if (ins.imm & 0x800) {  // If bit 11 (sign bit) is 1
            ins.imm |= 0xfffff000;  // Extend upper bits with 1s
        }
        ins.opcode = func3_func7_check(pc, ins.funct3, ins.funct7);
    }
    else if(opcode == 0x63) {
        //| imm[12] | imm[10:5] | rs2 (5 bits) | rs1 (5 bits) | funct3 (3 bits) | imm[4:1] | imm[11] | opcode (7 bits) |
        ins.type = 'B';
        ins.rs1 = (pc >> 15) & 0x1f;
        ins.rs2 = (pc >> 20) & 0x1f;
        ins.imm = ((pc >> 31) & 0x1) << 12 |
                  ((pc >> 7) & 0x1) << 11 |
                  ((pc >> 25) & 0x3f) << 5 |
                  ((pc >> 8) & 0xf) << 1;
        ins.funct3 = (pc >> 12) & 0x7;
         // **Sign Extension** for negative values
            if (ins.imm & 0x1000) {  // If bit 12 (sign bit) is 1
                ins.imm |= 0xffffe000;  // Extend upper bits with 1s
            }
        ins.opcode = func3_func7_check(pc, ins.funct3, ins.funct7);
    }
    else if(opcode == 0x37) {
        //| imm[31:12] (20 bits) | rd (5 bits) | opcode (7 bits) |
        ins.type = 'U';
        ins.rd = (pc >> 7) & 0x1f;
        ins.imm = (pc >> 12) & 0xfffff;
        // Sign extend to 32-bit if needed
         if (ins.imm & 0x80000000) {  
            ins.imm |= 0xfff00000;  // Fill upper 12 bits with 1s
        }

        ins.opcode = func3_func7_check(pc, ins.funct3, ins.funct7);
    }
    else if(opcode == 0x17){ 
        ins.type = 'U';
        ins.rd = (pc >> 7) & 0x1f;
        ins.imm = (pc >> 12) & 0xfffff;
        if (ins.imm & 0x80000000) {  
        ins.imm |= 0xfff00000;  // Fill upper bits with 1s
        }   
        ins.opcode = func3_func7_check(pc, ins.funct3, ins.funct7);
    }
    else if(opcode == 0x6f) {
        //| imm[20] | imm[10:1] | imm[11] | imm[19:12] | rd (5 bits) | opcode (7 bits) |
        ins.type = 'J';
        ins.rd = (pc >> 7) & 0x1f;
        ins.imm = ((pc >> 31) & 0x1) << 20 |
                  ((pc >> 12) & 0xff) << 12 |
                  ((pc >> 20) & 0x1) << 11 |
                  ((pc >> 21) & 0x3ff) << 1;
            // Sign-extend immediate to 32-bit signed int
        if (ins.imm & (1 << 20)) { // If sign bit (bit 20) is set
        ins.imm |= 0xFFF00000;  // Extend the sign to bits 31-21
        ins.opcode = func3_func7_check(pc, ins.funct3, ins.funct7);
    }
    }
    else if(opcode == 0x67){
        //| imm[11:0] (12 bits) | rs1 (5 bits) | funct3 (3 bits) | rd (5 bits) | opcode (7 bits) |
        ins.type = 'I';
        ins.rs1 = (pc >> 15) & 0x1f;
        ins.rd = (pc >> 7) & 0x1f;
        ins.imm = (pc >> 20) & 0xfff;
        // **Sign Extension for Negative Values**
        if (ins.imm & 0x800) {  // If bit 11 (imm[11]) is 1
            ins.imm |= 0xfffff000;  // Extend upper bits with 1s
        }
        ins.funct3 = (pc >> 12) & 0x7;
        ins.opcode = func3_func7_check(pc, ins.funct3, ins.funct7);
    }
    else{
        cout << "wrong input" << endl;
        exit(0);
    }  
    return ins;
}

string get_expression(instruction &ins){
    if(ins.type == 'R')return ins.opcode + " x"+ to_string(ins.rd) + " x"+ to_string(ins.rs1) + " x"+ to_string(ins.rs2) ;
    else if(ins.type == 'I') return ins.opcode + " x"+ to_string(ins.rd) + " x"+ to_string(ins.rs1) + " " + to_string(ins.imm) ;
    else if(ins.type == 'S')return ins.opcode + " x"+ to_string(ins.rs2)  + " " + to_string(ins.imm)+ " x"+ to_string(ins.rs1);
    else if(ins.type == 'B') return ins.opcode + " x"+ to_string(ins.rs1) + " x"+ to_string(ins.rs2) + " " + to_string(ins.imm);
    else if(ins.type == 'U')return ins.opcode + " x"+ to_string(ins.rd) + " " + to_string(ins.imm);
    else if(ins.type == 'J')return ins.opcode + " x"+ to_string(ins.rd) + ", " + to_string(ins.imm);
    return "";
}

int update_register_value(instruction &ins, int &next_pc_address, int j) { 
    if(ins.type == 'R') { 
        if(ins.opcode == "add") register_value[ins.rd] = register_value[ins.rs1] + register_value[ins.rs2];
        else if(ins.opcode == "sub") register_value[ins.rd] = register_value[ins.rs1] - register_value[ins.rs2];
        else if(ins.opcode == "sll") register_value[ins.rd] = register_value[ins.rs1] << register_value[ins.rs2];
        else if(ins.opcode == "slt") register_value[ins.rd] = (register_value[ins.rs1] < register_value[ins.rs2]) ? 1 : 0;
        else if(ins.opcode == "sltu") register_value[ins.rd] = (register_value[ins.rs1] < register_value[ins.rs2]) ? 1 : 0;
        else if(ins.opcode == "xor") register_value[ins.rd] = register_value[ins.rs1] ^ register_value[ins.rs2];
        else if(ins.opcode == "srl") register_value[ins.rd] = register_value[ins.rs1] >> register_value[ins.rs2];
        else if(ins.opcode == "sra") register_value[ins.rd] = register_value[ins.rs1] >> register_value[ins.rs2];
        else if(ins.opcode == "or") register_value[ins.rd] = register_value[ins.rs1] | register_value[ins.rs2];
    }
    else if(ins.type == 'I') {
        if(ins.opcode == "addi") register_value[ins.rd] = register_value[ins.rs1] + ins.imm;
        else if(ins.opcode == "slti") register_value[ins.rd] = (register_value[ins.rs1] < ins.imm) ? 1 : 0;
        else if(ins.opcode == "sltiu") register_value[ins.rd] = (register_value[ins.rs1] < ins.imm) ? 1 : 0;
        else if(ins.opcode == "xori") register_value[ins.rd] = register_value[ins.rs1] ^ ins.imm;
        else if(ins.opcode == "ori") register_value[ins.rd] = register_value[ins.rs1] | ins.imm;
        else if(ins.opcode == "andi") register_value[ins.rd] = register_value[ins.rs1] & ins.imm;
        else if(ins.opcode == "slli") register_value[ins.rd] = register_value[ins.rs1] << ins.imm;
        else if(ins.opcode == "srli") register_value[ins.rd] = register_value[ins.rs1] >> ins.imm;
        else if(ins.opcode == "srai") register_value[ins.rd] = register_value[ins.rs1] >> ins.imm;
        else if(ins.opcode == "lb") register_value[ins.rd] = memory[ins.rs1+ins.imm];
        else if(ins.opcode == "lh") register_value[ins.rd] = memory[ins.rs1+ins.imm];
        else if(ins.opcode == "lw") register_value[ins.rd] = memory[ins.rs1+ins.imm];
        else if(ins.opcode == "lbu") register_value[ins.rd] = memory[ins.rs1+ins.imm];
        else if(ins.opcode == "lhu") register_value[ins.rd] = memory[ins.rs1+ins.imm];
        else if(ins.opcode == "lwu") register_value[ins.rd] = memory[ins.rs1+ins.imm];
        else if(ins.opcode == "ld") register_value[ins.rd] = memory[ins.rs1+ins.imm]; 

    }
    else if(ins.type == 'S') {
        if(ins.opcode == "sb") memory[ins.imm] = register_value[ins.rs2];
        else if(ins.opcode == "sh") memory[ins.imm] = register_value[ins.rs2];
        else if(ins.opcode == "sw") memory[ins.imm] = register_value[ins.rs2];
        else if(ins.opcode == "sd") memory[ins.imm] = register_value[ins.rs2];
    }
    else if(ins.type == 'B') {
        if(ins.opcode == "beq" && register_value[ins.rs1] == register_value[ins.rs2]){ next_pc_address = j + ins.imm/4; return true; }
        else if(ins.opcode == "bne" && register_value[ins.rs1] != register_value[ins.rs2]){ next_pc_address = j + ins.imm/4; return true; }
        else if(ins.opcode == "blt" && register_value[ins.rs1] < register_value[ins.rs2]){ next_pc_address = j + ins.imm/4; return true; }
        else if(ins.opcode == "bge" && register_value[ins.rs1] >= register_value[ins.rs2]){ next_pc_address = j + ins.imm/4; return true; }
        else if(ins.opcode == "bltu" && register_value[ins.rs1] < register_value[ins.rs2]){ next_pc_address = j + ins.imm/4; return true; }
        else if(ins.opcode == "bgeu" && register_value[ins.rs1] >= register_value[ins.rs2]){ next_pc_address = j + ins.imm/4; return true; }
    }
    else if(ins.type == 'U') {
        if(ins.opcode == "lui") register_value[ins.rd] = ins.imm;
        else if(ins.opcode == "auipc") register_value[ins.rd] = ins.imm + ins.imm; 
    }
    else if(ins.type == 'J'){
        if(ins.opcode == "jal"){ next_pc_address = j + ins.imm/4; return true;}
        else if(ins.opcode == "jalr"){ next_pc_address = j + ins.imm/4; return true;}
    }  
    return false;
}

void Without_Forwarding(vector<instruction> &pc,int m){ 
    int n = pc.size();
    vector<string> expressions(n,"");
    for(int i =0; i<n; i++) expressions[i] = get_expression(pc[i]);
 
    vector<char> command_stage(n,'-');   // current stage on this instruction , - mean not in any stage
    map<int,int> mp ={ {'I',-1}, {'D',-1}, {'E',-1}, {'M',-1}, {'W',-1} };   // map to store whether stage is free or not
    memory[11]=12;
    vector<string> ans(n,"");
    int next_pc_address = 0;  // for jumping and initial pc address 
    int max_pc_address = 0;   // for next pc address for instruction fetch
    for(int i=0; i<min(25,m); i++){  // number of vectical columns to print
    int flg =-1;           // flg for flushing the instruction after that cycle
    int flg2=-1;    
        for(int j=0; j<n; j++){
            if(j==next_pc_address || (mp['I'] == -1 && j==max_pc_address+1)){
                ans[j] += "IF;";
                mp['I'] = j;
                command_stage[j] = 'I';
                next_pc_address = -1;
                max_pc_address = j;
            }else if(command_stage[j] =='-') ans[j] += " ;";
            else if(command_stage[j] == 'I' && mp['D'] == -1){
                command_stage[j] = 'D';
                ans[j] += "ID;";
                mp['D'] = j;
                mp['I'] = -1;
            }else if(command_stage[j] == 'D'&& mp['E'] == -1 
            &&( (pc[j].rs1 != 32 && write_enabled[pc[j].rs1] == false) || pc[j].rs1 == 32 )
            && ( (pc[j].rs2 != 32 && write_enabled[pc[j].rs2] == false) || pc[j].rs2 == 32 )
            ){ 
                command_stage[j] = 'E';
                ans[j] += "EX;";
                mp['E'] = j;
                mp['D'] = -1;
                if(update_register_value(pc[j],next_pc_address,j) == true){
                        // branch was taken
                        if(next_pc_address == j+1){// do nothing
                        }else if(next_pc_address == j+2){
                            // flush j+1 th instruction
                            flg2 = j;
                            
                        }else{
                            // flush both j+1 and j+2 th instruction
                            flg =j;
                            
                        } 
                }else if(pc[j].type == 'R' || pc[j].type == 'I'){ 
                    write_enabled[pc[j].rd] = true;  
                }
            }else if(command_stage[j] == 'E' && mp['M'] == -1 ){
            command_stage[j] = 'M';
            ans[j] += "MEM;";
            mp['M'] = j;
            mp['E'] = -1;
            }else if(command_stage[j] == 'M' && mp['W'] == -1){
                command_stage[j] = 'W';
                ans[j] += "WB;";
                mp['W'] = j;
                mp['M'] = -1; 
            }else if(command_stage[j] == 'W'){
                command_stage[j] = '-';
                ans[j] += " ;";
                mp['W'] = -1;
                if(pc[j].type == 'R' || pc[j].type == 'I'){ 
                    write_enabled[pc[j].rd] = false;
                }
            }else ans[j] += " ;";  
        }
            if(flg!=-1){
                    mp['I'] = -1;
                    if(flg+1<n)command_stage[flg+1] = '-';
                    if(flg+2<n)command_stage[flg+2] = '-';
                    mp['D'] = -1;
                    mp['E'] = -1;
                    mp['M'] = -1;
                    mp['W'] = -1;
                    flg =-1;
                    max_pc_address = next_pc_address;
            }
            if(flg2!=-1){
                mp['D'] = -1;
                if(flg2+1<n) command_stage[flg2+1] = '-';
                flg2 = -1;

            }
            
    }
    // printing the answer
    prettyPrint(expressions,ans); // for now pretty printing
}

int main(){
    vector<string> input_instructions;
    string instr;
    while(cin>>instr){
        input_instructions.push_back(instr);
    } 
    int n = input_instructions.size();
    vector<instruction> instructions(n);
    for(int i=0;i<n;i++){ 
        instructions[i] = process_instruction(input_instructions[i]);
    }

    int m=50; // input the number of vertical columns to print
    // printing the pipeline with 5 stages without Forwarding
    Without_Forwarding(instructions,m);
    return 0;
}