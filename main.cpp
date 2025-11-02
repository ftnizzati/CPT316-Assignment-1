#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <iomanip>
#include "parser.h" // insert the parser header file
#include "token.h" // insert the token structure header file
using namespace std;

// Function to check if a lexeme is a valid lowercase single-letter identifier. 

bool isSingleLetterID(const string& lexeme) {
    return lexeme.size() == 1 && islower(lexeme[0]); 
    
    // boolean expression that check whether the lexeme is a single lowercase letter. 
    //lexeme.size() == 1 ensures that the lexeme consists of exactly one character, and islower(lexeme[0]) checks if that character is a lowercase letter from 'a' to 'z'.
    // logical AND operator to ensure both condition are met. 
}

// Tokenize function to convert input string into tokens

vector<Token> tokenize(const string& input, vector<Token>& invalidTokens) {
    vector<Token> tokens;
    int i = 0;
    int n = input.length();

    while (i < n) {
        char ch = input[i];

        // Skip whitespace
        if (isspace(ch)) {
            i++;
            continue;
        }

        int start = i; // records the starting position of the token

        // call the built in function isalpha for Identifiers (single-letter lowercase)
        if (isalpha(ch)) {
            string lexeme;
            while (i < n && isalpha(input[i])) {
                lexeme += input[i++];
            }

        // call the function isSingleLetterID to check if the lexeme is a valid single-letter identifier.
            if (isSingleLetterID(lexeme))
                tokens.push_back({"ID", lexeme, start});
            else
                invalidTokens.push_back({"INVALID_ID", lexeme, start});

            continue;
        }

        // call the built in function isdigit for Integers (multi-digit allowed)
        if (isdigit(ch)) {
            string num;
            while (i < n && isdigit(input[i])) { // continue to build the number as long as the characters are digits
                num += input[i++];
            }
            tokens.push_back({"INT", num, start}); // create an INT token
            continue;
        }

        // Operators / Symbols
        switch (ch) {
            case '+': tokens.push_back({"PLUS", "+", i++}); break;
            case '-': tokens.push_back({"MINUS", "-", i++}); break;
            case '*': tokens.push_back({"MULT", "*", i++}); break;
            case '/': tokens.push_back({"DIV", "/", i++}); break;
            case '=': tokens.push_back({"ASSIGN", "=", i++}); break;
            case '(': tokens.push_back({"LPAREN", "(", i++}); break;
            case ')': tokens.push_back({"RPAREN", ")", i++}); break;
            case ';': tokens.push_back({"SEMI", ";", i++}); break;
            default: {
                string invalidLexeme(1, ch);
                i++;
                // capture consecutive invalid characters
                while (i < n && !isspace(input[i]) && !isalnum(input[i]) &&
                       input[i] != '+' && input[i] != '-' && input[i] != '*' &&
                       input[i] != '/' && input[i] != '=' &&
                       input[i] != '(' && input[i] != ')' && input[i] != ';') {
                    invalidLexeme += input[i++];
                }
                invalidTokens.push_back({"INVALID_CHAR", invalidLexeme, start});
                break;
            }
        }
    }
    return tokens;
}

void printTokens(const vector<Token>& tokens) {
    cout << "\n=== TOKEN TABLE ===\n";
    cout << left << setw(15) << "Type" << setw(15) << "Lexeme" << "Position\n";
    cout << string(40, '-') << endl;
    for (const auto& t : tokens) {
        cout << left << setw(15) << t.type << setw(15) << t.lexeme << t.position << endl;
    }
}

void printInvalids(const vector<Token>& invalidTokens) {
    cout << "\n=== INVALID TOKENS ===\n";
    if (invalidTokens.empty()) {
        cout << "None\n";
    } else {
        for (const auto& inv : invalidTokens) {
            cout << "LexicalError at position " << inv.position
                 << ": invalid token '" << inv.lexeme << "'\n";
        }
    }
}

void printSummary(const vector<Token>& tokens) {
    cout << "\n=== TOKEN COUNT SUMMARY ===\n";
    int idCount = 0, intCount = 0, opCount = 0, symCount = 0;

    for (const auto& t : tokens) {
        if (t.type == "ID") idCount++;
        else if (t.type == "INT") intCount++;
        else if (t.type == "PLUS" || t.type == "MINUS" || t.type == "MULT" || t.type == "DIV")
            opCount++;
        else
            symCount++;
    }

    cout << "Total Tokens: " << tokens.size() << endl;
    cout << "Identifiers : " << idCount << endl;
    cout << "Integers    : " << intCount << endl;
    cout << "Operators   : " << opCount << endl;
    cout << "Symbols     : " << symCount << endl;
}

int main() {
      cout << "=== Lexical Analyzer ===\n";
    cout << "Enter an expression to tokenize (type 'exit' to quit):\n";

    string input;
    while (true) {
        cout << "\n> ";
        getline(cin, input);

        if (input == "exit" || input == "quit") {
            cout << "\nExiting program...\n";
            break;
        }

        vector<Token> invalidTokens;
        vector<Token> tokens = tokenize(input, invalidTokens);

        printTokens(tokens);
        printInvalids(invalidTokens);
        printSummary(tokens);
        // ---- Run Syntax Analyzer ----
        if (invalidTokens.empty()) {
            cout << "\n=== SYNTAX ANALYSIS ===\n";
            Parser parser(tokens);
            parser.parse();
        } else {
            cout << "\nSkipping syntax analysis due to lexical errors.\n";
        }
    }
    return 0;
}
