
#pragma once
#include<string>
#include<vector>
#include<unordered_map>
#include "Token.hpp"
// Assume Token, TokentokenType, and SubTokenInfo are defined as provided earlier
using namespace std;
class TrieNode {
  public :  std::unordered_map<char, std::shared_ptr<TrieNode> > children;
  public :  bool isEndOfToken;
    public :Token token; // Store the token at the end node

    TrieNode();
};

class TokenTrie {
private:
    std::shared_ptr<TrieNode> root;
    std::vector<Token> tokens; // Store tokens in order of insertion

public:
    TokenTrie();

    void insert(const Token& token);

    Token getLastToken() const ;

    Token getFirstToken() const;

    bool deleteToken(const std::string &tokentokenValue);
    std::vector<Token> getAllTokens()const;
    void clearAllTokens() ;

    void reinitializeTrie();

    // Add more methods as needed...
};


