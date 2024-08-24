#include <string>
#include<vector>


class Utilities{

public : Utilities();
public : bool isWhiteSpace(char ch);
 bool isEndOfLine(char c);
 bool isValidIdentifierChar(char ch);
 bool isValidIdentifier(std::string tokenValue)const;
 std::string cheackNumberType(std::string tokenValue)const;
 bool isNumeric(char c);
 bool isLetter(char c);

};

