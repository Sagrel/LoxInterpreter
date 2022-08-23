#pragma once

#include "AST.h"

struct AST_Printer : Visitor
{
    int tabs = 0;

    void new_line();

    void visit_scope(AST_scope*) override;
    void visit_function(AST_function*) override;
    void visit_invocation(AST_invocation*) override;
    void visit_binary(AST_binary*) override;
    void visit_unary(AST_unary*) override;
    void visit_literal(AST_literal*) override;
    void visit_asigment(AST_asigment*) override;
};