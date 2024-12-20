#include "Scanner.h"
#include <cstring>

Scanner::Scanner(const char *it_s) : input(it_s), first(0), current(0) {}

bool is_white_space(char c) {
    return c == ' ' || c == '\n' || c == '\t' || c == '\r';
}

Token *Scanner::NextToken() {

    Token* token;
    while (current < input.length() && is_white_space(input[current])) current += 1;
    if (current >= input.length()) return new Token(Token::END);

    char c = input[current];
    first = current;

    if (isdigit(c)) {

        while (current < input.length() && isdigit(input[current])) ++current;

        token = new Token(Token::NUM, input, first, current - first);

    } else if (isalpha(c)) {

        while (current < input.length() && isalnum(input[current])) ++current;

        string word = input.substr(first, current - first);

        // Palabras clave añadidas
        if (word == "writeln") token = new Token(Token::WriteLn, word, 0, word.length());
        else if (word == "write") token = new Token(Token::Write, word, 0, word.length());
        else if (word == "if") token = new Token(Token::IF, word, 0, word.length());
        else if (word == "then") token = new Token(Token::THEN, word, 0, word.length());
        else if (word == "else") token = new Token(Token::ELSE, word, 0, word.length());
        else if (word == "begin") token = new Token(Token::BEGIN, word, 0, word.length());
        else if (word == "end") token = new Token(Token::ENDP, word, 0, word.length());
        else if (word == "for") token = new Token(Token::FOR, word, 0, word.length());
        else if (word == "to") token = new Token(Token::TO, word, 0, word.length());
        else if (word == "downto") token = new Token(Token::DOWNTO, word, 0, word.length());
        else if (word == "do") token = new Token(Token::DO, word, 0, word.length());
        else if (word == "while") token = new Token(Token::WHILE, word, 0, word.length());
        else if(word == "True" || word == "true") token = new Token(Token::TRUE, word, 0, word.length());
        else if(word == "False" || word == "false") token = new Token(Token::FALSE, word, 0, word.length());

            // Nuevas palabras clave para declaración de variables y funciones
        else if (word == "var") token = new Token(Token::VAR, word, 0, word.length());
        else if (word == "function") token = new Token(Token::FUNCTION, word, 0, word.length());
        else if (word == "integer") token = new Token(Token::INTEGER, word, 0, word.length());
        else if (word == "Integer") token = new Token(Token::INTEGER, word, 0, word.length());
        else if (word == "longint") token = new Token(Token::LONGINT, word, 0, word.length());
        else if (word == "Longint") token = new Token(Token::LONGINT, word, 0, word.length());
        else if (word == "boolean") token = new Token(Token::BOOLEAN, word, 0, word.length());
        else if (word == "Boolean") token = new Token(Token::BOOLEAN, word, 0, word.length());
        else if(word == "program") token = new Token(Token::PROGRAM, word, 0, word.length());
        else if(word == "procedure") token = new Token(Token::PROCEDURE, word, 0, word.length());
        else if(word == "repeat") token = new Token(Token::REPEAT, word, 0, word.length());
        else if(word == "until") token = new Token(Token::UNTIL, word, 0, word.length());


        else token = new Token(Token::ID, word, 0, word.length());

    } else if (strchr("+-/*():=;<>,.", c)) {
        switch (c) {
            case '+': token = new Token(Token::PLUS, c); break;
            case '-': token = new Token(Token::MINUS, c); break;
            case '*': token = new Token(Token::MUL, c); break;
            case '/': token = new Token(Token::DIV, c); break;
            case '(': token = new Token(Token::PI, c); break;
            case ')': token = new Token(Token::PD, c); break;
            case ':': {
                if (current + 1 < input.length() && input[current + 1] == '=') {
                    ++current;
                    token = new Token(Token::ASSIGN, input, first, current - first);
                } else token = new Token(Token::TypeDCL, c);
                break;
            }
            case '=': token = new Token(Token::EQ, c); break;
            case ';': token = new Token(Token::PC, c); break;
            case ',': token = new Token(Token::COMA, c); break;
            case '.': token = new Token(Token::PUNTO, c); break;
            case '>': {
                if (current + 1 < input.length() && input[current + 1] == '=') {
                    ++current;
                    token = new Token(Token::DE, input, first, current - first);
                } else token = new Token(Token::DT, c);
                break;
            }
            case '<': {
                if (current + 1 < input.length() && input[current + 1] == '=') {
                    ++current;
                    token = new Token(Token::LE, input, first, current - first);
                }
                else if ( current + 1 < input.length() && input[current + 1] == '>' ){
                    ++current;
                    token = new Token(Token::DIFERENTES, input, first, current - first);
                }
                else token = new Token(Token::LT, c);
                break;
            }
            default:
                token = new Token(Token::ERR, c);
                cout << "No debería poder llegar acá";
                break;
        }
        ++current;
    } else {
        token = new Token(Token::ERR, c);
        ++current;
    }

    return token;
}

void Scanner::reset() {
    first = 0;
    current = 0;
}

Scanner::~Scanner() = default;

void test_scanner(Scanner* scanner) {
    Token* current;
    cout << "Iniciando Scanner:" << endl << endl;
    while ((current = scanner->NextToken())->type != Token::END) {
        if (current->type == Token::ERR) {
            cout << "Error en scanner - carácter inválido: " << current->TypeText << endl;
            break;
        } else {
            cout << *current << endl;
        }
        delete current;
    }
    cout << "TOKEN(END)" << endl;
    delete current;
}

