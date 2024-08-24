#include "Parser.hpp"
#include <iostream>
#include <map>

using namespace std;

//yeah this requires a whole lotta work
map<string,int> symbolTable;

Parser::Parser(Lexer lexer) : lexer(lexer),currentTokenIndex(0){
    tokens = lexer.tokenize("");
}

void Parser::parse(){

}



/*
bool Parser::isValidIdentifier(const string& identifer){
    //replace with is valid identifier
    if(!identifer[0] == '_' || (identifer[0] >= 'a' && identifer[0] <= 'z')|| (identifer[0] >= 'A' && identifer[0] <= 'Z')){
        return false;
    }

    for(size_t i =1; i < identifer.size(); i++){
   if(!identifer[0] == '_' || (identifer[0] >= 'a' && identifer[0] <= 'z')|| (identifer[0] >= 'A' && identifer[0] <= 'Z')|| (identifer[0] >= '0' && identifer[0] <= '9')){
        return false;
    }
    }

    asm volatile(
    "movl $0, %%eax;"
    "movl $0, %%ecx;"
    "movl $0, %%edx;"
    "loop_start:"
    "movb (%[identifer],%[i]), %%al;"
    "cmpb $0, %%al;"
    "je loop_end;"
    "cmpb '_',[%%al];"
     "je loop_continue;"
     "cmpb 'a',[%%al]"
     "jl loop_invalid;"
     "cmpb 'z',[%%al];"
     "jg loop_invalid;"
     "loop_continue;"
     "incl %[i];"
     "jmp loop_start;"
     "loop_end:"
     "movl $1,%%eax;"
     "loop_invalid:"
     "movl $0,%%eax;"
     :
     :[identifer] "r" (identifer.c_str()),[i] "r" (i)
     : "%eax","%ecx", "%edx"
    );
return (bool)asm_output;

}
*/


