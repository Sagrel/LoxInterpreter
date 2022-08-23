#pragma once

#include "AST.h"

struct Parser
{
    vector<Token> tokens;

    int idx = 0;

    AST_node* root;

    Parser(vector<Token> t) : tokens(t) { root = parse_scope(); }

    bool match(vector<token_type>, vector<token_type>);

    void error(string mensaje);

    Token advance();

    AST_node* parse_statement();
    AST_node* parse_expresion();

    AST_scope* parse_scope();
    AST_function* parse_function();
    AST_invocation* parse_invocation();
    AST_node* parse_equality();
    AST_node* parse_comparison();
    AST_node* parse_addition();
    AST_node* parse_multilpication();
    AST_node* parse_unary();
    AST_node* parse_literal();
    AST_asigment* parse_asigment();
    AST_if_else* parse_if_else();
    AST_return* parse_return();
};
