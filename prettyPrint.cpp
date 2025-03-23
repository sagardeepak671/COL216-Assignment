#include "prettyPrint.h"

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

// Pretty print function that aligns columns
void prettyPrint(const vector<string>& ans2, const vector<string>& ans) {
    // 1. Find the maximum length for the instruction column (ans2)
    size_t maxInstrWidth = 0;
    for (const auto &instr : ans2) {
        if (instr.size() > maxInstrWidth)
            maxInstrWidth = instr.size();
    }
    
    // 2. Split each ans string into tokens (columns) using ';' as the delimiter
    vector<vector<string>> tokens;
    size_t maxCols = 0;
    for (const auto &line : ans) {
        vector<string> cols = split(line, ';');
        if (cols.size() > maxCols)
            maxCols = cols.size();
        tokens.push_back(cols);
    }
    
    // 3. For each column of the tokens, compute the maximum width
    vector<size_t> colWidths(maxCols, 0);
    for (const auto &row : tokens) {
        for (size_t j = 0; j < row.size(); j++) {
            // Remove any extra leading/trailing spaces (optional)
            string token = row[j];
            if (token.size() > colWidths[j])
                colWidths[j] = token.size();
        }
    }
    
    // 4. Print each row with columns aligned
    for (size_t i = 0; i < ans2.size(); i++) {
        // Print instruction part (ans2[i]) padded to maxInstrWidth
        cout << left << setw(maxInstrWidth) << ans2[i] << "; ";
        
        // Print the tokens from ans[i] using the calculated column widths
        const vector<string>& rowTokens = tokens[i];
        for (size_t j = 0; j < rowTokens.size(); j++) {
            cout << left << setw(colWidths[j]) << rowTokens[j];
            if (j < rowTokens.size() - 1)
                cout << "; ";
        }
        cout << endl;
    }
}
