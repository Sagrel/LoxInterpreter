#include "Tokenizer.h"

map<string, token_type> key_words = {{"function", FUNCTION}, {"if", IF}, {"else", ELSE}, {"return", RETURN} };

ostream& operator<<(ostream& os, const Token& t)
{  
    os << "(" << t.linea << ":" << t.columna << ") " << token_names[t.tipo];
    if (t.tipo == INT) os << ": " << t.valor; 
    return os;
}

bool is_number(char caracter)
{
    return (caracter >= '0' && caracter <= '9');
}

void Tokenizer::new_token(token_type tt)
{
    tokens.push_back({tt, linea, columna});
}

void Tokenizer::new_token(string s)
{
    if(isalpha(s[0])) 
    {
        auto i = key_words.find(s);
        if(i != key_words.end()) tokens.push_back({i->second, linea, columna, s});
        else tokens.push_back({IDENTIFIER, linea, columna, s});
    }
    else tokens.push_back({INT, linea, columna, s});
}

char Tokenizer::next()
{
    if((idx + 1) < source.length()) return source[idx + 1];
    return '\0';
}

void Tokenizer::advance()
{
    idx++;
    columna++;
    if(idx < source.length()) caracter = source[idx];
    else caracter = '\0';
}

void Tokenizer::tokenize()
{
    caracter = source[idx];
    while(idx < source.length())
    {
        switch (caracter)
        {
        case '+': new_token(PLUS);      break;
        case '-': new_token(MINUS);     break;
        case '*': new_token(MULT);      break;
        case '/': new_token(DIV);       break;
        case ',': new_token(COMMA);     break;
        case ';': new_token(SEMICOLON); break;
        case '(': new_token(OPEN_P);    break;
        case ')': new_token(CLOSE_P);   break;
        case '{': new_token(OPEN_B);   break;
        case '}': new_token(CLOSE_B);   break;
        case '<': if(next() == '=') { new_token(LESSEQ); advance(); } else new_token(LESS);   break;
        case '>': if (next() == '=') { new_token(MOREEQ); advance();} else new_token(MORE);   break;
        case '=': if (next() == '=') { new_token(EQEQ); advance(); } else new_token(EQ);   break;
        case '!': if(next() == '=') { new_token(NOTEQ); advance(); } else new_token(NOTEQ);   break;
        case ' ': break;
        case '\n': linea++; columna = 0;break;
        default:
            if(is_number(caracter)) 
            {
                int start = idx;
                while(is_number(next())) advance();
                new_token(source.substr(start, (idx + 1) - start));
            }
            else if(isalpha(caracter))
            {
                int start = idx;
                while(isalnum(next()) || next() == '_') advance();
                new_token(source.substr(start, (idx + 1) - start));
            }
            else
            {
                stringstream ss;
                ss << "Caracter no valido: " << caracter << " En: (" << linea << ", " << columna << ")";
                throw ss.str();
            }
            break;
        }

        advance();
    }
}