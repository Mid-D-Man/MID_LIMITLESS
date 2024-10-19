#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include "Parser.hpp"


Node* Parser::parse(std::vector<Token>& tokens) {
    Node* root = nullptr;
    size_t index = 0;
    while (index < tokens.size() && tokens[index].tokenType != TokenType::EndOfFile) {
        Node* statement = parse_statement(tokens, index);
        if (root == nullptr) {
            root = statement;
        } else {
            Node* current = root;
            while (current->right != nullptr) {
                current = current->right;
            }
            current->right = statement;
        }
    }
    return root;
}

Node* Parser::parse_statement(std::vector<Token>& tokens, size_t& index) {

    if(tokens[index].tokenType == TokenType::Identifier){
 if (tokens[index].tokenValue == "if") {
        // Implement if statement parsing
    } else if (tokens[index].tokenValue == "while") {
        // Implement while statement parsing
    } else {
        report_error("Unexpected token", tokens[index]);
        recover_from_error(tokens, index);
        return nullptr;
    }
}
    
    return nullptr;
}


Node* Parser::parse_expression(std::vector<Token>& tokens, size_t& index) {
    Node* node = parse_term(tokens, index);
    while (index < tokens.size() && tokens[index].tokenType == TokenType::Operator && (tokens[index].subTokenInfo.subTokenType == SubTokenType::Addition || tokens[index].subTokenInfo.subTokenType  == SubTokenType::Subtract)) {
        Node* new_node = new Node{tokens[index].tokenValue, node, nullptr};
        ++index;
        new_node->right = parse_term(tokens, index);
        node = new_node;
    }
    return node;
}

Node* Parser::parse_term(std::vector<Token>& tokens, size_t& index) {
    Node* node = parse_factor(tokens, index);
    while (index < tokens.size() && tokens[index].tokenType == TokenType::Operator && (tokens[index].tokenValue == "*" || tokens[index].tokenValue == "/")) {
        Node* new_node = new Node{tokens[index].tokenValue, node, nullptr};
        ++index;
        new_node->right = parse_factor(tokens, index);
        node = new_node;
    }
    return node;
}

Node* Parser::parse_factor(std::vector<Token>& tokens, size_t& index) {
    Node* node = nullptr;
    if (is_NumberToken(tokens[index]) || tokens[index].tokenType == TokenType::Identifier) {
        node = new Node{tokens[index].tokenValue, nullptr, nullptr};
        ++index;
    } else if (tokens[index].tokenType == TokenType::Operator && tokens[index].tokenValue == "(") {
        ++index;
        node = parse_expression(tokens, index);
        if (tokens[index].tokenType == TokenType::Operator && tokens[index].tokenValue == ")") {
            ++index;
        } else {
            report_error("Expected closing parenthesis", tokens[index]);
            recover_from_error(tokens, index);
        }
        
    } else {
        report_error("Unexpected token", tokens[index]);
        recover_from_error(tokens, index);
    }
    return node;
}


bool Parser::is_NumberToken(Token token){
    return token.subTokenInfo.subTokenType == SubTokenType::Literal_Integer |    token.subTokenInfo.subTokenType == SubTokenType::Literal_Float |   token.subTokenInfo.subTokenType == SubTokenType::Literal_Double;

}


void Parser::report_error(const std::string& message, const Token& token) {
    std::cerr << "Syntax error: " << message << " at token '" << token.tokenValue << "'\n";
}

void Parser::recover_from_error(std::vector<Token>& tokens, size_t& index) {
    while (index < tokens.size() && tokens[index].tokenType != TokenType::EndOfFile && tokens[index].tokenValue != ";" && tokens[index].tokenValue != "}") {
        ++index;
    }
    if (index < tokens.size()) {
        ++index;
    }
}