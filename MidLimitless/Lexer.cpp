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
#include "Utilities.cpp"
#include "Error.cpp"

using namespace std;
#pragma region Actual Lexer Calss
 Lexer::Lexer (){

 //stores the current state of the lexer
  currentState = LexerState::NormalState;
 //stores the symbol table for identifiers

 //gotta figure out the column num increment

currentTokenIndex = 0;
lenientMode = true;
currentLineNumber = 1;
currentColumnNumber = 1;
        tokens.clear();
        InitializeKeywordMap();
        InitializeSpecialCharacterMap();
        symbols.enterScope();
        //empty stack
        lexerStateStack = stack<LexerStateStack>();
     //stack<LexerStateStack>().swap(lexerStateStack);
    //this just for test
  
        // ... add other keywords and token types ...

        // Populate the trie with the keywords
        for (const auto& pair : keywordMap) {
         trie.insert(Token(pair.first, TokenType::Keyword, pair.second.subTokenInfo,pair.second.locationInfo));
        }
           for (const auto& pair : specialCharacterMap) {
           trie.insert(Token(string(1,pair.first), TokenType::Keyword, pair.second.subTokenInfo,pair.second.locationInfo));
        }
    }

#pragma region Token lookAheadBuffer
Token Lexer::readNextToken(istringstream& codeStream){
    char ch;
    string line;
    string tokenValue;
   

    //make sure token value is not empty
while(codeStream.get(ch)){
if(isspace(ch)){
 return readNextToken(codeStream);
}else if(ch == '\n'){
currentLineNumber ++;
currentColumnNumber = 1;
}else if(ch == '\t'){
currentColumnNumber += 4;
    }else if(isSpecialCharacter(ch)){
        if(ch =='.' && Utilities().isNumeric(codeStream.peek())){
tokenValue += ch;
        }else if(isCombinableSpecialCharacter(ch) && isCombinableSpecialCharacter(codeStream.peek())){
      string newVal;
      newVal += ch;
      newVal += codeStream.peek();

      if(isKeyword(newVal)){
codeStream.get();
return keywordMap.at(newVal);
      }
//comback and handle string literals latter
    }else if(ch == '"' || ch == '\''){
  return processStringLiteral(codeStream,ch);
  
    }else if(ch == '/' ){
    if(codeStream.peek() == '/'){
            //single line comment
        
      return processComment(codeStream,false);
   
      }else if(codeStream.peek() == '*'){
        //multiline comment
        return processComment(codeStream,true);
    
        }else{
           return specialCharacterMap.at(ch);
        }
//there should be another else for incomplete comment
    }else{      
 return specialCharacterMap.at(ch);
 }

//there is the case of . in float
}else{
    if(!tokenValue.empty()){
        tokenValue += ch;
break;
    }else{
tokenValue += ch;
    }

}
currentColumnNumber ++;
}


//pretty sure this is for putting stuff back into the codeStream
 while(codeStream.get(ch)){
if(isspace(ch) || isSpecialCharacter(ch)){
 
codeStream.unget();
break;

}else{
    tokenValue += ch;
}

//handling floats with optional e and stuff



}

/* i donno keep
 if(ch =='.' && Utilities().isNumeric(codeStream.peek())){
        tokenValue += ch;
        while(codeStream.get(ch) && Utilities().isNumeric(ch)){
            tokenValue += ch;
        }
        if(ch == 'e' || ch == 'E'){
            tokenValue += ch;
            if(codeStream.peek() == '-' || codeStream.peek() == '+'){
                tokenValue += codeStream.get();
            }
            while(codeStream.get(ch) && Utilities().isNumeric(ch)){
                tokenValue += ch;
            }
        }
        if(ch == 'f' || ch == 'F'){
            tokenValue += ch;
        }else{
            codeStream.unget();
        }
    }*/
//can change tis to determine default token
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
       bool isIdentifier = Utilities().isValidIdentifier(tokenValue);
       string inNumericLiteral = Utilities().cheackNumberType(tokenValue);

    if(isIdentifier){
    return Token(tokenValue,TokenType::Identifier,SubTokenInfo(),TokenLocationInfo(currentLineNumber,currentColumnNumber));
    }else if(inNumericLiteral == "Integer"){
    return Token(tokenValue,TokenType::Literial,SubTokenInfo(SubTokenType::Literal_Integer),TokenLocationInfo(currentLineNumber,currentColumnNumber));
    }else if( inNumericLiteral == "Float"){
    return Token(tokenValue,TokenType::Literial,SubTokenInfo(SubTokenType::Literal_Float),TokenLocationInfo(currentLineNumber,currentColumnNumber));
    }else if(inNumericLiteral == "Double"){
    //literal double
    return Token(tokenValue,TokenType::Literial,SubTokenInfo(SubTokenType::Literal_Float),TokenLocationInfo(currentLineNumber,currentColumnNumber));
    }else{
    return Token();
    }

    }
}

#pragma endregion

#pragma region  Helper Functionsh

void Lexer::InitializeKeywordMap(){
 // Initialize the keywordMap map with keywords and their subtoken info
//note only add keywords with one meaning no matter the context
 keywordMap["public"] = Token("public",TokenType::AccessModifier,SubTokenInfo(SubTokenType::Default),TokenLocationInfo(currentLineNumber,currentColumnNumber));
 keywordMap["private"] = Token("private",TokenType::AccessModifier,SubTokenInfo(SubTokenType::Default),TokenLocationInfo(currentLineNumber,currentColumnNumber));
 keywordMap["protected"] = Token("protected",TokenType::AccessModifier,SubTokenInfo(SubTokenType::Default),TokenLocationInfo(currentLineNumber,currentColumnNumber));
keywordMap["internal"] = Token("internal",TokenType::AccessModifier,SubTokenInfo(SubTokenType::Default),TokenLocationInfo(currentLineNumber,currentColumnNumber));
keywordMap["external"] = Token("external",TokenType::AccessModifier,SubTokenInfo(SubTokenType::Default),TokenLocationInfo(currentLineNumber,currentColumnNumber));
keywordMap["shegenYaro"] = Token("shegenYaro",TokenType::AccessModifier,SubTokenInfo(SubTokenType::Default),TokenLocationInfo(currentLineNumber,currentColumnNumber));
//cn = continue but for func declaration
keywordMap["cn"] = Token("cn",TokenType::Keyword,SubTokenInfo(SubTokenType::Default),TokenLocationInfo(currentLineNumber,currentColumnNumber));
/*


public internal bool returnMe:bool the:{
    internal bool returnme;
    some other stufff;
    Mlog.Out:"Me Is Awsome":

}
*/


        keywordMap["void"] = Token("void",TokenType::Keyword,SubTokenInfo(SubTokenType::ReturnType),TokenLocationInfo(currentLineNumber,currentColumnNumber));

        keywordMap["class"] = Token("class",TokenType::Keyword,SubTokenInfo(SubTokenType::Default),TokenLocationInfo(currentLineNumber,currentColumnNumber));

        keywordMap["int"] = Token("int",TokenType::Keyword,SubTokenInfo(SubTokenType::DataType),TokenLocationInfo(currentLineNumber,currentColumnNumber));
        keywordMap["bool"] = Token("bool",TokenType::Keyword,SubTokenInfo(SubTokenType::DataType),TokenLocationInfo(currentLineNumber,currentColumnNumber));
        keywordMap["char"] = Token("char",TokenType::Keyword,SubTokenInfo(SubTokenType::DataType),TokenLocationInfo(currentLineNumber,currentColumnNumber));
        keywordMap["string"] = Token("string",TokenType::Keyword,SubTokenInfo(SubTokenType::DataType),TokenLocationInfo(currentLineNumber,currentColumnNumber));
        keywordMap["float"] = Token("float",TokenType::Keyword,SubTokenInfo(SubTokenType::DataType),TokenLocationInfo(currentLineNumber,currentColumnNumber));
        keywordMap["double"] = Token("double",TokenType::Keyword,SubTokenInfo(SubTokenType::DataType),TokenLocationInfo(currentLineNumber,currentColumnNumber));
        keywordMap["var"] = Token("var",TokenType::Keyword,SubTokenInfo(SubTokenType::DataType),TokenLocationInfo(currentLineNumber,currentColumnNumber));
        keywordMap["virtual"] = Token("virtual",TokenType::Keyword,SubTokenInfo(SubTokenType::FunctionModifier),TokenLocationInfo(currentLineNumber,currentColumnNumber));
        keywordMap["abstract"] = Token("abstract",TokenType::Keyword,SubTokenInfo(SubTokenType::FunctionModifier),TokenLocationInfo(currentLineNumber,currentColumnNumber));


            keywordMap["=="] = Token("==",TokenType::Operator,SubTokenInfo(SubTokenType::EqualTo),TokenLocationInfo(currentLineNumber,currentColumnNumber));
            keywordMap["++"] = Token("++",TokenType::Operator,SubTokenInfo(SubTokenType::Increment),TokenLocationInfo(currentLineNumber,currentColumnNumber));
            keywordMap["--"] = Token("--",TokenType::Operator,SubTokenInfo(SubTokenType::Decrement),TokenLocationInfo(currentLineNumber,currentColumnNumber));
            keywordMap["+="] = Token("+=",TokenType::Operator,SubTokenInfo(SubTokenType::Add_Assign),TokenLocationInfo(currentLineNumber,currentColumnNumber));
            keywordMap["-="] = Token("-=",TokenType::Operator,SubTokenInfo(SubTokenType::Sub_Assign),TokenLocationInfo(currentLineNumber,currentColumnNumber));
            keywordMap["*="] = Token("*=",TokenType::Operator,SubTokenInfo(SubTokenType::Mul_Assign),TokenLocationInfo(currentLineNumber,currentColumnNumber));
            keywordMap["/="] = Token("/=",TokenType::Operator,SubTokenInfo(SubTokenType::Div_Assign),TokenLocationInfo(currentLineNumber,currentColumnNumber));
            keywordMap["%="] = Token("%=",TokenType::Operator,SubTokenInfo(SubTokenType::Mod_Assign),TokenLocationInfo(currentLineNumber,currentColumnNumber));
            keywordMap[">="] = Token(">=",TokenType::Operator,SubTokenInfo(SubTokenType::GreaterThanOrEqualTo),TokenLocationInfo(currentLineNumber,currentColumnNumber));
            keywordMap["<="] = Token("<=",TokenType::Operator,SubTokenInfo(SubTokenType::LessThanOrEqualTo),TokenLocationInfo(currentLineNumber,currentColumnNumber));
            //random range
            keywordMap[">_<"] = Token(">_<",TokenType::Operator,SubTokenInfo(SubTokenType::Mod_Assign),TokenLocationInfo(currentLineNumber,currentColumnNumber));

            keywordMap["if"] = Token("if",TokenType::Keyword,SubTokenInfo(SubTokenType::ControlStructure),TokenLocationInfo(currentLineNumber,currentColumnNumber));
            keywordMap["else"] = Token("else",TokenType::Keyword,SubTokenInfo(SubTokenType::ControlStructure),TokenLocationInfo(currentLineNumber,currentColumnNumber));
            keywordMap["do"] = Token("do",TokenType::Keyword,SubTokenInfo(SubTokenType::ControlStructure),TokenLocationInfo(currentLineNumber,currentColumnNumber));
            keywordMap["while"] = Token("while",TokenType::Keyword,SubTokenInfo(SubTokenType::ControlStructure),TokenLocationInfo(currentLineNumber,currentColumnNumber));
            keywordMap["for"] = Token("for",TokenType::Keyword,SubTokenInfo(SubTokenType::ControlStructure),TokenLocationInfo(currentLineNumber,currentColumnNumber));
            keywordMap["foreach"] = Token("foreach",TokenType::Keyword,SubTokenInfo(SubTokenType::ControlStructure),TokenLocationInfo(currentLineNumber,currentColumnNumber));

            keywordMap["struct"] = Token("struct",TokenType::Keyword,SubTokenInfo(SubTokenType::DataStructure),TokenLocationInfo(currentLineNumber,currentColumnNumber));

            keywordMap["use"] = Token("use",TokenType::PreprocessorDirectory,SubTokenInfo(SubTokenType::Default),TokenLocationInfo(currentLineNumber,currentColumnNumber));
            keywordMap["include"] = Token("include",TokenType::PreprocessorDirectory,SubTokenInfo(SubTokenType::Default),TokenLocationInfo(currentLineNumber,currentColumnNumber));
            keywordMap["summon"] = Token("summon",TokenType::PreprocessorDirectory,SubTokenInfo(SubTokenType::Default),TokenLocationInfo(currentLineNumber,currentColumnNumber));

            keywordMap["false"] = Token("false",TokenType::Literial,SubTokenInfo(SubTokenType::Literal_Bool),TokenLocationInfo(currentLineNumber,currentColumnNumber));
            keywordMap["true"] = Token("true",TokenType::Literial,SubTokenInfo(SubTokenType::Literal_Bool),TokenLocationInfo(currentLineNumber,currentColumnNumber));


}

void Lexer::InitializeSpecialCharacterMap(){

//deal with ' "" individually later use slice to return the actual text within the coats and not with the coats
//themselves

    //and as for these seprators sure they can have diffrent meaning
    //but still is best to know that these are seprators
specialCharacterMap['('] = Token("(",TokenType::Seprator,SubTokenInfo(SubTokenType::OpenParen),TokenLocationInfo(currentLineNumber,currentColumnNumber));
specialCharacterMap[')'] = Token(")",TokenType::Seprator,SubTokenInfo(SubTokenType::CloseParen),TokenLocationInfo(currentLineNumber,currentColumnNumber));
specialCharacterMap['{'] = Token("{",TokenType::Seprator,SubTokenInfo(SubTokenType::OpenBrace),TokenLocationInfo(currentLineNumber,currentColumnNumber));
specialCharacterMap['}'] = Token("}",TokenType::Seprator,SubTokenInfo(SubTokenType::CloseBrace),TokenLocationInfo(currentLineNumber,currentColumnNumber));
specialCharacterMap['['] = Token("[",TokenType::Seprator,SubTokenInfo(SubTokenType::OpenBracket),TokenLocationInfo(currentLineNumber,currentColumnNumber));
specialCharacterMap[']'] = Token("]",TokenType::Seprator,SubTokenInfo(SubTokenType::CloseBracket),TokenLocationInfo(currentLineNumber,currentColumnNumber));
specialCharacterMap[':'] = Token(":",TokenType::Seprator,SubTokenInfo(SubTokenType::Default),TokenLocationInfo(currentLineNumber,currentColumnNumber));
specialCharacterMap['"'] = Token("\"",TokenType::Seprator,SubTokenInfo(SubTokenType::Default),TokenLocationInfo(currentLineNumber,currentColumnNumber));
specialCharacterMap['\''] = Token("'",TokenType::Seprator,SubTokenInfo(SubTokenType::Default),TokenLocationInfo(currentLineNumber,currentColumnNumber));
specialCharacterMap[';'] = Token(";",TokenType::Seprator,SubTokenInfo(SubTokenType::SemiColon),TokenLocationInfo(currentLineNumber,currentColumnNumber));
specialCharacterMap[','] = Token(",",TokenType::Seprator,SubTokenInfo(SubTokenType::Comma),TokenLocationInfo(currentLineNumber,currentColumnNumber));
specialCharacterMap['='] = Token("=",TokenType::Operator,SubTokenInfo(SubTokenType::Assign),TokenLocationInfo(currentLineNumber,currentColumnNumber));
specialCharacterMap['+'] = Token("+",TokenType::Operator,SubTokenInfo(SubTokenType::Addition),TokenLocationInfo(currentLineNumber,currentColumnNumber));
specialCharacterMap['-'] = Token("-",TokenType::Operator,SubTokenInfo(SubTokenType::Subtract),TokenLocationInfo(currentLineNumber,currentColumnNumber));
specialCharacterMap['*'] = Token("*",TokenType::Operator,SubTokenInfo(SubTokenType::Multiply),TokenLocationInfo(currentLineNumber,currentColumnNumber));
specialCharacterMap['/'] = Token("/",TokenType::Operator,SubTokenInfo(SubTokenType::Divide),TokenLocationInfo(currentLineNumber,currentColumnNumber));
specialCharacterMap['%'] = Token("%",TokenType::Operator,SubTokenInfo(SubTokenType::Modulos),TokenLocationInfo(currentLineNumber,currentColumnNumber));
specialCharacterMap['>'] = Token(">",TokenType::Operator,SubTokenInfo(SubTokenType::GreaterThan),TokenLocationInfo(currentLineNumber,currentColumnNumber));
specialCharacterMap['<'] = Token("<",TokenType::Operator,SubTokenInfo(SubTokenType::LessThan),TokenLocationInfo(currentLineNumber,currentColumnNumber));
specialCharacterMap['~'] = Token("~",TokenType::SpecialSymbol,SubTokenInfo(SubTokenType::FunctionStarter),TokenLocationInfo(currentLineNumber,currentColumnNumber));
//now when we encounter ~ we expect a function defination either a modifier or a return type

}




Token Lexer::processComment(istringstream& codeStream, bool isMultiLine){
    try{
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
        //consume second /
        while(codeStream.get(ch) && ch != '\n'){
            comment += ch;
        }
    }

   return Token(comment,TokenType::Comment,SubTokenInfo(),TokenLocationInfo(currentLineNumber,currentColumnNumber));
    }catch(exception e){
        cout << e.what() << "\n";
    }
}



Token Lexer::processStringLiteral(istringstream& codeStream,char& quoteType){
    try{
    string tokenValue(1,quoteType);
    char ch;

    while(codeStream.get(ch)){
        tokenValue += ch;
        if(ch == '\\'){
            if(codeStream.get(ch)){
                tokenValue += ch;
            }
        }else if(ch == quoteType){
                //end of literal
                break;
            }
    }
    
    return Token(tokenValue,TokenType::Literial,SubTokenInfo(SubTokenType::Literal_String),TokenLocationInfo(currentLineNumber,currentColumnNumber));
    }catch(exception e){
        std::cout << e.what() << "\n";
          return Token();
    }
    
}


//just use special character map
bool Lexer::isSpecialCharacter(char ch)const{
    //define all special characters that should be tokenized individually
return specialCharacterMap.find(ch) != specialCharacterMap.end();
}


bool Lexer::isCombinableSpecialCharacter(char ch)const{
    //define all special characters that should be tokenized individually
    regex pattern("[-*%=+><]");
    bool isMatch = regex_match(string(1,ch),pattern);
    return isMatch;
}



bool  Lexer::isKeyword(string& tokenValue)const{
  if(keywordMap.find(tokenValue) != keywordMap.end()){
     return true;
    }else{
        return false;
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
//& = ref * = pointer
while(codeStream){
    currentToken = readNextToken(codeStream);

    if(currentTokenIndex > 0 && tokens.capacity() > 0 && tokens[currentTokenIndex -1].tokenType != TokenType::WhiteSpace &&  tokens[currentTokenIndex -1].tokenType != TokenType::Comment &&  tokens[currentTokenIndex -1].tokenType != TokenType::Null_Undefined ){
        lastKnownValidToken = tokens[currentTokenIndex -1];
    }else{
    lastKnownValidToken = Token();
    }
    nextToken = peekNextToken(codeStream);

   Token fullyClassifiedToken = classifyToken(currentToken,lastKnownValidToken,nextToken);
    //trie.insert(fullyClassifiedToken);
     tokens.push_back(fullyClassifiedToken);

    ++currentTokenIndex;

    cout << "After Cheacks Our Current Lex State Is : " << currentState << " : With Returned Value Of : " << fullyClassifiedToken.tokenValue << "\n" << endl;
   
}
//push an end of file token at end of code stream
tokens.push_back(Token("end",TokenType::EndOfFile,SubTokenInfo(),TokenLocationInfo(currentLineNumber,currentColumnNumber)));



  return tokens;

}

bool Lexer::isValidFunctionReturnType(Token token){
if(token.subTokenInfo.subTokenType == SubTokenType::DataType || token.subTokenInfo.subTokenType == SubTokenType::ReturnType || token.tokenValue == "void"){
return true;
}else{
    return false;
}
}

bool Lexer::isMemberDeclaration(Token currentToken,Token lastKnownToken,Token nextToken){

if(currentToken.tokenValue == "const" || currentToken.tokenValue == "var" ){
    return true;
}else if( lastKnownToken.tokenType == TokenType::AccessModifier && isPredefinedReturnType(currentToken.tokenValue)){
   return true;
}else if(isPredefinedReturnType(lastKnownToken.tokenValue) && Utilities().isValidIdentifier(currentToken.tokenValue)){
   return true;
}else{
    return false;
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

Token Lexer::tryGetDoubleCombinableToken(Token firstToken,Token secondToken){
string newTokenVal;
newTokenVal += firstToken.tokenValue[0];
newTokenVal += secondToken.tokenValue[0];

  cout << "Quick Look " << newTokenVal << "\n";
//mhh due to the fact we got operators with 3 chars we cant just use current,nexttoken,we gotta use last to
 if(isKeyword(newTokenVal)){
    //also find a way to remove the currentToken or both current Token And next,last token to make sure only the combine 
    //one gets returned may be use a pointer or something
   
   return keywordMap.at(newTokenVal);
    }else{
return Token();
    }

}

#pragma endregion

#pragma region  Token Classification

/*ok here is what we are going to do we need a class to store the default values of tokens the ones beign
processed by the toknize readtoken function because in some cases we might have something like > =  instead of >= which is also valid
but is actually only one token so in the defualt tken keeper we when we are classifying token and
we encounter such a case first edit the current token we are        classifying based on it and the next token
and then edit the > and = and combine them into one token in the token keeper or something like that just get it to work*/

Token Lexer::classifyToken( Token& currentToken,Token& lastKnownToken,Token& nextToken){
    //always start from lastknown token type
//this function will handle the actual tokenization and place tokens In Actual Token Or Use The Tier

//for code comsumption retokening
//already doing +,= += thing in processToken func


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

if(lastKnownToken.tokenValue == "class" && Utilities().isValidIdentifier(currentToken.tokenValue)){
    //that means current token should be an identifier
   Token returnToken = Token(currentToken.tokenValue,TokenType::Identifier,SubTokenInfo(SubTokenType::Default),TokenLocationInfo(currentLineNumber,currentColumnNumber));

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
}else if(currentToken.tokenValue == "~"){
    //then we expect a function
    currentState = FunctionDeclaration;
    lexerStateStack.push(LexerStateStack(currentState,currentToken.tokenValue));
}else if(isMemberDeclaration(currentToken,lastKnownToken,nextToken) ){
    //is member
       currentState = MemberDeclaration;
        lexerStateStack.push(LexerStateStack(currentState,currentToken.tokenValue));
      if(currentToken.tokenType == TokenType::Identifier){

Token returnToken = Token(currentToken.tokenValue,TokenType::Identifier,SubTokenInfo(SubTokenType::Default),TokenLocationInfo(currentLineNumber,currentColumnNumber));

if(symbols.getIdentifier(currentToken.tokenValue) == nullptr){
IdentifierInfo returnTokenIdentifierInfo = IdentifierInfo(IdentifierType::MemberIdentifier);
symbols.addIdentifier(returnToken.tokenValue,returnTokenIdentifierInfo);
cout << " Added Identifier " << currentToken.tokenValue << " As Member Identifier " << endl;
   }else{
    //retrive identifier value and type from stack
    auto retrivedId = symbols.getIdentifier(currentToken.tokenValue);
    cout << " Retrived Identifier From Stack " << retrivedId->identifierType << endl;
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
//make better identifier cheack
if(Utilities().isValidIdentifier(currentToken.tokenValue) && nextToken.tokenValue == "="){

Token returnToken = Token(currentToken.tokenValue,TokenType::Identifier,SubTokenInfo(SubTokenType::Default),TokenLocationInfo(currentLineNumber,currentColumnNumber));

if(symbols.getIdentifier(currentToken.tokenValue) == nullptr){
IdentifierInfo returnTokenIdentifierInfo = IdentifierInfo(IdentifierType::MemberIdentifier);
symbols.addIdentifier(returnToken.tokenValue,returnTokenIdentifierInfo);
cout << " Added Identifier " << currentToken.tokenValue << " As Member Identifier " << endl;
   }else{
    //retrive identifier value and type from stack
    auto retrivedId = symbols.getIdentifier(currentToken.tokenValue);
    cout << " Retrived Identifier From Stack " << retrivedId->identifierType << endl;
   }

   return returnToken;
}else if(currentToken.subTokenInfo.subTokenType == SubTokenType::SemiColon){
//go back to class body
currentState = ClassBody;
lexerStateStack.push(LexerStateStack(currentState,currentToken.tokenValue));
}else if(Utilities().isNumeric(currentToken.tokenValue[0])){
  
auto numType = Utilities().cheackNumberType(currentToken.tokenValue);
if(numType == "Integer"){
return Token(currentToken.tokenValue,TokenType::Literial,SubTokenInfo(SubTokenType::Literal_Integer),TokenLocationInfo(currentLineNumber,currentColumnNumber));

}else if(numType == "Float"){
return Token(currentToken.tokenValue,TokenType::Literial,SubTokenInfo(SubTokenType::Literal_Float),TokenLocationInfo(currentLineNumber,currentColumnNumber));

}else if(numType == "Double"){
return Token(currentToken.tokenValue,TokenType::Literial,SubTokenInfo(SubTokenType::Literal_Double),TokenLocationInfo(currentLineNumber,currentColumnNumber));

}else{
cout << "Unknown Number Type";
}

}else if(currentToken.tokenValue == "false" || currentToken.tokenValue == "true"){

return Token(currentToken.tokenValue,TokenType::Literial,SubTokenInfo(SubTokenType::Literal_Bool),TokenLocationInfo(currentLineNumber,currentColumnNumber));

 }else if(currentToken.subTokenInfo.subTokenType == SubTokenType::Literal_String){
 return Token(currentToken.tokenValue,TokenType::Literial,SubTokenInfo(SubTokenType::Literal_String),TokenLocationInfo(currentLineNumber,currentColumnNumber));
 }


//fix this place better
    

 }

    break;
      case FunctionDeclaration:{
        
if(Utilities().isValidIdentifier(currentToken.tokenValue) && nextToken.tokenValue == ":" || nextToken.tokenValue == "(" ){

Token returnToken = Token(currentToken.tokenValue,TokenType::Identifier,SubTokenInfo(SubTokenType::Default),TokenLocationInfo(currentLineNumber,currentColumnNumber));

if(symbols.getIdentifier(currentToken.tokenValue) == nullptr){
IdentifierInfo returnTokenIdentifierInfo = IdentifierInfo(IdentifierType::FunctionIdentifier);
symbols.addIdentifier(returnToken.tokenValue,returnTokenIdentifierInfo);
cout << " Added Identifier " << currentToken.tokenValue << " As Function Identifier " << endl;
   }else{
    //retrive identifier value and type from stack
    auto retrivedId = symbols.getIdentifier(currentToken.tokenValue);
    cout << " Retrived Identifier From Stack " << retrivedId->identifierType << endl;
   }

   return returnToken;
}else if(currentToken.tokenValue == ":" || currentToken.tokenValue == "("){
currentState = ParameterList;
lexerStateStack.push(LexerStateStack(currentState,currentToken.tokenValue));
encloseSymbolsStack.push(currentToken.tokenValue[0]);
if(currentToken.tokenValue == ":"){
return Token(currentToken.tokenValue,TokenType::Seprator,SubTokenInfo(SubTokenType::OpenColon),TokenLocationInfo(currentLineNumber,currentColumnNumber));
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
return Token(currentToken.tokenValue,TokenType::Seprator,SubTokenInfo(SubTokenType::CloseColon),TokenLocationInfo(currentLineNumber,currentColumnNumber));
}
//remeber diff stuff can be passed as params i.e classs and stuffin,bool,structs defined
}else if(lastKnownToken.subTokenInfo.subTokenType == SubTokenType::DataType && Utilities().isValidIdentifier(currentToken.tokenValue)){
    //that means currentToken is a parameter identifier
   Token returnToken = Token( currentToken.tokenValue,TokenType::Identifier,SubTokenInfo(SubTokenType::Default),TokenLocationInfo(currentLineNumber,currentColumnNumber));
   if(symbols.getIdentifier(returnToken.tokenValue) == nullptr){

IdentifierInfo returnTokenIdentifierInfo = IdentifierInfo(IdentifierType::ParameterIdentifier);
symbols.addIdentifier(returnToken.tokenValue,returnTokenIdentifierInfo);
cout << " Added Identifier " << currentToken.tokenValue << " As Parameter Identifier " << endl;
   }

   return returnToken;
}else if(currentToken.tokenValue == "{"){
    //basically for case of ~public void main:byte val{}
  currentState = FunctionBody;
    lexerStateStack.push(LexerStateStack(currentState,currentToken.tokenValue));
  if(!encloseSymbolsStack.empty()){
    encloseSymbolsStack.pop();
   }else{
    cout << "Error MissMatched Braces Too Many Braces" << endl;
   }
}
//dont forget about multiple params i.e int hhh,bool dude and so on
      }

    break;
     case FunctionBody:{
        //hmmm
if(currentToken.tokenValue == "{"){
encloseSymbolsStack.push(currentToken.tokenValue[0]);
  Token returnToken = Token( currentToken.tokenValue,TokenType::Seprator,SubTokenInfo(SubTokenType::OpenBrace),TokenLocationInfo(currentLineNumber,currentColumnNumber));
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
 
  Token returnToken = Token( currentToken.tokenValue,TokenType::Seprator,SubTokenInfo(SubTokenType::CloseBrace),TokenLocationInfo(currentLineNumber,currentColumnNumber));
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
    Token returnToken = Token( currentToken.tokenValue,TokenType::Seprator,SubTokenInfo(SubTokenType::OpenColon),TokenLocationInfo(currentLineNumber,currentColumnNumber));
}
}else if(currentToken.tokenValue == ":" || currentToken.tokenValue == ")" && nextToken.tokenValue == "{"){

currentState = StatementBody;
lexerStateStack.push(LexerStateStack(currentState,currentToken.tokenValue));

if( currentToken.tokenValue == ":"){
    Token returnToken = Token( currentToken.tokenValue,TokenType::Seprator,SubTokenInfo(SubTokenType::CloseColon),TokenLocationInfo(currentLineNumber,currentColumnNumber));
}
  if(!encloseSymbolsStack.empty()){
       encloseSymbolsStack.pop();
 }else{
cout <<  "\n Miss Matched Symbols in symbol stack";
    currentState = NormalState;
    lexerStateStack.push(LexerStateStack(currentState,currentToken.tokenValue));
 }
}else if(currentToken.tokenValue == "{"){
    encloseSymbolsStack.push(currentToken.tokenValue[0]);
    //that means this be else statement
    currentState = StatementBody;
lexerStateStack.push(LexerStateStack(currentState,currentToken.tokenValue));

}

}

    break;
    case StatementBody:{

        if(currentToken.tokenValue == "{"){
            encloseSymbolsStack.push(currentToken.tokenValue[0]);
        }else if(currentToken.tokenValue == "}"){
//that means we out of state body however we need to pop twice since last state is statement not func body
//so gotta make method to double pop
//also symbol stack
    currentState = getValidLastKnownLexerStateDoublePop(lexerStateStack).currentState;
      lexerStateStack.push(LexerStateStack(currentState,currentToken.tokenValue));
    
        }
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
        case ErrorState:{
//in error state try find way to fix error make recond=mendation to fix error then go back to privious state
//maybe after \n or ; ) } u get the gist
          }

    break;

}

return Token(currentToken.tokenValue,currentToken.tokenType,currentToken.subTokenInfo,TokenLocationInfo(currentLineNumber,currentColumnNumber));
}


#pragma endregion

#pragma region  Extra Important Token Functions

 LexerStateStack Lexer::getValidLastKnownLexerState(stack<LexerStateStack>& lexerStateStack){

lexerStateStack.pop();
return lexerStateStack.top();

 }
 
  LexerStateStack Lexer::getValidLastKnownLexerStateDoublePop(stack<LexerStateStack>& lexerStateStack){
lexerStateStack.pop();
lexerStateStack.pop();
return lexerStateStack.top();

 }
vector<LexerStateStack> Lexer::returnAllLexerStatesInAccendingOrder(stack<LexerStateStack>& lexerStateStack){
    vector<LexerStateStack> lexVector;
    while(!lexerStateStack.empty()){
lexVector.emplace_back(lexerStateStack.top());
    }
    return lexVector;

}

 void Lexer::handleError(){

 }

#pragma endregion



#pragma endregion
