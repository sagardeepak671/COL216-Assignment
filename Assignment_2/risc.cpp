#include <bits/stdc++.h>
using namespace std;

const string HEX_TO_BIN[] = {
    "0000", "0001", "0010", "0011", "0100", "0101", "0110", "0111",
    "1000", "1001", "1010", "1011", "1100", "1101", "1110", "1111"
};

struct instruction{
    char type;
    string opcode; // add, sub,....
    int rd;
    int rs1;
    int rs2;
    int imm; 
    int funct3;
    int funct7;
};

int binarystring_to_decimalint(string binary_format){
    int an = 0;
    for(int i = 0; i<binary_format.size(); i++){
        an = an*2 + (binary_format[i] - '0');
    }
    return an;
}

string func3_func7_check(int opcode,int funct3, int funct7){
    if(opcode == 0x33){
        //R-type
        /*   add	0x33	0x00	0x0	rd = rs1 + rs2
            sub	0x33	0x20	0x0	rd = rs1 - rs2
            sll	0x33	0x00	0x1	rd = rs1 << rs2 (Shift Left Logical)
            slt	0x33	0x00	0x2	rd = (rs1 < rs2) ? 1 : 0 (Set Less Than)
            sltu	0x33	0x00	0x3	rd = (rs1 < rs2) ? 1 : 0 (Unsigned)
            xor	0x33	0x00	0x4	rd = rs1 ^ rs2
            srl	0x33	0x00	0x5	rd = rs1 >> rs2 (Logical Shift Right)
            sra	0x33	0x20	0x5	rd = rs1 >> rs2 (Arithmetic Shift Right)
            or	0x33	0x00	0x6	`rd = rs1
            and	0x33	0x00	0x7	rd = rs1 & rs2*/
        
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
    else if(opcode == 0x13){    
        //I-type
        /*
        addi	0x13	0x0	Sign-extended	rd = rs1 + imm (Add Immediate)
        slti	0x13	0x2	Sign-extended	rd = (rs1 < imm) ? 1 : 0 (Set Less Than)
        sltiu	0x13	0x3	Zero-extended	rd = (rs1 < imm) ? 1 : 0 (Unsigned)
        xori	0x13	0x4	Sign-extended	rd = rs1 ^ imm
        ori	0x13	0x6	Sign-extended	`rd = rs1
        andi	0x13	0x7	Sign-extended	rd = rs1 & imm
        slli	0x13	0x1	Zero-extended	rd = rs1 << shamt (Shift Left Logical)
        srli	0x13	0x5	Zero-extended	rd = rs1 >> shamt (Logical Shift Right)
        srai	0x13	0x5	`0x20	shamt`
        */
       if(funct3 == 0x00 && funct7 == 0x00)  return "addi";
       else if(funct3 == 0x02 && funct7 == 0x00)  return "slti";
       else if(funct3 == 0x03 && funct7 == 0x00)  return "sltiu";
       else if(funct3 == 0x04 && funct7 == 0x00)  return "xori";
       else if(funct3 == 0x06 && funct7 == 0x00)  return "ori";
       else if(funct3 == 0x07 && funct7 == 0x00)  return "andi";
       else if(funct3 == 0x01 && funct7 == 0x00)  return "slli";
       else if(funct3 == 0x05 && funct7 == 0x00)  return "srli";
       else if(funct3 == 0x20 && funct7 == 0x05)  return "srai";
       else{cout << "wrong input" << endl;exit(0);}
    }
    else if(opcode == 0x23){
        //S-type
        /*
        sb	0x23	0x0	Split into two parts	Store Byte
        sh	0x23	0x1	Split into two parts	Store Halfword (16-bit)
        sw	0x23	0x2	Split into two parts	Store Word (32-bit)
        sd	0x23	0x3	Split into two parts	Store Doubleword (64-bit)
        */
       if(funct3 == 0x00 && funct7 == 0x00)  return "sb";
       else if(funct3 == 0x01 && funct7 == 0x00)  return "sh";
       else if(funct3 == 0x02 && funct7 == 0x00)  return "sw";
       else if(funct3 == 0x03 && funct7 == 0x00)  return "sd";
       else{cout << "wrong input" << endl;exit(0);}
    }
    else if(opcode == 0x63){
        //B-type
        /*
        beq	0x63	0x0	Split into two parts	Branch if Equal
        bne	0x63	0x1	Split into two parts	Branch if Not Equal
        blt	0x63	0x4	Split into two parts	Branch if Less Than
        bge	0x63	0x5	Split into two parts	Branch if Greater Than or Equal
        bltu	0x63	0x6	Split into two parts	Branch if Less Than Unsigned
        bgeu	0x63	0x7	Split into two parts	Branch if Greater Than or Equal Unsigned
        */
       if(funct3 == 0x00 && funct7 == 0x00)  return "beq";
       else if(funct3 == 0x01 && funct7 == 0x00)  return "bne";
       else if(funct3 == 0x04 && funct7 == 0x00)  return "blt";
       else if(funct3 == 0x05 && funct7 == 0x00)  return "bge";
       else if(funct3 == 0x06 && funct7 == 0x00)  return "bltu";
       else if(funct3 == 0x07 && funct7 == 0x00)  return "bgeu";
       else{cout << "wrong input" << endl;exit(0);}
    }
    else if(opcode == 0x37){
        //U-type
        /*
        lui	0x37	0x0	Split into two parts	Load Upper Immediate
        auipc	0x37	0x1	Split into two parts	Add Upper Immediate to PC 
        lw	0x3	0x2	Split into two parts	Load Word (32-bit)
        lh	0x3	0x1	Split into two parts	Load Halfword (16-bit)
        lb	0x3	0x0	Split into two parts	Load Byte (8-bit)
        lhu	0x3	0x5	Split into two parts	Load Halfword Unsigned (16-bit)
        lbu	0x3	0x4	Split into two parts	Load Byte Unsigned (8-bit)
        lwu	0x3	0x3	Split into two parts	Load Word Unsigned (32-bit)
        ld	0x3	0x3	Split into two parts	Load Doubleword (64-bit)
        sd	0x3	0x3	Split into two parts	Store Doubleword (64-bit)
        sw	0x3	0x2	Split into two parts	Store Word (32-bit)
        sh	0x3	0x1	Split into two parts	Store Halfword (16-bit)
        sb	0x3	0x0	Split into two parts	Store Byte
        */
       if(funct3 == 0x00 && funct7 == 0x00)  return "lui";
       else if(funct3 == 0x01 && funct7 == 0x00)  return "auipc"; 
       else if(funct3 == 0x02 && funct7 == 0x00)  return "lw";
       else if(funct3 == 0x01 && funct7 == 0x00)  return "lh";
       else if(funct3 == 0x00 && funct7 == 0x00)  return "lb";
       else if(funct3 == 0x05 && funct7 == 0x00)  return "lhu";
       else if(funct3 == 0x04 && funct7 == 0x00)  return "lbu";
       else if(funct3 == 0x03 && funct7 == 0x00)  return "lwu";
       else if(funct3 == 0x03 && funct7 == 0x00)  return "ld";
       else if(funct3 == 0x03 && funct7 == 0x00)  return "sd";
       else if(funct3 == 0x02 && funct7 == 0x00)  return "sw";
       else if(funct3 == 0x01 && funct7 == 0x00)  return "sh";
       else if(funct3 == 0x00 && funct7 == 0x00)  return "sb";
       else{cout << "wrong input" << endl;exit(0);}
    }
    else if(opcode == 0x6f){
        //J-type
        /*
        jal	0x6f	0x0	Split into two parts	Jump and Link
        */
       if(funct3 == 0x00 && funct7 == 0x00)  return "jal";
       else{cout << "wrong input" << endl;exit(0);}
    }
    return "";
}

void process_instruction(string input_format, instruction &ins){ 
    // first converting input_format to binary string
    string binary_format;
    for(char c : input_format) {
        if(c >= '0' && c <= '9') { binary_format += HEX_TO_BIN[c - '0'];}
        else if(c >= 'a' && c <= 'f') {binary_format += HEX_TO_BIN[c - 'a' + 10];}
        else if(c >= 'A' && c <= 'F') {binary_format += HEX_TO_BIN[c - 'A' + 10];}
        else {cout << "wrong input" << endl;exit(0);}
    } 
    int pc = binarystring_to_decimalint(binary_format.substr(0,32));
    // extracting opcode rd,rs1,rs2,imm
    int opcode = pc & 0x7f;
    if(opcode == 0x33){
        //| funct7 (7 bits) | rs2 (5 bits) | rs1 (5 bits) | funct3 (3 bits) | rd (5 bits) | opcode (7 bits) |
        ins.type = 'R';
        ins.rs1 = (opcode >> 5) & 0x1f;
        ins.rs2 = (opcode >> 10) & 0x1f;
        ins.rd = (opcode >> 15) & 0x1f;
        ins.opcode = (opcode >> 20) & 0x7f;
        ins.funct3 = (opcode >> 27) & 0x7;
        ins.funct7 = (opcode >> 32) & 0x7f;
        ins.opcode = func3_func7_check(opcode, ins.funct3, ins.funct7);

    }
    else if(opcode == 0x13){
        //| imm[11:0] (12 bits) | rs1 (5 bits) | funct3 (3 bits) | rd (5 bits) | opcode (7 bits) |
        ins.type = 'I';
        ins.rs1 = (opcode >> 5) & 0x1f;
        ins.rd = (opcode >> 10) & 0x1f;
        ins.imm = (opcode >> 22) & 0xfff;
        ins.opcode = func3_func7_check(opcode, ins.funct3, ins.funct7);
    }
    else if(opcode == 0x23){
        //| imm[11:5] (7 bits) | rs2 (5 bits) | rs1 (5 bits) | funct3 (3 bits) | imm[4:0] (5 bits) | opcode (7 bits) |
        ins.type = 'S';
        ins.rs1 = (opcode >> 5) & 0x1f;
        ins.rs2 = (opcode >> 10) & 0x1f;
        ins.imm = (opcode >> 22) & 0xfff;
        ins.opcode = func3_func7_check(opcode, ins.funct3, ins.funct7);
    }
    else if(opcode == 0x63){
        //| imm[12] | imm[10:5] | rs2 (5 bits) | rs1 (5 bits) | funct3 (3 bits) | imm[4:1] | imm[11] | opcode (7 bits) |
        ins.type = 'B';
        ins.rs1 = (opcode >> 5) & 0x1f;
        ins.rs2 = (opcode >> 10) & 0x1f;
        ins.imm = (opcode >> 22) & 0xfff;
        ins.opcode = func3_func7_check(opcode, ins.funct3, ins.funct7);
    }
    else if(opcode == 0x37){
        //| imm[31:12] (20 bits) | rd (5 bits) | opcode (7 bits) |
        ins.type = 'U';
        ins.rd = (opcode >> 10) & 0x1f;
        ins.imm = (opcode >> 22) & 0xfff;
        ins.opcode = func3_func7_check(opcode, ins.funct3, ins.funct7);
    }
    else if(opcode == 0x6f){
        //| imm[20] | imm[10:1] | imm[11] | imm[19:12] | rd (5 bits) | opcode (7 bits) |
        ins.type = 'J';
        ins.rd = (opcode >> 10) & 0x1f;
        ins.imm = (opcode >> 22) & 0xfff;
        ins.opcode = func3_func7_check(opcode, ins.funct3, ins.funct7);
    }
    else{
        cout << "wrong input" << endl;
        exit(0);
    } 

}

void print_expression(instruction &ins){
    if(ins.type == 'R')cout << ins.opcode << " " << ins.rs1 << " " << ins.rs2 << " " << ins.rd << endl;
    else if(ins.type == 'I') cout << ins.opcode << " " << ins.rs1 << " " << ins.imm << " " << ins.rd << endl;
    else if(ins.type == 'S')cout << ins.opcode << " " << ins.rs1 << " " << ins.rs2 << " " << ins.imm << endl;
    else if(ins.type == 'B') cout << ins.opcode << " " << ins.rs1 << " " << ins.rs2 << " " << ins.imm << endl;
    else if(ins.type == 'U')cout << ins.opcode << " " << ins.rd << " " << ins.imm << endl;
    else if(ins.type == 'J')cout << ins.opcode << " " << ins.rd << " " << ins.imm << endl;
}

void print_without_forwarding(vector<instruction> instructions){
    // printing the pipeline with 5 stages without Forwarding
    //IF;ID;EX;MEM;WB
    int cnt_spaces =0;
    for(int i=0;i<instructions.size();i++){
        print_expression(instructions[i]);
        for(int j=0;j<cnt_spaces;j++){cout<<" ;";}



        cout<<endl;
        cnt_spaces++;
    }
}


int main(){
    vector<string> input_instructions;
    string instr;
    while(cin>>instr){
        input_instructions.push_back(instr);
    }
    vector<instruction> instructions;
    for(int i=0;i<input_instructions.size();i++){
        instruction ins;
        process_instruction(input_instructions[i], ins);
        instructions.push_back(ins);
    }

    // printing the pipeline with 5 stages without Forwarding
    print_without_forwarding(instructions);



    return 0;
}