#include "instructions.hpp"
#include "main.hpp"


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


void update_register_value(instruction &ins) { 
    if(ins.type == 'R') { 
        register_value[ins.rd] = ins.result;
    }
    else if(ins.type == 'I') {
        if(ins.opcode == "lb" || ins.opcode == "lh" || ins.opcode == "lw" || ins.opcode == "lbu" || ins.opcode == "lhu")return;
        register_value[ins.rd] = ins.result;

    }
    else if(ins.type == 'U') {
        register_value[ins.rd] = ins.result;
    }
}

void memory_access(instruction &ins){
    if(ins.type == 'I'){
        if(ins.opcode == "lb" || ins.opcode == "lbu" || ins.opcode == "lh" || ins.opcode == "lhu" || ins.opcode == "lw") ins.result = memory[ins.result];
    }
    else if(ins.type == 'S'){
        if(ins.opcode == "sb" || ins.opcode == "sh" || ins.opcode == "sw" || ins.opcode == "sd") memory[ins.imm] = ins.result;  
    }    
}

int execute(instruction &ins){
    if(ins.type == 'R') { 
        if(ins.opcode == "add") return ins.rs1_value + ins.rs2_value;
        else if(ins.opcode == "sub") return ins.rs1_value - ins.rs2_value;
        else if(ins.opcode == "sll") return ins.rs1_value << ins.rs2_value;
        else if(ins.opcode == "slt") return (ins.rs1_value < ins.rs2_value) ? 1 : 0;
        else if(ins.opcode == "sltu") return (ins.rs1_value < ins.rs2_value) ? 1 : 0;
        else if(ins.opcode == "xor") return ins.rs1_value ^ ins.rs2_value;
        else if(ins.opcode == "srl") return ins.rs1_value >> ins.rs2_value;
        else if(ins.opcode == "sra") return ins.rs1_value >> ins.rs2_value;
        else if(ins.opcode == "or") return ins.rs1_value | ins.rs2_value;
    }
    else if(ins.type == 'I') {
        if(ins.opcode == "addi") return ins.rs1_value + ins.imm;
        else if(ins.opcode == "slti") return (ins.rs1_value < ins.imm) ? 1 : 0;
        else if(ins.opcode == "sltiu") return (ins.rs1_value < ins.imm) ? 1 : 0;
        else if(ins.opcode == "xori") return ins.rs1_value ^ ins.imm;
        else if(ins.opcode == "ori") return ins.rs1_value | ins.imm;
        else if(ins.opcode == "andi") return ins.rs1_value & ins.imm;
        else if(ins.opcode == "slli") return ins.rs1_value << ins.imm;
        else if(ins.opcode == "srli") return ins.rs1_value >> ins.imm;
        else if(ins.opcode == "srai") return ins.rs1_value >> ins.imm;
        else if(ins.opcode == "lb") return ins.rs1_value+ins.imm;
        else if(ins.opcode == "lh") return ins.rs1_value+ins.imm;
        else if(ins.opcode == "lw") return ins.rs1_value+ins.imm;
        else if(ins.opcode == "lbu") return ins.rs1_value+ins.imm;
        else if(ins.opcode == "lhu") return ins.rs1_value+ins.imm;
        else if(ins.opcode == "lwu") return ins.rs1_value+ins.imm;
        else if(ins.opcode == "ld") return ins.rs1_value+ins.imm; 

    }
    else if(ins.type == 'S') {
        if(ins.opcode == "sb") return ins.rs2_value;
        else if(ins.opcode == "sh") return ins.rs2_value;
        else if(ins.opcode == "sw") return ins.rs2_value;
        else if(ins.opcode == "sd") return ins.rs2_value;
    }
    else if(ins.type == 'U') {
        if(ins.opcode == "lui") return ins.imm;
        else if(ins.opcode == "auipc") return ins.imm + ins.imm; 
    }
    return 0;
}

int manage_branch(instruction &ins){
        if(ins.opcode == "beq" && register_value[ins.rs1] == register_value[ins.rs2]){return ins.imm/4; }
        else if(ins.opcode == "bne" && register_value[ins.rs1] != register_value[ins.rs2]){ return ins.imm/4; }
        else if(ins.opcode == "blt" && register_value[ins.rs1] < register_value[ins.rs2]){ return ins.imm/4; }
        else if(ins.opcode == "bge" && register_value[ins.rs1] >= register_value[ins.rs2]){ return ins.imm/4; }
        else if(ins.opcode == "bltu" && register_value[ins.rs1] < register_value[ins.rs2]){ return ins.imm/4; }
        else if(ins.opcode == "bgeu" && register_value[ins.rs1] >= register_value[ins.rs2]){ return ins.imm/4; }
        return 0;
}


int binarystring_to_decimalint(string binary_format){
    int an = 0;
    for(int i = 0; i<(int)binary_format.size(); i++){
        an = an*2 + (binary_format[i] - '0');
    }
    return an;
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
        ins.rs1 = ((pc >> 15) & 0x1f);
        ins.rd = ((pc >> 7) & 0x1f);
        ins.imm = ((pc >> 20) & 0xfff);
         // Sign Extension for negative numbers
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
         // Sign Extension for negative numbers
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
        // Sign Extension for negative values
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
         // Sign Extension for negative values
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

    else if(ins.type == 'I') {
        if(ins.opcode == "lb" || ins.opcode == "lh" || ins.opcode == "lw" || ins.opcode == "lbu" || ins.opcode == "lhu")
            return ins.opcode + " x"+ to_string(ins.rd) + " " + to_string(ins.imm) +  " x"+ to_string(ins.rs1) ;
        else
            return ins.opcode + " x"+ to_string(ins.rd) + " x"+ to_string(ins.rs1) + " " + to_string(ins.imm) ;
    }

    else if(ins.type == 'S')return ins.opcode + " x"+ to_string(ins.rs2)  + " " + to_string(ins.imm)+ " x"+ to_string(ins.rs1);
    else if(ins.type == 'B') return ins.opcode + " x"+ to_string(ins.rs1) + " x"+ to_string(ins.rs2) + " " + to_string(ins.imm);
    else if(ins.type == 'U')return ins.opcode + " x"+ to_string(ins.rd) + " " + to_string(ins.imm);
    else if(ins.type == 'J')return ins.opcode + " x"+ to_string(ins.rd) + ", " + to_string(ins.imm);
    return "";
}