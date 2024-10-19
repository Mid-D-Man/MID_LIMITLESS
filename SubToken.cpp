#include "SubToken.hpp"

const char* SubTokenTypeToString(SubTokenType subTokenType){
switch(subTokenType){
    case Default: return "Default";

    case DataStructure: return "DataStructure";
        case CustomDataStructure: return "CustomDataStructure";
        case ControlStructure: return "ControlStructure";

        case DataType: return "DataType";
        case ReturnType: return "ReturnType";

        case OpenParen: return "OpenParen";
     case CloseParen: return "CloseParen";
    case OpenBrace: return "OpenBrace";
        case CloseBrace: return "CloseBrace";
        case OpenBracket: return "OpenBracket";
        case CloseBracket: return "CloseBracket";
        case OpenColon: return "OpenColon";
        case CloseColon: return "CloseColon";
          case OpenDoubleQuote: return "OpenDoubleQuote";
            case CloseDoubleQuote: return "CloseDoubleQuote";
             case OpenSingleQuote: return "OpenSingleQuote";
            case CloseSingleQuote: return "CloseSingleQuote";
     case SemiColon: return "SemiColon";
      case Comma: return "Comma";
        case Elipses: return "Elipses";
        case PointerMemberAccess: return "PointerMemberAccess";


        case ArithmeticOperator: return "ArithmeticOperator";
        case LogicalOperator: return "LogicalOperator";
        case BitwiseOperator: return "BitwiseOperator";

    case ComparisonOperator: return "ComparisonOperator";

        case SingleLineComment: return "SingleLineComment";
        case MultiLineComment: return "DoubleLineComment";
        case ContiniousComment: return "ContiniousCommet";


        case Multiply: return "Multiplay";
        case Mul_Assign: return "Mul_Assign";
        case Addition: return "Addition";
     case Add_Assign: return "Add_Aassign";
    case Subtract: return "Subtract";
        case Sub_Assign: return "Aub_Assign";
        case Divide: return "Divide";
        case Div_Assign: return "Dive_Assign";
        case Modulos: return "Modulos";
        case Mod_Assign: return "Mod_Assign";
     case Assign: return "Assign";


        case Increment: return "Increment";
        case Decrement: return "Decrement";
        case Bit_And: return "Bit_And";

    case Bit_Or: return "Bit_Or";

        case Bit_Not: return "Bit_Not";
        case Bit_Xor: return "Bit_Xor";
        case Bit_ShiftRight: return "Bit_ShiftRight";

 case Bit_ShiftLeft: return "Bit_ShiftLeft";




        case Logic_And: return "Logic_And";
        case Logic_Or: return "Logic_Or";
        case Logic_Not: return "Logic_Not";

    case GreaterThan: return "ComparisonOperator";

        case LessThan: return "SingleLineComment";
        case GreaterThanOrEqualTo: return "DoubleLineComment";
        case LessThanOrEqualTo: return "ContiniousCommet";

           case EqualTo: return "EqualTo";
        case NotEqualTo: return "NotEqualTo";

          case Literal_String: return "Literal_String";

        case Literal_Integer: return "Literal_Integer";
        case Literal_Character: return "Literal_Character";
        case Literal_Float: return "Literal_Float";
        case Literal_Double: return "Literal_Double";
        case Literal_Bool: return "Literal_Bool";

        case ClassModifier: return "ClassModifier";
        case FunctionModifier: return "FunctionModifier";//abstrat,virtual
        case FunctionStarter: return "FunctionStarter";



}};
