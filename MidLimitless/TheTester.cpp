#include <iostream>
#include <memory>
#include "Lexer.cpp"
#include "SubToken.cpp"
#include "Parser.cpp"
using namespace std;
// Demonstration of MidTierCollector usage

std::string _codeToTest = R"( int i = 10; )";



 vector<Token> MakeLexerTest(){
Lexer lexer;
auto tokens = lexer.tokenize(_codeToTest);

return tokens;
}
 void MakeParserTest(vector<Token> tokens){

std::cout << "Starting Parsing \n";

    Parser parser;
    parser.parse(tokens);

    std::cout << "Parsing completed. \n" << std::endl;

    std::cout << "\n Parse Tree:" << std::endl;
    parser.print_tree(parser.getParseTreeRoot(),0);
}



int main() {
 cout << "Start" << endl;

    auto tokens = MakeLexerTest();
    // Output the tokens for testing
    for (const auto& token : tokens) {
        std::cout <<  " \n ( Token Of Name :> " << token.tokenValue + "\n Of Type :> \n" << TokenTypeToString(token.tokenType)
        << ",  With SubToken Type Of :> \n"  << SubTokenTypeToString(token.subTokenInfo.subTokenType) << "\n and Location Info :> Lin > " << token.locationInfo.lineNumber << " < Col > " << token.locationInfo.columnNumber << " ) - - - - - - - - \n" << std::endl;
    }

MakeParserTest(tokens);
return 0;
}
