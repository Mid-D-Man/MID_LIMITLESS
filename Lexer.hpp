#pragma once
#include <string>
#include <vector>
#include "TrieNode.hpp"
#include "StateMachine.hpp"
#include "Identifier.hpp"

class Lexer {
TokenTrie trie;
 //stores the current state of the lexer
 LexerState currentState;
 //stores the symbol table for identifiers
 SymbolTable symbols;
 //stores known important keyword tokens
std::unordered_map<std::string, Token> keywordMap;
//stores important character tokens
std::unordered_map<char, Token> specialCharacterMap;
//this is for handling processed tokens
std::vector<Token> tokens;
//keeps track of current token
size_t currentTokenIndex;

   public : Lexer() ;

Token readNextToken(istringstream& codeStream);
Token peekNextToken(istringstream& codeStream);
Token peekNextToken2(istringstream& codeStream);

TokenType determineDefaultTokenType(const std::string& tokenValue)const;

void InitializeKeywordMap();

void InitializeSpecialCharacterMap();

void processComment(istringstream& codeStream, std::vector<Token>& tokens, bool isMultiLine);

bool isSpecialCharacter(char ch)const;

bool isWhiteSpace(char ch);

bool isEndOfLine(char c);


 bool isValidIdentifierChar(char ch);

bool isValidIdentifier(std::string tokenValue)const;

std::string cheackNumberType(std::string tokenValue)const;


bool isNumeric(char c);

bool isLetter(char c);


public : std::vector<Token> tokenize(const std::string& sourceCode);
public :Token classifyToken(const Token& currentToken,Token lastKnownToken,Token nextToken);

bool isValidFunctionReturnType(Token token);

bool isFunctionDeclaration(Token currentToken,Token lastKnownToken,Token nextToken);

bool isMemberDeclaration(Token currentToken,Token lastKnownToken,Token nextToken);
Token getValidLastKnownToken(const std::string& tokenValue);

 void handleError();

//end of lexer class
};



