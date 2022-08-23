#include "Interpreter.h"

double Scope::get_variable(string name)
{
    auto it = variables.find(name);
    if (it == variables.end())
    {
        if (parent == nullptr) throw string("Undeclared variable: " + name);
        else return parent->get_variable(name);
    }
    return it->second;
}

pair<AST_function*, Scope*> Scope::get_function(string name)
{
    auto it = functions.find(name);
    if (it == functions.end())
    {
        if (parent == nullptr) throw string("Undeclared function: " + name);
        else return parent->get_function(name);
    }
    return it->second;
}

void Interpreter::visit_scope(AST_scope* s)
{
    if (create_scope) scopes.push_back({ scopes.back() });
    create_scope = true;
    for (AST_node* node : s->statements)
    {
        visit(node);
        if (returning) break;
    }
    scopes.pop_back();
}

void Interpreter::visit_function(AST_function* f) 
{
    scopes.back().functions.insert({ f->name, {f, &scopes.back()} });
}

void Interpreter::visit_invocation(AST_invocation* i) 
{
    auto f = scopes.back().get_function(i->name);

    Scope s = Scope(f.second);


    for(int idx = 0; idx < i->args.size(); idx++)
    {
        visit(i->args[idx]);
        s.variables.insert({f.first->args[idx], res});
    }
    
    scopes.push_back(s);

    create_scope = false;
    visit(f.first->body);
    returning = false;
}

void Interpreter::visit_binary(AST_binary* b) 
{
    visit(b->left);
    double left = res;
    visit(b->right);
    double right = res;

    switch (b->op)
    {
    case PLUS:  res = left + right; break;
    case MINUS:  res = left - right; break;
    case MULT:  res = left * right; break;
    case DIV:  res = left / right; break;
    case MORE:  res = left > right ? 1 : 0; break;
    case LESS:  res = left < right ? 1 : 0; break;
    case MOREEQ:  res = left >= right ? 1 : 0; break;
    case LESSEQ:  res = left <= right ? 1 : 0; break;
    case EQEQ:  res = left == right ? 1 : 0; break;
    case NOTEQ:  res = left != right ? 1 : 0; break;
    
    default: throw string(token_names[b->op] + " is not a valid binary operator");
        
    }
}

void Interpreter::visit_unary(AST_unary* u) 
{
    visit(u->right);
    if(u->op == MINUS) res = -res;
    else if(u->op == NOT) res = res == 0 ? 1 : 0;
    else throw string(token_names[u->op] + " is not a valid unary operator");
}

void Interpreter::visit_literal(AST_literal* l) 
{
    if(l->value[0] >= '0' && l->value[0] <= '9') res = stod(l->value);
    else res = scopes.back().get_variable(l->value);
}

void Interpreter::visit_asigment(AST_asigment* a) 
{
    visit(a->value);
    scopes.back().variables.insert({ a->name, res });
}

void Interpreter::visit_if_else(AST_if_else* ie)
{
    visit(ie->condition);

    if (res != 0) visit(ie->if_branch);
    else if(ie->else_branch) visit(ie->else_branch);
}

void Interpreter::visit_return(AST_return* r)
{
    visit(r->value);
    returning = true;
}


