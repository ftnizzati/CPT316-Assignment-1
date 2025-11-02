#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <string>
#include "token.h"
using namespace std;

class Parser {
private:
    vector<Token> tokens;
    int current; // index of current token
    Token getCurrentToken();
    void advance();
    bool match(const string& type);
    void error(const string& message);

    // Recursive descent parsing methods
    void stmt();
    void expr();
    void term();
    void factor();

public:
    Parser(const vector<Token>& tokenStream);
    void parse();
};

#endif