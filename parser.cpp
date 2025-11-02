#include "parser.h"
#include <iostream>
#include <stdexcept>
using namespace std;

Parser::Parser(const vector<Token>& tokenStream) {
    tokens = tokenStream;
    current = 0;
}

Token Parser::getCurrentToken() {
    if (current < (int)tokens.size()) return tokens[current];
    return {"EOF", "", (int)tokens.size()}; // end of tokens
}

void Parser::advance() {
    if (current < (int)tokens.size()) current++;
}

bool Parser::match(const string& type) {
    if (getCurrentToken().type == type) {
        advance();
        return true;
    }
    return false;
}

void Parser::error(const string& message) {
    Token t = getCurrentToken();
    cerr << "SyntaxError at position " << t.position
         << ": " << message << " near '" << t.lexeme << "'\n";
    throw runtime_error("Parsing failed");
}

// Grammar: <stmt> → id = <expr> ;
void Parser::stmt() {
    if (getCurrentToken().type != "ID")
        error("Expected identifier at start of statement");
    advance();

    if (!match("ASSIGN"))
        error("Expected '=' after identifier");

    expr();

    if (!match("SEMI"))
        error("Expected ';' at end of statement");
}

// Grammar: <expr> → <term> { (+|-) <term> }
void Parser::expr() {
    term();
    while (getCurrentToken().type == "PLUS" || getCurrentToken().type == "MINUS") {
        advance();
        term();
    }
}

// Grammar: <term> → <factor> { (*|/) <factor> }
void Parser::term() {
    factor();
    while (getCurrentToken().type == "MULT" || getCurrentToken().type == "DIV") {
        advance();
        factor();
    }
}

// Grammar: <factor> → id | int | ( <expr> )
void Parser::factor() {
    Token t = getCurrentToken();

    if (t.type == "ID" || t.type == "INT") {
        advance();
    } else if (t.type == "LPAREN") {
        advance();
        expr();
        if (!match("RPAREN"))
            error("Expected ')' after expression");
    } else {
        error("Unexpected token in expression");
    }
}

void Parser::parse() {
    try {
        if (tokens.empty()) {
            cout << "No tokens to parse.\n";
            return;
        }

        stmt();

        if (getCurrentToken().type != "EOF" && current < (int)tokens.size())
            error("Unexpected token after complete statement");

        cout << "\n✅ Syntax is correct!\n";
    } catch (const runtime_error&) {
        cout << "❌ Syntax analysis failed.\n";
    }
}
