#include <iostream>

#include <stack>
#include <memory>

#include <cctype>
#include <sstream>
#include<variant>
#include <regex>

#include "Lexer.hpp"
#include "TrieNode.cpp"
#include "Identifier.cpp"
#include "Token.cpp"
using namespace std;
#pragma region Actual Lexer Calss
 Lexer::Lexer (){

 //stores the current state of the lexer
  currentState = LexerState::NormalState;
 //stores the symbol table for identifiers


 currentTokenIndex = 0;

        tokens.clear();
        InitializeKeywordMap();
        InitializeSpecialCharacterMap();
       symbols.enterScope();

        // ... add other keywords and token types ...

        // Populate the trie with the keywords
        for (const auto& pair : keywordMap) {
         trie.insert(Token(pair.first, TokenType::Keyword, pair.second.subTokenInfo));
        }
           for (const auto& pair : specialCharacterMap) {
           trie.insert(Token(string(1,pair.first), TokenType::Keyword, pair.second.subTokenInfo));
        }
    }

#pragma region Token lookAheadBuffer
Token  Lexer::readNextToken(istringstream& codeStream){
    char ch;
    string tokenValue;
    //make sure token value is not empty
while(codeStream.get(ch)){

if(isspace(ch)){
 return readNextToken(codeStream);
}else if(isSpecialCharacter(ch)){
    //if its a secial character return it as a token imediately
 return specialCharacterMap.at(ch);

}else if(ch == '/' ){

        if(codeStream.peek() == '/'){
            //single line comment
        processComment(codeStream,tokens,false);
        tokenValue.clear();
      }else if(codeStream.peek() == '*'){
        //multiline comment
        processComment(codeStream,tokens,true);
        tokenValue.clear();
        }else{
            tokenValue += ch;
        }

      }else{
    if(!tokenValue.empty()){
        tokenValue += ch;
break;
    }else{
tokenValue += ch;
    }

}

}

 while(codeStream.get(ch)){
if(isspace(ch) || isSpecialCharacter(ch)){
codeStream.unget();
break;
}else{
    tokenValue += ch;
}

}

    TokenType type = determineDefaultTokenType(tokenValue);
    return Token(tokenValue,type,SubTokenInfo());
}

Token Lexer::peekNextToken(istringstream& codeStream){
    streampos ogPos = codeStream.tellg(); //save current pos
    Token nextToken;

nextToken = readNextToken(codeStream);
    codeStream.seekg(ogPos);
    //restore og pos
    return nextToken;

}
Token Lexer::peekNextToken2(istringstream& codeStream){
streampos ogPos = codeStream.tellg();
Token nextToken,nextToken2;

nextToken = readNextToken(codeStream);

nextToken2 = readNextToken(codeStream);

codeStream.seekg(ogPos);

return nextToken2;
}



TokenType Lexer::determineDefaultTokenType(const string& tokenValue)const{
    //make light deep cheack for token and token type here
    //also make identifier table for storing identifiers when creating the actual tokens
    if(keywordMap.find(tokenValue) != keywordMap.end()){
     return keywordMap.at(tokenValue).tokenType;
    }else{
       bool isIdentifier = isValidIdentifier(tokenValue);
       string inNumericLiteral = cheackNumberType(tokenValue);

    if(isIdentifier){
    return TokenType::Identifier;
    }else if(inNumericLiteral == "Integer"|| inNumericLiteral == "Float"||inNumericLiteral == "Double" ){
    return TokenType::Literial;
    }else{
    return TokenType::Null_Undefined;
    }

    }

}

#pragma endregion

#pragma region  Helper Functions

void Lexer::InitializeKeywordMap(){
 // Initialize the keywordMap map with keywords and their subtoken info
//note only add keywords with one meaning no matter the context
 keywordMap["public"] = Token("AccessModifier",TokenType::AccessModifier,SubTokenInfo(SubTokenType::Default));
 keywordMap["private"] = Token("AccessModifier",TokenType::AccessModifier,SubTokenInfo(SubTokenType::Default));
 keywordMap["protected"] = Token("AccessModifier",TokenType::AccessModifier,SubTokenInfo(SubTokenType::Default));
keywordMap["internal"] = Token("AccessModifier",TokenType::AccessModifier,SubTokenInfo(SubTokenType::Default));
keywordMap["external"] = Token("AccessModifier",TokenType::AccessModifier,SubTokenInfo(SubTokenType::Default));
//cn = continue but for func declaration
keywordMap["cn"] = Token("Keyword",TokenType::Keyword,SubTokenInfo(SubTokenType::Default));
/*

public internal bool returnMe:bool the:{
    internal bool returnme;
    some other stufff;
    Mlog.Out:"Me Is Awsome":

}
*/


keywordMap["void"] = Token("void",TokenType::Keyword,SubTokenInfo(SubTokenType::ReturnType));

        keywordMap["class"] = Token("class",TokenType::Keyword,SubTokenInfo(SubTokenType::Default));

        keywordMap["int"] = Token("int",TokenType::Keyword,SubTokenInfo(SubTokenType::DataType));
        keywordMap["bool"] = Token("bool",TokenType::Keyword,SubTokenInfo(SubTokenType::DataType));
        keywordMap["char"] = Token("char",TokenType::Keyword,SubTokenInfo(SubTokenType::DataType));
        keywordMap["string"] = Token("string",TokenType::Keyword,SubTokenInfo(SubTokenType::DataType));
        keywordMap["float"] = Token("float",TokenType::Keyword,SubTokenInfo(SubTokenType::DataType));
        keywordMap["double"] = Token("double",TokenType::Keyword,SubTokenInfo(SubTokenType::DataType));
        keywordMap["var"] = Token("var",TokenType::Keyword,SubTokenInfo(SubTokenType::DataType));
  keywordMap["virtual"] = Token("virtual",TokenType::Keyword,SubTokenInfo(SubTokenType::FunctionModifier));
  keywordMap["abstract"] = Token("abstract",TokenType::Keyword,SubTokenInfo(SubTokenType::FunctionModifier));


         keywordMap["="] = Token("=",TokenType::Operator,SubTokenInfo(SubTokenType::Assign));
         keywordMap["+"] = Token("+",TokenType::Operator,SubTokenInfo(SubTokenType::Addition));
            keywordMap["-"] = Token("-",TokenType::Operator,SubTokenInfo(SubTokenType::Subtract));
            keywordMap["*"] = Token("*",TokenType::Operator,SubTokenInfo(SubTokenType::Multiply));
            keywordMap["/"] = Token("/",TokenType::Operator,SubTokenInfo(SubTokenType::Divide));
            keywordMap["%"] = Token("%",TokenType::Operator,SubTokenInfo(SubTokenType::Modulos));

             keywordMap["=="] = Token("==",TokenType::Operator,SubTokenInfo(SubTokenType::EqualTo));
         keywordMap["+="] = Token("+=",TokenType::Operator,SubTokenInfo(SubTokenType::Add_Assign));
            keywordMap["-="] = Token("-=",TokenType::Operator,SubTokenInfo(SubTokenType::Sub_Assign));
            keywordMap["*="] = Token("*=",TokenType::Operator,SubTokenInfo(SubTokenType::Mul_Assign));
            keywordMap["/="] = Token("/=",TokenType::Operator,SubTokenInfo(SubTokenType::Div_Assign));
            keywordMap["%="] = Token("%=",TokenType::Operator,SubTokenInfo(SubTokenType::Mod_Assign));


            keywordMap["if"] = Token("if",TokenType::Keyword,SubTokenInfo(SubTokenType::ControlStructure));
            keywordMap["else"] = Token("else",TokenType::Keyword,SubTokenInfo(SubTokenType::ControlStructure));
            keywordMap["do"] = Token("do",TokenType::Keyword,SubTokenInfo(SubTokenType::ControlStructure));
            keywordMap["while"] = Token("while",TokenType::Keyword,SubTokenInfo(SubTokenType::ControlStructure));
            keywordMap["for"] = Token("for",TokenType::Keyword,SubTokenInfo(SubTokenType::ControlStructure));
            keywordMap["foreach"] = Token("foreach",TokenType::Keyword,SubTokenInfo(SubTokenType::ControlStructure));


            keywordMap["struct"] = Token("struct",TokenType::Keyword,SubTokenInfo(SubTokenType::DataStructure));





}

void Lexer::InitializeSpecialCharacterMap(){
    //and as for these seprators sure they can have diffrent meaning
    //but still is best to know that these are seprators
specialCharacterMap['('] = Token("(",TokenType::Seprator,SubTokenInfo(SubTokenType::OpenParen));
specialCharacterMap[')'] = Token(")",TokenType::Seprator,SubTokenInfo(SubTokenType::CloseParen));
specialCharacterMap['{'] = Token("{",TokenType::Seprator,SubTokenInfo(SubTokenType::OpenBrace));
specialCharacterMap['}'] = Token("}",TokenType::Seprator,SubTokenInfo(SubTokenType::CloseBrace));
specialCharacterMap['['] = Token("[",TokenType::Seprator,SubTokenInfo(SubTokenType::OpenBracket));
specialCharacterMap[']'] = Token("]",TokenType::Seprator,SubTokenInfo(SubTokenType::CloseBracket));
specialCharacterMap[':'] = Token(":",TokenType::Seprator,SubTokenInfo(SubTokenType::Default));
specialCharacterMap[';'] = Token(";",TokenType::Seprator,SubTokenInfo(SubTokenType::SemiColon));
specialCharacterMap[','] = Token(",",TokenType::Seprator,SubTokenInfo(SubTokenType::Comma));

}


void Lexer::processComment(istringstream& codeStream, vector<Token>& tokens, bool isMultiLine){
    string comment;
    char ch;
    if(isMultiLine){
        comment = "/*";
        //consume *
        codeStream.get();
        while(codeStream.get(ch)){
            comment += ch;
            if(ch == '*' && codeStream.peek() == '/'){
                comment += codeStream.get();
                //add /
                break;
                //end MultiLineComment
            }
        }
    }else{
        //handle single line comment
        comment += "//"; //start of single line comment
        codeStream.get();
        while(codeStream.get(ch) && ch != '\n'){
            comment += ch;
        }
    }

    tokens.push_back(Token(comment,TokenType::Comment,SubTokenInfo()));
}


bool Lexer::isSpecialCharacter(char ch)const{
    //define all special characters that should be tokenized individually
return specialCharacterMap.find(ch) != specialCharacterMap.end();
    const string specialCharacters = ";<>{}[]:(),.";
}

bool Lexer::isWhiteSpace(char ch){
    return isspace(ch);
}

bool Lexer::isEndOfLine(char c){
    bool result = (c == '\n') || (c == '\r');
    return result;
}

bool Lexer::isNumeric(char c){
    bool result = false;

    if((c >= '0')&& (c <='9')) result = true;

    return result;
}

bool Lexer::isLetter(char c){
    bool result = false;
    if((c >= 'A') && (c <= 'Z')) result = true;
       if((c >= 'a') && (c <= 'z')) result = true;

       return result;
}


 bool Lexer::isValidIdentifierChar(char ch){
    return isalnum(ch) || ch == '_';
}

bool Lexer::isValidIdentifier(string tokenValue)const{

regex identifierRex(R"(@?[a-zA-Z_]\w*)");
return regex_match(tokenValue,identifierRex);

}

string Lexer::cheackNumberType(string tokenValue)const{
    regex numRegex(R"([+-]?(\d+\.\d*|\.\d+|\d+)([eE][-+]?\d+)?)");
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
#pragma endregion


    vector<Token> Lexer::tokenize(const string& sourceCode) {

        istringstream codeStream(sourceCode);
        char ch;
        string currentTokenValue;
 Token currentToken,lastKnownValidToken,nextToken;

//notice we must handle whitespace tokenization,comment tokenization seprate from regular token tokenization,
//for they must be processed in chars while regular tokens must be processed in words
//keep in mind we dont actually have to handle whitespace unless we have a use for them
while(codeStream){
    currentToken = readNextToken(codeStream);
//this works now fix handle comments and fixify
   /* if(!currentToken.isValid()){
        break;
    }
    */
    if(currentTokenIndex > 0 && tokens.capacity() > 0 && tokens[currentTokenIndex -1].tokenType != TokenType::WhiteSpace &&  tokens[currentTokenIndex -1].tokenType != TokenType::Comment ){
        lastKnownValidToken =  tokens[currentTokenIndex -1];
    }
    nextToken = peekNextToken(codeStream);

//we already modifying current token based on &
    ++currentTokenIndex;
   Token fullyClassifiedToken = classifyToken(currentToken,lastKnownValidToken,nextToken);
    //trie.insert(fullyClassifiedToken);
     tokens.push_back(fullyClassifiedToken);

    cout << "Current Token Value Is :: " << currentToken.tokenValue << " :: And Next Token IS :: " << nextToken.tokenValue << "\n :: And Last Known Token From Current Token Is ::" << lastKnownValidToken.tokenValue << endl ;
}

  return tokens;

}

bool Lexer::isValidFunctionReturnType(Token token){
if(token.subTokenInfo.subTokenType == SubTokenType::DataType || token.subTokenInfo.subTokenType == SubTokenType::ReturnType || token.tokenValue == "void"){
return true;
}else{
    return false;
}
}

bool Lexer::isFunctionDeclaration(Token currentToken,Token lastKnownToken,Token nextToken){
if(isValidFunctionReturnType(currentToken) && lastKnownToken.tokenType == TokenType::AccessModifier){
return true;
}else if(currentToken.tokenType == TokenType::Identifier || currentToken.tokenType == TokenType::Null_Undefined && isValidFunctionReturnType(lastKnownToken) && nextToken.tokenValue == ":" ){
return true;
}else{
    return false;
}

}

bool Lexer::isMemberDeclaration(Token currentToken,Token lastKnownToken,Token nextToken){
    if(currentToken.tokenValue == "const"){
        return true;
    }else if((currentToken.subTokenInfo.subTokenType == SubTokenType::DataType )&& (lastKnownToken.tokenType == TokenType::AccessModifier)){
        return true;
    }else if((currentToken.subTokenInfo.subTokenType == SubTokenType::DataType) && (nextToken.tokenType == TokenType::Null_Undefined || nextToken.tokenType == TokenType::Identifier)){
        return true;
    }else if((lastKnownToken.subTokenInfo.subTokenType == SubTokenType::DataType )&& (currentToken.tokenType == TokenType::Null_Undefined || currentToken.tokenType == TokenType::Identifier && nextToken.tokenValue == "=" || TokenType::Identifier && nextToken.tokenValue == ";") ){
return true;
    }else{
        return false;
    }
}


/*ok here is what we are going to do we need a class to store the default values of tokens the ones beign
processed by the toknize readtoken function because in some cases we might have something like > =  instead of >= which is also valid
but is actually only one token so in the defualt tken keeper we when we are classifying token and
we encounter such a case first edit the current token we are        classifying based on it and the next token
and then edit the > and = and combine them into one token in the token keeper or something like that just get it to work*/

Token Lexer::classifyToken(const Token& currentToken,Token lastKnownToken,Token nextToken){
    //always start from lastknown token type
//this function will handle the actual tokenization and place tokens In Actual Token Or Use The Tier

//before we run the state based switch we decide the current state
//encounterd prob when we read float 0.255 it reads . and  255 as two seprate tokens

switch(currentState){

    case LexerState::NormalState:{
 if(currentToken.tokenValue == "class"){

currentState = ClassDeclaration;

return Token(currentToken.tokenValue,TokenType::Keyword,SubTokenInfo(SubTokenType::Default));
}else if(currentToken.tokenValue == "+" && nextToken.tokenValue == "="){
        return Token("+=",TokenType::Operator,SubTokenInfo(SubTokenType::Bit_ShiftRight));

    }else if(isFunctionDeclaration(currentToken,lastKnownToken,nextToken)){
currentState = FunctionDeclaration;
}else if(lastKnownToken.tokenType == TokenType::Keyword && lastKnownToken.subTokenInfo.subTokenType == SubTokenType::DataType){

    //meaning we has int,bool or the likes
    //that means the current token is an identifire but we also need to cheack the next token
    //if the next token is a ( or : that means we are entering function declaration state
    //but if the next token is a = or ; it means we are creating a member since we are in normal state
     Token returnToken = Token( currentToken.tokenValue,TokenType::Identifier,SubTokenInfo(SubTokenType::Default));
   if(symbols.getIdentifier(returnToken.tokenValue) == nullptr){

IdentifierInfo returnTokenIdentifierInfo = IdentifierInfo(IdentifierType::MemberIdentifier);
symbols.addIdentifier(returnToken.tokenValue,returnTokenIdentifierInfo);

   }
return returnToken;

}else if(isMemberDeclaration(currentToken,lastKnownToken,nextToken)){
    currentState = MemberDeclaration;
}

    }break;
case ClassBody:{

}
break;
case ClassInheritanceList:{

}

break;
case MemberList:{

}
break;
      case ClassDeclaration:{}
break;
       case MemberDeclaration:{

 if(currentToken.subTokenInfo.subTokenType == SubTokenType::DataType && lastKnownToken.tokenType == TokenType::AccessModifier || lastKnownToken.subTokenInfo.subTokenType == SubTokenType::DataType){
       Token returnToken = Token( currentToken.tokenValue,TokenType::Identifier,SubTokenInfo(SubTokenType::Default));
   if(symbols.getIdentifier(returnToken.tokenValue) == nullptr){

IdentifierInfo returnTokenIdentifierInfo = IdentifierInfo(IdentifierType::MemberIdentifier);
symbols.addIdentifier(returnToken.tokenValue,returnTokenIdentifierInfo);
return returnToken;
   }
    }
       }

    break;
      case FunctionDeclaration:{

if(lastKnownToken.tokenType == TokenType::Identifier && currentToken.tokenValue == ":"){
currentState = ParameterList;
Token( currentToken.tokenValue,TokenType::Seprator,SubTokenInfo(SubTokenType::OpenColon));
}else if(lastKnownToken.tokenValue == ":" && currentToken.tokenValue == "{")
Token( currentToken.tokenValue,TokenType::Seprator,SubTokenInfo(SubTokenType::OpenBrace));
 currentState = FunctionBody;
}

    break;
      case ParameterList:{
if(lastKnownToken.subTokenInfo.subTokenType == SubTokenType::DataType){
   Token returnToken = Token( currentToken.tokenValue,TokenType::Identifier,SubTokenInfo(SubTokenType::Default));
   if(symbols.getIdentifier(returnToken.tokenValue) == nullptr){

IdentifierInfo returnTokenIdentifierInfo = IdentifierInfo(IdentifierType::ParameterIdentifier);
symbols.addIdentifier(returnToken.tokenValue,returnTokenIdentifierInfo);

   }

   return returnToken;
}
      }




    break;
     case VaraibleDeclaration:{

     }

    break;

     case FunctionBody:{

     }

    break;
         case Statement:{

         }

    break;
          case StringLiteral:{

          }

    break;
         case EndOfStatement:{

         }

    break;

}

cout << "After Cheacks Our Current Lex State Is" << currentState;
currentState = NormalState;
return Token(currentToken.tokenValue,currentToken.tokenType,currentToken.subTokenInfo);
}





#pragma region  Extra Important Token Functions

Token Lexer::getValidLastKnownToken(const std::string& tokenValue){
  //cheack for last known token if any can also cheack token befor last token for in case of for loop or something
    //ok we have already handled if the last knwon token is white space or comment in which case
    //we cheak if there is a token before that and cheack the token type for it then
    Token lastKnownToken =  Token();

    if(currentTokenIndex > 0 && tokens[currentTokenIndex -1].tokenType != TokenType::WhiteSpace &&  tokens[currentTokenIndex -1].tokenType != TokenType::Comment ){
        lastKnownToken =  tokens[currentTokenIndex -1];
    }else{
cout << "Last Known Token Type Was Comment Or White Space";
if(currentTokenIndex > 1 && tokens[currentTokenIndex -1].tokenType != TokenType::WhiteSpace &&  tokens[currentTokenIndex -1].tokenType != TokenType::Comment ){
 lastKnownToken =  tokens[currentTokenIndex -2];
}
    }

    //logic to classify the current token

cout << "Valid Known Token Is : " << TokenTypeToString(lastKnownToken.tokenType) << "With value : " << lastKnownToken.tokenValue << endl;
return lastKnownToken;

}




 void Lexer::handleError(){

 }

#pragma endregion



#pragma endregion
