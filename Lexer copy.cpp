#pragma region PreprocessorDirectory
#include <iostream>
#include <string>
#include <unordered_map>
#include <stack>
#include <string>
#include <memory>
#include <vector>
#include <cctype>
#include <sstream>
#include<variant>
#pragma endregion

#pragma region  NameSpaces

using namespace std;
#pragma endregion

#pragma region Enums

//data type , return type fall under keywords
    enum TokenType{
        Null_Undefined,
        Keyword,
        Identifier,
        Literial,
        Operator,
        OperatorWithSpecialMeaning,
        Punctuation,Seprator,
        AccessModifier,
        WhiteSpace,
        Comment,
        PreprocessorDirectory,
        SpecialSymbol,
        EscapeSequence,
        Annotation_Attribute,
   
        Function_MethodCall,
        UserDefinedToken
    };


    const char* TokenTypeToString(TokenType tokenType){
switch(tokenType){
    case Keyword: return "Keyword";
    case Identifier: return "Identifier";
        case Literial: return "Literal";
        case Operator: return "Operator";
        case OperatorWithSpecialMeaning: return "OperatorWithSpecialMeaning";
        case Punctuation: return "Punctuation";
        case Seprator: return "Seprator";
    case AccessModifier: return "AccessModifier";
        case WhiteSpace: return "WhiteSpace";
        case Comment: return "Comment";
        case PreprocessorDirectory: return "PreprocessorDirectory";
        case SpecialSymbol: return "SpecialSymbol";
        case EscapeSequence: return "EscapeSequence";
     case Annotation_Attribute: return "Annotation_Attribute";
    

        case Null_Undefined: return "Null_Undefined";
        case Function_MethodCall: return "Function_MethodCall";
        case UserDefinedToken: return "UserDefinedToken";
      
    

    
}
    }

enum SubTokenType{
    //the default value
    Default,
    //the keyword type
    DataStructure,
    CustomDataStructure,
    ControlStructure,
    //the type type
    DataType,
    ReturnType,
    //the seprator type
    OpenParen,
    CloseParen,
    OpenBrace,
    CloseBrace,
    OpenBracket,
    CloseBracket,
    OpenColon,
    CloseColon,
    SemiColon,
    Comma,
    Elipses,
    PointerMemberAccess,
    //the operator type
    ArithmeticOperator,
    BitwiseOperator,
    ComparisonOperator,
    LogicalOperator,
 //arithmetic operator
  Multiply,
  Mul_Assign,
  Divide,
  Div_Assign,
  Modulos,
  Mod_Assign,
  Addition,
  Add_Assign,
  Subtract,
  Sub_Assign,
  //special operators
  Assign,
  Increment,
  Decrement,
  //bitwise operators
  Bit_And,
  Bit_Or,
  Bit_Not,
  Bit_Xor,
  Bit_ShiftRight,
  Bit_ShiftLeft,
//logical Opertors
Logic_And,
Logic_Or,
Logic_Not,
//comparision operators
GreaterThan,
GreaterThanOrEqualTo,
LessThan,
LessThanOrEqualTo,
//==
EqualTo,
NotEqualTo,

    //the comment type
   SingleLineComment,
    MultiLineComment,
    ContiniousComment,
    //the modifier type i.e abstract ,virtual,static
    ClassModifier,
    FunctionModifier,


    //literal type
    Literal_String,
    Literal_Float,
    Literal_Integer,
    Literal_Character
};
//note : is colon, ; is semi colon also called terminator note for our programming language ;,},: can also act
//as statement terminators
    const char* SubTokenTypeToString(SubTokenType subTokenType){
switch(subTokenType){
    case Default: return "Default";

    case DataStructure: return "DataStructure";
        case CustomDataStructure: return "CustomDataStructure";
        case ControlStructure: return "ControlStructure";

        case DataType: return "DataType";
        case ReturnType: return "ReturnType";

        case OpenParen: return "OpenParen";
     case CloseParen: return "CloseParen";
    case OpenBrace: return "OpenBrace";
        case CloseBrace: return "CloseBrace";
        case OpenBracket: return "OpenBracket";
        case CloseBracket: return "CloseBracket";
        case OpenColon: return "OpenColon";
        case CloseColon: return "CloseColon";
     case SemiColon: return "SemiColon";
      case Comma: return "Comma";
        case Elipses: return "Elipses";
        case PointerMemberAccess: return "PointerMemberAccess";
    

        case ArithmeticOperator: return "ArithmeticOperator";
        case LogicalOperator: return "LogicalOperator";
        case BitwiseOperator: return "BitwiseOperator";
      
    case ComparisonOperator: return "ComparisonOperator";

        case SingleLineComment: return "SingleLineComment";
        case MultiLineComment: return "DoubleLineComment";
        case ContiniousComment: return "ContiniousCommet";
      
      
        case Multiply: return "Multiplay";
        case Mul_Assign: return "Mul_Assign";
        case Addition: return "Addition";
     case Add_Assign: return "Add_Aassign";
    case Subtract: return "Subtract";
        case Sub_Assign: return "Aub_Assign";
        case Divide: return "Divide";
        case Div_Assign: return "Dive_Assign";
        case Modulos: return "Modulos";
        case Mod_Assign: return "Mod_Assign";
     case Assign: return "Assign";
    

        case Increment: return "Increment";
        case Decrement: return "Decrement";
        case Bit_And: return "Bit_And";
      
    case Bit_Or: return "Bit_Or";

        case Bit_Not: return "Bit_Not";
        case Bit_Xor: return "Bit_Xor";
        case Bit_ShiftRight: return "Bit_ShiftRight";
      
 case Bit_ShiftLeft: return "Bit_ShiftLeft";
      

    

        case Logic_And: return "Logic_And";
        case Logic_Or: return "Logic_Or";
        case Logic_Not: return "Logic_Not";
      
    case GreaterThan: return "ComparisonOperator";

        case LessThan: return "SingleLineComment";
        case GreaterThanOrEqualTo: return "DoubleLineComment";
        case LessThanOrEqualTo: return "ContiniousCommet";
      
           case EqualTo: return "EqualTo";
        case NotEqualTo: return "NotEqualTo";
      
          case Literal_String: return "Literal_String";

        case Literal_Integer: return "Literal_Integer";
        case Literal_Character: return "Literal_Character";
        case Literal_Float: return "Literal_Float";
      
        case ClassModifier: return "ClassModifier";
        case FunctionModifier: return "FunctionModifier";
      
}

}
enum LexerState{
    NormalState,
    ClassDeclaration,
    MemberDeclaration,
    FunctionDeclaration,
    ParameterList,
    VaraibleDeclaration,
    FunctionBody,
    Statement,
    StringLiteral,
    EndOfStatement
};

enum IdentifierType{
 ClassIdentifier,
    FunctionIdentifier,
    ParameterIdentifier,
    VariableIdentifier
};
#pragma endregion

#pragma region  Structs

struct SubTokenInfo{

  SubTokenType subTokenType;

  SubTokenInfo() :subTokenType(Default){}

 SubTokenInfo(SubTokenType type) : subTokenType(type){}

};


#pragma endregion

#pragma region  Symbol Table

struct IdentifierInfo{
    IdentifierType identifierType;
};
class SymbolTable{
    private : unordered_map<string,IdentifierInfo> globalTable;
private : stack<unordered_map<string,IdentifierInfo>> scopeStack;

public : void enterScope(){
    scopeStack.push({});
}
public : void exitScope(){
    if(!scopeStack.empty()){
    scopeStack.pop();
    }
}

    public : 
    void addIdentifier(const std::string& name,const IdentifierInfo& info){
        if(scopeStack.empty()){
globalTable[name] = info;
        }else{
            scopeStack.top()[name] = info;
        }

    }

public :
    IdentifierInfo* getIdentifier(const string& name){
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



};

#pragma endregion

#pragma region  Token class

struct Token{
    public:
    string tokenValue;
    TokenType tokenType;
    SubTokenInfo subTokenInfo;
//default constructor
Token(): tokenValue("Null_Undefined"),tokenType(TokenType::Null_Undefined),subTokenInfo(SubTokenInfo()){}
//constructor that takes arguments
    Token(string val, TokenType typ, SubTokenInfo subInfo) :
    tokenValue(val),tokenType(typ),subTokenInfo(subInfo){}

    bool isValid()const{
        return !tokenValue.empty() && tokenType != TokenType::Null_Undefined;
    }
};

#pragma endregion

#pragma region MidTierCollector


// Assume Token, TokentokenType, and SubTokenInfo are defined as provided earlier

class TrieNode {
public:
    std::unordered_map<char, std::shared_ptr<TrieNode>> children;
    bool isEndOfToken;
    Token token; // Store the token at the end node

  
    TrieNode() :  isEndOfToken(false) {}
};

class TokenTrie {
private:
    std::shared_ptr<TrieNode> root;
    std::vector<Token> tokens; // Store tokens in order of insertion

public:
    TokenTrie() : root(std::make_shared<TrieNode>()) {}

    void insert(const Token& token) {
        auto current = root;
        for (char ch : token.tokenValue) {
            if (current->children.find(ch) == current->children.end()) {
                current->children[ch] = std::make_shared<TrieNode>();
            }
            current = current->children[ch];
        }
        current->isEndOfToken = true;
        current->token = token;
        tokens.push_back(token);
    }

    Token getLastToken() const {
        if (!tokens.empty()) {
            return tokens.back();
        }
        // Return a default Token if there are no tokens
        return Token("", TokenType::Identifier, SubTokenInfo());
    }

    Token getFirstToken() const {
        if (!tokens.empty()) {
            return tokens.front();
        }
        // Return a default Token if there are no tokens
        return Token("", TokenType::Identifier, SubTokenInfo());
    }

    bool deleteToken(const std::string &tokentokenValue) {
        for (auto it = tokens.begin(); it != tokens.end(); ++it) {
            if (it->tokenValue == tokentokenValue) {
                tokens.erase(it);
                return true;
            }
        }
        return false; // Token not found
    }

    vector<Token> getAllTokens()const{
        return tokens;
    }

    void clearAllTokens() {
        tokens.clear();
    }

    void reinitializeTrie() {
        root = std::make_shared<TrieNode>(); // Reset the root of the trie
        tokens.clear(); // Clear the tokens vector
    }

    // Add more methods as needed...
};



#pragma endregion

#pragma region Actual Lexer Calss


class Lexer {
private:
//the tokentrie
 TokenTrie trie;
 //stores the current state of the lexer
 LexerState currentState;
 //stores the symbol table for identifiers
 SymbolTable symbols;
 //stores known important keyword tokens
std::unordered_map<std::string, Token> keywordMap;
//stores important character tokens
std::unordered_map<std::string, Token> characterMap;
//this is for handling processed tokens
std::vector<Token> tokens;
//keeps track of current token
size_t currentTokenIndex;


public:
    Lexer() : currentTokenIndex(0),currentState(LexerState::NormalState) {
        
        tokens.clear();
        InitializeKeywordMap();
        InitializeSpecialCharacterMap();
        // ... add other keywords and token types ...

        // Populate the trie with the keywords
        for (const auto& pair : keywordMap) {
            trie.insert(Token(pair.first, TokenType::Keyword, pair.second.subTokenInfo));
        }
           for (const auto& pair : characterMap) {
            trie.insert(Token(pair.first, TokenType::Keyword, pair.second.subTokenInfo));
        }
    }

#pragma region Token lookAheadBuffer
Token readNextToken(istringstream& codeStream){
    char ch;
    string tokenValue;
while(codeStream.get(ch)){
 
if(std::isspace(ch)){
 
 return readNextToken(codeStream);
}else if(isSeprator(ch)){
    //if its a secial character return it as a token imediately
    return Token(std::string(1,ch),TokenType::Seprator,SubTokenInfo());
    
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
if(isspace(ch) || isSeprator(ch)){
codeStream.unget();
break;
}else{
    tokenValue += ch;
}

}

    TokenType type = determineDefaultTokenType(tokenValue);
    return Token(tokenValue,type,SubTokenInfo());
}

Token peekNextToken(istringstream& codeStream){
    streampos ogPos = codeStream.tellg(); //save current pos
    Token nextToken;

nextToken = readNextToken(codeStream);
    codeStream.seekg(ogPos);
    //restore og pos
    return nextToken;

}
Token peekNextToken2(istringstream& codeStream){
streampos ogPos = codeStream.tellg();
Token nextToken,nextToken2;

nextToken = readNextToken(codeStream);

nextToken2 = readNextToken(codeStream);

codeStream.seekg(ogPos);

return nextToken2;
}



TokenType determineDefaultTokenType(const string& tokenValue)const{
    //make light deep cheack for token and token type here 
    //also make identifier table for storing identifiers when creating the actual tokens
    if(keywordMap.find(tokenValue) != keywordMap.end()){
        return TokenType::Keyword;
    }else{
    return TokenType::Null_Undefined;
    }
    
}

#pragma endregion

#pragma region  Helper Functions

private : 
void InitializeKeywordMap(){
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


keywordMap["void"] = Token("Keyword",TokenType::Keyword,SubTokenInfo(SubTokenType::ReturnType));

        keywordMap["class"] = Token("Keyword",TokenType::Keyword,SubTokenInfo(SubTokenType::Default));

        keywordMap["int"] = Token("Keyword",TokenType::Keyword,SubTokenInfo(SubTokenType::DataType));
        keywordMap["bool"] = Token("Keyword",TokenType::Keyword,SubTokenInfo(SubTokenType::DataType));
        keywordMap["char"] = Token("Keyword",TokenType::Keyword,SubTokenInfo(SubTokenType::DataType));
        keywordMap["string"] = Token("Keyword",TokenType::Keyword,SubTokenInfo(SubTokenType::DataType));
        keywordMap["float"] = Token("Keyword",TokenType::Keyword,SubTokenInfo(SubTokenType::DataType));
        keywordMap["double"] = Token("Keyword",TokenType::Keyword,SubTokenInfo(SubTokenType::DataType));
        keywordMap["var"] = Token("Keyword",TokenType::Keyword,SubTokenInfo(SubTokenType::DataType));


         keywordMap["="] = Token("Operator",TokenType::Operator,SubTokenInfo(SubTokenType::Assign));
         keywordMap["+"] = Token("Operator",TokenType::Operator,SubTokenInfo(SubTokenType::Addition));
            keywordMap["-"] = Token("Operator",TokenType::Operator,SubTokenInfo(SubTokenType::Subtract));
            keywordMap["*"] = Token("Operator",TokenType::Operator,SubTokenInfo(SubTokenType::Multiply));
            keywordMap["/"] = Token("Operator",TokenType::Operator,SubTokenInfo(SubTokenType::Divide));
            keywordMap["%"] = Token("Operator",TokenType::Operator,SubTokenInfo(SubTokenType::Modulos));
         
             keywordMap["=="] = Token("Operator",TokenType::Operator,SubTokenInfo(SubTokenType::EqualTo));
         keywordMap["+="] = Token("Operator",TokenType::Operator,SubTokenInfo(SubTokenType::Add_Assign));
            keywordMap["-="] = Token("Operator",TokenType::Operator,SubTokenInfo(SubTokenType::Sub_Assign));
            keywordMap["*="] = Token("Operator",TokenType::Operator,SubTokenInfo(SubTokenType::Mul_Assign));
            keywordMap["/="] = Token("Operator",TokenType::Operator,SubTokenInfo(SubTokenType::Div_Assign));
            keywordMap["%="] = Token("Operator",TokenType::Operator,SubTokenInfo(SubTokenType::Mod_Assign));
         
        
            keywordMap["if"] = Token("Keyword",TokenType::Keyword,SubTokenInfo(SubTokenType::ControlStructure));
            keywordMap["else"] = Token("Keyword",TokenType::Keyword,SubTokenInfo(SubTokenType::ControlStructure));
            keywordMap["do"] = Token("Keyword",TokenType::Keyword,SubTokenInfo(SubTokenType::ControlStructure));
            keywordMap["while"] = Token("Keyword",TokenType::Keyword,SubTokenInfo(SubTokenType::ControlStructure));
            keywordMap["for"] = Token("Keyword",TokenType::Keyword,SubTokenInfo(SubTokenType::ControlStructure));
            keywordMap["foreach"] = Token("Keyword",TokenType::Keyword,SubTokenInfo(SubTokenType::ControlStructure));
         
         
            keywordMap["struct"] = Token("Keyword",TokenType::Keyword,SubTokenInfo(SubTokenType::DataStructure));
         
         
         

         
}

private:
void InitializeSpecialCharacterMap(){
    //and as for these seprators sure they can have diffrent meaning
    //but still is best to know that these are seprators
characterMap["("] = Token("Seprator",TokenType::Seprator,SubTokenInfo(SubTokenType::OpenParen));
characterMap[")"] = Token("Seprator",TokenType::Seprator,SubTokenInfo(SubTokenType::CloseParen));
characterMap["{"] = Token("Seprator",TokenType::Seprator,SubTokenInfo(SubTokenType::OpenBrace));
characterMap["}"] = Token("Seprator",TokenType::Seprator,SubTokenInfo(SubTokenType::CloseBrace));
characterMap["["] = Token("Seprator",TokenType::Seprator,SubTokenInfo(SubTokenType::OpenBracket));
characterMap["]"] = Token("Seprator",TokenType::Seprator,SubTokenInfo(SubTokenType::CloseBracket));
characterMap[":"] = Token("Seprator",TokenType::Seprator,SubTokenInfo(SubTokenType::Default));
characterMap[";"] = Token("Seprator",TokenType::Seprator,SubTokenInfo(SubTokenType::SemiColon));
characterMap[","] = Token("Seprator",TokenType::Seprator,SubTokenInfo(SubTokenType::Comma));

}

private :
void processComment(istringstream& codeStream, vector<Token>& tokens, bool isMultiLine){
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


bool isSeprator(char ch)const{
    //define all special characters that should be tokenized individually

    const std::string specialCharacters = ";<>{}[]:(),.";
    return specialCharacters.find(ch) != std::string::npos;
}

bool isWhiteSpace(char ch){
    return std::isspace(ch);
}

bool isEndOfLine(char c){
    bool result = (c == '\n') || (c == '\r');
    return result;
}

bool isNumeric(char c){
    bool result = false;

    if((c >= '0')&& (c <='9')) result = true;

    return result;
}

TokenType Lexer::isNumber()
{
    while (isNumeric(PEEK())) NEXT();

    if (PEEK() == '.' && IS_DIGIT(PEEK_ON(1))) {
        NEXT(); // The . itelf
        while (IS_DIGIT(PEEK())) NEXT();
        return TOKEN_FLOAT;
    }

    return TOKEN_INTEGER;
}

bool isLetter(char c){
    bool result = false;
    if((c >= 'A') && (c <= 'Z')) result = true;
       if((c >= 'a') && (c <= 'z')) result = true;

       return result;
}

#pragma endregion

public :
    std::vector<Token> tokenize(const std::string& sourceCode) {
      
        std::istringstream codeStream(sourceCode);
        char ch;
        std::string currentTokenValue;
 Token currentToken,lastKnownValidToken,nextToken,nextToken2;

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
   
    nextToken2 = peekNextToken2(codeStream);

 
//we already modifying current token based on & 
    ++currentTokenIndex;
    classifyToken(currentToken,lastKnownValidToken,nextToken,nextToken2);
     tokens.push_back(currentToken);

    cout << "Current Token Value Is :: " << currentToken.tokenValue << " :: And Next Token IS :: " << nextToken.tokenValue << ":: And Next Token After That Is :: " << nextToken2.tokenValue << "\n :: And Last Known Token From Current Token Is ::" << lastKnownValidToken.tokenValue << endl ;
}

/*

while(codeStream.get(ch)){
      if(ch == '/' ){

        if(codeStream.peek() == '/'){
            //single line comment
        processComment(codeStream,tokens,false);
        currentTokenValue.clear();
      }else if(codeStream.peek() == '*'){
        //multiline comment
        processComment(codeStream,tokens,true);
        currentTokenValue.clear();
        }else{
            currentTokenValue += ch;
        }  

      }else if(std::isspace(ch) || ch == ';' || ch == ')'){
      
        if( ch == ';'){
            //handling seprator
tokens.push_back(Token(string (1,ch),TokenType::Seprator,SubTokenInfo(SubTokenType::SemiColon)));
        }else{
        //handle white spacestring white
        string whiteSpace(1,ch);
        while(codeStream.peek() && isspace(codeStream.peek())){
            whiteSpace += codeStream.get();
        }
    //dont need to tokenize white spaces but can tokenize here
        }

            if(!currentTokenValue.empty()){
           
         //   processToken(currentTokenValue);
            currentTokenValue.clear();
        }
      
    }else{
        currentTokenValue += ch;
    }

 
}
*/
  return tokens;
  
}

/*ok here is what we are going to do we need a class to store the default values of tokens the ones beign
processed by the toknize readtoken function because in some cases we might have something like > =  instead of >= which is also valid 
but is actually only one token so in the defualt tken keeper we when we are classifying token and 
we encounter such a case first edit the current token we are classifying based on it and the next token
and then edit the > and = and combine them into one token in the token keeper or something like that just get it to work*/

Token classifyToken(const Token& currentToken,Token lastKnownToken,Token nextToken,Token nextToken2){
    
//this function will handle the actual tokenization and place tokens In Actual Token Or Use The Tier

//before we run the state based switch we decide the current state 
if(currentToken.tokenValue == "class"){
currentState = ClassDeclaration;

}else if(lastKnownToken.tokenType == TokenType::AccessModifier && currentToken.subTokenInfo.subTokenType == SubTokenType::DataType || currentToken.subTokenInfo.subTokenType == SubTokenType::ReturnType || currentToken.tokenValue == "void"){
currentState = FunctionDeclaration;

}
 else if(lastKnownToken.subTokenInfo.subTokenType == SubTokenType::DataType || currentToken.subTokenInfo.subTokenType == SubTokenType::ReturnType || currentToken.subTokenInfo.subTokenType == SubTokenType::ReturnType || lastKnownToken.tokenValue == "void" && currentToken.tokenType == TokenType::Identifier && nextToken.tokenValue == ":"){
currentState = FunctionDeclaration;

}

switch(currentState){

    case LexerState::NormalState:{
  if(currentToken.tokenValue == "+" && nextToken.tokenValue == "="){
        return Token("+=",TokenType::Operator,SubTokenInfo(SubTokenType::Bit_ShiftRight));

    }
if(currentToken.tokenValue == "class"){

currentState = ClassDeclaration;

return Token(currentToken.tokenValue,TokenType::Keyword,SubTokenInfo(SubTokenType::Default));
}else if(currentToken.tokenValue == "void"){

currentState = FunctionDeclaration;

return Token(currentToken.tokenValue,TokenType::Keyword,SubTokenInfo(SubTokenType::Default));

}else if(lastKnownToken.tokenType == TokenType::Keyword && lastKnownToken.subTokenInfo.subTokenType == SubTokenType::DataType){
    
    //meaning we has int,bool or the likes
    //that means the current token is an identifire but we also need to cheack the next token
    //if the next token is a ( or : that means we are entering function declaration state
    //but if the next token is a = or ; it means we are creating a member since we are in normal state
}else{
     

}
    }break;

      case ClassDeclaration:{
if(currentToken.tokenValue == "{"){
//open brace
}else if(currentToken.tokenValue == "}"){
//close brace
currentState = NormalState;
}else if(currentToken.tokenValue == ":" && lastKnownToken.tokenValue == "class"){
//the next thing should be something we in herit
}else{
    return Token(currentToken.tokenValue,TokenType::Null_Undefined,SubTokenInfo(SubTokenType::Default));
}

      }break;

       case MemberDeclaration:{

       }

    break;
      case FunctionDeclaration:{

      }

    break;
      case ParameterList:{

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
return Token();
}



#pragma region  Extra Important Token Functions
private :
Token GetValidLastKnownToken(const std::string& tokenValue){
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



bool isValidIdentifierChar(char ch){
    return isalnum(ch) || ch == '_';
}

bool isValidIdentifier(const string& tokenValue){

    if(tokenValue.empty() || isalpha(tokenValue[0] && tokenValue[0] != '_')){
        return false;
    }
bool isvalidchar = isValidIdentifierChar(tokenValue[0]);

    return isvalidchar; //std::all_of(tokenValue.begin(),tokenValue.end(),isvalidchar);
}

private : 
 void HandleError(){

 }

#pragma endregion

//end of lexer class
};


#pragma endregion 

#pragma region Int main


// Demonstration of MidTierCollector usage
int main() {
 cout << "Start" << endl;

//for lexet testing 
    Lexer lexer;
    std::string sourceCode = "private class Abstractor : Base : { int value; float val2 = 0.225f; public bool meSS = false;  public virtual void GetStuff:byte value,bool data:{//do some stuff}}";
    auto tokens = lexer.tokenize(sourceCode);

    // Output the tokens for testing
    for (const auto& token : tokens) {
        std::cout << "( Token Of Name : " << token.tokenValue <<  "\n, Type : \n" << TokenTypeToString(token.tokenType)
         << ",  With : SubToken  Type Of : \n"  << SubTokenTypeToString(token.subTokenInfo.subTokenType) << " ) |||| \n" << std::endl;
    }

return 0;

}

#pragma endregion