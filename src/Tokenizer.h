#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <map>

using namespace std;

const string token_names[] = {  "PLUS", "MINUS", "MULT", "DIV", "COMMA", "SEMICOLON", "OPEN_P", "CLOSE_P", "INT", 
                                "OPEN_B", "CLOSE_B", "MORE", "LESS", "EQ", "NOT", "MOREEQ", "LESSEQ", "EQEQ", 
                                "NOTEQ", "IDENTIFIER", "FUNCTION", "IF", "ELSE", "RETURN", "END"  };

enum token_type { PLUS, MINUS, MULT, DIV, COMMA, SEMICOLON, OPEN_P, CLOSE_P, INT, OPEN_B, CLOSE_B, MORE, LESS, EQ,
                  NOT, MOREEQ, LESSEQ, EQEQ, NOTEQ, IDENTIFIER, FUNCTION, IF, ELSE, RETURN, END };

struct Token
{
    token_type tipo;
    int linea = -1, columna = -1;
    string valor;
};

ostream& operator<<(ostream& os, const Token& t);

struct Tokenizer
{
    string source;
    vector<Token> tokens;

    int linea = 1, columna = 1, idx = 0; 
    char caracter;

    Tokenizer(string s) : source(s) { new_token(OPEN_B); tokenize(); new_token(END); }

    void new_token(token_type);
    void new_token(string);
    char next();
    void advance();
    void tokenize();
};