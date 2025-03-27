#include <bits/stdc++.h>
using namespace std;
#include "utils.hpp"
#include "main.hpp"
#include "instructions.hpp"

// bool write_enabled[33]= {false};
int register_value[33]= {0};
// map<int,int> memory;   // RAM
uint8_t memory[10000]; 
vector<vector<int>> ans;
string input_file_name;
string output_file_name;
int number_of_instructions;
//ans is a vector of vector of size 5 which contains the instruction number at each stage 

class pipeline_stage{
public:
    int pc_address=0;
    int IF_instruction_number=-1;
    string instruction_fetched;//used in ID stage
    int ID_instruction_number=-1;
    instruction instruction_decoded;//used in EX stage
    int EX_instruction_number=-1;
    instruction instruction_executed;//used in MEM stage
    int MEM_instruction_number=-1;
    instruction instruction_memory_accessed;//used in WB stage
    int WB_instruction_number=-1;
};

//push instruction to next stage
void push_next_stage(pipeline_stage &pipeline,string instruction_fetched,instruction instruction_decoded,bool forwarding,bool stall){

    //pushing the instruction from MEM stage to WB stage
    pipeline.instruction_memory_accessed = pipeline.instruction_executed;
    pipeline.WB_instruction_number = pipeline.MEM_instruction_number;

    
    //pushing the instruction from EX stage to MEM stage
    pipeline.instruction_executed = pipeline.instruction_decoded;
    pipeline.MEM_instruction_number = pipeline.EX_instruction_number; 
    
    //ID to EX stage
    if(!stall){
        pipeline.instruction_decoded= instruction_decoded;
        pipeline.EX_instruction_number = pipeline.ID_instruction_number;
    }
    else{
        pipeline.EX_instruction_number = -1;
    }

    
    //IF to ID stage
    if(!stall){
        pipeline.instruction_fetched = instruction_fetched;
        pipeline.ID_instruction_number = pipeline.IF_instruction_number;
    }

    if(pipeline.pc_address >= number_of_instructions || pipeline.pc_address < 0)
        pipeline.IF_instruction_number = -1;
    else
        pipeline.IF_instruction_number = pipeline.pc_address;
    
}

bool get_rs_values(pipeline_stage &pipeline,instruction &instruction_decoded,bool forwarding){
    //stores the rs values in instuction_decoded struct
    //returns true if stall needed
    bool stall=false;
    if(instruction_decoded.rs1!=32 ){
        if(pipeline.EX_instruction_number != -1 && instruction_decoded.rs1==pipeline.instruction_decoded.rd){
            if(forwarding &&( !load_opcode(pipeline.instruction_decoded.opcode) && instruction_decoded.type!='B'))
                instruction_decoded.rs1_value = pipeline.instruction_decoded.result;
                
            else stall=true;
        }
        else if(pipeline.MEM_instruction_number != -1 && instruction_decoded.rs1==pipeline.instruction_executed.rd){
            cout<<"here"<<endl;
            if(forwarding && instruction_decoded.type!='B')
                instruction_decoded.rs1_value = pipeline.instruction_executed.result;
            
            else stall=true;
        }
        else{
            instruction_decoded.rs1_value = register_value[instruction_decoded.rs1];
        }
    }
    if(instruction_decoded.rs2!=32 ){
        if(pipeline.EX_instruction_number != -1 && instruction_decoded.rs2==pipeline.instruction_executed.rd){
            if(forwarding &&( !load_opcode(pipeline.instruction_decoded.opcode) && instruction_decoded.type!='B'))
                instruction_decoded.rs2_value = pipeline.instruction_executed.result;
                
            else stall=true;
        }
        else if(pipeline.MEM_instruction_number != -1 && instruction_decoded.rs2==pipeline.instruction_executed.rd){
            if(forwarding && instruction_decoded.type!='B') 
                instruction_decoded.rs2_value = pipeline.instruction_executed.result;
            
            else stall=true;
        }
        else{
            instruction_decoded.rs2_value = register_value[instruction_decoded.rs2];
        }
    }
    cout<< " stall for instruction: "<<instruction_decoded.opcode<<" "<<instruction_decoded.rd<<" "<<instruction_decoded.rs1<<" "<<instruction_decoded.rs2<<" is "<<stall<<endl;
    return stall;
}

void flush(pipeline_stage &pipeline){
    pipeline.IF_instruction_number=-1;
}


void compute(pipeline_stage &pipeline,bool forwarding){
    // computations for each instruction as per the stage it is at in pipeline
    instruction instruction_decoded;
    string instruction_fetched;
    bool stall=false;
    int jump=1;


    ans.push_back({pipeline.IF_instruction_number,pipeline.ID_instruction_number,pipeline.EX_instruction_number,pipeline.MEM_instruction_number,pipeline.WB_instruction_number});
    // cout<<"IF"<<" "<<"ID"<<" "<<"EX"<<" "<<"MEM"<<" "<<"WB"<<endl;
    // cout<<pipeline.IF_instruction_number<<"  "<<pipeline.ID_instruction_number<<"  "<<pipeline.EX_instruction_number<<"  "<<pipeline.MEM_instruction_number<<"  "<<pipeline.WB_instruction_number<<endl;

    if(pipeline.WB_instruction_number != -1){
        cout<<"updating register value for instruction: "<<pipeline.instruction_memory_accessed.opcode<<" "<<pipeline.instruction_memory_accessed.rd<<" "<<pipeline.instruction_memory_accessed.rs1<<" "<<pipeline.instruction_memory_accessed.rs2<<endl;
        update_register_value(pipeline.instruction_memory_accessed); 
    }

    if(pipeline.MEM_instruction_number != -1){
        cout<<"memory access instruction: "<<pipeline.instruction_executed.opcode<<" "<<(int)pipeline.instruction_executed.rd<<" "<<(int)pipeline.instruction_executed.rs1<<" "<<(int)pipeline.instruction_executed.rs2<<endl;
        cout<<"address"<<pipeline.instruction_executed.result<<endl;
        memory_access(pipeline.instruction_executed);
        cout<<"memory aca"<<(int)pipeline.instruction_executed.result<<endl;
    }

    if(pipeline.EX_instruction_number != -1){
        cout<<"executing instruction: "<<pipeline.instruction_decoded.opcode<<" "<<(int)pipeline.instruction_decoded.rd<<" "<<(int)pipeline.instruction_decoded.rs1<<" "<<(int)pipeline.instruction_decoded.rs2<<endl;
        int result =execute(pipeline.instruction_decoded);
        cout<<"result: "<<result<<endl;
        pipeline.instruction_decoded.result = result;
        cout<< pipeline.instruction_decoded.result<<endl;
    }

    if(pipeline.ID_instruction_number != -1){
        cout<<"decoding instruction: "<<pipeline.instruction_fetched<<endl;
        instruction_decoded = process_instruction(pipeline.instruction_fetched);
        cout<<"instruction decoded: "<<instruction_decoded.opcode<<" "<<(int)instruction_decoded.rd<<"-"<<(int)instruction_decoded.rs1<<" "<<(int)instruction_decoded.rs2<<(int)instruction_decoded.imm<<endl;
        stall = get_rs_values(pipeline,instruction_decoded,forwarding);
        //by default branch not taken
        if(instruction_decoded.type == 'B'){
            cout<<"lol"<<instruction_decoded.rs1_value<<endl;
            bool take_branch= manage_branch(instruction_decoded);   
            if(take_branch){
                flush(pipeline);
                jump=instruction_decoded.imm/4 - 1;
            }
        }
        if(instruction_decoded.type == 'J'){
            if(instruction_decoded.opcode == "jal"){
                jump = get_jump(instruction_decoded) - 1;
            }else if(instruction_decoded.opcode == "jalr"){
                // i will get the next target location so jump will be ,, now+jump = get_jump()  => jump = get_jump()-now
                jump = get_jump(instruction_decoded) - pipeline.pc_address;
                cout<<get_jump(instruction_decoded)<<"(HAHA)"<<endl;
                cout<<jump<<endl;
            }
            flush(pipeline);
        }
        if(stall)jump=0;
        cout<<"jump: "<<jump<<endl;
        cout<<"stall: "<<stall<<endl;
    }

    if(pipeline.IF_instruction_number != -1){
        instruction_fetched = read_line(pipeline.pc_address);
        cout<<"instruction fetched: "<<instruction_fetched<<endl;
    }

    pipeline.pc_address += jump;


    cout<<"pc address: "<<pipeline.pc_address<<endl;
    
    push_next_stage(pipeline,instruction_fetched,instruction_decoded,forwarding,stall);
}




void proccessor(bool forwarding, int number_of_cycles,string input_file,string output_file){
    memory[6] = 4;
    memory[0] = 4;
    input_file_name = input_file;
    output_file_name = output_file;
    int current_cycle_number=0;
    pipeline_stage pipeline;
    number_of_instructions = count_number_of_instructions();
    cout<<"number of instructions: "<<number_of_instructions<<endl;
    pipeline.IF_instruction_number = 0;
    while(current_cycle_number < number_of_cycles){
        cout<<"CYCLE NUMBER: "<<current_cycle_number<<endl;
        compute(pipeline,forwarding);
        cout<<"--------------------------------"<<endl;
        current_cycle_number++;

    }
    // for(int i=0;i<ans.size();i++){
    //     for(int j=0;j<ans[i].size();j++){
    //         cout<<ans[i][j]<<" ";
    //     }
    //     cout<<endl;
    // }
    vector<string> ans_str=extract_second_column();
    prettyPrint(ans_str,ans,number_of_instructions,current_cycle_number);

}








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