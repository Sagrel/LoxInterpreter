#include "AST_Printer.h"

void AST_Printer::new_line()
{
    cout << endl;
    for(int i = 0; i < tabs; i++)   cout << "\t";
}
void AST_Printer::visit_scope(AST_scope* s) 
{
    new_line();
    tabs++;
    cout << "{";
    for(AST_node* node : s->statements)
    {
        new_line();
        visit(node);
        cout << ";";
    }
    tabs--;
    new_line();
    cout << "}";    
}

void AST_Printer::visit_function(AST_function* f) 
{
    cout << "function " << f->name << "(";
    for(string& node : f->args)
    {        
        cout << node << ", ";
    }
    cout << ")";
    visit(f->body);
}

void AST_Printer::visit_invocation(AST_invocation* i) 
{
    cout << i->name<< "(";
    for(AST_node* node : i->args)
    {
        visit(node);
        cout << ", ";
    }
    cout << ")";
}

void AST_Printer::visit_binary(AST_binary* b) 
{
    cout << "(";
    visit(b->left);
    cout << " " << token_names[b->op] << " ";
    visit(b->right);
    cout << ")";
}

void AST_Printer::visit_unary(AST_unary* u) 
{
    cout << "(" << token_names[u->op] << " ";
    visit(u->right);
    cout << ")";
}

void AST_Printer::visit_literal(AST_literal* l) 
{
    cout << l->value;
}

void AST_Printer::visit_asigment(AST_asigment* a) 
{
    cout << a->name << " = ";
    visit(a->value);
}
