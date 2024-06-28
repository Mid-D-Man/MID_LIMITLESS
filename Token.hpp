
#pragma once
#include <string>
#include "SubToken.hpp"



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
        UserDefinedToken
    };


struct Token{
    public:
    std::string tokenValue;
    TokenType tokenType;
    SubTokenInfo subTokenInfo;
//default constructor
Token(): tokenValue("Null_Undefined"),tokenType(TokenType::Null_Undefined),subTokenInfo(SubTokenInfo()){}
//constructor that takes arguments
    Token(std::string val, TokenType typ, SubTokenInfo subInfo) :
    tokenValue(val),tokenType(typ),subTokenInfo(subInfo){}

    bool isValid()const{
        return !tokenValue.empty() && tokenType != TokenType::Null_Undefined;
    }
};

const char* TokenTypeToString(TokenType tokenType);
