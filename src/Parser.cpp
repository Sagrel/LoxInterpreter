#include "Parser.h"


void Parser::error(string mensaje)
{    
    stringstream ss;
    ss << "Error: (" << tokens[idx].linea << ":" << tokens[idx].columna << ") -" << mensaje; 
    ss << "\t current token number "<< idx << " : " << tokens[idx];
    if(tokens[idx].tipo == IDENTIFIER || tokens[idx].tipo == INT) ss << " " << tokens[idx].valor;
    throw ss.str();
}

bool Parser::match(vector<token_type> query, vector<token_type> query2 = {})
{
    bool res = false;

    if(query2.size() != 0)
    {
        if((idx+1) > tokens.size()) return false;
        for (auto tt : query2)
        {
            if(tokens[idx + 1].tipo == tt) 
            {
                res = true;
                break;
            }
        }
        if(!res) return false;  
    }    
    for (auto tt : query)
    {
        if(tokens[idx].tipo == tt) return true;
    }

    return false;
}

Token Parser::advance()
{   
    if (idx >= tokens.size()) { idx = tokens.size() - 1; return tokens.back(); }
    Token t = tokens[idx];    
    idx++; 
    return t;
}

AST_node* Parser::parse_statement()
{
    if(match({IDENTIFIER}, {EQ})) return parse_asigment();
    if(match({IDENTIFIER}, {OPEN_P})) return parse_invocation();
    if(match({FUNCTION})) return parse_function();
    if(match({RETURN})) return parse_return();
    if(match({IF})) return parse_if_else(); 
    if(match({OPEN_B})) return parse_scope();

    error(string("Statements can not start with a token of type: " + token_names[tokens[idx].tipo]));
    return nullptr;
}

AST_node* Parser::parse_expresion()
{    
    return parse_equality();
}

AST_scope* Parser::parse_scope()
{
    AST_scope* node = new AST_scope();
    if (advance().tipo != OPEN_B) error(string("Expected '{' at the begining of a scope"));
    while(!match({ CLOSE_B, END }))
    {
        node->statements.push_back(parse_statement());
        if (!match({ SEMICOLON }))  error(string("Missing ';' at the end of the statement"));
        advance();
    }
    advance(); // CLOSE_B token
    return node;
}

AST_function* Parser::parse_function()
{
    AST_function* node = new AST_function();
    advance(); // Function token
    node->name = advance().valor;

    if(advance().tipo != OPEN_P) error(string("Expected '(' before argument list in function declaration: " + node->name));
    if(match({CLOSE_P})) advance();
    else
    {
        while(idx < tokens.size())
        {
            node->args.push_back(advance().valor);
            token_type type = advance().tipo;
            if(type == CLOSE_P) break;
            else if (type != COMMA) error(string("Expected ',' or ')' after argument in function declaration: " + node->name));
        }
    }   
    
    node->body = parse_scope();

    return node;
}

AST_invocation* Parser::parse_invocation()
{
    AST_invocation* node = new AST_invocation();

    node->name = advance().valor;

    if(advance().tipo != OPEN_P) error(string("Expected '(' before argument list in function call: " + node->name));
    if(match({CLOSE_P})) advance();
    else
    {
        while(idx < tokens.size())
        {
            node->args.push_back(parse_expresion());
            token_type type = advance().tipo;
            if(type == CLOSE_P) break;
            else if (type != COMMA) error(string("Expected ',' or ')' after argument in function call: " + node->name));
        }
    }    

    return node;
}

AST_node* Parser::parse_equality()
{
    AST_node* node = parse_comparison();

    while(match({NOTEQ, EQEQ}))
    {
        token_type op = advance().tipo;
        node = new AST_binary(node, op, parse_comparison());
    }

    return node;
}

AST_node* Parser::parse_comparison()
{
    AST_node* node = parse_addition();

    while(match({LESS, LESSEQ, MORE, MOREEQ}))
    {
        token_type op = advance().tipo;
        node = new AST_binary(node, op, parse_addition());
    }

    return node;
}


AST_node* Parser::parse_addition()
{
    AST_node* node = parse_multilpication();

    while(match({PLUS, MINUS}))
    {
        token_type op = advance().tipo;
        node = new AST_binary(node, op, parse_multilpication());
    }

    return node;
}

AST_node* Parser::parse_multilpication()
{
    AST_node* node = parse_unary();

    while(match({MULT, DIV}))
    {
        token_type op = advance().tipo;
        node = new AST_binary(node, op, parse_unary());
    }

    return node;
}

AST_node* Parser::parse_unary()
{
    AST_node* node = nullptr;

    while(match({MINUS, NOT}))
    {
        token_type op = advance().tipo;
        node = new AST_binary(node, op, parse_unary());
    }

    if(node != nullptr) return node;    

    return parse_literal();
}

AST_node* Parser::parse_literal()
{    
    if (match({ IDENTIFIER }, { OPEN_P })) return parse_invocation();
    if(match({OPEN_P})) 
    {
        advance(); // (
        AST_node* node = parse_expresion();
        advance(); // )
        return node;
    }
    if(match({INT, IDENTIFIER})) 
    {
        AST_literal* node = new AST_literal();
        node->value = advance().valor;
        return node;
    }
    error("Expresion expected");
    return nullptr;
}

AST_asigment* Parser::parse_asigment()
{
    AST_asigment* node = new AST_asigment();
    
    node->name = advance().valor;
    if(advance().tipo != EQ) error(string("Expected '=' after identifier: " + node->name));
    node->value = parse_expresion();

    return node;
}

AST_if_else* Parser::parse_if_else()
{
    AST_if_else* node = new AST_if_else();

    advance(); // if

    node->condition = parse_expresion();

    node->if_branch = parse_statement();

    if (match({ ELSE }))
    {
        advance(); // else
        node->else_branch = parse_statement();
    }

    return node;
}

AST_return* Parser::parse_return()
{
    AST_return* node = new AST_return();
    advance(); // return

    node->value = parse_expresion();
    return node;
}
