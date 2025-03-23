#include <bits/stdc++.h>
using namespace std;
#include "utils.hpp"
#include "main.hpp"
#include "instructions.hpp"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "enter the input file name"<< endl;
        return 1;
    }

    ifstream input_file(argv[1]);
    vector<instruction> input_instructions = takeInput(input_file);
    input_file.close();


    int m = 50; // number of cycles
    processor(input_instructions, m, false);
    return 0;
}
