#include "Scanner.h"


Scanner::Scanner(const char *it_s) : input(it_s), first(0), current(0){}

bool is_white_space(char c){
    return c == ' ' || c == '\n' || c == '\t' || c == '\r';
}

Token *Scanner::NextToken() {

    Token* token;
    while (current < input.length() && is_white_space(input[current])) current+=1;
    if(current >= input.length()) return new Token(Token::END);

    char c = input[current];
    first = current;

    if(isdigit(c)){

        while (current < input.length() && isdigit(input[current])) ++current;

        token = new Token(Token::NUM, input, first, current-first);

    }
    else if(isalpha(c)){

        while (current < input.length() && isalpha(input[current])) ++current;

        string word = input.substr(first, current-first);

        if(word == "WriteLn") token = new Token(Token::WriteLn, word, 0, word.length());
        else if(word == "Write") token = new Token(Token::Write, word, 0, word.length());
        else token = new Token(Token::ID, word, 0, word.length());

    }
    else if(strchr("+-/*():=;<>", c)){
        switch (c) {
            case '+': token = new Token(Token::PLUS, c); break;
            case '-': token = new Token(Token::MINUS, c); break;
            case '*': token = new Token(Token::MUL, c); break;
            case '/': token = new Token(Token::DIV, c); break;
            case '(': token = new Token(Token::PI, c); break;
            case ')': token = new Token(Token::PD, c); break;
            case ':': {
                if(current + 1 < input.length()  && input[current+1] == '='){
                    ++current;
                    token = new Token(Token::ASSIGN, input, first, current-first);
                }
                else token = new Token(Token::TypeDCL, c);
                break;
            }
            case '=': token = new Token(Token::EQ, c); break;
            case ';': token = new Token(Token::PC, c); break;
            case '>': {
                if(current + 1 < input.length() && input[current+1] == '='){
                    ++current;
                    token = new Token(Token::DT, input, first, current-first);
                }
                else token = new Token(Token::DE, c);
                break;
            }
            case '<': {
                if(current + 1 < input.length() && input[current+1] == '='){
                    ++current;
                    token = new Token(Token::LT, input, first, current-first);
                }
                else token = new Token(Token::LE, c);
                break;
            }
            default :
                token = new Token(Token::ERR, c);
                cout<< "No debería poder llegar acá";
                break;
        }
        ++current;
    }
    else{
        token = new Token(Token::ERR, c);
        ++current;
    }

    return token;

}

void Scanner::reset() {
    first = 0;
    current = 0;
}

Scanner::~Scanner() {}

void test_scanner(Scanner* scanner) {
    Token* current;
    cout << "Iniciando Scanner:" << endl<< endl;
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
