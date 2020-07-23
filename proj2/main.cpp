//
//  main.cpp
//  PLproject2
//
//  Created by 黃警鋒 on 2020/7/23.
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

# define RECOGN "[\\\\+\\*\\/\\-:=();\\._<>()\"\'\}\{\!\%\^\&\|]"
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
    DigitalType_int, DigitalType_float, DigitalType_bool
} ;
enum SignType{
    SignType_plus, SignType_subtraction, SignType_multiplication, SignType_division, SignType_Doubleplus, SignType_Doublesubtraction
} ;
enum BooleanOperator {
    BooleanOperator_equal, BooleanOperator_notequal, BooleanOperator_more, BooleanOperator_less, BooleanOperator_moreequal, BooleanOperator_lessequal, BooleanOperator_AND, BooleanOperator_OR
} ;

enum OtherRECOGN{
    OtherRECOGN_Annotation, OtherRECOGN_Define, OtherRECOGN_LEFT_BRACKET, OtherRECOGN_RIGHT_BRACKET, OtherRECOGN_SEMICOLON
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
        mMsg = "Unrecognized token with first char : '" + x + "'" ;
    } // Unrecognized()
} ; // class Exception1

class Unexpected : public Exception {
public:
    Unexpected( string x ) {
        mMsg = "Unexpected token : '" + x + "'" ;
    } // Unexpected()
} ; // class Unexpected

class Undefined : public Exception {
public:
    Undefined( string x ) {
        mMsg = "Undefined identifier : '" + x + "'" ;
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


/* function define end */
void Trace( string msg ){
//    cout << msg << endl ;
} // Trace()

void GetNextInputAndPushTogOnelinecommand(){
    TokenObject TokenObj = GetNextToken() ;
    gOneLineCommand.push_back( TokenObj ) ;
    gOneLineCommand_NowPlace ++ ;
//    cout << TokenObj.Token << endl ;
} //GetNextInputAndPushTogOnelinecommand()
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
    string nextchar = PeekNextChar_chartostring() ;
    if ( swapToken + nextchar == "//" ){
        // Get //
        returnToken =  swapToken + GetNextChar_chartostring() ;
        RECONG_TYPE = TokenType_OtherRECOGN ;
        RECONG_TYPE_Advanced = OtherRECOGN_Annotation ;
        ClearAllLine() ;
    } // if()
    
    else if ( regex_match( swapToken, regex_SIGN ) ) {
        returnToken = swapToken ;
        RECONG_TYPE = TokenType_SIGN ;
        if ( returnToken == "+" ) {
            if ( cin.peek() == '+' ) {
                returnToken =  swapToken + GetNextChar_chartostring() ;
                RECONG_TYPE_Advanced = SignType_Doubleplus ;
            } // if()
            else RECONG_TYPE_Advanced = SignType_plus ;
        } // if()
        if ( returnToken == "-" ) {
            if ( cin.peek() == '-' ) {
                returnToken =  swapToken + GetNextChar_chartostring() ;
                RECONG_TYPE_Advanced = SignType_Doublesubtraction ;
            } // if()
            else RECONG_TYPE_Advanced = SignType_subtraction ;
        } // if()
        if ( returnToken == "*" ) {
            if ( cin.peek() == '=' ) {
                returnToken =  swapToken + GetNextChar_chartostring() ;
            } // if()
            else RECONG_TYPE_Advanced = SignType_multiplication ;
        } // if()

        if ( returnToken == "/" ) {
            if ( cin.peek() == '=' ) {
                swapToken += cin.get() ;
                returnToken = swapToken ;
            } // if()
            else RECONG_TYPE_Advanced = SignType_division ;
        } // if()
    } // if()
    else if ( regex_match( swapToken, regex_OPERATOR ) ){
        RECONG_TYPE = TokenType_BooleanOperator ;
        if ( swapToken == "<" ) {
            // check <, <=, <>
            if ( nextchar == ">" || nextchar == "=" ) {
                returnToken =  swapToken + GetNextChar_chartostring() ;
                if ( returnToken == "<>" ) RECONG_TYPE_Advanced = BooleanOperator_notequal ;
                if ( returnToken == "<=" ) RECONG_TYPE_Advanced = BooleanOperator_lessequal ;
            } // if()
            else if ( swapToken + nextchar == "<<"  ) {
                returnToken =  swapToken + GetNextChar_chartostring() ;
                RECONG_TYPE = TokenType_OtherRECOGN ;
            } // else if()
            else {
                RECONG_TYPE_Advanced = BooleanOperator_less ;
                returnToken = swapToken ;
            } // else
        } // if()
        else if ( swapToken == ">" ) {
            // check >, >=
            if ( nextchar == "=" ) {
                nextchar = GetNextChar_chartostring() ;
                returnToken = swapToken + nextchar ;
                if ( returnToken == ">=" ) RECONG_TYPE_Advanced = BooleanOperator_moreequal ;
            } // if()
            else if ( swapToken + nextchar == ">>"  ) {
                swapToken += cin.get() ;
                RECONG_TYPE = TokenType_OtherRECOGN ;
            } // else if()
            else {
                RECONG_TYPE_Advanced = BooleanOperator_more ;
                returnToken = swapToken ;
            } // else
        } // else if()
        else if ( swapToken == "=" ) {
            if ( swapToken + nextchar == "=="  ) {
                           returnToken = swapToken + GetNextChar_chartostring() ;
                           RECONG_TYPE = TokenType_OtherRECOGN ;
            } // else if()
            else {
                returnToken = swapToken ;
                RECONG_TYPE_Advanced = BooleanOperator_equal ;
            } // else
        } // else if()
    } // else if()
    else if ( swapToken == ":") {
        // must :=
        if ( swapToken + nextchar == ":=" ) {
            returnToken = swapToken + GetNextChar_chartostring() ;
            RECONG_TYPE = TokenType_OtherRECOGN ;
            RECONG_TYPE_Advanced = OtherRECOGN_Define ;
        } // if()
        else {
            returnToken = swapToken ;
            RECONG_TYPE = TokenType_OtherRECOGN ;
        } // else
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
    else {
        if ( swapToken + nextchar == "&&" ) returnToken = swapToken + GetNextChar_chartostring() ;
        else if ( swapToken + nextchar == "||" ) returnToken = swapToken + GetNextChar_chartostring() ;
        else if ( swapToken + nextchar == "%=" ) returnToken = swapToken + GetNextChar_chartostring() ;
        else if ( swapToken + nextchar == "!=" ) returnToken = swapToken + GetNextChar_chartostring() ;
        else if (swapToken == "\"" ) {
            for (string nextChar_Doublequotes = PeekNextChar_chartostring() ;  nextChar_Doublequotes != "\"" ; nextChar_Doublequotes = PeekNextChar_chartostring() ) swapToken += GetNextChar_chartostring() ;
            returnToken = swapToken + GetNextChar_chartostring() ;
        } // else if
        else if ( swapToken == "\'" ){
            for (string nextChar_Singlequotes = PeekNextChar_chartostring() ;  nextChar_Singlequotes != "\"" ; nextChar_Singlequotes = PeekNextChar_chartostring() ) swapToken += GetNextChar_chartostring() ;
            returnToken = swapToken + GetNextChar_chartostring() ;
        } // else if()
        else
            returnToken = swapToken ;
        RECONG_TYPE = TokenType_OtherRECOGN ;
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
    int switchnum = DecideWhichType( PeekNextChar_chartostring() ) ;
    //    switch ( DecideWhichType( PeekNextChar_chartostring() ) ) {
    if ( switchnum == TokenType_IDENT )
        TokenObj = GetToken_IDENT() ;
    else if ( switchnum == TokenType_Digital )
        TokenObj = GetToken_DIGITAL() ;
    else if ( switchnum == TokenType_RECOGN ) {
        TokenObj = GetToken_RECOGN() ;
        if ( TokenObj.Token == "//" ) TokenObj = GetNextToken() ;
    } // else if
    else {
        
        
        string UnrecognizedToken = "" ;
        UnrecognizedToken += cin.get() ;
        throw new Unrecognized( UnrecognizedToken ) ;
        
    } // switch()
    
    //    cout << "GetToken : " << TokenObj.Token << endl ;
    return TokenObj ;
} // GetNextToken()

void PrintCommandLine(vector<TokenObject> _TokenVector){
    for ( int i = 0 ; i < _TokenVector.size() ; i ++ ) cout << _TokenVector[i].Token << " " ;
    cout << endl ;
} // PrintCommandLine


void Test(){
    try {
    TokenObject Token = GetNextToken() ;
    cout << "Token is " << Token.Token << endl ;
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
} //


int main(int argc, const char * argv[]) {
    // insert code here...
    while(1) {
        Test() ;
    } // while()
        
    return 0;
}
