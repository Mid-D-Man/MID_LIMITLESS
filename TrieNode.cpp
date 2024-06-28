#include<string>
#include<vector>
#include "TrieNode.hpp"
#include<unordered_map>
#pragma region MidTierCollector
using namespace std;
// Assume Token, TokentokenType, and SubTokenInfo are defined as provided earlier

  TrieNode::TrieNode(){

    unordered_map<char, shared_ptr<TrieNode> > children;
    bool isEndOfToken = false;
    Token token; // Store the token at the end node

};

 TokenTrie::TokenTrie (){

 shared_ptr<TrieNode> root(make_shared<TrieNode>());
  vector<Token> tokens; // Store tokens in order of insertion


};

    void TokenTrie::insert(const Token& token) {
        auto current = root;
        for (char ch : token.tokenValue) {
            if (current->children.find(ch) == current->children.end()) {
                current->children[ch] = make_shared<TrieNode>();
            }
            current = current->children[ch];
        }
        current->isEndOfToken = true;
        current->token = token;
        tokens.push_back(token);
    }

    Token  TokenTrie::getLastToken() const {
        if (!tokens.empty()) {
            return tokens.back();
        }
        // Return a default Token if there are no tokens
        return Token("", TokenType::Identifier, SubTokenInfo());
    }

    Token  TokenTrie::getFirstToken() const {
        if (!tokens.empty()) {
            return tokens.front();
        }
        // Return a default Token if there are no tokens
        return Token("", TokenType::Identifier, SubTokenInfo());
    }

    bool  TokenTrie::deleteToken(const string &tokentokenValue) {
        for (auto it = tokens.begin(); it != tokens.end(); ++it) {
            if (it->tokenValue == tokentokenValue) {
                tokens.erase(it);
                return true;
            }
        }
        return false; // Token not found
    }

    vector<Token> TokenTrie::getAllTokens()const{
        return tokens;
    }

    void TokenTrie::clearAllTokens() {
        tokens.clear();
    }

    void  TokenTrie::reinitializeTrie() {
        root = make_shared<TrieNode>(); // Reset the root of the trie
        tokens.clear(); // Clear the tokens vector
    }

    // Add more methods as needed...


#pragma endregion
