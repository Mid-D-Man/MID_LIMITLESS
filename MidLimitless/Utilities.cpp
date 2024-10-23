#include "Utilities.hpp"
#include <regex>

using namespace std;

Utilities::Utilities(){

};

bool Utilities::isWhiteSpace(char ch){
    return isspace(ch);
}

bool Utilities::isEndOfLine(char c){
    bool result = (c == '\n') || (c == '\r');
    return result;
}

bool Utilities::isNumeric(char c){
    bool result = false;

    if((c >= '0')&& (c <='9')) result = true;

    return result;
}

bool Utilities::isLetter(char c){
    bool result = false;
    if((c >= 'A') && (c <= 'Z')) result = true;
       if((c >= 'a') && (c <= 'z')) result = true;

       return result;
}

 bool Utilities::isValidIdentifierChar(char ch){
    return isalnum(ch) || ch == '_';
}

bool Utilities::isValidIdentifier(string tokenValue)const{

regex identifierRex(R"(@?[a-zA-Z_]\w*)");
return regex_match(tokenValue,identifierRex);

}

string Utilities::cheackNumberType(string tokenValue)const{
    regex numRegex(R"([+-]?(\d+\.\d*|\.\d+|\d+)([eE][-+]?\d+)?[fF]?)");
if(regex_match(tokenValue,numRegex)){
    if(tokenValue.find('.') == std::string::npos){
return "Integer";
    }else{
        if(sizeof(float) == sizeof(double)){
            return "Double";
        }else{
            return "Float";
        }
    }
}else{
    return "Not A Valid Number";
}

}

