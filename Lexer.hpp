#pragma once
#include <string>
#include <vector>
#include <stack>
#include "TrieNode.hpp"
#include "StateMachine.hpp"
#include "Identifier.hpp"


class Lexer {
public : Lexer();

public : TokenTrie trie;
 //stores the current state of the lexer
 public : LexerState currentState;
 //stores the symbol table for identifiers
 public : SymbolTable symbols;
 //stores known important keyword tokens
public : std::unordered_map<std::string, Token> keywordMap;
//stores important character tokens
public : std::unordered_map<char, Token> specialCharacterMap;
//this is for handling processed tokens
public : std::vector<Token> tokens;
//keeps track of current token
public : size_t currentTokenIndex;

public : std::stack<char> encloseSymbolsStack;

public : Token readNextToken(istringstream& codeStream);
public : Token peekNextToken(istringstream& codeStream);
public : Token peekNextToken2(istringstream& codeStream);

public : TokenType determineDefaultTokenType(const std::string& tokenValue)const;

public : void InitializeKeywordMap();

public : void InitializeSpecialCharacterMap();

public : void processComment(istringstream& codeStream, std::vector<Token>& tokens, bool isMultiLine);

public : bool isSpecialCharacter(char ch)const;

public : bool isWhiteSpace(char ch);

public : bool isEndOfLine(char c);


 public : bool isValidIdentifierChar(char ch);

public : bool isValidIdentifier(std::string tokenValue)const;

public : std::string cheackNumberType(std::string tokenValue)const;


public : bool isNumeric(char c);

public : bool isLetter(char c);


public : std::vector<Token> tokenize(const std::string& sourceCode);
public :Token classifyToken(const Token& currentToken,Token lastKnownToken,Token nextToken);

public : bool isValidFunctionReturnType(Token token);


public : bool isFunctionDeclaration(Token currentToken,Token lastKnownToken,Token nextToken);

public : bool isMemberDeclaration(Token currentToken,Token lastKnownToken,Token nextToken);
public : Token getValidLastKnownToken(const std::string& tokenValue);

public : void handleError();

//end of lexer class
};



