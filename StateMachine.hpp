#pragma once
enum LexerState{
    NormalState,
    ClassDeclaration,
    ClassInheritanceList,
    ClassBody,
    MemberDeclaration,
    MemberList,
    FunctionDeclaration,
    FunctionBody,
    ParameterList,
    VaraibleDeclaration,
    Statement,
    StringLiteral,
    EndOfStatement
};


