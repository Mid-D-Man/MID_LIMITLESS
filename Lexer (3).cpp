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
        //empty stack
        lexerStateStack =stack<LexerStateStack>();
     //stack<LexerStateStack>().swap(lexerStateStack);
    
        // ... add other keywords and token types ...

        // Populate the trie with the keywords
        for (const auto& pair : keywordMap) {
         trie.insert(Token(pair.first, TokenType::Keyword, pair.second.subTokenInfo));
        }
           for (const auto& pair : specialCharacterMap) {
           trie.insert(Token(string(1,pair.first), TokenType::Keyword, pair.second.subTokenInfo));
        }
    }
//problem here not reading +,++,+= and also adding AccessModifier As Token value
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
    if(ch =='.' && isNumeric(codeStream.peek())){
        tokenValue += ch;
    }else{
 return specialCharacterMap.at(ch);
    }
//there is the case of . in float
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
   if(ch =='.' && isNumeric(codeStream.peek())){
        tokenValue += ch;
    }else{
 codeStream.unget();
break;
    }
}else{
    tokenValue += ch;
}
}
//can change this to determine default token
    Token defaultToken = determineDefaultTokenType(tokenValue);
    //ok for stuff like int,bool and the likes that never change
    //return them with from keyword map forom here
   return defaultToken;

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



Token Lexer::determineDefaultTokenType(string& tokenValue)const{
    //make light deep cheack for token and token type here
    //also make identifier table for storing identifiers when creating the actual tokens
    if(isKeyword(tokenValue)){
      return keywordMap.at(tokenValue);
    }else{
       bool isIdentifier = isValidIdentifier(tokenValue);
       string inNumericLiteral = cheackNumberType(tokenValue);

    if(isIdentifier){
    return Token(tokenValue,TokenType::Identifier,SubTokenInfo());
    }else if(inNumericLiteral == "Integer"){
    return Token(tokenValue,TokenType::Literial,SubTokenInfo(SubTokenType::Literal_Integer));
    }else if( inNumericLiteral == "Float"){
    return Token(tokenValue,TokenType::Literial,SubTokenInfo(SubTokenType::Literal_Float));
    }else if(inNumericLiteral == "Double"){
    //literal double
    return Token(tokenValue,TokenType::Literial,SubTokenInfo(SubTokenType::Literal_Float));
    }else{
    return Token();
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
            keywordMap["++"] = Token("++",TokenType::Operator,SubTokenInfo(SubTokenType::Increment));
            keywordMap["--"] = Token("--",TokenType::Operator,SubTokenInfo(SubTokenType::Decrement));
            keywordMap["+="] = Token("+=",TokenType::Operator,SubTokenInfo(SubTokenType::Add_Assign));
            keywordMap["-="] = Token("-=",TokenType::Operator,SubTokenInfo(SubTokenType::Sub_Assign));
            keywordMap["*="] = Token("*=",TokenType::Operator,SubTokenInfo(SubTokenType::Mul_Assign));
            keywordMap["/="] = Token("/=",TokenType::Operator,SubTokenInfo(SubTokenType::Div_Assign));
            keywordMap["%="] = Token("%=",TokenType::Operator,SubTokenInfo(SubTokenType::Mod_Assign));
            //random range
            keywordMap[">_<"] = Token(">_<",TokenType::Operator,SubTokenInfo(SubTokenType::Mod_Assign));


            keywordMap["if"] = Token("if",TokenType::Keyword,SubTokenInfo(SubTokenType::ControlStructure));
            keywordMap["else"] = Token("else",TokenType::Keyword,SubTokenInfo(SubTokenType::ControlStructure));
            keywordMap["do"] = Token("do",TokenType::Keyword,SubTokenInfo(SubTokenType::ControlStructure));
            keywordMap["while"] = Token("while",TokenType::Keyword,SubTokenInfo(SubTokenType::ControlStructure));
            keywordMap["for"] = Token("for",TokenType::Keyword,SubTokenInfo(SubTokenType::ControlStructure));
            keywordMap["foreach"] = Token("foreach",TokenType::Keyword,SubTokenInfo(SubTokenType::ControlStructure));


            keywordMap["struct"] = Token("struct",TokenType::Keyword,SubTokenInfo(SubTokenType::DataStructure));

            keywordMap["use"] = Token("use",TokenType::PreprocessorDirectory,SubTokenInfo(SubTokenType::Default));
            keywordMap["include"] = Token("include",TokenType::PreprocessorDirectory,SubTokenInfo(SubTokenType::Default));
            keywordMap["summon"] = Token("summon",TokenType::PreprocessorDirectory,SubTokenInfo(SubTokenType::Default));

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
specialCharacterMap['='] = Token("=",TokenType::Operator,SubTokenInfo(SubTokenType::Assign));
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

//just use special character map
bool Lexer::isSpecialCharacter(char ch)const{
    //define all special characters that should be tokenized individually
return specialCharacterMap.find(ch) != specialCharacterMap.end();
    const string specialCharacters = ";<>{}[]:(),.";
}

bool  Lexer::isKeyword(string& tokenValue)const{
  if(keywordMap.find(tokenValue) != keywordMap.end()){
     return true;
    }else{
        return false;
    }
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

#pragma region  Tokinization
//ok for now we are going  use the regular () for func and stuff how ever at the end we shalt
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
    if(currentTokenIndex > 0 && tokens.capacity() > 0 && tokens[currentTokenIndex -1].tokenType != TokenType::WhiteSpace &&  tokens[currentTokenIndex -1].tokenType != TokenType::Comment &&  tokens[currentTokenIndex -1].tokenType != TokenType::Null_Undefined ){
        lastKnownValidToken = tokens[currentTokenIndex -1];
    }else{
    lastKnownValidToken = Token();
    }
    nextToken = peekNextToken(codeStream);
//we already modifying current token based on &
    ++currentTokenIndex;
   Token fullyClassifiedToken = classifyToken(currentToken,lastKnownValidToken,nextToken);
    //trie.insert(fullyClassifiedToken);
     tokens.push_back(fullyClassifiedToken);
    cout << "After Cheacks Our Current Lex State Is : " << currentState << " : With Returned Value Of : " << fullyClassifiedToken.tokenValue << "\n" << endl;
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
string Lexer::getDeclarationType(Token& currentToken,Token& lastKnownToken,Token& nextToken){

if(currentToken.tokenValue == "const" || currentToken.tokenValue == "var" ){
    return "Member";
}else if(isPredefinedReturnType(lastKnownToken.tokenValue) && isValidIdentifier(currentToken.tokenValue) && nextToken.tokenValue == "=" ||  nextToken.tokenValue == ";"){
   return "Member";
}else if(isPredefinedReturnType(lastKnownToken.tokenValue) && isValidIdentifier(currentToken.tokenValue) && nextToken.tokenValue == ":" ||  nextToken.tokenValue == "("){
    return "Function";
}else{
    return "Error";
}

}

bool Lexer::isPredefinedReturnType(std::string& tokenVal){
    //this needs better implementation
    if(tokenVal == "int" || tokenVal == "bool" || tokenVal == "float"|| tokenVal == "double" || tokenVal == "string" || tokenVal == "char" || tokenVal == "void"){
        return true;
    }else{
        return false;
    }
}

#pragma endregion

#pragma region  Token Classification

/*ok here is what we are going to do we need a class to store the default values of tokens the ones beign
processed by the toknize readtoken function because in some cases we might have something like > =  instead of >= which is also valid
but is actually only one token so in the defualt tken keeper we when we are classifying token and
we encounter such a case first edit the current token we are        classifying based on it and the next token
and then edit the > and = and combine them into one token in the token keeper or something like that just get it to work*/

Token Lexer::classifyToken( Token& currentToken,Token lastKnownToken,Token nextToken){
    //always start from lastknown token type
//this function will handle the actual tokenization and place tokens In Actual Token Or Use The Tier

//before we run the state based switch we decide the current state
//encounterd prob when we read float 0.255 it reads . and  255 as two seprate tokens
//for code comsumption retokening
/*
else if(currentToken.tokenValue == "+" && nextToken.tokenValue == "="){
    //this code consumption retokening is for latter
     return Token("+=",TokenType::Operator,SubTokenInfo(SubTokenType::Bit_ShiftRight));

    }
*/


switch(currentState){

    case LexerState::NormalState:{
        //note when we are in normal state anything aside from
        //class ,struct,interface,preprossesordirectiory,namespace,and the likes 
        //is an error
 if(currentToken.tokenValue == "class"){

currentState = ClassDeclaration;
lexerStateStack.push(LexerStateStack(currentState,currentToken.tokenValue));
}else{
    //also an error
      cout << "Some Error Except For Case Of " << endl;
      //make for sturct to but start with class
}

}break;
       case ClassDeclaration:{

if(lastKnownToken.tokenValue == "class" && isValidIdentifier(currentToken.tokenValue)){
    //that means current token should be an identifier
   Token returnToken = Token(currentToken.tokenValue,TokenType::Identifier,SubTokenInfo(SubTokenType::Default));

   if(symbols.getIdentifier(currentToken.tokenValue) == nullptr){
IdentifierInfo returnTokenIdentifierInfo = IdentifierInfo(IdentifierType::ClassIdentifier);
symbols.addIdentifier(returnToken.tokenValue,returnTokenIdentifierInfo);
cout << " Added Identifier " << currentToken.tokenValue << " As Class Identifier " << endl;
   }

   return returnToken;
}else if(lastKnownToken.tokenType == TokenType::Identifier && currentToken.tokenValue == ":"){
currentState = ClassInheritanceList;
}else if(currentToken.tokenValue == "{"){
    encloseSymbolsStack.push(currentToken.tokenValue[0]);
    currentState = ClassBody;

  lexerStateStack.push(LexerStateStack(currentState,currentToken.tokenValue));
}

}
break;
case ClassInheritanceList:{
//deal with this latter and remove from the tester for now just incase
}

case ClassBody:{
if(currentToken.tokenValue == "}"){
    //that means either we have reached the end of the class or there is some excess close brace
    //prbably an error
   if(!encloseSymbolsStack.empty()){
    encloseSymbolsStack.pop();
    currentState = NormalState;
    lexerStateStack.push(LexerStateStack(currentState,currentToken.tokenValue));
   }else{
    cout << "Error MissMatched Braces Too Many Braces" << endl;
   }
}

//this requires serious work
    //change state to member declaration or something
    string declarationType = getDeclarationType(currentToken,lastKnownToken,nextToken);

    if(declarationType == "Member"){
//deal with case of public in myval,int,bah,dud; latter
        currentState = MemberDeclaration;
        lexerStateStack.push(LexerStateStack(currentState,currentToken.tokenValue));
      if(currentToken.tokenType == TokenType::Identifier){

      Token returnToken = Token(currentToken.tokenValue,TokenType::Identifier,SubTokenInfo(SubTokenType::Default));

   if(symbols.getIdentifier(currentToken.tokenValue) == nullptr){
IdentifierInfo returnTokenIdentifierInfo = IdentifierInfo(IdentifierType::MemberIdentifier);
symbols.addIdentifier(returnToken.tokenValue,returnTokenIdentifierInfo);
cout << " Added Identifier " << currentToken.tokenValue << " As Member Identifier " << endl;
   }
   return returnToken;
    }
}else if(declarationType == "Function"){
        currentState = FunctionDeclaration;
        lexerStateStack.push(LexerStateStack(currentState,currentToken.tokenValue));
    if(currentToken.tokenType == TokenType::Identifier){

      Token returnToken = Token(currentToken.tokenValue,TokenType::Identifier,SubTokenInfo(SubTokenType::Default));

   if(symbols.getIdentifier(currentToken.tokenValue) == nullptr){
IdentifierInfo returnTokenIdentifierInfo = IdentifierInfo(IdentifierType::FunctionIdentifier);
symbols.addIdentifier(returnToken.tokenValue,returnTokenIdentifierInfo);
cout << " Added Identifier " << currentToken.tokenValue << " As Function Identifier " << endl;
   }

   return returnToken;
    }

}

//also at the end make cheack of stack
//nou dont cheack if its emty here because we hav diff symbols /{} : and the likes make
//better cheack algo for each diff symbol latter
if(encloseSymbolsStack.empty()){
    cout << "All Open And Close Symbols Are Matched Perfectly To Error" << endl;
      currentState = NormalState;
      lexerStateStack.push(LexerStateStack(currentState,currentToken.tokenValue));
}else{
   cout << "Error MissMatched Open And Close Symbols Too Many Open And Close Symbols" << endl;
}
}
break;

break;
case MemberList:{

}
break;
       case MemberDeclaration:{
//prob
if(currentToken.subTokenInfo.subTokenType == SubTokenType::SemiColon){
//go back to class body
currentState = ClassBody;
lexerStateStack.push(LexerStateStack(currentState,currentToken.tokenValue));
}else if(isNumeric(currentToken.tokenValue[0])){
  
auto numType = cheackNumberType(currentToken.tokenValue);
if(numType == "Integer"){
return Token(currentToken.tokenValue,TokenType::Literial,SubTokenInfo(SubTokenType::Literal_Integer));

}else if(numType == "Float"){
return Token(currentToken.tokenValue,TokenType::Literial,SubTokenInfo(SubTokenType::Literal_Float));

}else if(numType == "Double"){
return Token(currentToken.tokenValue,TokenType::Literial,SubTokenInfo(SubTokenType::Literal_Float));

}else{
cout << "Unknown Number Type";
}

}else if(lastKnownToken.tokenValue == "=" || lastKnownToken.tokenValue == "@" && currentToken.tokenValue[0] == '"'){
    //we be in string literal state or concactianation
    currentState = StringLiteral;
    lexerStateStack.push(LexerStateStack(currentState,currentToken.tokenValue));
}
    
 }

    break;
      case FunctionDeclaration:{
if(currentToken.tokenValue == ":" || currentToken.tokenValue == "("){
currentState = ParameterList;
lexerStateStack.push(LexerStateStack(currentState,currentToken.tokenValue));
encloseSymbolsStack.push(currentToken.tokenValue[0]);
if(currentToken.tokenValue == ":"){
return Token(currentToken.tokenValue,TokenType::Seprator,SubTokenInfo(SubTokenType::OpenColon));
}
}

        }  break;
      case ParameterList:{
if(currentToken.tokenValue == ":" || currentToken.tokenValue == ")" ){
    //that means we out of func param list
    currentState = FunctionBody;
    lexerStateStack.push(LexerStateStack(currentState,currentToken.tokenValue));
  if(!encloseSymbolsStack.empty()){
    encloseSymbolsStack.pop();
   }else{
    cout << "Error MissMatched Braces Too Many Braces" << endl;
   }
   if(currentToken.tokenValue == ":"){
return Token(currentToken.tokenValue,TokenType::Seprator,SubTokenInfo(SubTokenType::CloseColon));
}
//remeber diff stuff can be passed as params i.e classs and stuffin,bool,structs defined
}else if(lastKnownToken.subTokenInfo.subTokenType == SubTokenType::DataType && isValidIdentifier(currentToken.tokenValue)){
    //that means currentToken is a parameter identifier
   Token returnToken = Token( currentToken.tokenValue,TokenType::Identifier,SubTokenInfo(SubTokenType::Default));
   if(symbols.getIdentifier(returnToken.tokenValue) == nullptr){

IdentifierInfo returnTokenIdentifierInfo = IdentifierInfo(IdentifierType::ParameterIdentifier);
symbols.addIdentifier(returnToken.tokenValue,returnTokenIdentifierInfo);
cout << " Added Identifier " << currentToken.tokenValue << " As Parameter Identifier " << endl;
   }

   return returnToken;
}
//dont forget about multiple params i.e int hhh,bool dude and so on
      }

    break;
     case FunctionBody:{
        //hmmm
if(currentToken.tokenValue == "{"){

encloseSymbolsStack.push(currentToken.tokenValue[0]);
  Token returnToken = Token( currentToken.tokenValue,TokenType::Seprator,SubTokenInfo(SubTokenType::OpenBrace));
  return returnToken;
}else if(currentToken.tokenValue == "}"){

     if(!encloseSymbolsStack.empty()){
       encloseSymbolsStack.pop();
       //return to class body cause u can only have funcs within a class
        currentState = ClassBody;
        lexerStateStack.push(LexerStateStack(currentState,currentToken.tokenValue));
 }else{
cout <<  "\n Miss Matched Symbols in symbol stack";
    currentState = NormalState;
    lexerStateStack.push(LexerStateStack(currentState,currentToken.tokenValue));
 }
 
  Token returnToken = Token( currentToken.tokenValue,TokenType::Seprator,SubTokenInfo(SubTokenType::CloseBrace));
  return returnToken;
}else if(currentToken.subTokenInfo.subTokenType == SubTokenType::ControlStructure){
    currentState = Statement;
    lexerStateStack.push(LexerStateStack(currentState,currentToken.tokenValue));
    }

 }
    case VaraibleDeclaration:{

     }

    break;
    break;
    //probably replace with if,else,do,while,swich statement states i think
         case Statement:{

if(lastKnownToken.subTokenInfo.subTokenType == SubTokenType::ControlStructure && currentToken.tokenValue == ":" || currentToken.tokenValue == "("){
encloseSymbolsStack.push(currentToken.tokenValue[0]);
if( currentToken.tokenValue == ":"){
    Token returnToken = Token( currentToken.tokenValue,TokenType::Seprator,SubTokenInfo(SubTokenType::OpenColon));
}
}else if(currentToken.tokenValue == ":" || currentToken.tokenValue == ")" && nextToken.tokenValue == "{"){
  if(!encloseSymbolsStack.empty()){
       encloseSymbolsStack.pop();
      //now we have to keep tabs on the last known state so we can return to the suff
        currentState = lexerStateStack.top().currentState;
        lexerStateStack.push(LexerStateStack(currentState,currentToken.tokenValue));
 }else{
cout <<  "\n Miss Matched Symbols in symbol stack";
    currentState = NormalState;
    lexerStateStack.push(LexerStateStack(currentState,currentToken.tokenValue));
 }
if( currentToken.tokenValue == ":"){
    Token returnToken = Token( currentToken.tokenValue,TokenType::Seprator,SubTokenInfo(SubTokenType::CloseColon));
}
}



         }

    break;
          case StringLiteral:{

          }

    break;
         case EndOfStatement:{

         }

    break;
       case NameSpaceDeclaration:{

         }

    break;
       case NameSpaceBody:{

         }

    break;

}

return Token(currentToken.tokenValue,currentToken.tokenType,currentToken.subTokenInfo);
}



#pragma endregion

#pragma region  Extra Important Token Functions


 void Lexer::handleError(){

 }

#pragma endregion



#pragma endregion
