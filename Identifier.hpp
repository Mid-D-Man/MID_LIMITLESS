#pragma once
#include <string>
#include <unordered_map>
#include <stack>

using namespace std;

enum IdentifierType{
 ClassIdentifier,
 MemberIdentifier,
    FunctionIdentifier,
    ParameterIdentifier,
    VariableIdentifier
};

struct IdentifierInfo{
    IdentifierType identifierType;
      IdentifierInfo() :identifierType(ClassIdentifier){}
 IdentifierInfo(IdentifierType type) : identifierType(type){}

};
class SymbolTable{
public :
    SymbolTable();
    private : unordered_map<string,IdentifierInfo> globalTable;
      private : stack<unordered_map<string,IdentifierInfo> > scopeStack;

public :
     void enterScope();
public :
     void exitScope();

public :
    void addIdentifier(const string& name,const IdentifierInfo& info);

public :
    IdentifierInfo* getIdentifier(const string& name);

};

