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

void processor(vector<instruction> &pc,int m,bool forwarding){ 
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
            if(forwarding && (pc[j].type == 'R' || pc[j].type == 'I')){ 
                if(pc[j].opcode != "lb" && pc[j].opcode != "lh" && pc[j].opcode != "lw" && pc[j].opcode != "lbu" && pc[j].opcode != "lhu"){
                    write_enabled[pc[j].rd] = false;
                } 
                else {
                    write_enabled[pc[j].rd] = true;
                }
            }
            }else if(command_stage[j] == 'M' && mp['W'] == -1){
                command_stage[j] = 'W';
                ans[j] += "WB;";
                mp['W'] = j;
                mp['M'] = -1; 
                if(forwarding && (pc[j].type == 'R' || pc[j].type == 'I')){ 
                     write_enabled[pc[j].rd] = false;  
                }
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
