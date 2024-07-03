#include<stack>
#include<string>
#include <unordered_map>
#include "Identifier.hpp"



using namespace std;

//don
SymbolTable::SymbolTable(){

};




    void SymbolTable::enterScope(){
        unordered_map<string,IdentifierInfo> defaultVal ;
        defaultVal[""] = IdentifierInfo();
    scopeStack.push(defaultVal);
}

     void SymbolTable::exitScope(){
    if(!scopeStack.empty()){
    scopeStack.pop();
    }
}


    void SymbolTable::addIdentifier(const string& name,const IdentifierInfo& info){
        if(scopeStack.empty()){
globalTable[name] = info;
        }else{
            scopeStack.top()[name] = info;
        }

    }


    IdentifierInfo* SymbolTable::getIdentifier(const string& name){
if(!scopeStack.empty()){
    auto& currentScope = scopeStack.top();
    if(currentScope.find(name) != currentScope.end()){
        return &currentScope[name];
    }
}
         auto globalIt = globalTable.find(name);
    if(globalIt != globalTable.end()){
        return &globalIt ->second;
    }else{
        return nullptr;
    }
    }


