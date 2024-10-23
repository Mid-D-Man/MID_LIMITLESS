#pragma once
#include <string>
#include <vector>
#include <stack>
#include "TrieNode.hpp"
#include "StateMachine.hpp"
#include "Identifier.hpp"

//note we moving some of the functions we gonna use in other files to our utilities class

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
public : std::stack<LexerStateStack> lexerStateStack;
//we may require a stack of lexerstate,string to keep track of what lead to the state changing
public : Token readNextToken(istringstream& codeStream);
public : Token peekNextToken(istringstream& codeStream);
public : Token peekNextToken2(istringstream& codeStream);

private:bool lenientMode;

public:size_t currentLineNumber;
public:size_t currentColumnNumber;


public : Token determineDefaultTokenType(std::string& tokenValue)const;

public : void InitializeKeywordMap();

public : void InitializeSpecialCharacterMap();

public : Token processComment(istringstream& codeStream, bool isMultiLine);

public : Token processStringLiteral(istringstream& codeStream,char& quoteType);

public : bool isSpecialCharacter(char ch)const;
public : bool isCombinableSpecialCharacter(char ch)const;

public : bool isKeyword(std::string& tokenValue)const;

//moved

public : std::vector<Token> tokenize(const std::string& sourceCode);
public :Token classifyToken(Token& currentToken,Token& lastKnownToken,Token& nextToken);

public : Token tryGetDoubleCombinableToken(Token firstToken,Token secondToken);


public : bool isValidFunctionReturnType(Token token);



public : bool isMemberDeclaration(Token currentToken,Token lastKnownToken,Token nextToken);
public : void handleError();

public : bool isPredefinedReturnType(std::string& tokenVal);
//end of lexer class
public : LexerStateStack getValidLastKnownLexerState(std::stack<LexerStateStack>& lexerStateStack);
public : LexerStateStack getValidLastKnownLexerStateDoublePop(std::stack<LexerStateStack>& lexerStateStack);

public : std::vector<LexerStateStack> returnAllLexerStatesInAccendingOrder(std::stack<LexerStateStack>& lexerStateStack);

};






