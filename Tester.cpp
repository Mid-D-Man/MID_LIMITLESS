
#include "Lexer.hpp"
#include <iostream>
#include <memory>
#pragma region Int main

// Demonstration of MidTierCollector usage
int main() {

 cout << "Start" << endl;


//for lexet testing
    Lexer lexer;
    std::string sourceCode = "private class Abstractor : Base : { int value; float val2 = 0.225; public bool meSS = false;  public virtual void GetStuff:byte value,bool data:{/*do some stuff */}} public again; //Thats That";
    auto tokens = lexer.tokenize(sourceCode);

    // Output the tokens for testing
    for (const auto& token : tokens) {
        std::cout << " \n ( Token Of Name : " << token.tokenValue <<  "\n, Type : \n" << TokenTypeToString(token.tokenType)
         << ",  With : SubToken  Type Of : \n"  << SubTokenTypeToString(token.subTokenInfo.subTokenType) << " ) |||| \n" << std::endl;
    }


return 0;
}

#pragma endregion
