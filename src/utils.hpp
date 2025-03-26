#ifndef PRETTY_PRINT_H
#define PRETTY_PRINT_H

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include "instructions.hpp"
using namespace std;

// Helper function to split a string by a delimiter
vector<string> split(const string &s, char delim);

// Function to pretty print the instruction pipeline stages
void prettyPrint(vector<string> commands, vector<vector<int>>& stages,int number_of_instructions,int number_of_cycles);

vector<instruction> takeInput(ifstream &input_file);
string read_line(int n);
int count_number_of_instructions();
vector<string> extract_second_column();
// void print_output(vector<string> &ans_str,vector<string> &ans);
#endif // PRETTY_PRINT_H
