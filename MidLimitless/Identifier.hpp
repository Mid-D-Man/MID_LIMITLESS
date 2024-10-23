#pragma once
#include <string>
#include <unordered_map>
#include <stack>

using namespace std;
//keeps track of diffrent identifiertypes based no were the identifier was declared
enum IdentifierType{
    ClassIdentifier,
    MemberIdentifier,
    FunctionIdentifier,
    ParameterIdentifier,
    VariableIdentifier
};

//this stores info about the identifier i.e type,extend to store value it holds,line it wad declared in and
//stuff of such
struct IdentifierInfo{
    IdentifierType identifierType;
      IdentifierInfo() :identifierType(ClassIdentifier){}
 IdentifierInfo(IdentifierType type) : identifierType(type){}

};
//this holds and keeps tracks of all our identifiers
//making it easier to cheack if an identifier has already been declared in the current file
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

