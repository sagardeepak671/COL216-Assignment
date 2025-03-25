#include <bits/stdc++.h>
using namespace std;
#include "utils.hpp"
#include "main.hpp"
#include "instructions.hpp"


bool write_enabled[33]= {false};
int register_value[33]= {0};
map<int,int> memory;   // RAM


// - mean not in any stage 
// I mean in IF stage
// D mean in ID stage
// E mean in EX stage
// M mean in MEM stage
// W mean in WB stage

// void processor(int m,bool forwarding){ 
//     vector<string> expressions(n,"");
//     for(int i =0; i<n; i++) expressions[i] = get_expression(pc[i]);
 
//     vector<char> command_stage(n,'-');   // current stage on this instruction , - mean not in any stage
//     map<int,int> mp ={ {'I',-1}, {'D',-1}, {'E',-1}, {'M',-1}, {'W',-1} };   // map to store whether stage is free or not
//     memory[11]=12;
//     vector<string> ans(n,"");
//     int next_pc_address = 0;  // for jumping and initial pc address 
//     int max_pc_address = 0;   // for next pc address for instruction fetch
//     for(int i=0; i<min(25,m); i++){  // number of vectical columns to print
//     int flg =-1;           // flg for flushing the instruction after that cycle
//     int flg2=-1;    
//         for(int j=0; j<n; j++){
//             if(j==next_pc_address || (mp['I'] == -1 && j==max_pc_address+1)){
//                 ans[j] += "IF;";
//                 mp['I'] = j;
//                 command_stage[j] = 'I';
//                 next_pc_address = -1;
//                 max_pc_address = j;
//             }else if(command_stage[j] =='-') ans[j] += " ;";
//             else if(command_stage[j] == 'I' && mp['D'] == -1){
//                 command_stage[j] = 'D';
//                 ans[j] += "ID;";
//                 mp['D'] = j;
//                 mp['I'] = -1;
//             }else if(command_stage[j] == 'D'&& mp['E'] == -1 
//             &&( (pc[j].rs1 != 32 && write_enabled[pc[j].rs1] == false) || pc[j].rs1 == 32 )
//             && ( (pc[j].rs2 != 32 && write_enabled[pc[j].rs2] == false) || pc[j].rs2 == 32 )
//             ){ 
//                 command_stage[j] = 'E';
//                 ans[j] += "EX;";
//                 mp['E'] = j;
//                 mp['D'] = -1;
//                 if(update_register_value(pc[j],next_pc_address,j) == true){
//                         // branch was taken
//                         if(next_pc_address == j+1){// do nothing
//                         }else if(next_pc_address == j+2){
//                             // flush j+1 th instruction
//                             flg2 = j;
                            
//                         }else{
//                             flg =j;
//                         } 
//                 }else if(pc[j].type == 'R' || pc[j].type == 'I'){
//                     write_enabled[pc[j].rd] = true;
//                 }


//             }else if(command_stage[j] == 'E' && mp['M'] == -1 ){
//             command_stage[j] = 'M';
//             ans[j] += "MEM;";
//             mp['M'] = j;
//             mp['E'] = -1;
//             if(forwarding && (pc[j].type == 'R' || pc[j].type == 'I')){ 
//                 if(pc[j].opcode != "lb" && pc[j].opcode != "lh" && pc[j].opcode != "lw" && pc[j].opcode != "lbu" && pc[j].opcode != "lhu"){
//                     write_enabled[pc[j].rd] = false;
//                 } 
//                 else {
//                     write_enabled[pc[j].rd] = true;
//                 }
//             }
//             }else if(command_stage[j] == 'M' && mp['W'] == -1){
//                 command_stage[j] = 'W';
//                 ans[j] += "WB;";
//                 mp['W'] = j;
//                 mp['M'] = -1; 
//                 if(forwarding && (pc[j].type == 'R' || pc[j].type == 'I')){ 
//                      write_enabled[pc[j].rd] = false;  
//                 }
//             }else if(command_stage[j] == 'W'){
//                 command_stage[j] = '-';
//                 ans[j] += " ;";
//                 mp['W'] = -1;
//                 if(pc[j].type == 'R' || pc[j].type == 'I'){ 
//                     write_enabled[pc[j].rd] = false;
//                 }
//             }else ans[j] += " ;";  
//         }
//             if(flg!=-1){
//                     mp['I'] = -1;
//                     if(flg+1<n)command_stage[flg+1] = '-';
//                     if(flg+2<n)command_stage[flg+2] = '-';
//                     mp['D'] = -1;
//                     mp['E'] = -1;
//                     mp['M'] = -1;
//                     mp['W'] = -1;
//                     flg =-1;
//                     max_pc_address = next_pc_address;
//             }
//             if(flg2!=-1){
//                 mp['D'] = -1;
//                 if(flg2+1<n) command_stage[flg2+1] = '-';
//                 flg2 = -1;

//             }
            
//     }
//     // printing the answer
//     prettyPrint(expressions,ans); // for now pretty printing
// }

class pipeline_stage{
public:
    int pc_address=0;
    bool IF_free=true;
    string instruction_fetched;//used in ID stage
    bool ID_free=true;
    instruction instruction_decoded;//used in EX stage
    bool EX_free=true;
    instruction instruction_executed;//used in MEM stage
    bool MEM_free=true;
    instruction instruction_memory_accessed;//used in WB stage
    bool WB_free=true;
};

void push_next_stage(pipeline_stage &pipeline,string instruction_fetched,instruction instruction_decoded,bool forwarding){
    //push instruction to next stage
    if(!pipeline.MEM_free){
        pipeline.instruction_memory_accessed = pipeline.instruction_executed;
        pipeline.WB_free = false;
    }
    if(!pipeline.EX_free){
        pipeline.instruction_executed = pipeline.instruction_decoded;
        pipeline.MEM_free = false;
    }
    if(!pipeline.ID_free){
        pipeline.instruction_decoded= instruction_decoded;
        pipeline.EX_free = false;
    }
    if(!pipeline.IF_free){
        pipeline.instruction_fetched = instruction_fetched;
        pipeline.ID_free = false;
    }
}

bool get_rs_values(pipeline_stage &pipeline,instruction &instruction_decoded,bool forwarding){
    //stores the rs values in instuction_decoded struct
    //returns true if stall needed
    bool stall=false;
    if(instruction_decoded.rs1!=32 ){
        if(!pipeline.EX_free && instruction_decoded.rs1==pipeline.instruction_decoded.rd){
            if(forwarding &&( pipeline.instruction_decoded.opcode!="ld" && pipeline.instruction_decoded.opcode!="lw" && pipeline.instruction_decoded.opcode!="lbu" && pipeline.instruction_decoded.opcode!="lhu"))
                instruction_decoded.rs1 = pipeline.instruction_decoded.result;
                
            else stall=true;
        }
        else if(!pipeline.MEM_free && instruction_decoded.rs1==pipeline.instruction_memory_accessed.rd){
            if(forwarding)
                instruction_decoded.rs1 = pipeline.instruction_memory_accessed.result;
            
            else stall=true;
        }
        else{
            instruction_decoded.rs1_value = register_value[instruction_decoded.rs1];
        }
    }
    if(instruction_decoded.rs2!=32 ){
        if(!pipeline.EX_free && instruction_decoded.rs2==pipeline.instruction_executed.rd){
            if(forwarding)
                instruction_decoded.rs2 = pipeline.instruction_executed.result;
                
            else stall=true;
        }
        else if(!pipeline.MEM_free && instruction_decoded.rs2==pipeline.instruction_memory_accessed.rd){
            if(forwarding)
                instruction_decoded.rs2 = pipeline.instruction_memory_accessed.result;
            
            else stall=true;
        }
        else{
            instruction_decoded.rs2_value = register_value[instruction_decoded.rs2];
        }
    }
    return stall;
}

void compute(pipeline_stage &pipeline,bool forwarding){
    // computations for each instruction as per the stage it is at in pipeline
    instruction instruction_decoded;
    string instruction_fetched;
    bool stall=false;
    if(!pipeline.IF_free){
        instruction_fetched = read_line(pipeline.pc_address);
    }
    if(!pipeline.ID_free){
        instruction_decoded = process_instruction(pipeline.instruction_fetched);
        int jump=1;
        stall = get_rs_values(pipeline,instruction_decoded,forwarding);
        if(instruction_decoded.type == 'B'){
            jump = manage_branch(instruction_decoded);
        }
        else if(instruction_decoded.type == 'J'){
            jump = instruction_decoded.imm/4;
        }
        if(stall)jump=0;
        pipeline.pc_address += jump;
    }
    if(!pipeline.EX_free){
        int result =execute(pipeline.instruction_decoded);
        pipeline.instruction_decoded.result = result;
    }
    if(!pipeline.MEM_free){
        memory_access(pipeline.instruction_decoded);
    }
    if(!pipeline.WB_free){
        update_register_value(pipeline.instruction_memory_accessed);
    }

    push_next_stage(pipeline,instruction_fetched,instruction_decoded,forwarding);
}

void proccessor(bool forwarding, int number_of_cycles){
    int current_cycle_number=0;
    pipeline_stage pipeline;
    while(current_cycle_number < number_of_cycles){
        pipeline.IF_free = false;
        compute(pipeline,forwarding);

    }
}