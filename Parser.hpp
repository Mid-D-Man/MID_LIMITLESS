#ifndef PARSER_HPP
#define PARSER_HPP

#include <vector>
#include <string>
#include <unordered_map>
#include <memory>
#include "Lexer.hpp"

//the node tha represents a leaf in the parsers ast and its childeren
struct Node {
    std::string value;
    std::vector<std::unique_ptr<Node>> children;

    Node(std::string val) : value(val) {}
};
//pars constructor
class Parser {
public:
    Parser();
    void parse(const std::vector<Token>& tokens);
//regular funcs
private:
    void lr_parse(const std::vector<Token>& tokens, size_t& index);
    void glr_parse(const std::vector<Token>& tokens, size_t& index);
    void shift(const Token& token);
    void reduce(int production);
    //error handling
    void error(const std::string& message, const Token& token);
    void recover(const std::string& message, size_t& index, const std::vector<Token>& tokens);
    bool can_recover(const std::vector<Token>& tokens, size_t index);
    void unrecoverable_error(const std::string& message, const Token& token);
    //parser tree printing
   public : void print_tree(Node* root, size_t depth);
   public : Node* getParseTreeRoot();;
    bool is_lr_section(const Token& token);
    bool is_glr_section(const Token& token);

     //parsing specific stuff
    std::unique_ptr<Node> parseExpression();
    std::unique_ptr<Node> parseStatement();
    std::unique_ptr<Node> parseAssignment();
    std::unique_ptr<Node> parseConditional();
    std::unique_ptr<Node> parseFunctionCall();
    //handling importing
    void parseImport();
    void importScript(const std::string& filename);
//tokens stack
    std::vector<std::unique_ptr<Node>> token_stack;
    std::vector<int> state_stack;
    Node* parse_tree_root;
//parse table
    std::unordered_map<int, std::unordered_map<std::string, std::string>> parse_table;

    // Lookahead buffer
    std::vector<Token> lookahead_buffer;
    Token lookahead(size_t n);
    Token consumeToken();
    
    //tokens keeper
    Token currentToken();
    std::vector<Token> tokens;
    size_t currentTokenIndex;
};

#endif // PARSER_HPP
