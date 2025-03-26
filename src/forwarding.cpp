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

    string input_file_name = argv[1];
    string output_file_name = "output_forwarding.txt";

    int number_of_cycles = 25;
    proccessor(true, number_of_cycles,input_file_name,output_file_name);
    return 0;
}











