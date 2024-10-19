
#pragma once
#include <string>
#include "SubToken.hpp"
//all our token types
    enum TokenType{
        Null_Undefined,
        Keyword,
        Identifier,
        Literial,
        Operator,
        OperatorWithSpecialMeaning,
        Punctuation,Seprator,
        AccessModifier,
        WhiteSpace,
        Comment,
        PreprocessorDirectory,
        SpecialSymbol,
        EscapeSequence,
        Annotation_Attribute,

        Function_MethodCall,
        UserDefinedToken,
        Error,
        EndOfFile
    };

struct TokenLocationInfo{
    public:
     size_t lineNumber;
    size_t columnNumber;

TokenLocationInfo(): lineNumber(1),columnNumber(1){}
TokenLocationInfo(size_t lineNum,size_t columnNum): lineNumber(lineNum),columnNumber(columnNum){}
};

struct Token{
    public:
    std::string tokenValue;
    TokenType tokenType;
    SubTokenInfo subTokenInfo;
    TokenLocationInfo locationInfo;
//default constructor
Token(): tokenValue("Null_Undefined"),tokenType(TokenType::Null_Undefined),subTokenInfo(SubTokenInfo()),locationInfo(TokenLocationInfo()){}
//constructor that takes arguments
    Token(std::string val, TokenType typ, SubTokenInfo subInfo,TokenLocationInfo locInfo) :
    tokenValue(val),tokenType(typ),subTokenInfo(subInfo),locationInfo(locInfo){}

    bool isValid()const{
        return !tokenValue.empty() && tokenType != TokenType::Null_Undefined;
    };

};



const char* TokenTypeToString(TokenType tokenType);
