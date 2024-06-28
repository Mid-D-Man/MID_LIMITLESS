
#pragma once

enum SubTokenType{
    //the default value
    Default,
    //the keyword type
    DataStructure,
    CustomDataStructure,
    ControlStructure,
    //the type type
    DataType,
    ReturnType,
    //the seprator type
    OpenParen,
    CloseParen,
    OpenBrace,
    CloseBrace,
    OpenBracket,
    CloseBracket,
    OpenColon,
    CloseColon,
    SemiColon,
    Comma,
    Elipses,
    PointerMemberAccess,
    //the operator type
    ArithmeticOperator,
    BitwiseOperator,
    ComparisonOperator,
    LogicalOperator,
 //arithmetic operator
  Multiply,
  Mul_Assign,
  Divide,
  Div_Assign,
  Modulos,
  Mod_Assign,
  Addition,
  Add_Assign,
  Subtract,
  Sub_Assign,
  //special operators
  Assign,
  Increment,
  Decrement,
  //bitwise operators
  Bit_And,
  Bit_Or,
  Bit_Not,
  Bit_Xor,
  Bit_ShiftRight,
  Bit_ShiftLeft,
//logical Opertors
Logic_And,
Logic_Or,
Logic_Not,
//comparision operators
GreaterThan,
GreaterThanOrEqualTo,
LessThan,
LessThanOrEqualTo,
//==
EqualTo,
NotEqualTo,

    //the comment type
   SingleLineComment,
    MultiLineComment,
    ContiniousComment,
    //the modifier type i.e abstract ,virtual,static
    ClassModifier,
    FunctionModifier,


    //literal type
    Literal_String,
    Literal_Float,
    Literal_Integer,
    Literal_Character
};


struct SubTokenInfo{

  SubTokenType subTokenType;

  SubTokenInfo() :subTokenType(Default){}

 SubTokenInfo(SubTokenType type) : subTokenType(type){}

};

//note : is colon, ; is semi colon also called terminator note for our programming language ;,},: can also act
//as statement terminators
 const char* SubTokenTypeToString(SubTokenType subTokenType);
