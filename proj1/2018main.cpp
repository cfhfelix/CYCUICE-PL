//
//  main.cpp
//  PLproject
//
//  Created by 黃警鋒 on 2018/3/12.
//  Copyright © 2018年 project. All rights reserved.
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

using namespace std;

struct R {
  float value  ;
  string id ;
  int type ;
  // type == 0 -> int
  // type == 1 -> float
} ;

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

vector< R> gRegister ;
vector<string> getToken ;
void Term( bool & correct, float & value ) ;
void Factor( bool & correct, float & value ) ;
bool IfArthToken( char c_char ) ;
string TokenID() ;
void Exp( bool & correct, float & value ) ;
void NOT_IDStartArithExpOrBxp( float & value ) ;
void Peektoken(  string & token ) ;
void BooleanExp( bool correct ) ;
void Endline() ;
void IDlessArithExpOrBexp( bool & correct, string id, bool & borS, float & value, bool & boolOP ) ;
void BooleanArithExp( float fT1Value, float fT2Value, string tokenType, bool & borS ) ;
bool IsOp( string s_str ) ;
bool BooleanOperator( string & type ) ;
void Not_ID_StartFactor( float & value ) ;
void Not_ID_StartTerm( float & value ) ;
void Endline() ;
int gHasPoint = 0 ;
int gTokenPoint = 0 ;
int gPoint = 0 ;
bool gOpToOp = false ;

bool IfArthToken( char c_char ) {
  //    cout << _key << endl ;
  if ( c_char >= 'a' && c_char <= 'z' ) return true ;
  else if ( c_char >= 'A' && c_char <= 'Z' ) return true ;
  else if ( c_char >= '0' && c_char <= '9' ) return true ;
  else if ( c_char == '_' ) return true ;
  return false ;
} // IfArthToken()

bool IfIDENT( char c_char ) {
  // 是否id
  if ( c_char >= 'a' && c_char <= 'z' ) return true ;
  else if ( c_char >= 'A' && c_char <= 'Z' ) return true ;
  else if ( c_char == '_' ) return true ;
  return false ;
} // IfIDENT()

bool IfNot_ID( char c_char ) {
  if ( c_char >= '0' && c_char <= '9' ) return true ;
  else if ( c_char == '(' || c_char == '+' || c_char == '-' ) return true ;
  else if ( c_char == '.' ) return true ;
  return false ;
} // IfNot_ID()

bool IfArith( char c_char ) {
  if ( c_char >= '0' && c_char <= '9' ) return true ;
  else if ( c_char == '.' ) return true ;
  return false ;
} // IfArith()

bool IfBool( char c_char ) {
  if ( c_char == '=' || c_char == '>' || c_char == '<' ) return true ;
  return false ;
} // IfBool()

bool IfOper( char c_char ) {
  if ( c_char == '+' || c_char == '-' || c_char == '*' || c_char == '/' ) return true ;
  if ( c_char == ';' || c_char == '=' || c_char == '>' || c_char == '<' ) return true ;
  return false ;
} // IfOper()

bool Unrecogn( char c_char ) {
  if ( c_char == '+' || c_char == '-' || c_char == '*' || c_char == '/' ) return true ;
  if ( c_char == '(' || c_char == ')' || c_char == ':' || c_char == '.' ) return true ;
  if ( c_char == ';' || c_char == '=' || c_char == '>' || c_char == '<' ) return true ;
  if ( c_char == '_' ||  c_char == ':' ) return true ;
  if ( c_char >= 'a' && c_char <= 'z' ) return true ;
  if ( c_char >= 'A' && c_char <= 'Z' ) return true ;
  if ( c_char >= '0' && c_char <= '9' ) return true ;
  string swap = "" ;
  swap += c_char ;
  throw new Unrecognized( swap ) ;
  return false ;
} // Unrecogn()

bool IfArithHasPoint( char c_char ) {
  if ( c_char >= '0' && c_char <= '9' ) return true ;
  if ( c_char == '.' ) {
    if ( gPoint == 0 ) {
      gHasPoint = 1 ;
      gPoint = 1 ;
      gTokenPoint = 1 ;
    } // if()
    
    else {
      gPoint = 2 ;
      return false ;
    } // else
    
    return true ;
  } // if()
  
  return false ;
} // IfArithHasPoint()

void BooleanArithExp( float fT1Value, float fT2Value, string tokenType, bool & borS ) {
  
  float k = fT1Value - fT2Value ;
  if ( tokenType == "=" )  {
    if ( ( abs( k ) < 0.0001 ) )  borS = true ;
    else borS = false ;
  } // if()
  
  else if ( tokenType == ">" ) {
    if ( ( abs( k ) < 0.0001 ) )  borS = false ;
    else if ( fT1Value > fT2Value ) borS = true ;
    else borS = false ;
  } // else if()
  
  else if ( tokenType == "<" ) {
    if ( ( abs( k ) < 0.0001 ) )  borS = false ;
    else if ( fT1Value < fT2Value ) borS = true ;
    else borS = false ;
  } // else if()
  
  else if ( tokenType == ">=" ) {
    if ( ( abs( k ) < 0.0001 ) )  borS = false ;
    else if ( fT1Value >= fT2Value ) borS = true ;
    else borS = false ;
  } // else if()
  
  else if ( tokenType == "<=" ) {
    if ( ( abs( k ) < 0.0001 ) )  borS = false ;
    else if ( fT1Value <= fT2Value ) borS = true ;
    else borS = false ;
  } // else if()
  
  else if ( tokenType == "<>" ) {
    if ( ( abs( k ) < 0.0001 ) )  borS = false ;
    else if ( fT1Value != fT2Value ) borS = true ;
    else borS = false ;
  } // else if()
} // BooleanArithExp()

bool IsOp( string s_str ) {
  if ( s_str == ">=" || s_str == "<>" || s_str == "<=" ) return true ;
  return false ;
} // IsOp()

string TokenID() {
  string c_Token = "" ;
  string swapC = "", swapS ;
  while ( cin.peek() == ' ' || cin.peek() == '\n' || cin.peek() == '\t' ) cin.get() ;
  if ( IfIDENT( cin.peek() ) ) {
    while ( IfArthToken( cin.peek() ) ) {
      c_Token += cin.get() ;
    } // while()
  } // if()
  
  else if ( IfArith( cin.peek() ) ) {
    gPoint = 0 ;
    while ( IfArithHasPoint( cin.peek() ) ) {
      c_Token += cin.get() ; // 切數字
    } // while()
    
    if ( c_Token == "." ) throw new Unrecognized( c_Token ) ;
    if ( gPoint == 1 ) ;
    else if ( gPoint == 2 ) {
      c_Token = cin.get() ;
      while ( IfArithHasPoint( cin.peek() ) )  c_Token += cin.get() ; // 切數字
      throw new Unexpected( c_Token ) ;
    } // else if()
  } // else if()
  
  else if ( Unrecogn( cin.peek() ) ) {
    if ( cin.peek() == '>' || cin.peek() == '<' || cin.peek() == '=' )
      BooleanOperator( c_Token ) ;
    
    else if ( cin.peek() == ':' ) {
      c_Token += cin.get() ;
      if ( cin.peek() == '=' ) c_Token += cin.get() ;
      else {
        throw new Unrecognized( c_Token ) ;
      } // else
    } // else if()
    
    else c_Token = cin.get() ;
  } // else if()
  
  
  if ( c_Token == "/" ) {
    if ( cin.peek() == '/' ) {
      Endline() ;
      c_Token = TokenID() ;
    } // if ()
  } // if()
  
  // cout << "Token:  " << c_Token << endl ;
  return c_Token ;
} // TokenID()

void Endline() {
  char end[1000] = "" ;
  cin.getline( end, 1000 ) ;
} // Endline()

void Peektoken(  string & token ) {
  //  cout << "Peektoken     " << endl ;
  while ( cin.peek() == ' ' || cin.peek() == '\n' || cin.peek() == '\t' ) cin.get() ;
  token = cin.peek() ;
  if ( token == "/" ) {
    char swap = cin.get() ;
    if ( cin.peek() == '/' ) {
      Endline() ;
      Peektoken( token ) ;
    } // if ()
    
    else cin.putback( swap ) ;
  } // if()
  //  cout << "outpeek" << endl ;
} // Peektoken()

void PushReg( string c_id, float c_value ) {
  bool hasDate = false ;
  for ( int i = 0; i < gRegister.size() && !hasDate ; i ++ ) {
    if ( gRegister[i].id == c_id ) {
      gRegister[i].value = c_value ;
      // cout << "tokenpoint " << gTokenPoint << endl ;
      gRegister[i].type = gTokenPoint ;
      return ;
    } // if()
  } // for()
  
  R data ;
  data.value = c_value ;
  data.id = c_id ;
  data.type = gHasPoint ;
  gRegister.push_back( data ) ;
} // PushReg()


bool BooleanOperator( string & type ) {
  string peek = "" ;
  string error = "", swap = "" ;
  Peektoken( peek ) ;
  if ( peek == "=" ) {
    type = cin.get() ;
    peek = cin.peek() ;
    return true ;
  } // if()
  
  else if ( peek == "<" || peek == ">" ) {
    type = cin.get() ;
    peek = cin.peek() ;
    string bOp = type + peek ;
    if ( IsOp( bOp ) ) type += cin.get() ;
    return true ;
  } // else if()
  
  return false ;
} // BooleanOperator()



void Factor( bool & correct, float & value )
// <factor>     ::= NUM | IDENT | (<Exp>)
{
  // cout << "Factor" << endl ;
  string tokenValue, tokenType = "" ;
  bool expCorrect = false ;
  float expValue = 0.0 ;
  tokenValue = TokenID() ;  // 讀取一次
  if ( tokenValue == "+"
      || tokenValue == "-" ) {
    tokenType = tokenValue ;
    tokenValue = TokenID() ;
    if ( !IfArthToken( tokenValue[0] ) ) throw new Unexpected( tokenValue ) ;
  } // if()
  
  if ( IfArith( tokenValue[0] ) ) {
    correct = true ;
    //  cout << "339  " << tokenValue << endl ;
    value = atof( tokenValue.c_str() ) ;
    if ( tokenType == "-" ) value = 0 - value ;
    return ;
  } // if()
  
  else if ( IfIDENT( tokenValue[0] ) ) {
    correct = true;
    bool define = false ;
    for ( int i = 0 ; i < gRegister.size() && define == false ; i ++ ) {
      if ( gRegister[i].id == tokenValue ) {
        value = gRegister[i].value ;    // 把value找出
        gHasPoint = gRegister[i].type ;
        if ( gTokenPoint == 0 ) gTokenPoint = gHasPoint ;
        define = true ;
      } // if()
    } // for ()
    
    if ( !define ) {
      correct = false ;
      throw new Undefined( tokenValue ) ;
    } // if()
    
    return ;
  } // else if()
  
  else if ( tokenValue == "(" ) { // tokenType = LEFT_PAREN
    Exp( expCorrect, expValue );
    if ( !expCorrect ) {
      correct = false;
      value = 0.0 ;
      return ;
    } // if()
    
    else { // expCorrect; but still neet to check RIGHT_PAREN
      string tokenType = TokenID() ;
      if (  tokenType  != ")" ) { // <> RIGHT_PAREN
        correct = false ;
        value = 0.0 ;
        throw new Unexpected( tokenType ) ;
      } // if()
      
      else {
        correct = true ;
        value = expValue;
        return ;
      } // else()  tokenType = RIGHT_PAREN
    } // else() expCorrect()
  } // else if()  tokenType = LEFT_PAREN
  
  else  throw new Unexpected( tokenValue ) ;
} // Factor()

void Term( bool &correct, float & value )
// <term>       ::= <factor> {* <factor> | / <factor>}
{
  // cout << "term" << endl ;
  bool factor1Correct = false, factor2Correct = false ;
  float factor1Value = 0.0, factor2Value = 0.0 ;
  Factor( factor1Correct, factor1Value );
  string tokenType = "" ;
  //  判斷factor的
  do {
    
    //        PeekToken(tokenType, tokenValue);
    //        if (tokenType = EOF) or (not tokenType in [MULTIPLICATION, DIVISION])
    //        then { correct := true; value := factor1Value; return; }
    Peektoken( tokenType ) ;
    if ( ( tokenType != "/" && tokenType != "*" ) || ( tokenType == ";" ) ) {
      correct = true ;
      value = factor1Value ;
      return ;
    } // if()
    // there is '*' or '/' behind the first factor
    tokenType = TokenID() ; // MULTIPLICATION or DIVISION
    Factor( factor2Correct, factor2Value );
    if ( !factor2Correct ) {
      correct = false ;
      value = 0.0 ;
      return ;
    } // if()
    // second factor ok.
    correct = true;
    if ( tokenType == "*" ) factor1Value = factor1Value * factor2Value;
    else {
      if ( factor2Value == 0 ) throw  ( "Error" ) ;
      else {
        factor1Value = factor1Value / factor2Value;
        factor1Value = ( int ) factor1Value ;
      } // else
    } // else
  } while ( correct ) ;
} // Term()

void Exp( bool & correct, float & value )
// <Exp>        ::= <term> {+ <term> | - <term>}
{
  // cout << "Exp" << endl ;
  bool term1Correct = false, term2Correct = false ;
  float term1Value  = 0.0, term2Value = 0.0 ;
  Term( term1Correct, term1Value ) ;
  string tokenType = "" ;
  if ( term1Correct ) {
    correct = false;
    value   = 0.0;
  } // if()
  
  do {
    //        PeekToken(tokenType, tokenValue);
    //        if (tokenType = EOF) or (not tokenType in [PLUS, MINUS])
    //        then { correct := true; value := term1Value; return; }
    Peektoken( tokenType ) ;
    if ( ( tokenType != "+" && tokenType != "-" ) || tokenType == ";" ) {
      correct = true ;
      value = term1Value ;
      return ;
    } // if()
    // there is '+' or '-' behind the first term
    tokenType = TokenID() ;
    //   GetToken(tokenType, tokenValue);  // tokenType : PLUS or MINUS
    Term( term2Correct, term2Value );
    if ( !term2Correct ) {
      correct = false;
      value  = 0.0  ;
    } // if()
    // second term ok.
    correct = true;
    if ( tokenType == "+" ) term1Value = term1Value + term2Value;
    else  term1Value = term1Value - term2Value;
    
  } while ( correct ) ;
} // Exp()

void IDlessArithExpOrBexp( bool & correct, bool & borS, float & value, bool & boolOP ) {
  string tokenType = "" ;
  string id = TokenID() ;
  float fT1Value = 0.0, fT2Value = 0.0, c_num = 0.0 ;
  bool  fT1Correct = false, fT2Correct = false ;
  bool done = false, define = false ;
  for ( int i = 0 ; i < gRegister.size() && define == false ; i ++ ) {
    if ( gRegister[i].id == id ) {
      c_num = gRegister[i].value ;    // 把value找出
      gTokenPoint = gRegister[i].type ;
      define = true ;
    } // if()
  } // for ()
  
  do {
    Peektoken( tokenType ) ;
    if ( tokenType == ";" ) {
      if ( !define ) throw new Undefined( id ) ;
      value = c_num ;
      correct = true ;
      done = true ;
    } // if ()
    
    else if ( ( tokenType != "+" && tokenType != "-" && tokenType != "*" && tokenType != "/" ) ) {
      correct = true ;
      done = true ;
    } // else if ()
    
    else {
      tokenType = TokenID() ;
      if ( !define ) throw new Undefined( id ) ;
      
      //   GetToken(tokenType, tokenValue);  // tokenType : +-*/
      // 讀掉下一個 <+>|| <->
      if ( tokenType == "+" || tokenType == "-" ) Term( fT1Correct, fT1Value ) ;
      else if ( tokenType == "*" || tokenType == "/" ) Factor( fT1Correct, fT1Value ) ;
      
      if ( !fT1Correct && !fT2Correct ) {
        correct = false;
        fT1Value = 0.0  ;
        return ;
      } // if()
      // second term ok.
      correct = true;
      if ( tokenType == "+" ) c_num =  c_num + fT1Value ;
      else if ( tokenType == "-" ) c_num =  c_num - fT1Value ;
      else if ( tokenType == "*" ) c_num =  c_num * fT1Value ;
      else if ( tokenType == "/" ) {
        if ( fT1Value == 0 ) throw  ( "Error" ) ;
        c_num =  c_num / fT1Value ;
        if ( gTokenPoint == 0 ) c_num = ( int ) c_num ;
      } // else if()
      
    } // else()
    
    gHasPoint = 0 ;
  } while ( !done ) ;
  
  Unrecogn( tokenType[0] ) ;
  if ( !IfOper( tokenType[0] ) ) throw new Unexpected( TokenID() ) ;
  if ( !define ) throw new Undefined( id ) ;
  fT1Value = c_num ;
  if ( BooleanOperator( tokenType ) ) {
    Exp( fT2Correct, fT2Value ) ;
    if ( fT2Correct ) BooleanArithExp( fT1Value, fT2Value, tokenType, borS ) ;
    boolOP = true ;
    return ;
  } // if()
  
  else if ( tokenType == ";" ) {
    value = c_num ;
    correct = true ;
  } // else if()
} // IDlessArithExpOrBexp()


void NOT_ID_StartArithExp( float & value ) {
  // cout <<  "NOT_ID_StartArithExp  "  << endl ;
  bool expcorrect = false ;
  float expvalue = 0.0 ;
  Exp( expcorrect, expvalue ) ;
  if ( expcorrect ) value = expvalue ;
} // NOT_ID_StartArithExp()

void NOT_IDStartArithExpOrBxp( float & value, bool & borS, bool & boolorArith ) {
  // cout << "NOT_IDStartArithExpOrBxp  "  << endl ;
  string tokentype = "" ;
  float exp2Value = 0.0 ;
  bool exp2Correct = false ;
  NOT_ID_StartArithExp( value ) ;
  Peektoken( tokentype ) ;
  if ( tokentype != ";" ) {
    if ( BooleanOperator( tokentype ) ) {
      Exp( exp2Correct, exp2Value ) ;
      if ( exp2Correct ) {
        BooleanArithExp( value, exp2Value, tokentype, borS ) ;
        boolorArith = true ;
      } // if()
      
      return ;
    } // if()
    
    else if ( Unrecogn( tokentype[0] ) ) {
      throw new Unexpected( TokenID() ) ;
    } // else if()
  } // if()
  
  else {
    boolorArith = false ;
    return ;
  } // else
  // <NOT_ID_StartArithExp>
  // [ <BooleanOperator> < ArithExp> ]
} // NOT_IDStartArithExpOrBxp()

void Commond() {
  string token = "" ;
  bool stop = false, correct = false  ;
  float value = 0.0 ;
  string peekToken = "", pushToken = "" ;
  bool push = false ;
  int type = 0 ; // type = 1 NOT_ID  type = 2 boolen
  bool borS = false, define = false, boolorArith = false   ;
  while ( !stop ) {
    try {
      Peektoken( token ) ;
      if ( IfIDENT( token[0] ) ) {
        token = TokenID();
        if ( token != "quit" ) Peektoken( peekToken ) ;
        if ( token == "quit" ) return ;  // 結束程式
        else if ( peekToken == ":" ) {
          string defineOp = TokenID() ;
          if ( defineOp == ":=" ) {
            Peektoken( peekToken ) ;
            Unrecogn( peekToken[0] ) ;
            Exp( correct, value ) ;
            type = 1 ;
            pushToken = token ;
            push = true ;
          } // if()
        } // else if()
        
        else {
          borS = false ;
          define = false  ;
          Unrecogn( peekToken[0] ) ;
          if ( IfArthToken( peekToken[0] ) ) throw new Unexpected( TokenID() ) ;
          IDlessArithExpOrBexp( correct, token, borS, value, boolorArith ) ;
          if ( boolorArith ) type = 2 ;
          else type = 1 ;
        }  // else <IDlessArithExpOrBexp>
      } // if()
      
      else if ( IfNot_ID( token[0] ) ) {
        
        NOT_IDStartArithExpOrBxp( value, borS, boolorArith ) ;
        if ( boolorArith ) type = 2 ;
        else type = 1 ;
      } // if ()
      
      
      
      Peektoken( token ) ;
      if ( token == ";" ) {
        // cout << " 635" << endl;
        cin.get() ; // get掉";"
        if ( push ) PushReg( pushToken, value ) ;
        if ( type == 1 ) {
          cout << "> " ;
          if ( gTokenPoint == 1 ) printf( "%.3f\n", value );
          // cout << "value: " << fixed <<  setprecision(3)  << value  <<  endl;
          else if ( gTokenPoint == 0 ) {
            
            printf( "%.f\n", value );
          } // else if()
        } // if()
        
        else if ( type == 2 ) {
          cout << "> " ;
          if ( borS ) cout << "true" << endl  ;
          else cout << "false" << endl ;
        } // else if()
        
        else throw new Unexpected( token ) ;
        type = 0 ;
      } // if ()
      
      else if ( !Unrecogn( token[0] ) ) ;
      else throw new Unexpected( TokenID() ) ;
      
    } // catch()
    
    catch ( Undefined * Undefined1 ) {
      cout << Undefined1->mMsg  << endl ;
      Endline() ;
    } // catch()
    
    catch ( Unrecognized * Unrecognized1 ) {
      cout << Unrecognized1->mMsg  << endl ;
      Endline() ;
    } // catch()
    
    catch ( Unexpected * Unexpected1 ) {
      cout << Unexpected1->mMsg  << endl ;
      Endline() ;
    } // catch()
    
    catch ( const char * x  ) {
      cout << "> " << x << endl ;
      Endline() ;
    } // catch
    // 以下重置行 //////
    value = 0.0 ;
    type = 0 ;
    push = false ;
    boolorArith = false ;
    token.clear() ; // 清空此行
    gHasPoint = 0 ;
    gTokenPoint = 0 ;
  } // while()
  
} // Commond()

int main()
{
  int startP = 0 ;
  cin >> startP ;
  cout << "Program starts..." << endl ;
  Commond() ;
  cout << "> Program exits..." << endl ;
  
  return 0;
} // main()










