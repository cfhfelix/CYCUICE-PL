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

# define RECOGN "[\\\\+\\*\\/\\-:=();\\._<>()\"\'\},\{\!\%\^\&\|]"
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
    TokenType_IDENT, TokenType_Digital, TokenType_RECOGN, TokenType_SIGN, TokenType_BooleanOperator, TokenType_OtherRECOGN, TokenType_Constant
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
bool IsStatement( TokenObject TokenObj) ;
bool sign( TokenObject TolenObj);
bool Type_specifier(TokenObject TokenObj) ;
void Definition() ;
void user_input() ;
void Function_definition_without_ID() ;
void Formal_parameter_list() ;
void Compound_statement() ;
void Declaration() ;
void Rest_of_declarators();
void Containconstant() ;
bool IsExpression( TokenObject TokenObj) ;
bool IsCompound_statement( TokenObject TokenObj ) ;
void User_input() ;
void Basic_expression() ;
void Rest_of_Identifier_started_basic_exp();
void Expression() ;
bool Assignment_operator( TokenObject TokenObj ) ;
void Actual_parameter_list() ;
void rest_of_maybe_conditional_exp_and_rest_of_maybe_logical_OR_exp() ;
void Rest_of_maybe_logical_OR_exp() ;
void Rest_of_maybe_logical_AND_exp() ;
void Maybe_logical_AND_exp() ;
void Maybe_bit_OR_exp();
void Rest_of_maybe_bit_OR_exp();
void Maybe_bit_ex_OR_exp() ;
void Rest_of_maybe_bit_ex_OR_exp();
void Rest_of_maybe_bit_AND_exp() ;
void Maybe_bit_AND_exp() ;






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
    if( TokenObj.Token == "true" || TokenObj.Token == "false" )
        TokenObj.TokenType[TokenObjectType_TokenTyped] = TokenType_Constant ;
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
            RECONG_TYPE = TokenType_Constant ;
        } // else if
        else if ( swapToken == "\'" ){
            for (string nextChar_Singlequotes = PeekNextChar_chartostring() ;  nextChar_Singlequotes != "\'" ; nextChar_Singlequotes = PeekNextChar_chartostring() ) swapToken += GetNextChar_chartostring() ;
            returnToken = swapToken + GetNextChar_chartostring() ;
            RECONG_TYPE = TokenType_Constant ;
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


void Containconstant(){
    /* '[' Constant ']' */
    GetNextInputAndPushTogOnelinecommand() ;
    if ( gOneLineCommand[gOneLineCommand_NowPlace].TokenType[TokenObjectType_TokenTyped] != TokenType_Digital || gOneLineCommand[gOneLineCommand_NowPlace].TokenType[TokenObjectType_TokenTyped] != TokenType_Constant ) throw new Unexpected ( gOneLineCommand[gOneLineCommand_NowPlace].Token ) ;
    GetNextInputAndPushTogOnelinecommand() ;
    if ( gOneLineCommand[gOneLineCommand_NowPlace].Token != "]" ) throw new Unexpected ( gOneLineCommand[gOneLineCommand_NowPlace].Token ) ;
} //Containconstant()


void Unsigned_unary_exp(){
  /*  : Identifier [ '(' [ actual_parameter_list ] ')'
                   |
                   [ '[' expression ']' ] [ ( PP | MM ) ]
                 ]
    | Constant
    | '(' expression ')' */
    Trace("Unsigned_unary_exp start") ;
    if (  gOneLineCommand[gOneLineCommand_NowPlace].TokenType[TokenObjectType_TokenTyped] == TokenType_IDENT) {
        GetNextInputAndPushTogOnelinecommand() ;
        if (  gOneLineCommand[gOneLineCommand_NowPlace].Token == "(" ) {
            GetNextInputAndPushTogOnelinecommand() ;
            Actual_parameter_list();
            if (  gOneLineCommand[gOneLineCommand_NowPlace].Token != ")" ) throw new Unexpected( gOneLineCommand[gOneLineCommand_NowPlace].Token ) ;
            GetNextInputAndPushTogOnelinecommand() ;
        } // if()
        else if ( gOneLineCommand[gOneLineCommand_NowPlace].Token == "[" ){
             GetNextInputAndPushTogOnelinecommand() ;
            Expression() ;
            if (  gOneLineCommand[gOneLineCommand_NowPlace].Token != "]" ) throw new Unexpected( gOneLineCommand[gOneLineCommand_NowPlace].Token ) ;
            GetNextInputAndPushTogOnelinecommand() ;
            if ( gOneLineCommand[gOneLineCommand_NowPlace].Token == "++" || gOneLineCommand[gOneLineCommand_NowPlace].Token == "--" )GetNextInputAndPushTogOnelinecommand() ;
        } // else if()
    } // if()
    else if ( gOneLineCommand[gOneLineCommand_NowPlace].Token == "("  ){
        GetNextInputAndPushTogOnelinecommand() ;
                   Expression() ;
        if (  gOneLineCommand[gOneLineCommand_NowPlace].Token != ")" ) throw new Unexpected( gOneLineCommand[gOneLineCommand_NowPlace].Token ) ;
    } //else if()
    else if ( gOneLineCommand[gOneLineCommand_NowPlace].TokenType[TokenObjectType_TokenTyped] == TokenType_Digital || gOneLineCommand[gOneLineCommand_NowPlace].TokenType[TokenObjectType_TokenTyped] == TokenType_Constant )  ;
    else throw new Unexpected( gOneLineCommand[gOneLineCommand_NowPlace].Token ) ;

    GetNextInputAndPushTogOnelinecommand() ;
    Trace("Unsigned_unary_exp end") ;
} //

void Signed_unary_exp(){
    // 已經預取了
    /*
: Identifier [ '(' [ actual_parameter_list ] ')'
               |
               '[' expression ']'
             ]
| Constant
| '(' expression ')' */
    Trace("Signed_unary_exp start") ;
    if (  gOneLineCommand[gOneLineCommand_NowPlace].TokenType[TokenObjectType_TokenTyped] == TokenType_IDENT) {
        GetNextInputAndPushTogOnelinecommand() ;
        if (  gOneLineCommand[gOneLineCommand_NowPlace].Token == "(" ) {
            GetNextInputAndPushTogOnelinecommand() ;
            Actual_parameter_list();
            if (  gOneLineCommand[gOneLineCommand_NowPlace].Token != ")" ) throw new Unexpected( gOneLineCommand[gOneLineCommand_NowPlace].Token ) ;
            GetNextInputAndPushTogOnelinecommand() ;
        } // if()
        else if ( gOneLineCommand[gOneLineCommand_NowPlace].Token == "[" ){
             GetNextInputAndPushTogOnelinecommand() ;
            Expression() ;
            if (  gOneLineCommand[gOneLineCommand_NowPlace].Token != "]" ) throw new Unexpected( gOneLineCommand[gOneLineCommand_NowPlace].Token ) ;
            GetNextInputAndPushTogOnelinecommand() ;
        } // else if()
    } // if()
    else if ( gOneLineCommand[gOneLineCommand_NowPlace].Token == "("  ){
        GetNextInputAndPushTogOnelinecommand() ;
                   Expression() ;
        if (  gOneLineCommand[gOneLineCommand_NowPlace].Token != ")" ) throw new Unexpected( gOneLineCommand[gOneLineCommand_NowPlace].Token ) ;
        GetNextInputAndPushTogOnelinecommand() ;
    } //else if()
    else if ( gOneLineCommand[gOneLineCommand_NowPlace].TokenType[TokenObjectType_TokenTyped] == TokenType_Digital || gOneLineCommand[gOneLineCommand_NowPlace].TokenType[TokenObjectType_TokenTyped] == TokenType_Constant )  GetNextInputAndPushTogOnelinecommand() ;
    else throw new Unexpected( gOneLineCommand[gOneLineCommand_NowPlace].Token ) ;

    Trace("Signed_unary_exp end") ;
} //Signed_unary_exp()

void Unary_exp(){
    /*
: sign { sign } signed_unary_exp
| unsigned_unary_exp
| ( PP | MM ) Identifier [ '[' expression ']' ]
*/
    // 已經預取了
    Trace("Unary_exp start") ;
    if ( gOneLineCommand[gOneLineCommand_NowPlace].Token == "++" || gOneLineCommand[gOneLineCommand_NowPlace].Token == "--" ) {
        /* ( PP | MM ) Identifier [ '[' expression ']' ] */
        GetNextInputAndPushTogOnelinecommand() ;
        if (  gOneLineCommand[gOneLineCommand_NowPlace].TokenType[TokenObjectType_TokenTyped] != TokenType_IDENT) throw new Unexpected( gOneLineCommand[gOneLineCommand_NowPlace].Token ) ;
        GetNextInputAndPushTogOnelinecommand() ;
        if ( gOneLineCommand[gOneLineCommand_NowPlace].Token == "[" ){
            GetNextInputAndPushTogOnelinecommand() ;
            Expression() ;
            if ( gOneLineCommand[gOneLineCommand_NowPlace].Token != "]" ) throw new Unexpected( gOneLineCommand[gOneLineCommand_NowPlace].Token ) ;
        } // if()
        // 完成欲取
    } // if()
    else if ( gOneLineCommand[gOneLineCommand_NowPlace].Token == "+" || gOneLineCommand[gOneLineCommand_NowPlace].Token == "-" || gOneLineCommand[gOneLineCommand_NowPlace].Token == "!"){
        GetNextInputAndPushTogOnelinecommand() ;
       /* sign { sign } signed_unary_exp */
        while (  gOneLineCommand[gOneLineCommand_NowPlace].Token == "+" || gOneLineCommand[gOneLineCommand_NowPlace].Token == "-" || gOneLineCommand[gOneLineCommand_NowPlace].Token == "!" ) GetNextInputAndPushTogOnelinecommand() ;
        Signed_unary_exp();
    } //else if()
    else Unsigned_unary_exp() ;
    
    Trace("Unary_exp done") ;
} // Unary_exp

void Rest_of_maybe_mult_exp(){
/* : { ( '*' | '/' | '%' ) unary_exp }  could be empty ! */
    Trace("Rest_of_maybe_mult_exp start") ;
    while (  gOneLineCommand[gOneLineCommand_NowPlace].Token == "*" || gOneLineCommand[gOneLineCommand_NowPlace].Token == "/" || gOneLineCommand[gOneLineCommand_NowPlace].Token == "%" ) {
        GetNextInputAndPushTogOnelinecommand() ;
        Unary_exp();
    } // while()
    Trace("Rest_of_maybe_mult_exp done") ;
} // Rest_of_maybe_mult_exp()

void Maybe_mult_exp(){
/* : unary_exp rest_of_maybe_mult_exp  */
} // Maybe_mult_exp()

void Rest_of_maybe_additive_exp(){
/* : rest_of_maybe_mult_exp { ( '+' | '-' ) maybe_mult_exp } */
    Trace("Rest_of_maybe_additive_exp start") ;
        Rest_of_maybe_mult_exp() ;
       while ( gOneLineCommand[gOneLineCommand_NowPlace].Token == "+" || gOneLineCommand[gOneLineCommand_NowPlace].Token == "-" ) {
               GetNextInputAndPushTogOnelinecommand() ;
               Maybe_mult_exp() ;
       } // while()
        Trace("Rest_of_maybe_additive_exp done") ;
} //Rest_of_maybe_additive_exp()

void Maybe_additive_exp(){
/* : maybe_mult_exp { ( '+' | '-' ) maybe_mult_exp } */
} // Maybe_additive_exp()

void Rest_of_maybe_shift_exp(){
 /*   : rest_of_maybe_additive_exp { ( LS | RS ) maybe_additive_exp } */
    Trace("Rest_of_maybe_shift_exp start") ;
        Rest_of_maybe_additive_exp() ;
    while ( gOneLineCommand[gOneLineCommand_NowPlace].Token == "<<" || gOneLineCommand[gOneLineCommand_NowPlace].Token == ">>" ) {
            GetNextInputAndPushTogOnelinecommand() ;
            Maybe_additive_exp() ;
    } // while()
     Trace("Rest_of_maybe_shift_exp done") ;
}


void Maybe_shift_exp() {
/* : maybe_additive_exp { ( LS | RS ) maybe_additive_exp } */
}

void Rest_of_maybe_relational_exp(){
 /*   : rest_of_maybe_shift_exp
      { ( '<' | '>' | LE | GE ) maybe_shift_exp } */
    Trace("Rest_of_maybe_relational_exp start") ;
           Rest_of_maybe_shift_exp() ;
       while ( gOneLineCommand[gOneLineCommand_NowPlace].Token == "<" || gOneLineCommand[gOneLineCommand_NowPlace].Token == ">" || gOneLineCommand[gOneLineCommand_NowPlace].Token == ">="|| gOneLineCommand[gOneLineCommand_NowPlace].Token == "<=") {
               GetNextInputAndPushTogOnelinecommand() ;
               Maybe_shift_exp() ;
       } // while()
           
    Trace("Rest_of_maybe_relational_exp done") ;
}

void Maybe_relational_exp(){
    /*
: maybe_shift_exp
  { ( '<' | '>' | LE | GE ) maybe_shift_exp }
*/
}

void Rest_of_maybe_equality_exp(){
/* : rest_of_maybe_relational_exp
  { ( EQ | NEQ ) maybe_relational_exp }
 */
    Trace("Rest_of_maybe_equality_exp start") ;
        Rest_of_maybe_relational_exp() ;
    while ( gOneLineCommand[gOneLineCommand_NowPlace].Token == "==" || gOneLineCommand[gOneLineCommand_NowPlace].Token == "!=" ) {
            GetNextInputAndPushTogOnelinecommand() ;
            Maybe_relational_exp() ;
    } // while()
        
    Trace("Rest_of_maybe_equality_exp done") ;
} // Rest_of_maybe_equality_exp()

void Maybe_equality_exp(){
/* : maybe_relational_exp
  { ( EQ | NEQ ) maybe_relational_exp}
*/
    
} //Maybe_equality_exp()
void Rest_of_maybe_bit_AND_exp(){
/* : rest_of_maybe_equality_exp { '&' maybe_equality_exp } */
    Trace("Rest_of_maybe_bit_AND_exp start") ;
     Rest_of_maybe_equality_exp() ;
     while ( gOneLineCommand[gOneLineCommand_NowPlace].Token == "&" ) {
         GetNextInputAndPushTogOnelinecommand() ;
         Maybe_equality_exp() ;
     } // while()
     
     Trace("Rest_of_maybe_bit_AND_exp done") ;
     
} // Rest_of_maybe_bit_AND_exp

void Maybe_bit_AND_exp(){
/* : maybe_equality_exp { '&' maybe_equality_exp } */
} // Maybe_bit_AND_exp

void Rest_of_maybe_bit_ex_OR_exp(){
/* : rest_of_maybe_bit_AND_exp { '^' maybe_bit_AND_exp } */
    Trace("Rest_of_maybe_bit_ex_OR_exp start") ;
    Rest_of_maybe_bit_AND_exp() ;
    while ( gOneLineCommand[gOneLineCommand_NowPlace].Token == "^" ) {
        GetNextInputAndPushTogOnelinecommand() ;
        Maybe_bit_AND_exp() ;
    } // while()
    
    Trace("Rest_of_maybe_bit_ex_OR_exp done") ;
    
} //Rest_of_maybe_bit_ex_OR_exp()

void Maybe_bit_ex_OR_exp(){
/* : maybe_bit_AND_exp { '^' maybe_bit_AND_exp } */
}

void Rest_of_maybe_bit_OR_exp(){
     // 已經預取了
   /* : rest_of_maybe_bit_ex_OR_exp { '|' maybe_bit_ex_OR_exp } */
    Trace("rest_of_maybe_bit_OR_exp start") ;
    Rest_of_maybe_bit_ex_OR_exp() ;
    while ( gOneLineCommand[gOneLineCommand_NowPlace].Token == "|" ) {
           GetNextInputAndPushTogOnelinecommand() ;
           Maybe_bit_ex_OR_exp() ;
    } // while()
    Trace("rest_of_maybe_bit_OR_exp done") ;
    
} // rest_of_maybe_bit_OR_exp()


void Maybe_bit_OR_exp(){
     // 已經預取了
/* : maybe_bit_ex_OR_exp { '|' maybe_bit_ex_OR_exp } */
}

void Rest_of_maybe_logical_AND_exp(){
    // 已經預取了
/* : rest_of_maybe_bit_OR_exp { AND maybe_bit_OR_exp } */
    Trace("Rest_of_maybe_logical_AND_exp start") ;
     
    while ( gOneLineCommand[gOneLineCommand_NowPlace].Token == "&&" ) {
           GetNextInputAndPushTogOnelinecommand() ;
           Maybe_bit_OR_exp() ;
    } // while()
       // 已經幫別人預取了
    Trace("Rest_of_maybe_logical_AND_exp done") ;
} // Rest_of_maybe_logical_AND_exp()


void Maybe_logical_AND_exp(){
/* : maybe_bit_OR_exp { AND maybe_bit_OR_exp }  */
    Trace("Maybe_logical_AND_exp start") ;
    
    Trace("Maybe_logical_AND_exp done") ;
} // Maybe_logical_AND_exp()

void Rest_of_maybe_logical_OR_exp(){
    // 已經預取了
 /*   : rest_of_maybe_logical_AND_exp { OR maybe_logical_AND_exp } */
    Trace("Rest_of_maybe_logical_OR_exp start") ;
    Rest_of_maybe_logical_AND_exp() ;
    while ( gOneLineCommand[gOneLineCommand_NowPlace].Token == "||" ) {
        GetNextInputAndPushTogOnelinecommand() ;
        Maybe_logical_AND_exp() ;
    } // while()
    // 已經幫別人預取了
    
    Trace("Rest_of_maybe_logical_OR_exp done") ;
    
} // Rest_of_maybe_logical_OR_exp()


void rest_of_maybe_conditional_exp_and_rest_of_maybe_logical_OR_exp(){ // 即romce_and_romloe
    // 已經預取了
/* : rest_of_maybe_logical_OR_exp [ '?' basic_expression ':' basic_expression ] */
    Trace("rest_of_maybe_conditional_exp_and_rest_of_maybe_logical_OR_exp start") ;
    Rest_of_maybe_logical_OR_exp() ;
    if ( gOneLineCommand[gOneLineCommand_NowPlace].Token == "?" ) {
        GetNextInputAndPushTogOnelinecommand() ;
        Basic_expression() ;
        if ( gOneLineCommand[gOneLineCommand_NowPlace].Token != ":" ) throw new Unexpected(gOneLineCommand[gOneLineCommand_NowPlace].Token ) ;
        Basic_expression() ;
        
    } // if()
    
    // 已幫別人預取
    Trace("rest_of_maybe_conditional_exp_and_rest_of_maybe_logical_OR_exp done") ;
} // rest_of_maybe_conditional_exp_and_rest_of_maybe_logical_OR_exp()


bool Assignment_operator( TokenObject TokenObj ){
/*: '=' | TE | DE | RE | PE | ME*/
    if ( TokenObj.Token == "=" ||  TokenObj.Token == "*=" ||  TokenObj.Token == "/=" ||  TokenObj.Token == "%=" || TokenObj.Token == "+=" ||  TokenObj.Token == "-=" ) return true ;
    return false ;
} // Assignment_operator()

void Actual_parameter_list(){
    // 已經預先取了
/*: basic_expression { ',' basic_expression } */
    Trace("Actual_parameter_list start") ;
    Basic_expression() ;
    while ( gOneLineCommand[gOneLineCommand_NowPlace].Token != "," ) {
        GetNextInputAndPushTogOnelinecommand() ;
        Basic_expression() ;
    } // while()
    
    Trace("Actual_parameter_list done") ;
    // 到這邊應該要是下一個 ==預先取
} //Actual_parameter_list()

bool sign( TokenObject TokenObj){
/*: '+' | '-' | '!'*/
    if ( TokenObj.Token == "+" || TokenObj.Token == "-" || TokenObj.Token == "!" ) return true ;
    return false ;
} // sign()

void rest_of_PPMM_Identifier_started_basic_exp(){
/*
: [ '[' expression ']' ] romce_and_romloe
*/
}


void Rest_of_Identifier_started_basic_exp(){
    // 已經預先取了
  /*  : [ '[' expression ']' ]
      ( assignment_operator basic_expression
        |
        [ PP | MM ] romce_and_romloe
      )
    | '(' [ actual_parameter_list ] ')' romce_and_romloe
   */
    Trace("Rest_of_Identifier_started_basic_exp start") ;
    if ( gOneLineCommand[gOneLineCommand_NowPlace].Token == "(" ) {
        // UNDO '(' [ actual_parameter_list ] ')' romce_and_romloe
        GetNextInputAndPushTogOnelinecommand() ;
        Actual_parameter_list() ;
        if ( gOneLineCommand[gOneLineCommand_NowPlace].Token != ")" ) throw new Unexpected(gOneLineCommand[gOneLineCommand_NowPlace].Token ) ;
        // UNDO romce_and_romloe

    } //if()
    
    else {
        if ( gOneLineCommand[gOneLineCommand_NowPlace].Token == "[" ) {
            GetNextInputAndPushTogOnelinecommand() ;
            Expression() ;
            if ( gOneLineCommand[gOneLineCommand_NowPlace].Token != "]" ) throw new Unexpected(  gOneLineCommand[gOneLineCommand_NowPlace].Token ) ;
        } // if()
        
        if ( Assignment_operator(  gOneLineCommand[gOneLineCommand_NowPlace] ) ) {
            /* assignment_operator basic_expression */
            GetNextInputAndPushTogOnelinecommand() ;
            Basic_expression() ;
            // 已經幫下一個預取
        } // if()
        else {
            /* [ PP | MM ] romce_and_romloe */
            if ( gOneLineCommand[gOneLineCommand_NowPlace].Token == "++" || gOneLineCommand[gOneLineCommand_NowPlace].Token == "--" ) GetNextInputAndPushTogOnelinecommand() ;
            // UNDO romce_and_romloe
        }
    } // else
    
    Trace("Rest_of_Identifier_started_basic_exp Done") ;
} // Rest_of_Identifier_started_basic_exp

void Basic_expression(){
    // 已經預先取了
  /*  : Identifier rest_of_Identifier_started_basic_exp
    | ( PP | MM ) Identifier rest_of_PPMM_Identifier_started_basic_exp
    | sign { sign } signed_unary_exp romce_and_romloe
    | ( Constant | '(' expression ')' ) romce_and_romloe */
    Trace("Basic_expression start") ;
    if ( gOneLineCommand[gOneLineCommand_NowPlace].TokenType[TokenObjectType_TokenTyped] == TokenType_IDENT ) {
        GetNextInputAndPushTogOnelinecommand() ;
        Rest_of_Identifier_started_basic_exp() ;
    } // if()
    else if ( gOneLineCommand[gOneLineCommand_NowPlace].Token == "++" || gOneLineCommand[gOneLineCommand_NowPlace].Token == "--" ) {
        //UNDO Identifier rest_of_PPMM_Identifier_started_basic_exp
    } // else if()
    else if ( gOneLineCommand[gOneLineCommand_NowPlace].Token == "+" || gOneLineCommand[gOneLineCommand_NowPlace].Token == "-" ) {
        // UNDO sign { sign } signed_unary_exp romce_and_romloe
    } //else if
    else {
        // undo ( Constant | '(' expression ')' ) romce_and_romloe */
        if ( gOneLineCommand[gOneLineCommand_NowPlace].TokenType[TokenObjectType_TokenTyped] == TokenType_Digital || gOneLineCommand[gOneLineCommand_NowPlace].TokenType[TokenObjectType_TokenTyped] == TokenType_Constant ) {
            
        } // if()
        else if ( gOneLineCommand[gOneLineCommand_NowPlace].Token == "(" ) {
            
        } // if()
        else throw new Unexpected( gOneLineCommand[gOneLineCommand_NowPlace].Token ) ;
    } // else
    
    // 這裡應該要幫下一個人先取
    Trace("Basic_expression Done") ;

} // Basic_expression()

void Expression(){
    // 已經預先取了
/*: basic_expression { ',' basic_expression } */
    Trace(" Expression Start " ) ;
    Basic_expression() ;
    
    while ( gOneLineCommand[gOneLineCommand_NowPlace].Token != "," ) {
        GetNextInputAndPushTogOnelinecommand() ;
        Basic_expression() ;
    } // while()
    
    // 到這邊應該要是下一個 ==預先取
    Trace(" Expression Start " ) ;
} //expression()
 
bool IsCompound_statement( TokenObject TokenObj ){
    if ( TokenObj.Token == "{" ) return true ;
    return false ;
} //IsCompound_statement()

bool IsExpression( TokenObject TokenObj) {
  if ( TokenObj.TokenType[TokenObjectType_TokenTyped] == TokenType_Constant || TokenType_Constant == TokenType_Digital || TokenObj.TokenType[TokenObjectType_TokenTyped] == TokenType_IDENT )  return true ;
    if ( TokenObj.Token == "++" || TokenObj.Token == "--")  return true ;
    if ( TokenObj.Token == "+" || TokenObj.Token == "-" || TokenObj.Token == "(" ) return true ;
    return false ;
} //IsExpression()

bool IsStatement( TokenObject TokenObj){
    // 判斷是不是statement
    if ( TokenObj.Token == ";" || TokenObj.Token == "return" | TokenObj.Token == "if" || TokenObj.Token == "while" || TokenObj.Token == "while" ) return true ;
    if ( IsExpression( TokenObj) ) return true ;
    if ( IsCompound_statement( TokenObj ) ) return true ;
    return false ;
} //IsStatement()
void Statement(){
    // 已經預先取了
    /*
: ';'     // the null statement
| expression ';'
| RETURN [ expression ] ';'
| compound_statement
| IF '(' expression ')' statement [ ELSE statement ]
| WHILE '(' expression ')' statement
| DO statement WHILE '(' expression ')' ';'
    */
    Trace(" statement Start " ) ;
    if ( gOneLineCommand[gOneLineCommand_NowPlace].Token == ";" ) ;
    else if ( IsExpression( gOneLineCommand[gOneLineCommand_NowPlace] ) ){
        Expression();
        // undo ";"
    } // else if()
    else if ( gOneLineCommand[gOneLineCommand_NowPlace].Token == "return" ) {
        
    } //
    else if ( IsCompound_statement( gOneLineCommand[gOneLineCommand_NowPlace] )) {
        
    }
    else if ( gOneLineCommand[gOneLineCommand_NowPlace].Token == "if" ) {
        
    } // else if
    else if ( gOneLineCommand[gOneLineCommand_NowPlace].Token == "while" ){
        
    }
    else if ( gOneLineCommand[gOneLineCommand_NowPlace].Token == "do" ){
        
    }
    else throw new Unexpected( gOneLineCommand[gOneLineCommand_NowPlace].Token ) ;
    
    
    Trace(" statement End " ) ;
} //

void Declaration(){
    // 已經預先取了
/*: type_specifier Identifier rest_of_declarators*/
    Trace(" declaration start " ) ;
    GetNextInputAndPushTogOnelinecommand() ;
    if ( gOneLineCommand[gOneLineCommand_NowPlace].TokenType[TokenObjectType_TokenTyped] != TokenType_IDENT ) throw new Unexpected( gOneLineCommand[gOneLineCommand_NowPlace].Token ) ;
    GetNextInputAndPushTogOnelinecommand() ;
    Rest_of_declarators() ;
    
    Trace(" declaration end " ) ;
    
} //declaration()


void Compound_statement(){
    // 已經預先取了
/*: '{' { declaration | statement } '}'*/
    Trace(" compound_statement start " ) ;
    if ( gOneLineCommand[gOneLineCommand_NowPlace].Token != "{" ) throw new Unexpected( gOneLineCommand[gOneLineCommand_NowPlace].Token ) ;
    bool stop = false ;
    while( ! stop ) {
        GetNextInputAndPushTogOnelinecommand() ;
        if ( Type_specifier( gOneLineCommand[gOneLineCommand_NowPlace] ) ) Declaration() ;
        else if ( IsStatement( gOneLineCommand[gOneLineCommand_NowPlace] ) ) Statement() ;
        else stop = 0 ;
    } // while()
    
    if ( gOneLineCommand[gOneLineCommand_NowPlace].Token != "}" ) throw new Unexpected( gOneLineCommand[gOneLineCommand_NowPlace].Token ) ;
    
    
    Trace(" compound_statement End " ) ;
} // compound_statement()

void Formal_parameter_list(){
/*: type_specifier [ '&' ] Identifier [ '[' Constant ']' ]
  { ',' type_specifier [ '&' ] Identifier [ '[' Constant ']' ] }*/
    // 已經預先取type)specifier
    Trace(" formal_parameter_list start " ) ;
    GetNextInputAndPushTogOnelinecommand() ; // 取& 或 Identitifer
    if ( gOneLineCommand[gOneLineCommand_NowPlace].Token == "&" ) GetNextInputAndPushTogOnelinecommand() ;  // 取&
    if (  gOneLineCommand[gOneLineCommand_NowPlace].TokenType[TokenObjectType_TokenTyped] != TokenType_IDENT ) throw new Unexpected( gOneLineCommand[gOneLineCommand_NowPlace].Token ) ;
    GetNextInputAndPushTogOnelinecommand() ;
    if ( gOneLineCommand[gOneLineCommand_NowPlace].Token == "[" ) {
        Containconstant() ;
    } // if ()
      
    GetNextInputAndPushTogOnelinecommand() ; // 拿到下一個 可能, 或其他的 其他就結束
    while( gOneLineCommand[gOneLineCommand_NowPlace].Token != "," ) {
        GetNextInputAndPushTogOnelinecommand() ;
        if ( !Type_specifier( gOneLineCommand[gOneLineCommand_NowPlace] ) ) throw new Unexpected( gOneLineCommand[gOneLineCommand_NowPlace].Token ) ;
        GetNextInputAndPushTogOnelinecommand() ;
        if ( gOneLineCommand[gOneLineCommand_NowPlace].Token == "&" ) GetNextInputAndPushTogOnelinecommand() ;  // 取&
        if (  gOneLineCommand[gOneLineCommand_NowPlace].TokenType[TokenObjectType_TokenTyped] != TokenType_IDENT ) throw new Unexpected( gOneLineCommand[gOneLineCommand_NowPlace].Token ) ;
        GetNextInputAndPushTogOnelinecommand() ;
        if ( gOneLineCommand[gOneLineCommand_NowPlace].Token == "[" ) {
            Containconstant();
        } // if ()
        GetNextInputAndPushTogOnelinecommand() ;
    } // while()
    
    Trace(" formal_parameter_list Done " ) ;
} // formal_parameter_list()

void Function_definition_without_ID(){
/*: '(' [ VOID | formal_parameter_list ] ')' compound_statement*/
    Trace(" function_definition_without_ID start " ) ;
    GetNextInputAndPushTogOnelinecommand() ;
    if ( gOneLineCommand[gOneLineCommand_NowPlace].Token == "(" ) {
        GetNextInputAndPushTogOnelinecommand() ;
        /* [ VOID | formal_parameter_list ] */
        if ( gOneLineCommand[gOneLineCommand_NowPlace].Token == "void" ) {
           GetNextInputAndPushTogOnelinecommand() ;
        } // if()
        else if ( Type_specifier( gOneLineCommand[gOneLineCommand_NowPlace] ) ) {
            Formal_parameter_list() ;
        } // else if()
        
        if ( gOneLineCommand[gOneLineCommand_NowPlace].Token != ")" ) throw new Unexpected( gOneLineCommand[gOneLineCommand_NowPlace].Token ) ;
    } // if()
    else throw new Unexpected( gOneLineCommand[gOneLineCommand_NowPlace].Token );
    
    GetNextInputAndPushTogOnelinecommand() ;
    Compound_statement() ;
    // 到這邊應該會是"}"
    Trace(" function_definition_without_ID END") ;
} // function_definition_without_ID()

void Rest_of_declarators(){
    // 已經預先取 一定會進來
/*: [ '[' Constant ']' ]
  { ',' Identifier [ '[' Constant ']' ] } ';'*/
    Trace("rest_of_declarators Start") ;
     if ( gOneLineCommand[gOneLineCommand_NowPlace].Token == "[" ) Containconstant() ;
    GetNextInputAndPushTogOnelinecommand() ;
    while( gOneLineCommand[gOneLineCommand_NowPlace].Token != "," ) {
        GetNextInputAndPushTogOnelinecommand() ;
        if (  gOneLineCommand[gOneLineCommand_NowPlace].TokenType[TokenObjectType_TokenTyped] != TokenType_IDENT ) throw new Unexpected( gOneLineCommand[gOneLineCommand_NowPlace].Token ) ;
        GetNextInputAndPushTogOnelinecommand() ;
        if ( gOneLineCommand[gOneLineCommand_NowPlace].Token == "[" ) Containconstant() ;
        GetNextInputAndPushTogOnelinecommand() ;
    } // while()
    //這邊結束拿到的應該要是;
    if ( gOneLineCommand[gOneLineCommand_NowPlace].Token != ";" ) throw new Unexpected( gOneLineCommand[gOneLineCommand_NowPlace].Token ) ;
    
    Trace("rest_of_declarators End") ;
    
} // rest_of_declarators()

void Function_definition_or_declarators(){
/*: function_definition_without_ID
| rest_of_declarators*/
    
}

bool IsDefinition( TokenObject TokenObj){
    if ( Type_specifier(TokenObj) ) return true ;
    if ( TokenObj.Token == "void" ) return true ;
    return false ;
} //IsDefinition()

bool Type_specifier(TokenObject TokenObj){
   /* INT | CHAR | FLOAT | STRING | BOOL*/
    if ( TokenObj.Token == "int" || TokenObj.Token == "char" || TokenObj.Token == "float" || TokenObj.Token == "string" || TokenObj.Token == "bool" ) return true ;
    return false ;
} //type_specifier()

void Definition(){
    Trace("definition Start") ;
   /* :           VOID Identifier function_definition_without_ID
    | type_specifier Identifier function_definition_or_declarators*/
    if ( gOneLineCommand[gOneLineCommand_NowPlace].Token == "void" ) {
        GetNextInputAndPushTogOnelinecommand() ;
        if ( gOneLineCommand[gOneLineCommand_NowPlace].TokenType[TokenObjectType_TokenTyped] == TokenType_IDENT ) {
            // undo function_definition_without_ID
            // 此處只讀到identity 沒有讀( 要再讀
            Function_definition_without_ID() ;
        } // if()
        else throw new Unexpected( gOneLineCommand[gOneLineCommand_NowPlace].Token ) ;
    } // if()
    else if ( Type_specifier( gOneLineCommand[gOneLineCommand_NowPlace] ) ) {
       // undo type_specifier Identifier function_definition_or_declarators*/
    }
    else Trace("671 some error " ) ;
    
    Trace("definition Done") ;
} // definition()


void User_input(){
    Trace("user_input Start") ;
    /*( definition | statement ) { definition | statement }*/
    TokenObject TokenObj = GetNextToken() ;
    gOneLineCommand.push_back( TokenObj ) ;
    if ( IsDefinition( TokenObj ) ) Definition() ;
    else {
        // undo statement
        Statement() ;
    }
    // undo { definition | statement }
    
    GetNextInputAndPushTogOnelinecommand() ;
    while ( 1 ) {
        if ( IsDefinition( TokenObj ) ) Definition() ;
        else if ( IsStatement( TokenObj) ) Statement() ;
        GetNextInputAndPushTogOnelinecommand() ;
    } // while()
    
    Trace("user_input Done") ;
    
    
} //user_input()

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
