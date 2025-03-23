#ifndef PRETTY_PRINT_H
#define PRETTY_PRINT_H

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>

using namespace std;

// Helper function to split a string by a delimiter
vector<string> split(const string &s, char delim);

// Function to pretty print the instruction pipeline stages
void prettyPrint(const vector<string>& ans2, const vector<string>& ans);

#endif // PRETTY_PRINT_H
