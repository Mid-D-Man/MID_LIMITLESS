#include <stdio.h>
#include "Token.hpp"


const char* TokenTypeToString(TokenType tokenType){
switch(tokenType){
    case Keyword: return "Keyword";
    case Identifier: return "Identifier";
        case Literial: return "Literal";
        case Operator: return "Operator";
        case OperatorWithSpecialMeaning: return "OperatorWithSpecialMeaning";
        case Punctuation: return "Punctuation";
        case Seprator: return "Seprator";
    case AccessModifier: return "AccessModifier";
        case WhiteSpace: return "WhiteSpace";
        case Comment: return "Comment";
        case PreprocessorDirectory: return "PreprocessorDirectory";
        case SpecialSymbol: return "SpecialSymbol";
        case EscapeSequence: return "EscapeSequence";
     case Annotation_Attribute: return "Annotation_Attribute";


        case Null_Undefined: return "Null_Undefined";
        case Function_MethodCall: return "Function_MethodCall";
        case UserDefinedToken: return "UserDefinedToken";


}
    }

