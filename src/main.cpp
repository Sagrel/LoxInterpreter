#include <iostream>
#include <fstream>

#include "AST_Printer.h"
#include "Interpreter.h"
#include "Parser.h"

int main()
{
    ifstream in("src/source.lox");

    string source((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());

    try 
    {
        Tokenizer tokenizer(source);
        
        /*
        cout << "Number of tokens: " << tokenizer.tokens.size() << endl;
        for (auto e : tokenizer.tokens)
        {
            cout << e << endl;
        }
        */
        Parser parser(tokenizer.tokens);
        
        //AST_Printer printer;

        //printer.visit(parser.root);

        Interpreter interpreter;

        interpreter.visit(parser.root);

        cout << interpreter.res << endl;
        
        return 0;
    }
    catch (string e)
    {
        cerr << e;
        return -1;
    }
    catch(...)
    {
        cerr << "error no controlado";
    }
    
}