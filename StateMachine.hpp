#pragma once
//keeps track of what state the lexer is in for better token classification
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
    StatementBody,
    StringLiteral,
    EndOfStatement
};

//our lexer state stack for keeping track of all states the lexer has been in trought the entire tokenization process
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
