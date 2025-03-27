#include "instructions.hpp"
#include "main.hpp"
#define PRINT_ERR cout << "wrong input" << endl;exit(0);

string func3_func7_check(int pc,int funct3, int funct7){
    int opcode = pc & 0x7f;
    if(opcode == 0x33){ // R-type
        if(funct3 == 0x00 && funct7 == 0x00)  return "add"; 
        if(funct3 == 0x20 && funct7 == 0x00)  return "sub"; 
        if(funct3 == 0x00 && funct7 == 0x01)  return "sll"; 
        if(funct3 == 0x00 && funct7 == 0x02)  return "slt";
        if(funct3 == 0x00 && funct7 == 0x03)  return "sltu";
        if(funct3 == 0x00 && funct7 == 0x04)  return "xor";
        if(funct3 == 0x00 && funct7 == 0x05)  return "srl";
        if(funct3 == 0x20 && funct7 == 0x05)  return "sra";
        if(funct3 == 0x00 && funct7 == 0x06)  return "or";
        if(funct3 == 0x00 && funct7 == 0x07)  return "and";
        PRINT_ERR
    }
    if(opcode == 0x13){     // I type
       if(funct3 == 0x00 )  return "addi";
        if(funct3 == 0x02 )  return "slti";
        if(funct3 == 0x03 )  return "sltiu";
        if(funct3 == 0x04 )  return "xori";
        if(funct3 == 0x06 )  return "ori";
        if(funct3 == 0x07 )  return "andi";
        if(funct3 == 0x01 )  return "slli";
        if(funct3 == 0x05 )  return "srli";
        PRINT_ERR
    }
    if(opcode == 0x03){     // I-type
       if(funct3 == 0x00 )  return "lb";
        if(funct3 == 0x01 )  return "lh";
        if(funct3 == 0x02 )  return "lw";
        if(funct3 == 0x03 )  return "lbu";
        if(funct3 == 0x04 )  return "lhu";
        if(funct3 == 0x05)  return "lwu";
        PRINT_ERR
    }   
    if(opcode == 0x23){     // S type  
        if(funct3 == 0x00 )  return "sb";
        if(funct3 == 0x01 )  return "sh";
        if(funct3 == 0x02 )  return "sw"; 
        PRINT_ERR
    }
    if(opcode == 0x63){ //B-type 
        if(funct3 == 0x00 )  return "beq";
        if(funct3 == 0x01 )  return "bne";
        if(funct3 == 0x04 )  return "blt";
        if(funct3 == 0x05 )  return "bge";
        if(funct3 == 0x06 )  return "bltu";
        if(funct3 == 0x07 )  return "bgeu";
        PRINT_ERR
    }
    if(opcode == 0x37 )  return "lui";   
    if(opcode == 0x6f ) return "jal";  
    if(opcode == 0x67 )  return "jalr"; 
    PRINT_ERR
    return "";
}

void update_register_value(instruction &ins) { 
    if(ins.type == 'R' || ins.type == 'I' || ins.type == 'U')  register_value[ins.rd] = ins.result; 
    register_value[0] = 0; // x0 is always 0
}

void memory_access(instruction &ins) { 
    uint32_t addr = ins.result; 
    if(ins.type == 'I') { 
        if(ins.opcode == "lb") { 
            int8_t byteVal = static_cast<int8_t>(memory[addr]);
            ins.result = static_cast<int32_t>(byteVal);
        }
        else if(ins.opcode == "lbu") { 
            uint8_t byteVal = memory[addr];
            ins.result = static_cast<uint32_t>(byteVal);
        }
        else if(ins.opcode == "lh") { 
            int16_t half = static_cast<int16_t>(memory[addr] | (memory[addr+1] << 8));
            ins.result = static_cast<int32_t>(half);
        }
        else if(ins.opcode == "lhu") { 
            uint16_t half = static_cast<uint16_t>(memory[addr] | (memory[addr+1] << 8));
            ins.result = static_cast<uint32_t>(half);
        }
        else if(ins.opcode == "lw") { 
            int32_t word = static_cast<int32_t>(
                memory[addr]        |
                (memory[addr+1] << 8) |
                (memory[addr+2] << 16) |
                (memory[addr+3] << 24)
            );
            ins.result = word;
        } else if(ins.opcode == "lwu") {
            uint32_t word = static_cast<uint32_t>(
                memory[addr]        |
                (memory[addr+1] << 8) | 
                (memory[addr+2] << 16) |
                (memory[addr+3] << 24)
            );
            ins.result = word;
        }
    }
    else if(ins.type == 'S') { 
        if(ins.opcode == "sb") { 
            memory[addr] = ins.rs1_value & 0xFF;
        }
        else if(ins.opcode == "sh") { 
            memory[addr]   = ins.rs1_value & 0xFF;
            memory[addr+1] = (ins.rs1_value >> 8) & 0xFF;
        }
        else if(ins.opcode == "sw") { 
            memory[addr]   = ins.rs1_value & 0xFF;
            memory[addr+1] = (ins.rs1_value >> 8) & 0xFF;
            memory[addr+2] = (ins.rs1_value >> 16) & 0xFF;
            memory[addr+3] = (ins.rs1_value >> 24) & 0xFF;
        } 
    }
}

int execute(instruction &ins){
    if(ins.type == 'R') { 
        if(ins.opcode == "add") return ins.rs1_value + ins.rs2_value;
        if(ins.opcode == "sub") return ins.rs1_value - ins.rs2_value;
        if(ins.opcode == "sll") return ins.rs1_value << ins.rs2_value;
        if(ins.opcode == "slt") return (ins.rs1_value < ins.rs2_value) ? 1 : 0;
        if(ins.opcode == "sltu") return (static_cast<unsigned>(ins.rs1_value) < static_cast<unsigned>(ins.rs2_value)) ? 1 : 0;
        if(ins.opcode == "xor") return ins.rs1_value ^ ins.rs2_value;
        if(ins.opcode == "srl") return static_cast<unsigned>(ins.rs1_value) >> ins.rs2_value;
        if(ins.opcode == "sra") return ins.rs1_value >> ins.rs2_value; 
        if(ins.opcode == "or")  return ins.rs1_value | ins.rs2_value;
        if(ins.opcode == "and") return ins.rs1_value & ins.rs2_value;
    }
    if(ins.type == 'I') {
        if(ins.opcode == "addi")  return ins.rs1_value + ins.imm;
        if(ins.opcode == "slti")  return (ins.rs1_value < ins.imm) ? 1 : 0;
        if(ins.opcode == "sltiu") return (static_cast<unsigned>(ins.rs1_value) < static_cast<unsigned>(ins.imm)) ? 1 : 0;
        if(ins.opcode == "xori")  return ins.rs1_value ^ ins.imm;
        if(ins.opcode == "ori")   return ins.rs1_value | ins.imm;
        if(ins.opcode == "andi")  return ins.rs1_value & ins.imm;
        if(ins.opcode == "slli")  return ins.rs1_value << ins.imm;
        if(ins.opcode == "srli")  return static_cast<unsigned>(ins.rs1_value) >> ins.imm;
        if(ins.opcode == "srai")  return ins.rs1_value >> ins.imm; 
        if(ins.opcode == "lb"   || ins.opcode == "lbu" ||ins.opcode == "lh"   || ins.opcode == "lhu" ||
                ins.opcode == "lw"   || ins.opcode == "lwu" )
            return ins.rs1_value + ins.imm;
    }
    if(ins.type == 'S') return ins.rs1_value + ins.imm;
    if(ins.type == 'U') return ins.imm;  
    return 0;
}

int get_jump(instruction &ins){
    if(ins.opcode=="jal")return ins.imm/4;
    if(ins.opcode=="jalr")return (ins.rs1_value + ins.imm)/4;
    return 0;
}

bool manage_branch(instruction &ins){
    cout<<ins.rs1_value<<" "<<ins.rs2_value<<endl;
    if (ins.opcode == "beq" && ins.rs1_value == ins.rs2_value)return true;
    if (ins.opcode == "bne" && ins.rs1_value != ins.rs2_value)return true;
    if (ins.opcode == "blt" && ins.rs1_value < ins.rs2_value)return true;
    if (ins.opcode == "bge" && ins.rs1_value >= ins.rs2_value)return true;
    if (ins.opcode == "bltu" && static_cast<unsigned int>(ins.rs1_value) < static_cast<unsigned int>(ins.rs2_value))return true;
    if (ins.opcode == "bgeu" && static_cast<unsigned int>(ins.rs1_value) >= static_cast<unsigned int>(ins.rs2_value))return true; 
    return false; 
}

int binarystring_to_decimalint(string binary_format){
    int an = 0;
    for(int i = 0; i<(int)binary_format.size(); i++){
        an = an*2 + (binary_format[i] - '0');
    }
    return an;
} 

// processing the machine code instruction to form the struct
instruction process_instruction(string input_format){ 
    // first converting input_format to binary string
    string binary_format;
    for(char c : input_format) {
        if(c >= '0' && c <= '9') { binary_format += HEX_TO_BIN[c - '0'];}
        else if(c >= 'a' && c <= 'f') {binary_format += HEX_TO_BIN[c - 'a' + 10];}
        else if(c >= 'A' && c <= 'F') {binary_format += HEX_TO_BIN[c - 'A' + 10];}
        else {PRINT_ERR}
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
         // Sign Extension for -ve numbers
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
        if (ins.imm & 0x800) {   
            ins.imm |= 0xfffff000;  
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
        if (ins.imm & 0x800) {   
            ins.imm |= 0xfffff000;  
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
            if (ins.imm & 0x1000) {   
                ins.imm |= 0xffffe000;   
            }
        ins.opcode = func3_func7_check(pc, ins.funct3, ins.funct7);
    }
    else if(opcode == 0x37) {
        //| imm[31:12] (20 bits) | rd (5 bits) | opcode (7 bits) |
        ins.type = 'U';
        ins.rd = (pc >> 7) & 0x1f;
        ins.imm = (pc >> 12) & 0xfffff; 
         if (ins.imm & 0x80000000) {  
            ins.imm |= 0xfff00000;   
        }

        ins.opcode = func3_func7_check(pc, ins.funct3, ins.funct7);
    }
    else if(opcode == 0x17){ 
        ins.type = 'U';
        ins.rd = (pc >> 7) & 0x1f;
        ins.imm = (pc >> 12) & 0xfffff;
        if (ins.imm & 0x80000000) {  
        ins.imm |= 0xfff00000;   
        }   
        ins.opcode = func3_func7_check(pc, ins.funct3, ins.funct7);
    }
    else if(opcode == 0x6f) {  // for jal
        //| imm[20] | imm[10:1] | imm[11] | imm[19:12] | rd (5 bits) | opcode (7 bits) |
        ins.type = 'J';
        ins.rd = (pc >> 7) & 0x1f;
        ins.imm = ((pc >> 31) & 0x1) << 20 |
                  ((pc >> 12) & 0xff) << 12 |
                  ((pc >> 20) & 0x1) << 11 |
                  ((pc >> 21) & 0x3ff) << 1; 
        if (ins.imm & (1 << 20)) {  
        ins.imm |= 0xFFF00000;   
        ins.opcode = func3_func7_check(pc, ins.funct3, ins.funct7);
        }
    }
    else if(opcode == 0x67){  // for jalr
        //| imm[11:0] (12 bits) | rs1 (5 bits) | funct3 (3 bits) | rd (5 bits) | opcode (7 bits) |
        ins.type = 'J';
        ins.rs1 = (pc >> 15) & 0x1f;
        ins.rd = (pc >> 7) & 0x1f;
        ins.imm = (pc >> 20) & 0xfff; 
        if (ins.imm & 0x800) {   
            ins.imm |= 0xfffff000;   
        }
        ins.funct3 = (pc >> 12) & 0x7;
        ins.opcode = func3_func7_check(pc, ins.funct3, ins.funct7);
    }
    else{PRINT_ERR}  
    return ins;
}

// to get the expression like add x1 x0 0 from the instruction struct
string get_expression(instruction &ins){
    if(ins.type == 'R')return ins.opcode + " x"+ to_string(ins.rd) + " x"+ to_string(ins.rs1) + " x"+ to_string(ins.rs2) ;
    if(ins.type == 'I') {
        if(ins.opcode == "lb" || ins.opcode == "lh" || ins.opcode == "lw" || ins.opcode == "lbu" || ins.opcode == "lhu")
            return ins.opcode + " x"+ to_string(ins.rd) + " " + to_string(ins.imm) +  " x"+ to_string(ins.rs1) ;
        else
            return ins.opcode + " x"+ to_string(ins.rd) + " x"+ to_string(ins.rs1) + " " + to_string(ins.imm) ;
    }
    if(ins.type == 'S')return ins.opcode + " x"+ to_string(ins.rs2)  + " " + to_string(ins.imm)+ " x"+ to_string(ins.rs1);
    if(ins.type == 'B') return ins.opcode + " x"+ to_string(ins.rs1) + " x"+ to_string(ins.rs2) + " " + to_string(ins.imm);
    if(ins.type == 'U')return ins.opcode + " x"+ to_string(ins.rd) + " " + to_string(ins.imm);
    if(ins.type == 'J')return ins.opcode + " x"+ to_string(ins.rd) + ", " + to_string(ins.imm);
    return "";
}