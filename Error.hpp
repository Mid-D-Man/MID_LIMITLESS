#pragma once
#include<string>
#include "Token.hpp"


enum class ErrorType{
    SyntaxError,
    LexicalError,
    SematicError,
    UnexpectedCharacter,
    InvalidToken,
    UnknownError,
    JustBecauseWeFeltLikeItError
};

 struct ErrorInfo{
    ErrorType errorType;
    int errorCode;
    std::string message;
    size_t lineNumber;
    size_t columnNumber;
ErrorInfo():errorType(ErrorType::UnknownError),errorCode(0000), message("Unknown Error"),lineNumber(-1),columnNumber(-1){}
//constructor that takes arguments
    ErrorInfo(ErrorType errType,int errCode,std::string msg, size_t lineNum,size_t columnNum) :
    errorType(errType),errorCode(errCode),message(msg),lineNumber(lineNum),columnNumber(columnNum){}
 };

 class Error{
   public : Error();

public :
ErrorInfo processTokenError(const std::string& tokenValue);
ErrorInfo classifyTokenError(Token lastKnownToken,Token currentToken,Token nextToken);


};
 