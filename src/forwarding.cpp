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
    string file_name="";
    for(int i=13;i<input_file_name.length();i++){
        file_name+=input_file_name[i];
    }
    cout<<file_name<<endl;
    string output_file_name = "./outputfiles/forwarding/"+file_name;

    int number_of_cycles = 25;
    proccessor(true, number_of_cycles,input_file_name,output_file_name);
    cout<<file_name<<endl;
    cout<<output_file_name<<endl;
    return 0;
}











