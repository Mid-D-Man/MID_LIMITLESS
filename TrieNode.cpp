
#include<string>
#include<vector>
#include "TrieNode.hpp"
#include<unordered_map>

#pragma region MidTierCollector

// Assume Token, TokentokenType, and SubTokenInfo are defined as provided earlier

TrieNode::TrieNode() {

     isEndOfToken = false;

};


 TokenTrie::TokenTrie(){
root = std::make_shared<TrieNode>();

};

void TokenTrie::insert(const Token& token) {
        auto current = root;
        for (char ch : token.tokenValue) {
            if (current->children.find(ch) == current->children.end()) {
                current->children[ch] = std::make_shared<TrieNode>();
            }
            current = current->children[ch];
        }
        current->isEndOfToken = true;
        current->token = token;
        tokens.push_back(token);
    }

    Token TokenTrie::getLastToken() const {
        if (!tokens.empty()) {
            return tokens.back();
        }
        // Return a default Token if there are no tokens
        return Token("", TokenType::Identifier, SubTokenInfo());
    }

    Token TokenTrie::getFirstToken() const {
        if (!tokens.empty()) {
            return tokens.front();
        }
        // Return a default Token if there are no tokens
        return Token("", TokenType::Identifier, SubTokenInfo());
    }

    bool TokenTrie::deleteToken(const std::string &tokentokenValue) {
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

    void TokenTrie::reinitializeTrie() {
        root = std::make_shared<TrieNode>(); // Reset the root of the trie
        tokens.clear(); // Clear the tokens vector
    }


#pragma endregion

