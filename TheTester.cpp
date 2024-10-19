#include <iostream>
#include <memory>
#include "Lexer.cpp"
#include "SubToken.cpp"
using namespace std;
// Demonstration of MidTierCollector usage
int main() {
 cout << "Start" << endl;



//for lexet testing 
    Lexer lexer;
    std::string sourceCode = "private class _MyClass {  public int Val33 = true; \n/*string _theStei = \"This Be The String Init \";*/  \n ~private void return_MyVal:int dider,float cocain,bool moreData:{  var determiner = dider / 14; var contp += 15;  var add = = 14;if(determiner > 12){moreData = false; var _dat ++;}else{dider+=12; cocain / 0.5;}  } }} \t \n //leFinal Comment \n /*Extra Comment var bool*/";
    auto tokens = lexer.tokenize(sourceCode);
    // Output the tokens for testing
    for (const auto& token : tokens) {
        std::cout <<  " \n ( Token Of Name :> " << token.tokenValue + "\n Of Type :> \n" << TokenTypeToString(token.tokenType)
        << ",  With SubToken Type Of :> \n"  << SubTokenTypeToString(token.subTokenInfo.subTokenType) << "\n and Location Info :> Lin > " << token.locationInfo.lineNumber << " < Col > " << token.locationInfo.columnNumber << " ) - - - - - - - - \n" << std::endl;
    }


return 0;
}
