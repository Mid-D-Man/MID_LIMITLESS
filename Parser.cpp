#include "Parser.hpp"
#include "SubToken.hpp"
#include <iostream>

#pragma region Parsing Table
Parser::Parser() : parse_tree_root(nullptr), currentTokenIndex(0) {
    parse_table = {
        // Initial state transitions (0)
        {0, {{"int", "shift 1"}, {"bool", "shift 2"}, {"identifier", "shift 3"}, {"if", "shift 4"}}},
        // State 1: after shifting 'int'
        {1, {{"identifier", "shift 5"}}},
        // State 2: after shifting 'bool'
        {2, {{"identifier", "shift 5"}}},
        // State 3: after shifting 'identifier'
        {3, {{"=", "shift 6"}, {";", "reduce 1"}}},
        // State 4: after shifting 'if'
        {4, {{"(", "shift 7"}, {":", "shift 8"}, {"else", "shift 21"}, {"elseif", "shift 22"}}},
        // State 5: after shifting 'identifier' for declaration
        {5, {{"=", "shift 9"}, {";", "reduce 2"}}},
        // State 6: after shifting '=' in an assignment
        {6, {{"number", "shift 10"}, {"identifier", "shift 11"}}},
        // State 7: after shifting '(' in an if-statement
        {7, {{"identifier", "shift 12"}, {"number", "shift 13"}}},
        // State 8: after shifting ':' in an if-statement
        {8, {{"identifier", "shift 12"}, {"number", "shift 13"}}},
        // State 9: after shifting '=' in a declaration
        {9, {{"number", "shift 10"}, {"identifier", "shift 11"}}},
        // State 10: after shifting 'number' (for expression/assignment)
        {10, {{";", "reduce 3"}}},
        // State 11: after shifting 'identifier' (for expression/assignment)
        {11, {{";", "reduce 3"}}},
        // State 12: parsing expressions (identifiers)
        {12, {{"==", "shift 14"}, {"!=", "shift 15"}, {">", "shift 16"}, {"<", "shift 17"}, {"<=", "shift 18"}, {">=", "shift 19"}, {"+", "shift 20"}}},
        // State 13: parsing expressions (numbers)
        {13, {{"==", "shift 14"}, {"!=", "shift 15"}, {">", "shift 16"}, {"<", "shift 17"}, {"<=", "shift 18"}, {">=", "shift 19"}, {"+", "shift 20"}}},
        // State 14: after shifting '==' in an expression
        {14, {{"identifier", "shift 12"}, {"number", "shift 13"}}},
        // State 15: after shifting '!=' in an expression
        {15, {{"identifier", "shift 12"}, {"number", "shift 13"}}},
        // State 16: after shifting '>' in an expression
        {16, {{"identifier", "shift 12"}, {"number", "shift 13"}}},
        // State 17: after shifting '<' in an expression
        {17, {{"identifier", "shift 12"}, {"number", "shift 13"}}},
        // State 18: after shifting '<=' in an expression
        {18, {{"identifier", "shift 12"}, {"number", "shift 13"}}},
        // State 19: after shifting '>=' in an expression
        {19, {{"identifier", "shift 12"}, {"number", "shift 13"}}},
        // State 20: after shifting '+' in an expression
        {20, {{"identifier", "shift 12"}, {"number", "shift 13"}}},
        // Additional states for 'else' and 'elseif'
        {21, {{"if", "shift 4"}, {"{", "shift 23"}}},
        {22, {{"(", "shift 7"}, {":", "shift 8"}}},
        {23, {{"}", "shift 24"}}},
        {24, {{"", "reduce 4"}}},  // Reducing after block for 'else'
    };
}
#pragma endregion

#pragma region actual parseFuncion
void Parser::parse(const std::vector<Token>& tokens) {
    size_t index = 0;
    this->tokens = tokens;
    state_stack.push_back(0); // Start state
    while (index < tokens.size()) {
        if (is_lr_section(tokens[index])) {
            lr_parse(tokens, index);
        } else if (is_glr_section(tokens[index])) {
            glr_parse(tokens, index);
        } else {
            error("Unrecognized parsing section", tokens[index]);
        }
    }
}

#pragma endregion

#pragma region Parse Type Decition
// Decide if the current section is suitable for LR parsing
bool Parser::is_lr_section(const Token& token) {
    return token.tokenType == TokenType::Keyword || token.tokenType == TokenType::Identifier;
}

// Decide if the current section requires GLR parsing
bool Parser::is_glr_section(const Token& token) {
    // Implement logic based on your grammar rules
    // For example, if the token is an 'if', 'while', etc. that can use different syntactic forms:
    return token.subTokenInfo.subTokenType == SubTokenType::ControlStructure;
}
#pragma endregion

#pragma region Lr Parse
void Parser::lr_parse(const std::vector<Token>& tokens, size_t& index) {
    int current_state = state_stack.back();
    Token current_token = lookahead(0);
    std::string action = parse_table[current_state][current_token.tokenValue];
    
    if (action.find("shift") == 0) {
        shift(current_token);
        ++index;
    } else if (action.find("reduce") == 0) {
        int production = std::stoi(action.substr(7));
        reduce(production);
    } else {
        error("Unexpected token", current_token);
        if (can_recover(tokens, index)) {
            recover("Recovered from error", index, tokens);
        } else {
            unrecoverable_error("Unrecoverable error", current_token);
            ++index;
        }
    }
}
#pragma endregion

#pragma region Glr Parse

void Parser::glr_parse(const std::vector<Token>& tokens, size_t& index) {
    int current_state = state_stack.back();
    Token current_token = lookahead(0);

    if (current_token.tokenValue == "if") {
        std::unique_ptr<Node> if_node = std::make_unique<Node>("if_statement");
        consumeToken();  // Consume 'if'

        // Parse the condition
        if (lookahead(0).tokenValue == "(") {
            consumeToken();  // Consume '('
            if_node->children.push_back(parseExpression());
            consumeToken();  // Consume ')'
        } else if (lookahead(0).tokenValue == ":") {
            consumeToken();  // Consume ':'
            if_node->children.push_back(parseExpression());
            consumeToken();  // Consume ':'
        } else {
            error("Expected '(' or ':' after 'if'", current_token);
        }

        // Parse the block or single statement
        if (lookahead(0).tokenValue == "{") {
            consumeToken();  // Consume '{'
            while (lookahead(0).tokenValue != "}") {
                if_node->children.push_back(parseStatement());
                consumeToken();
            }
            consumeToken();  // Consume '}'
        } else {
            if_node->children.push_back(parseStatement());
        }

        // Handle 'else' and 'elseif'
        while (lookahead(0).tokenValue == "else" || lookahead(0).tokenValue == "elif") {
            if (lookahead(0).tokenValue == "else") {
                consumeToken();  // Consume 'else'
                std::unique_ptr<Node> else_node = std::make_unique<Node>("else_statement");
                if (lookahead(0).tokenValue == "{") {
                    consumeToken();  // Consume '{'
                    while (lookahead(0).tokenValue != "}") {
                        else_node->children.push_back(parseStatement());
                        consumeToken();
                    }
                    consumeToken();  // Consume '}'
                } else {
                    else_node->children.push_back(parseStatement());
                }
                if_node->children.push_back(std::move(else_node));
            } else if (lookahead(0).tokenValue == "elif") {
                consumeToken();  // Consume 'elseif'
                std::unique_ptr<Node> elseif_node = std::make_unique<Node>("elseif_statement");

                if (lookahead(0).tokenValue == "(") {
                    consumeToken();  // Consume '('
                    elseif_node->children.push_back(parseExpression());
                    consumeToken();  // Consume ')'
                } else if (lookahead(0).tokenValue == ":") {
                    consumeToken();  // Consume ':'
                    elseif_node->children.push_back(parseExpression());
                    consumeToken();  // Consume ':'
                } else {
                    error("Expected '(' or ':' after 'elseif'", current_token);
                }

                if (lookahead(0).tokenValue == "{") {
                    consumeToken();  // Consume '{'
                    while (lookahead(0).tokenValue != "}") {
                        elseif_node->children.push_back(parseStatement());
                        consumeToken();
                    }
                    consumeToken();  // Consume '}'
                } else {
                    elseif_node->children.push_back(parseStatement());
                }

                if_node->children.push_back(std::move(elseif_node));
            }
        }

        token_stack.push_back(std::move(if_node));
    } else {
        error("Unexpected token in GLR parse", current_token);
    }
}
#pragma endregion

#pragma region lookAheadBuffer
Token Parser::currentToken(){
    if(currentTokenIndex < tokens.size()){
        return tokens[currentTokenIndex];
    }else{
        return Token("end",TokenType::EndOfFile, SubTokenInfo(),TokenLocationInfo(-1,-1));
    }
}
//The consumeToken function is used to advance the parser to the next token, consuming the current token and returning it
Token Parser::consumeToken() {
    if (!lookahead_buffer.empty()) {
        Token token = lookahead_buffer.front();
        lookahead_buffer.erase(lookahead_buffer.begin());
        return token;
    } else if (currentTokenIndex < tokens.size()) {
        return tokens[currentTokenIndex++];
    } else {
        return Token("end",TokenType::EndOfFile, SubTokenInfo(),TokenLocationInfo(-1,-1));
    }
}

Token Parser::lookahead(size_t n) {
    while (lookahead_buffer.size() < n + 1 && currentTokenIndex < tokens.size()) {
        lookahead_buffer.push_back(tokens[currentTokenIndex++]);
    }
    if (n < lookahead_buffer.size()) {
        return lookahead_buffer[n];
    } else {
        return Token("end",TokenType::EndOfFile, SubTokenInfo(),TokenLocationInfo(-1,-1));
    }
}

#pragma endregion

#pragma region  Shif And Reducre

void Parser::shift(const Token& token) {
    token_stack.push_back(std::make_unique<Node>(token.tokenValue));
    state_stack.push_back(std::stoi(parse_table[state_stack.back()][token.tokenValue].substr(6)));
}
// Parsing a simple variable declaration (e.g., int x;)
void Parser::reduce(int production) {
    std::unique_ptr<Node> new_node = nullptr;
    switch (production) {
        case 1: // var_declaration -> type identifier ;
            new_node = std::make_unique<Node>("var_declaration");
            for (int i = 0; i < 3; ++i) {
                state_stack.pop_back();
                new_node->children.push_back(std::move(token_stack.back()));
                token_stack.pop_back();
            }
            token_stack.push_back(std::move(new_node));
            state_stack.push_back(std::stoi(parse_table[state_stack.back()]["var_declaration"]));
            if (!parse_tree_root) {
                parse_tree_root = token_stack.back().get();
            }
            break;
        case 2: // initialization -> identifier = expression ;
            new_node = std::make_unique<Node>("initialization");
            for (int i = 0; i < 4; ++i) {
                state_stack.pop_back();
                new_node->children.push_back(std::move(token_stack.back()));
                token_stack.pop_back();
            }
            token_stack.push_back(std::move(new_node));
            state_stack.push_back(std::stoi(parse_table[state_stack.back()]["initialization"]));
            if (!parse_tree_root) {
                parse_tree_root = token_stack.back().get();
            }
            break;
        // Handle more production rules as needed
    }
}

#pragma endregion

#pragma region  Parsing Specifics


std::unique_ptr<Node> Parser::parseExpression() {
    // Simplified version of parsing an expression
    Token token = consumeToken();
    return std::make_unique<Node>(token.tokenValue);
}

// Example parseStatement function (basic structure)
std::unique_ptr<Node> Parser::parseStatement() {
    Token token = currentToken();
    // Handling different statement types (simplified)
    if (token.tokenType == TokenType::Identifier) {
        return parseAssignment();
    } else if (token.tokenValue == "if") {
        return std::move(token_stack.back());  // Assuming the if statement is parsed here
    } else {
        error("Unexpected token in parseStatement", token);
        return nullptr;
    }
}

// Example parseAssignment function (basic structure)
std::unique_ptr<Node> Parser::parseAssignment() {
    // Parse an assignment statement
    Token identifier = consumeToken();  // Assume identifier
    consumeToken();  // Assume '='
    std::unique_ptr<Node> expr = parseExpression();
    consumeToken();  // Assume ';'

    std::unique_ptr<Node> assign_node = std::make_unique<Node>("assignment");
    assign_node->children.push_back(std::make_unique<Node>(identifier.tokenValue));
    assign_node->children.push_back(std::move(expr));
    return assign_node;
}

std::unique_ptr<Node> Parser::parseConditional() {
    std::unique_ptr<Node> conditional = std::make_unique<Node>("conditional");
    consumeToken(); // Consume 'if' or ':'
    conditional->children.push_back(parseExpression());

    // Handle different forms
    if (currentToken().tokenValue == "(") {
        // Handle parenthesis form
        consumeToken(); // Consume '('
        conditional->children.push_back(parseExpression());
        consumeToken(); // Consume ')'
    } else if (currentToken().tokenValue == ":") {
        // Handle colon form
        consumeToken(); // Consume ':'
        conditional->children.push_back(parseExpression());
        consumeToken(); // Consume ':'
    }

    return conditional;
}

std::unique_ptr<Node> Parser::parseFunctionCall() {
    std::unique_ptr<Node> func_call = std::make_unique<Node>("function_call");
    consumeToken(); // Consume '(' or ':'

    // Parse arguments
    while (currentToken().tokenValue != ")" && currentToken().tokenValue != ":") {
        func_call->children.push_back(parseExpression());
        if (currentToken().tokenValue == ",") {
            consumeToken(); // Consume ','
        }
    }

    consumeToken(); // Consume ')' or ':'
    return func_call;
}



#pragma endregion

#pragma region Error Handling
void Parser::error(const std::string& message, const Token& token) {
    std::cerr << "Parse error: " << message << " at token '" << token.tokenValue << "' on line " << token.locationInfo.lineNumber << std::endl;
}

bool Parser::can_recover(const std::vector<Token>& tokens, size_t index) {
    return tokens[index].tokenValue == ";" || tokens[index].tokenValue == "}";
}

void Parser::recover(const std::string& message, size_t& index, const std::vector<Token>& tokens) {
    std::cerr << "Recovery: " << message << std::endl;
    while (index < tokens.size() && tokens[index].tokenValue != ";" && tokens[index].tokenValue != "}") {
        ++index;
    }
    if (index < tokens.size()) {
        ++index; // Move past the recovery point
    }
}

void Parser::unrecoverable_error(const std::string& message, const Token& token) {
    std::cerr << "Unrecoverable parse error: " << message << " at token '" << token.tokenValue << "' on line " << token.locationInfo.lineNumber << ". Aborting parsing." << std::endl;
    // You might choose to throw an exception or set a flag to abort the parsing process
}
#pragma endregion

#pragma region  Printing
Node* Parser::getParseTreeRoot(){
    return parse_tree_root;
}

void Parser::print_tree(Node* root, size_t depth) {
    if (!root) return;
    for (size_t i = 0; i < depth; ++i) {
        std::cout << "  ";
    }
    std::cout << root->value << std::endl;
    for (const auto& child : root->children) {
        print_tree(child.get(), depth + 1);
    }
}
#pragma endregion

//stuff not yet done
/*
void Parser::parseImport() {
    Token importToken = consumeToken(); // Consume 'import'
    Token fileToken = consumeToken(); // Consume the file name
    importScript(fileToken.tokenValue);
}

void Parser::importScript(const std::string& filename) {
    // Read the file and tokenize it
    std::vector<Token> importedTokens = lexer.tokenize(readFile(filename));
    // Parse the imported tokens
    parse(importedTokens);
}
*/

