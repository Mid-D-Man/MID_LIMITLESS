#pragma once
enum LexerState{
    NormalState,
    NameSpaceDeclaration,
    NameSpaceBody,
    ClassDeclaration,
    ClassInheritanceList,
    ClassBody,
    MemberDeclaration,
    MemberList,
    FunctionDeclaration,
    ParameterList,
    FunctionBody,
    VaraibleDeclaration,
    Statement,
    StringLiteral,
    EndOfStatement
};
struct LexerStateStack{
    public:
    LexerState currentState;
    string stateChangeValue;
//default constructor
LexerStateStack(): currentState(LexerState::NormalState),stateChangeValue(""){}
//constructor that takes arguments
    LexerStateStack(LexerState state, string val) :
    currentState(state),stateChangeValue(val){}

    };
