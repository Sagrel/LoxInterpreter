#pragma once

#include <string>
#include <vector>
#include "Tokenizer.h"

using namespace std;

struct Visitor;

struct AST_node
{
    virtual void accept(Visitor* visitor) = 0;

    virtual ~AST_node() = default;
};

struct AST_node;
struct AST_scope;
struct AST_function;
struct AST_invocation;
struct AST_binary;
struct AST_unary;
struct AST_literal;
struct AST_asigment;
struct AST_if_else;
struct AST_return;

struct Visitor
{
    virtual void visit(AST_node* n) { n->accept(this);   }

    virtual void visit_scope(AST_scope*) {}
    virtual void visit_function(AST_function*) {}
    virtual void visit_invocation(AST_invocation*) {}
    virtual void visit_binary(AST_binary*) {}
    virtual void visit_unary(AST_unary*) {}
    virtual void visit_literal(AST_literal*) {}
    virtual void visit_asigment(AST_asigment*) {}
    virtual void visit_if_else(AST_if_else*) {}
    virtual void visit_return(AST_return*) {}

    virtual ~Visitor() = default;
};



struct AST_scope : AST_node
{
    vector<AST_node*> statements;   

    virtual void accept(Visitor* visitor) override { visitor->visit_scope(this); }
    ~AST_scope() {for(AST_node* node : statements) delete node;} 
};

struct AST_function : AST_node
{
    string name;
    vector<string> args; // TODO cambiar esto por vector<AST_declaration>
    AST_scope* body;

    virtual void accept(Visitor* visitor) override { visitor->visit_function(this); }
    ~AST_function() {delete body;} 
};

struct AST_invocation : AST_node
{
    string name;
    vector<AST_node*> args;

    virtual void accept(Visitor* visitor) override { visitor->visit_invocation(this); }
    ~AST_invocation() {for(AST_node* node : args) delete node;} 
};

struct AST_binary : AST_node
{
    AST_node* left;
    AST_node* right;
    token_type op;

    AST_binary(AST_node* l, token_type o, AST_node* r) : left(l), op(o), right(r) {}

    virtual void accept(Visitor* visitor) override { visitor->visit_binary(this); }
    ~AST_binary() {delete left; delete right;} 
};

struct AST_unary : AST_node
{
    AST_node* right;
    token_type op;

    virtual void accept(Visitor* visitor) override { visitor->visit_unary(this); }
    ~AST_unary() {delete right;} 
};

struct AST_literal : AST_node
{
    virtual void accept(Visitor* visitor) override { visitor->visit_literal(this); }

    string value;
};

struct AST_asigment : AST_node
{
    string name;
    AST_node* value;

    virtual void accept(Visitor* visitor) override { visitor->visit_asigment(this); }
    ~AST_asigment() {delete value;} 
};

struct AST_if_else : AST_node
{
    AST_node* condition;
    AST_node* if_branch;
    AST_node* else_branch = nullptr;

    virtual void accept(Visitor* visitor) override { visitor->visit_if_else(this); }
    ~AST_if_else() { delete condition; delete if_branch;    delete else_branch; }
};

struct AST_return : AST_node
{
    AST_node* value;

    virtual void accept(Visitor* visitor) override { visitor->visit_return(this); }
    ~AST_return() { delete value; }
};

