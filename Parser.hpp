#ifndef PARSER_HPP
#define PARSER_HPP

#include <vector>
#include <string>
#include "Lexer.hpp"

class Parser{
    private:
    Lexer lexer;
    std::vector<Token> tokens;
    size_t currentTokenIndex;

    public:
    Parser(Lexer lexer);
    void parse();
private:
void parserIdentifier();
bool isValidIdentifier(const string& identifier);
void parseProgram();
void parseStatement();
void parseDeclaration();
void parseIfStatement();
void parseWhileStatement();
void parseForStatement();
void error(const std::string& message);
};

#endif //PARSE_HPP
