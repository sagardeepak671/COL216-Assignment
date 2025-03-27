#include "utils.hpp"
#include "instructions.hpp"
#include <bits/stdc++.h>
#include "main.hpp"
using namespace std;



// Helper function to split a string by a delimiter
vector<string> split(const string &s, char delim) {
    vector<string> tokens;
    string token;
    istringstream tokenStream(s);
    while (getline(tokenStream, token, delim)) {
        tokens.push_back(token);
    }
    return tokens;
}


void prettyPrint(vector<string> commands, vector<vector<int>>& stages,int number_of_instructions,int number_of_cycles){

    


    vector<vector<string>> tokens(number_of_instructions,vector<string>(number_of_cycles,"   "));
    for(int i=0;i<number_of_cycles;i++){
        if(stages[i][0]!=-1){
            if(tokens[stages[i][0]][i]=="   "){
                tokens[stages[i][0]][i]="IF ";
            }
            else{
                tokens[stages[i][0]][i]+="/IF";
            }
        }
        if(stages[i][1]!=-1){
            if(tokens[stages[i][1]][i]=="   "){
                tokens[stages[i][1]][i]="ID ";
            }
            else{
                tokens[stages[i][1]][i]+="/ID";
            }
        }
        if(stages[i][2]!=-1){
            if(tokens[stages[i][2]][i]=="   "){
                tokens[stages[i][2]][i]="EX ";
            }
            else{
                tokens[stages[i][2]][i]+="/EX";
            }
        }
        if(stages[i][3]!=-1){
            if(tokens[stages[i][3]][i]=="   "){
                tokens[stages[i][3]][i]="MEM";
            }
            else{
                tokens[stages[i][3]][i]+="/MEM";
            }
        }
        if(stages[i][4]!=-1){
            if(tokens[stages[i][4]][i]=="   "){
                tokens[stages[i][4]][i]="WB ";
            }
            else{
                tokens[stages[i][4]][i]+="/WB";
            }
        }
    }  
    ofstream output_file(output_file_name);
    for(int i=0;i<number_of_instructions;i++){
        output_file<<left<<setw(20)<<commands[i]<<"-> ";
        for(int j=0;j<number_of_cycles;j++){
            if(j>1 && tokens[i][j]!="   " && tokens[i][j-1]==tokens[i][j]){
                output_file<<" - "<<";";
            }
            else{
                output_file<<tokens[i][j]<<";";
            }
        }
        output_file<<endl;
    }
    output_file.close();
}


vector<instruction> takeInput(ifstream &input_file){
    vector<string> input_instructions;
    string instr;
    if(!input_file.is_open()){
        cerr << "error opening the file " << endl;
        return vector<instruction>();
    }
    while(input_file >> instr) {
        input_instructions.push_back(instr);
    } 
    input_file.close();

    int n = input_instructions.size();
    vector<instruction> instructions(n);
    for(int i = 0; i < n; i++) { 
        instructions[i] = process_instruction(input_instructions[i]);
    }

    return instructions;
}


string read_line(int n){
    ifstream input_file(input_file_name);
    string instr;
    for(int i = 0; i <= n; i++){
        input_file >> instr;
        input_file.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    input_file.close();
    return instr;
}


vector<string> extract_second_column() { 
    vector<string> result;
    ifstream input_file(input_file_name);
    string line,second_col="";
    while (getline(input_file, line)) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        for(int i=9;i<line.size();i++){
            second_col+=line[i];
        }
        result.push_back(second_col);
        second_col="";
    }
    input_file.close();
    return result;
}


int count_number_of_instructions(){
    ifstream input_file(input_file_name);
    string instr;
    int count = 0;
    while(getline(input_file,instr)){
        count++;
    }
    input_file.close();
    return count;
}

