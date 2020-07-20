//
//  main.cpp
//  PLproject1
//
//  Created by 黃警鋒 on 2020/7/15.
//  Copyright © 2020 黃警鋒. All rights reserved.
//
# include <stdio.h>
# include <stdlib.h>
# include <iostream> // cout, endl
# include <string> // string
# include <vector> // vector
# include <cstdlib> // atoi, system
# include <iomanip> // setw
# include <string.h>
# include <sstream>
# include <cmath>
# include <regex>
using namespace std ;
/* define regex */
# define IDENT "^[a-zA-Z_][a-zA-Z0-9_]*"
# define DIGITAL "[.]{0,1}[0-9]{0,1}"
# define DIGITAL_pointFirst "(^[.]([0-9]){1,})"
# define DIGITAL_pointMidden "(([0-9]){1,}[.][0-9]*)"
# define DIGITAL_pointLast "(([0-9]){1,}[.]{0,1})"

# define SIGN "[\\+\\-\\*\\/]"
# define SIGN_doublePlus "([+]{2,})"
# define SIGN_doubleSubtraction "((-){2,})"

# define OPERATOR "[<>=]"

# define RECOGN "[\\+\\*\\/\\-:=();\\._<>()]"
# define QUIT "quit"
# define OURC_DEFINE ":="
# define ANNOTATION "//"
# define LEFT_BRACKET "("
# define RIGHT_BRACKET ")"
# define SEMICOLON ";"

/* regex define start */
regex regex_IDENT( IDENT ) ;
regex regex_DIGITAL( DIGITAL ) ;
regex regex_DIGITAL_pointFirst( DIGITAL_pointFirst ) ;
regex regex_DIGITAL_pointMidden( DIGITAL_pointMidden ) ;
regex regex_DIGITAL_pointLast( DIGITAL_pointLast ) ;
regex regex_RECOGN( RECOGN ) ;
regex regex_OPERATOR( OPERATOR ) ;
regex regex_SIGN( SIGN ) ;
regex regex_SIGN_doublePlus( SIGN_doublePlus ) ;
regex regex_SIGN_doubleSubtraction( SIGN_doubleSubtraction ) ;
smatch smatch_string ; // for get regex string
/* regex define end */
/* enum define start */
/* TokenObjectType_Advance start */
enum DigitalType{
  DigitalType_int, DigitalType_float
} ;
enum SignType{
    SignType_plus, SignType_subtraction, SignType_multiplication, SignType_division
} ;
enum BooleanOperator {
    BooleanOperator_equal, BooleanOperator_notequal, BooleanOperator_more, BooleanOperator_less, BooleanOperator_moreequal, BooleanOperator_lessequal
} ;

enum OtherRECOGN{
    OtherRECOGN_Annotation, OtherRECOGN_Define, OtherRECOGN_LEFT_BRACKET, OtherRECOGN_RIGHT_BRACKET,
    OtherRECOGN_SEMICOLON
} ;
/* TokenObjectType_Advance start */
/* TokenObjectType_TokenTyped start */
enum TokenType{
    TokenType_IDENT, TokenType_Digital, TokenType_RECOGN, TokenType_SIGN, TokenType_BooleanOperator, TokenType_OtherRECOGN
} ;
/* TokenObjectType_TokenTyped  end */
enum TokenObjectType{
    TokenObjectType_TokenTyped, TokenObjectType_Advance
} ;
/* enum define end */

/* struct start */

struct RegValue {
    float value = 0 ;
    int value_type = 0 ;
} ;
struct TokenObject{
    int TokenType[2] ;
    string Token ;
} ;
struct Parameter{
    string id ;
    RegValue parameter_value ;
} ;
/* struct end */
/* vector start*/
vector<TokenObject> gOneLineCommand ; // for store a line command
vector<vector<TokenObject>> gAllLineCommand ; // for store all line command
vector<Parameter> gRegester ; // for store all had defined parameter
/* vector end */
/* public parameter start */
int gNowLine = 0 ;
int gOneLineCommand_NowPlace = 0 ;
/* public parameter end */


/* error msg start */
class Exception {
public:
  string mMsg ;
  Exception() {  }
  
  Exception( string x ) {
    mMsg = x ;
  } // Exception()
} ; // class Expception

class Unrecognized : public Exception {
public:
  Unrecognized( string x ) {
    mMsg = "> Unrecognized token with first char : '" + x + "'" ;
  } // Unrecognized()
} ; // class Exception1

class Unexpected : public Exception {
public:
  Unexpected( string x ) {
    mMsg = "> Unexpected token : '" + x + "'" ;
  } // Unexpected()
} ; // class Unexpected

class Undefined : public Exception {
public:
  Undefined( string x ) {
    mMsg = "> Undefined identifier : '" + x + "'" ;
  } // Undefined()
} ; // class Undefined
/* error msg end */

/* function define start */
void JumpToNextReadableChar() ;
void ClearAllLine() ;
string PeekNextChar_chartostring() ;
string GetNextChar_chartostring() ;
string GetNextChar_chartostring();
TokenObject GetToken_IDENT() ;
TokenObject GetToken_DIGITAL() ;
int DecideWhichType( string peektoken ) ;
TokenObject GetNextToken() ;
void PrintCommandLine(vector<TokenObject> _TokenVector) ;

RegValue Factor() ;
RegValue Term() ;
RegValue ArithExp() ;
/* function define end */
void Trace( string msg ){
    cout << msg << endl ;
} // Trace()

void JumpToNextReadableChar(){
    while ( cin.peek() == ' ' || cin.peek() == '\n' || cin.peek() == '\t' ) {
        char a = cin.get() ;
        if ( a == '\n' ) gNowLine ++ ;
    } // while() ;
} //JumpToNextReadableChar()
void ClearAllLine(){
    while( cin.get() != '\n' ) ;
    gNowLine ++ ;
} // ClearAllLine()

string PeekNextChar_chartostring(){
    // peek出下一個可用字元
    string token = "" ;
    char token_char = cin.peek() ;
    if ( token_char == '\n') gNowLine ++ ;
    token += token_char ;
    return token ;
} //GetNextChar_chartostring()

string GetNextChar_chartostring(){
    // get出下一個字元
    string token = "" ;
    char token_char = cin.get() ;
    if ( token_char == '\n') gNowLine ++ ;
    token += token_char ;
    return token ;
} //GetNextChar_chartostring()

TokenObject GetToken_IDENT(){
    string swapToken = "" ; // 暫存token 字串會慢慢往上長 直到和IDENT不匹配
    string returnToken = "" ;
    for ( swapToken = PeekNextChar_chartostring() ; regex_match( swapToken, regex_IDENT) ; ) {
        /* GetNextChar_chartostring() ; 此行目的是因為前面已經cin.peek存起來了，此時再去cin.get他會重複get一個字元 */
        GetNextChar_chartostring() ;
        returnToken = swapToken ;
        swapToken += PeekNextChar_chartostring() ;
    } // for()
    
    TokenObject TokenObj ;
    TokenObj.Token = returnToken ;
    TokenObj.TokenType[TokenObjectType_TokenTyped] = TokenType_IDENT ;
    TokenObj.TokenType[TokenObjectType_Advance] = 0 ;
    return TokenObj ;
} // GetToken_IDENT()

TokenObject GetToken_DIGITAL(){
    string swapToken = "" ; // 暫存token 字串會慢慢往上長 直到和IDENT不匹配
    string returnToken = "" ;
    regex regexDIGITAL_type = regex_DIGITAL ;
    int DigitalType = DigitalType_int ;
    for ( swapToken = PeekNextChar_chartostring() ; regex_match( swapToken, regexDIGITAL_type) ; ) {
        /* GetNextChar_chartostring() ; 此行目的是因為前面已經cin.peek存起來了，此時再去cin.get他會重複get一個字元 */
        if ( GetNextChar_chartostring() == ".") DigitalType = DigitalType_float ;
        if ( regex_match( swapToken, regex_DIGITAL_pointFirst ) ) {
            regexDIGITAL_type =  regex_DIGITAL_pointFirst ;
        } // if()
        else if ( regex_match( swapToken, regex_DIGITAL_pointMidden ) )
        {
            regexDIGITAL_type = regex_DIGITAL_pointMidden ;
        } // if()
        else if ( regex_match( swapToken, regex_DIGITAL_pointLast ) ) {
            regexDIGITAL_type = regex_DIGITAL_pointLast ;
        }
        else {
            // UNDO
            // 數字切到這裡就結束 這裡負責處理ERROR Message
        } // else
        returnToken = swapToken ;
        swapToken += PeekNextChar_chartostring() ;
        // 更改標準
    } // for()

    if ( returnToken == "." ) throw new Unexpected( returnToken ) ;
    TokenObject TokenObj ;
    TokenObj.Token = returnToken ;
    TokenObj.TokenType[TokenObjectType_TokenTyped] = TokenType_Digital ;
    TokenObj.TokenType[TokenObjectType_Advance] = DigitalType ;
    return TokenObj ;
} // GetToken_DIGITAL()


TokenObject GetToken_RECOGN(){
    string swapToken = "" ; // 暫存token 字串會慢慢往上長 直到和IDENT不匹配
    string returnToken = "" ;
    int RECONG_TYPE = 0 ;
    int RECONG_TYPE_Advanced = 0 ;
    swapToken = GetNextChar_chartostring() ;
    if ( swapToken == "/" && cin.peek() == '/'){
        // Get //
        swapToken += GetNextChar_chartostring() ;
        returnToken = swapToken ;
        RECONG_TYPE = TokenType_OtherRECOGN ;
        RECONG_TYPE_Advanced = OtherRECOGN_Annotation ;
    } // if()
    
    else if ( regex_match( swapToken, regex_SIGN ) ) {
        returnToken = swapToken ;
        RECONG_TYPE = TokenType_SIGN ;
        if ( returnToken == "+" ) RECONG_TYPE_Advanced = SignType_plus ;
        if ( returnToken == "-" ) RECONG_TYPE_Advanced = SignType_subtraction ;
        if ( returnToken == "*" ) RECONG_TYPE_Advanced = SignType_multiplication ;
        if ( returnToken == "/" ) RECONG_TYPE_Advanced = SignType_division ;
    } // if()
    else if ( regex_match( swapToken, regex_OPERATOR ) ){
        RECONG_TYPE = TokenType_SIGN ;
        if ( swapToken == "<" ) {
            // check <, <=, <>
            string nextchar = PeekNextChar_chartostring() ;
            if ( nextchar == ">" || nextchar == "=" ) {
                nextchar = GetNextChar_chartostring() ;
                returnToken = swapToken + nextchar ;
                if ( returnToken == "<>" ) RECONG_TYPE_Advanced = BooleanOperator_notequal ;
                if ( returnToken == "<=" ) RECONG_TYPE_Advanced = BooleanOperator_lessequal ;
            } // if()
            else {
                RECONG_TYPE_Advanced = BooleanOperator_less ;
                returnToken = swapToken ;
            } // else
        } // if()
        else if ( swapToken == ">" ) {
            // check >, >=
            string nextchar = PeekNextChar_chartostring() ;
            if ( nextchar == "=" ) {
                nextchar = GetNextChar_chartostring() ;
                returnToken = swapToken + nextchar ;
                if ( returnToken == ">=" ) RECONG_TYPE_Advanced = BooleanOperator_moreequal ;
            } // if()
            else {
                RECONG_TYPE_Advanced = BooleanOperator_more ;
                returnToken = swapToken ;
            } // else
        } // else if()
        else if ( swapToken == "=" ) {
            returnToken = swapToken ;
            RECONG_TYPE_Advanced = BooleanOperator_equal ;
        } // else if()
    } // else if()
    else if ( swapToken == ":") {
        // must :=
        string nextchar = GetNextChar_chartostring() ;
        if ( nextchar != "=" ) throw new Unexpected( nextchar ) ;
        returnToken = swapToken + nextchar ;
        RECONG_TYPE = TokenType_OtherRECOGN ;
        RECONG_TYPE_Advanced = OtherRECOGN_Define ;
    } // else if()
    else if ( swapToken == "(" || swapToken == ")" ) {
        returnToken = swapToken ;
        
        RECONG_TYPE = TokenType_OtherRECOGN ;
        if ( swapToken == "(" ) RECONG_TYPE_Advanced = OtherRECOGN_LEFT_BRACKET ;
        if ( swapToken == ")" ) RECONG_TYPE_Advanced = OtherRECOGN_RIGHT_BRACKET ;
    } // else if()
    else if ( swapToken == ";" ) {
        returnToken = swapToken ;
        RECONG_TYPE = TokenType_OtherRECOGN ;
        RECONG_TYPE_Advanced = OtherRECOGN_SEMICOLON ;
    } // else()
     
    TokenObject TokenObj ;
    TokenObj.Token = returnToken ;
    TokenObj.TokenType[TokenObjectType_TokenTyped] = RECONG_TYPE ;
    TokenObj.TokenType[TokenObjectType_Advance] = RECONG_TYPE_Advanced ;
    return TokenObj ;
} // GetToken_RECOGN()


int DecideWhichType( string peektoken ){
    string line = peektoken ;
    if ( regex_match( line, regex_IDENT) ){
        return TokenType_IDENT ;
    } // if()
    else if ( regex_match( line, regex_DIGITAL) ) {
        return TokenType_Digital ;
    } // else if()
    else if ( regex_match( line, regex_RECOGN ) ) {
        return TokenType_RECOGN ;
    } // else if()
    else {
        return -1 ;
    } // else if()
} // DecideWhichType()

TokenObject GetNextToken() {
    string Token = "" ;
    TokenObject TokenObj ;
    JumpToNextReadableChar() ;
    switch ( DecideWhichType( PeekNextChar_chartostring() ) ) {
        case TokenType_IDENT:
            TokenObj = GetToken_IDENT() ;
            break ;
        case TokenType_Digital:
            TokenObj = GetToken_DIGITAL() ;
            break ;
        case TokenType_RECOGN :
            TokenObj = GetToken_RECOGN() ;
            break ;
        case -1 :
            string UnrecognizedToken = "" ;
            UnrecognizedToken += cin.get() ;
            throw new Unrecognized( UnrecognizedToken ) ;
            break;
    } // switch()
    
//    cout << "GetToken : " << TokenObj.Token << endl ;
    return TokenObj ;
} // GetNextToken()

void PrintCommandLine(vector<TokenObject> _TokenVector){
    for ( int i = 0 ; i < _TokenVector.size() ; i ++ ) cout << _TokenVector[i].Token << " " ;
    cout << endl ;
} // PrintCommandLine

RegValue Factor(){
    // UNDO <Factor>       ::= [ SIGN ] NUM | IDENT | '(' <ArithExp> ')'
    Trace("Factor") ;
    RegValue Reg_Value ;
    cout << "Factor : " << gOneLineCommand[gOneLineCommand_NowPlace].Token << endl ;
    if ( gOneLineCommand[gOneLineCommand_NowPlace].TokenType[TokenObjectType_TokenTyped] == TokenType_SIGN ){
       //  [ SIGN ] NUM
        if ( gOneLineCommand[gOneLineCommand_NowPlace].Token == "+" || gOneLineCommand[gOneLineCommand_NowPlace].Token == "-"){
            if ( gOneLineCommand[gOneLineCommand_NowPlace+1].TokenType[TokenObjectType_TokenTyped] != TokenType_Digital) throw new Unexpected( gOneLineCommand[gOneLineCommand_NowPlace].Token ) ;
            // 判斷下一個是不是數字 不是的話就下去
            else {
            // 確定是數字後 計算數值
                if ( gOneLineCommand[gOneLineCommand_NowPlace].Token == "+" ) {
                    gOneLineCommand_NowPlace ++ ;
                    Reg_Value.value = atof(gOneLineCommand[gOneLineCommand_NowPlace].Token.c_str());
                    
                } // if()
                else if ( gOneLineCommand[gOneLineCommand_NowPlace].Token == "-") {
                    gOneLineCommand_NowPlace ++ ;
                    Reg_Value.value = 0 - atof(gOneLineCommand[gOneLineCommand_NowPlace].Token.c_str());
                } // if()
                // 只要存在過float 整串就應該是float
                if ( gOneLineCommand[gOneLineCommand_NowPlace].TokenType[TokenObjectType_Advance] == DigitalType_float ) Reg_Value.value_type = DigitalType_float ;
            } // else()
        } // if()
    } // if()
    else if ( gOneLineCommand[gOneLineCommand_NowPlace].TokenType[TokenObjectType_TokenTyped] == TokenType_Digital  ){
        Reg_Value.value = atof(gOneLineCommand[gOneLineCommand_NowPlace].Token.c_str());
         if ( gOneLineCommand[gOneLineCommand_NowPlace].TokenType[TokenObjectType_Advance] == DigitalType_float ) Reg_Value.value_type = DigitalType_float ;
    } // else if()
    else if ( gOneLineCommand[gOneLineCommand_NowPlace].TokenType[TokenObjectType_TokenTyped] == TokenType_IDENT ) {
        // UNDO IDENT
        int haddefine = 0 ;
        for ( int i = 0 ; i < gRegester.size() ; i ++ ) {
            if( gRegester[i].id == gOneLineCommand[gOneLineCommand_NowPlace].Token ){
                haddefine = 1 ;
                Reg_Value = gRegester[i].parameter_value ;
            } // if()
        } // for()
        
        if ( !haddefine ) throw new Undefined( gOneLineCommand[gOneLineCommand_NowPlace].Token ) ;
    } // else if()
    else if ( gOneLineCommand[gOneLineCommand_NowPlace].Token == "(") {
        gOneLineCommand_NowPlace ++ ;
        Reg_Value = ArithExp() ;
        cout << "417 " << gOneLineCommand[gOneLineCommand_NowPlace].Token << endl ;
        if ( !( gOneLineCommand[gOneLineCommand_NowPlace].Token == ")" ))
            throw new Unexpected( gOneLineCommand[gOneLineCommand_NowPlace].Token ) ;
    } // else if()
    
    gOneLineCommand_NowPlace ++ ; // 自己的數據用完就結束 換到下一個位置等待別人
    return Reg_Value ;
} // Factor()

RegValue Term(){
    // UNDO <Term>         ::= <Factor> | <Term> '*' <Factor> | <Term> '/' <Factor>
    Trace("Term") ;
    RegValue Reg_Value ;
    Reg_Value = Factor() ;
    cout << "Term : " << gOneLineCommand[gOneLineCommand_NowPlace].Token << endl ;
    while ( gOneLineCommand[gOneLineCommand_NowPlace].Token != ";" && ( gOneLineCommand[gOneLineCommand_NowPlace].Token == "*" || gOneLineCommand[gOneLineCommand_NowPlace].Token == "/")) {
         /*
          { '*' <Factor> | '/' <Factor> }
          當不是 * / 的時候停止
          */
        cout << "Term dddddddd: " << gOneLineCommand[gOneLineCommand_NowPlace].Token << endl ;
        
        if (gOneLineCommand[gOneLineCommand_NowPlace].Token == "*") {
            gOneLineCommand_NowPlace ++ ;
            RegValue Factor_Reg =  Factor() ;
            Reg_Value.value = Reg_Value.value * Factor_Reg.value ;
            Reg_Value.value_type = Factor_Reg.value_type ;
        } // if()
        else {
            gOneLineCommand_NowPlace ++ ;
            RegValue Factor_Reg =  Factor() ;
            if ( Factor_Reg.value == 0 ) throw ( "Error" ) ;
            Reg_Value.value = Reg_Value.value / Factor_Reg.value ;
            Reg_Value.value_type = Factor_Reg.value_type ;
        } // else()
    } // while()
    
    return Reg_Value ;
} // Term()
RegValue ArithExp(){
    // UNDO <ArithExp>     ::= <Term> | <ArithExp> '+' <Term> | <ArithExp> '-' <Term>
    Trace("ArithExp") ;
    RegValue Reg_Value ;
    cout << "ArithExp : " << gOneLineCommand[gOneLineCommand_NowPlace].Token << endl ;
    Reg_Value = Term() ;
    cout << "ArithExp : " << gOneLineCommand[gOneLineCommand_NowPlace].Token << endl ;
    while ( gOneLineCommand[gOneLineCommand_NowPlace].Token != ";" && ( gOneLineCommand[gOneLineCommand_NowPlace].Token == "+" || gOneLineCommand[gOneLineCommand_NowPlace].Token == "-") ){
        // 當不是 +- 的時候停止
        if ( gOneLineCommand[gOneLineCommand_NowPlace].Token == "+") {
                       // '+' <Term>
            gOneLineCommand_NowPlace ++ ;
            RegValue Factor_Reg =  Factor() ;
            Reg_Value.value = Reg_Value.value +  Factor_Reg.value ;
            Reg_Value.value_type =  Factor_Reg.value_type ;
        } // if()
        else {
                       // '-' <Term>
            gOneLineCommand_NowPlace ++ ;
            RegValue Factor_Reg =  Factor() ;
            Reg_Value.value = Reg_Value.value - Factor_Reg.value ;
            Reg_Value.value_type =  Factor_Reg.value_type ;
        } // else()
    } //while()

    return Reg_Value ;
} //ArithExp()
void BooleanExp(){
    // UNDO <BooleanExp>   ::= <ArithExp> ( '=' | '<>' | '>' | '<' | '>=' | '<=' ) <ArithExp>
} // BooleanExp()
void Statement(){
    // UNDO <Statement>    ::= IDENT ':=' <ArithExp>
    Trace("Statement") ;
    Parameter IDENT_parameter ;
    IDENT_parameter.id = gOneLineCommand[gOneLineCommand_NowPlace].Token ;
    gOneLineCommand_NowPlace ++ ; // get :=
    if ( gOneLineCommand[gOneLineCommand_NowPlace].Token == ":=" ) {
       /* IDENT ':=' <ArithExp> */
        gOneLineCommand_NowPlace ++ ;
        IDENT_parameter.parameter_value = ArithExp() ;
        cout << "statement " << IDENT_parameter.parameter_value.value ;
    } // if()
    // 檢查有沒有存在過
    int hadParameter = 0 ;
    for ( int i = 0 ; i < gRegester.size() ; i ++ ){
        if ( gRegester[i].id == IDENT_parameter.id ) {
            hadParameter = 1 ;
            gRegester[i] = IDENT_parameter ;
        } // if()
    } // for()
    if ( hadParameter == 0 ) gRegester.push_back( IDENT_parameter ) ;
    // 如果還沒被定義過 就存進去 如果被定義過  就取代
    cout << "ans: " ;
    printf( "%.3f\n", IDENT_parameter.parameter_value.value );
} // Statement()

void Command() {
    // UNDO <Command>      ::= <Statement> ';' | <BooleanExp> ';' | <ArithExp> ';' | QUIT
    // 先直接讀到分號 然後再進行後續處理
    TokenObject TokenObj = GetNextToken() ;
    if ( TokenObj.Token == QUIT ) return ;
    while( TokenObj.Token != QUIT ) {
        while( TokenObj.Token != ";" ) {
            /* 這個迴圈用作於讀到分號為止 並會將整行command放到 gOneLineCommand 這個vector */
            gOneLineCommand.push_back( TokenObj ) ;
            cout << "command: " << TokenObj.Token << endl ;
            TokenObj = GetNextToken() ;
//            cout << "command: " << TokenObj.Token << endl ;
        } // while()
//        cout <<"line size : " << gOneLineCommand.size() ;
        gOneLineCommand.push_back( TokenObj ) ; // 將 ";"放到裡面去
        PrintCommandLine(gOneLineCommand) ;
        if ( gOneLineCommand[1].Token == ":=" ){
            Statement() ;
            
        } // if()
        else {
            // UNDO <BooleanExp> ';' | <ArithExp> ';'
            cout << "??" << endl ;
        } // else
        
        TokenObj = GetNextToken() ;
        gAllLineCommand.push_back( gOneLineCommand ) ; // 將單行command存起來
        gOneLineCommand.clear() ; // 把單行command紀錄清乾淨
        gOneLineCommand_NowPlace = 0 ; // 行數歸0
    } // while()
} // Command()


int main()
{
    try {
//        Test() ;
        while(1 )
            Command() ;
//            GetNextToken() ;
    }
    catch ( Undefined * Undefined1 ) {
        cout << Undefined1->mMsg  << endl ;
    } // catch()
    
    catch ( Unrecognized * Unrecognized1 ) {
        cout << Unrecognized1->mMsg  << endl ;
        
    } // catch()
    
    catch ( Unexpected * Unexpected1 ) {
        cout << Unexpected1->mMsg  << endl ;
        
    } // catch()
    
    catch ( const char * x  ) {
        cout << "> " << x << endl ;
        
    } // catch
    return 0;
} // main()
