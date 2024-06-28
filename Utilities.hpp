#include <string>
#include<vector>
#include "Token.cpp"
Token classifyToken(const Token& currentToken,Token lastKnownToken,Token nextToken);
    Lexer() : currentTokenIndex(0),currentState(LexerState::NormalState);
    Token readNextToken(istringstream& codeStream);
    Token peekNextToken(istringstream& codeStream);
    Token peekNextToken2(istringstream& codeStream);
    TokenType determineDefaultTokenType(const string& tokenValue)const;
    void InitializeKeywordMap();
    void InitializeSpecialCharacterMap();
void processComment(istringstream& codeStream, std::vector<Token>& tokens, bool isMultiLine);

bool isSpecialCharacter(char ch)const;

bool isWhiteSpace(char ch);

bool isEndOfLine(char c);

bool isNumeric(char c);

bool isLetter(char c);

 bool isValidIdentifierChar(char ch);

bool isValidIdentifier(std::string tokenValue)const;

std::string cheackNumberType(std::string tokenValue)const;

std::vector<Token> tokenize(const std::string& sourceCode) {


bool isValidFunctionReturnType(Token token);
}

bool isFunctionDeclaration(Token currentToken,Token lastKnownToken,Token nextToken);

bool isMemberDeclaration(Token currentToken,Token lastKnownToken,Token nextToken);

Token classifyToken(const Token& currentToken,Token lastKnownToken,Token nextToken);

Token GetValidLastKnownToken(const std::string& tokenValue);


