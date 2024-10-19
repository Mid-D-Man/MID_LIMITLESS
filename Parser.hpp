#ifndef PARSER_HPP
#define PARSER_HPP

#include <vector>
#include "Lexer.hpp"
#include <vector>
#include <string>

struct Node {
    std::string value;
    Node* left;
    Node* right;
};

class Parser{
public :Parser();
Node* parse(std::vector<Token>& tokens);
Node* parse_expression(std::vector<Token>& tokens, size_t& index);

Node* parse_term(std::vector<Token>& tokens, size_t& index);

Node* parse_factor(std::vector<Token>& tokens, size_t& index);

Node* parse_statement(std::vector<Token>& tokens, size_t& index);
void report_error(const std::string& message, const Token& token);
void recover_from_error(std::vector<Token>& tokens, size_t& index);
bool is_NumberToken(Token token);
};

#endif