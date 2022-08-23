#pragma once

#include <unordered_map>
#include <deque>

#include "AST.h"

struct Scope
{
    Scope* parent;
    unordered_map<string, pair<AST_function*, Scope*>> functions;
    unordered_map<string, double> variables;

    Scope(Scope* p) : parent(p) {}

    double get_variable(string name);
    pair<AST_function*, Scope*> get_function(string);
};


struct Interpreter : Visitor
{
    deque<Scope> scopes = {{nullptr}};

    double res;

    bool create_scope = false;
    bool returning = false;

    void visit_scope(AST_scope*) override;
    void visit_function(AST_function*) override;
    void visit_invocation(AST_invocation*) override;
    void visit_binary(AST_binary*) override;
    void visit_unary(AST_unary*) override;
    void visit_literal(AST_literal*) override;
    void visit_asigment(AST_asigment*) override;    
    void visit_if_else(AST_if_else*) override;
    void visit_return(AST_return*) override;
};