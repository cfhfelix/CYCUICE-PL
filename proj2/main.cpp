//
//  main.cpp
//  proj2
//
//  Created by 黃警鋒 on 2018/5/7.
//  Copyright © 2018年 project. All rights reserved.
//

using namespace std;

struct R {
  float value  ;
  string id ;
  int type ;
  // type == 0 -> int
  // type == 1 -> float
} ;

struct AllVar {
  string id ;
  string type ;
  int size ;
} ;

struct StringS {
  string id ;
  string value ;
  int size ;
  vector<string> sArray ;
} ;

struct BoolS {
  string id ;
  bool value ;
  int size ;
  vector<bool> bArray ;
} ;
struct IntS {
  string id ;
  int value ;
  int size ;
  vector<int> iArray ;
} ;
struct FloatS {
  string id ;
  float value ;
  int size ;
  vector<float> fArray ;
} ;

struct CharS {
  string id ;
  char value ;
  int size ;
  vector<char> cArray ;
} ;

struct Function{
  string id ;
  vector<string> token ;
} ;
// ///////////////////////////////////////////////////////////////////////
// ///////////////////////////////////////////////////////////////////////
// ///////////////////////////////////////////////////////////////////////
int gline  = 0 ;

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
    mMsg = "unrecognized token with first char : '" + x + "'" ;
  } // Unrecognized()
} ; // class Exception1

class Unexpected : public Exception {
public:
  Unexpected( string x ) {
    mMsg = "unexpected token : '" + x + "'" ;
  } // Unexpected()
} ; // class Unexpected

class Undefined : public Exception {
public:
  Undefined( string x ) {
    mMsg = "undefined identifier : '" + x + "'" ;
  } // Undefined()
} ; // class Undefined

class ExitOurC : public Exception {
public:
  ExitOurC( string x ) {
    mMsg = "undefined identifier : '" + x + "'" ;
  } // ExitOurC()
} ; // class ExitOurC
// ///////////////////////////////////////////////////////////////////////
// ///////////////////////////////////////////////////////////////////////
// ///////////////////////////////////////////////////////////////////////
vector<R> gRegister ;
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
void IDlessArithExpOrBexp( bool & correct, bool & borS,
                           string id, float & value, bool & boolOP ) ;
void BooleanArithExp( float fT1Value, float fT2Value, string tokenType, bool & borS ) ;
bool IsOp( string s_str ) ;
bool BooleanOperator( string & type ) ;
void Not_ID_StartFactor( float & value ) ;
void Not_ID_StartTerm( float & value ) ;
void Endline() ;

// //////// globe




int gHasPoint = 0 ;
int gTokenPoint = 0 ;
int gPoint = 0 ;
bool gOpToOp = false ;
// ////////
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
  if ( c_char == '_' ||  c_char == ':' || c_char == '|' ) return true ;
  if ( c_char >= 'a' && c_char <= 'z' ) return true ;
  if ( c_char >= 'A' && c_char <= 'Z' ) return true ;
  if ( c_char >= '0' && c_char <= '9'  ) return true ;
  if ( c_char == '[' || c_char == ']' || c_char == '\'' ) return true ;
  if ( c_char == '{' || c_char == '}' || c_char == '\\' ) return true ;
  if ( c_char == '%' || c_char == '^' || c_char == '"' ) return true ;
  if ( c_char == '!' || c_char == '?' || c_char == ',' || c_char == '&' ) return true ;
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
  if ( s_str == ">=" || s_str == "<>" || s_str == "<=" || s_str == "<<" ) return true ;
  if ( s_str == ">>" || s_str == "!=" || s_str == "++" || s_str == "--" ) return true ;
  if ( s_str == "+=" || s_str == "-=" || s_str == "*=" || s_str == "/=" ) return true ;
  if ( s_str == "%=" || s_str == "||" || s_str == "==" || s_str == "!=" ) return true ;
  return false ;
} // IsOp()

string TokenID() {
  string c_Token = "" ;
  string swapC = "", swapS ;
  bool idtoken = true ;
  while ( cin.peek() == ' ' || cin.peek() == '\n' || cin.peek() == '\t' ) {
    if ( cin.peek() == '\n' )
      gline ++ ;
    cin.get() ;
  } // while()
  
  if ( cin.peek() == '/' ) {
    char token123 = cin.get() ;
    if ( cin.peek() == '/' ) {
      cin.get() ;
      Endline() ;
     
      while ( cin.peek() == ' ' || cin.peek() == '\n' || cin.peek() == '\t' ) {
        if ( cin.peek() == '\n' )
          gline ++ ;
        cin.get() ;
      } // while()
    } // if ()
    
    else cin.putback( token123 ) ;
  } // if()


  if ( IfIDENT( cin.peek() ) ) {
    while ( IfArthToken( cin.peek() ) && idtoken ) {
      c_Token += cin.get() ;
      // if ( IfDefineToken( c_Token ) ) idtoken = false ;
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
    if ( cin.peek() == '>' || cin.peek() == '<' || cin.peek() == '='
         || cin.peek() == '+' || cin.peek() == '-' || cin.peek() == '*'
         || cin.peek() == '/' || cin.peek() == '%' || cin.peek() == '|'
         || cin.peek() == '&' || cin.peek() == '!' )
      BooleanOperator( c_Token ) ;
    
    
    else {
      c_Token = cin.get() ;
    } // else
  } // else if()
  
  
  if ( c_Token == "/" ) {
    if ( cin.peek() == '/' ) {
      cin.get() ;
      Endline() ;
      c_Token = TokenID() ;
      //  cout << c_Token << "  after //" << endl ;
    } // if ()
  } // if()
  
  string swap, peektoken ;
  if ( c_Token == "'" ) {
    swap = cin.get() ;
    c_Token += swap ;
    swap = cin.get() ;
    if ( swap == "'" ) c_Token += swap ;
    else throw new Unexpected( swap ) ;
  } // if()
  
  if ( c_Token == "\"" ) {
    while ( cin.peek() != '"' ) {
      swap = cin.get() ;
      c_Token += swap ;
    } // while()
    
    swap = cin.get() ;
    c_Token += swap ;
  } // if()
  
  // cout << "Token:  " << c_Token << endl ;
  return c_Token ;
} // TokenID()

void Endline() {
  char end[1000] = "" ;
  cin.getline( end, 1000 ) ;
  gline ++ ;
} // Endline()

void Peektoken(  string & token ) {
  token = TokenID() ;
  // cout << "peek : " << token << endl ;
  int swap = token.size() ;
  for ( int i = swap - 1 ; i > -1  ; i -- )
    cin.putback( token[i] ) ;
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

void PeekChar(  string & token ) {
  //  cout << "Peektoken     " << endl ;
  while ( cin.peek() == ' ' || cin.peek() == '\n' || cin.peek() == '\t' ) cin.get() ;
  token = cin.peek() ;
  if ( token == "/" ) {
    char swap = cin.get() ;
    if ( cin.peek() == '/' ) {
      Endline() ;
      PeekChar( token ) ;
    } // if ()
    
    else cin.putback( swap ) ;
  } // if()
  //  cout << "outpeek" << endl ;
} // PeekChar()

bool BooleanOperator( string & type ) {
  string peek = "" ;
  string error = "", swap = "" ;
  
  // PeekChar( peek ) ;
  type = cin.get() ;
  peek = cin.peek() ;
  string bOp = type + peek ;
  if ( IsOp( bOp ) ) type += cin.get() ;
  return true ;
  
} // BooleanOperator()


// ///////////  以下老大文法  ////////////
//
//                       _oo0oo_
//                      o8888888o
//                      88" . "88
//                      (| -_- |)
//                      0\  =  /0
//                    ___/`---'\___
//                  .' \\|     |// '.
//                 / \\|||  :  |||// \  -
//                / _||||| -:- |||||- \ -
//               |   | \\\  -  /// |   |
//               | \_|  ''\---/''  |_/ |
//               \  .-\__  '-'  ___/-. /  -
//             ___'. .'  /--.--\  `. .'___
//          ."" '<  `.___\_<|>_/___.' >' "".
//         | | :  `- \`.;`\ _ /`;.`/ - ` : | |
//         \  \ `_.   \_ __\ /__ _/   .-` /  /  -
//     =====`-.____`.___ \_____/___.-`___.-'=====
//                       `=---='
//
//
//     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//               佛祖保佑         永無BUG
//
//
//

void Trace( string function ) {
  // cout << "Function : "  << function << endl ;
} // Trace()

// ///////
bool Sign( string token )  ;
bool Assignment_operator( string token ) ;
void TokenAndVector( string & token, vector<string> & HasToken )  ;
bool Type_specifier( string token ) ;
bool Rest_of_declarators( string id, string type ) ;
bool Formal_parameter_list() ;
bool Declaration() ;
bool Function_definition_without_ID( string id, string type ) ;
bool Function_definition_or_declarators( string id, string type  ) ;
bool Compound_statement() ;
bool Statement() ;
bool Definition() ;
bool Expression() ;
bool User_input() ;
bool Basic_expression() ;
bool Rest_of_Identifier_started_basic_exp() ;
bool Rest_of_PPMM_Identifier_started_basic_exp() ;
bool Signed_unary_exp()  ;
bool Romce_and_romloe() ;
bool Actual_parameter_list() ;
bool Rest_of_maybe_logical_OR_exp()  ;
bool Maybe_logical_AND_exp() ;
bool Rest_of_maybe_logical_AND_exp() ;
bool Maybe_bit_OR_exp() ;
bool Rest_of_maybe_bit_OR_exp() ;
bool Maybe_bit_ex_OR_exp() ;
bool Rest_of_maybe_bit_ex_OR_exp() ;
bool Maybe_bit_AND_exp() ;
bool Rest_of_maybe_bit_AND_exp() ;
bool Maybe_equality_exp() ;
bool Rest_of_maybe_equality_exp() ;
bool Maybe_relational_exp() ;
bool Rest_of_maybe_relational_exp() ;
bool Maybe_shift_exp() ;
bool Rest_of_maybe_shift_exp() ;
bool Maybe_additive_exp() ;
bool Rest_of_maybe_additive_exp() ;
bool Maybe_mult_exp() ;
bool Rest_of_maybe_mult_exp() ;
bool Unary_exp() ;
bool Unsigned_unary_exp() ;
void PushNewFunction( string id ) ;
bool DefinedIDorFunction( string id ) ;

vector<AllVar>  gAllVarDefine ;
vector<Function> gDefinedFunction  ;
vector<string> gidpoll ;  // 放連續定義
vector<int>    gconstpoll ; //  放變數
vector<AllVar> gFunctionDefine ;
vector<string> gFunctionToken ;

vector<vector<AllVar> > gwhileFunction ;
bool gIfFunction = false ;
bool glikeFunction = false ;
int gCinOrCout = 0 ;


 
void PushNewFunction( string id ) {
  
  Function swap ;
  swap.id = id ;
  for ( int i = 0 ; i < gFunctionToken.size() ; i ++ )
    swap.token.push_back( gFunctionToken[i] ) ;
  bool hasDef = false ;
  for ( int i = 0 ; i < gDefinedFunction.size() ; i ++ ) {
    if ( gDefinedFunction[i].id == id ) hasDef = true ;
  } // for()
  
  if ( hasDef ) {
    for ( int i = 0 ; i < gDefinedFunction.size() ; i ++ ) {
      if ( gDefinedFunction[i].id == id ) {
        gDefinedFunction[i] = swap ;
      } // if()
    } // for()
  } // if()
  
  else
    gDefinedFunction.push_back( swap ) ;
  gFunctionToken.clear() ;
  gidpoll.clear() ;
} // PushNewFunction()

void PushNewAddress( string id, string type, int size ) {
  //  cout << id << endl ;
  if ( gIfFunction ) {
    if ( DefinedIDorFunction( id ) ) {
      for ( int i = 0 ; i < gFunctionDefine.size() ; i ++ ) {
        if ( gFunctionDefine[i].id == id ) {
          gFunctionDefine[i].type = type ;
          gFunctionDefine[i].size = size ;
        } // if()
      } // for()
    } // if()
    
    else {
      AllVar swap ;
      swap.id = id ;
      swap.type = type ;
      swap.size = size ;
      //  cout << id << endl ;
      if ( gwhileFunction.size() != 0 )
        gwhileFunction[gwhileFunction.size() - 1].push_back( swap ) ;

    }  // else
  } // if ()
  
  else {
    if ( DefinedIDorFunction( id ) ) {
      for ( int i = 0 ; i < gAllVarDefine.size() ; i ++ ) {
        if ( gAllVarDefine[i].id == id ) {
          gAllVarDefine[i].type = type ;
          gAllVarDefine[i].size = size ;
        } // if()
      } // for()
    } // if()
    
    else {
      AllVar swap ;
      swap.id = id ;
      swap.size = size ;
      swap.type = type ;
      gAllVarDefine.push_back( swap ) ;
    } // else
  } // else
} // PushNewAddress()

bool DefinedIDorFunctionTWO( string id ) {  // 判斷是否存在
  string peektoken ;
  if ( gIfFunction ) {
    //    cout << " ???" << endl ;
    for ( int i = 0 ; i < gwhileFunction.size() ; i ++ ) {
      for ( int k = 0 ; k < gwhileFunction[i].size() ; k ++ )
        if ( gwhileFunction[i][k].id == id ) return true ;
    } // for()
    
    for ( int i = 0 ; i < gAllVarDefine.size() ; i ++ )
      if ( gAllVarDefine[i].id == id ) return true ;
    for ( int i = 0 ; i < gFunctionDefine.size() ; i ++ )
      if ( gFunctionDefine[i].id == id ) return true ;
    for ( int i = 0 ; i < gDefinedFunction.size() ; i ++ ) {
      if ( gDefinedFunction[i].id == id ) {
        Peektoken( peektoken ) ;
        if ( peektoken != "(" ) throw new Undefined( id ) ;
        return true ;
      } // if()
    } // for()
  
    return false ;
  } // if()
  
  else {
    for ( int i = 0 ; i < gAllVarDefine.size() ; i ++ )
      if ( gAllVarDefine[i].id == id ) return true ;
    for ( int i = 0 ; i < gDefinedFunction.size() ; i ++ ) {
      if ( gDefinedFunction[i].id == id ) {
        Peektoken( peektoken ) ;
        if ( peektoken != "(" ) throw new Undefined( id ) ;
        return true ;
      } // if()
    } // for()
  } // else
  
  return false ;
} // DefinedIDorFunctionTWO()

bool DefinedIDorFunction( string id ) {  // 判斷是否存在
  if ( gIfFunction ) {
    //    cout << " ???" << endl ;
    for ( int i = 0 ; i < gwhileFunction.size() ; i ++ ) {
      for ( int k = 0 ; k < gwhileFunction[i].size() ; k ++ )
        if ( gwhileFunction[i][k].id == id ) return true ;
    } // for()
    
    for ( int i = 0 ; i < gFunctionDefine.size() ; i ++ )
      if ( gFunctionDefine[i].id == id ) return true ;
    for ( int i = 0 ; i < gDefinedFunction.size() ; i ++ )
      if ( gDefinedFunction[i].id == id ) return true ;
    for ( int i = 0 ; i < gAllVarDefine.size() ; i ++ )
      if ( gAllVarDefine[i].id == id ) return true ;
    return false ;
  } // if()
  
  else {
    for ( int i = 0 ; i < gAllVarDefine.size() ; i ++ )
      if ( gAllVarDefine[i].id == id ) return true ;
    for ( int i = 0 ; i < gDefinedFunction.size() ; i ++ )
      if ( gDefinedFunction[i].id == id ) return true ;
  } // else
  
  return false ;
} // DefinedIDorFunction()

void ListVariable( string id ) {
  string type ;
  int size = 0 ;
  for ( int i = 0 ; i < gAllVarDefine.size() ; i ++ ) {
    if ( gAllVarDefine[i].id == id ) {
      type = gAllVarDefine[i].type ;
      size = gAllVarDefine[i].size ;
    } // if()
  } // for()
  
  cout << "> " << type << " " << id  ;
  if ( size != 0 )
    cout << "[ " << size << " ]" ;
  cout << " ;" << endl ;
} // ListVariable()

void ListFunction( string id ) {
  vector<string> allVariables ;
  
  for ( int i = 0 ; i < gDefinedFunction.size() ; i ++ ) {
    if ( gDefinedFunction[i].id == id ) {
      for ( int j = 0 ; j < gDefinedFunction[i].token.size() ; j ++ ) {
        allVariables.push_back(  gDefinedFunction[i].token[j] ) ;
      } // for()
    } // if()
  } // for ()

  
  cout << "> " ;
  int spaceNUM = 0 ;
  for ( int k = 0 ; k < allVariables.size() ; k ++ ) {
    if ( k + 1 < allVariables.size() ) {
      if ( allVariables[k+1] == "++" || allVariables[k+1] == "--"
           || allVariables[k+1] == "[" || allVariables[k+1] == ","
           || allVariables[k+1] == "(" ) cout << allVariables[k] ;
      else if ( allVariables[k] == "{" || allVariables[k] == ";"
                || allVariables[k] == "}" ) cout << allVariables[k] ;
      else if ( allVariables[k] == "(" && allVariables[k+1] == ")" )
        cout << allVariables[k] ;
      else cout << allVariables[k] << " " ;
      
      if ( allVariables[k] == "while" || allVariables[k] == "if" ) cout << " " ;
      if ( allVariables[k] == "do"  )
        spaceNUM ++ ;
      else if ( allVariables[k] == "{" ) spaceNUM ++ ;
      //  if ( allVariables[k] == "}" ) spaceNUM -- ;
      if ( allVariables[k] == ";" || allVariables[k] == "{" || allVariables[k] == "}" ) {
        if ( k + 1 < allVariables.size() && allVariables[k+1] == "}" )
          spaceNUM -- ;
        cout << endl ;
        for ( int space = spaceNUM ; space != 0 ; space -- )
          cout << "  " ;
      } // if()
    } // if()
  } // for ()
  
  cout << allVariables[ allVariables.size() -1 ] ;
  
  cout << endl ;
} // ListFunction()

void ListAllFunctions() {
  Trace( "ListAllFunctions" ) ;
  //  cout << "ListAllFunctions" << endl ;
  cout << "> " ;
  for ( int i = ( int ) gDefinedFunction.size() - 1 ; i > 0 ; --i )
    for ( int j = 0 ; j < i ; ++j )
      if ( gDefinedFunction[j].id > gDefinedFunction[j + 1].id ) {
        Function swap = gDefinedFunction[j] ;
        gDefinedFunction[ j ] = gDefinedFunction[ j + 1 ] ;
        gDefinedFunction[ j + 1 ] = swap ;
      } // if()
  
  for ( int i = 0 ; i < gDefinedFunction.size() ; i ++ )
    cout << gDefinedFunction[i].id << "()" << endl ;
} // ListAllFunctions()

void ListAllVariables() {
  //  vector<string> allVariables ;
  cout << "> " ;
  for ( int i = ( int ) gAllVarDefine.size() - 1; i > 0 ; --i )
    for ( int j = 0 ; j < i ; ++j )
      if ( gAllVarDefine[ j ].id > gAllVarDefine[j + 1].id ) {
        AllVar swap = gAllVarDefine[j] ;
        gAllVarDefine[ j ] = gAllVarDefine[ j + 1 ] ;
        gAllVarDefine[ j + 1 ] = swap ;
      } // if()
  
  for ( int i = 0 ; i < gAllVarDefine.size() ; i ++ )
    cout << gAllVarDefine[i].id << endl ;
  
} // ListAllVariables()

bool Assignment_operator( string token ) {
  if ( token == "=" ) return true ;
  else if ( token == "*=" ) return true ;
  else if ( token == "/=" ) return true ;
  else if ( token == "%=" ) return true ;
  else if ( token == "+=" ) return true ;
  else if ( token == "-=" ) return true ;
  
  return false ;
  // : '=' | TE | DE | RE | PE | ME
} // Assignment_operator()

bool Sign( string token ) {
  if ( token == "+" || token == "-" || token == "!" ) return true ;
  return false ;
  // : '+' | '-' | '!'
} // Sign()

void TokenAndVector( string & token, vector<string> & HasToken ) {
  token = TokenID() ;
  if ( gIfFunction == true ) gFunctionToken.push_back( token ) ;
  HasToken.push_back( token ) ;
} // TokenAndVector()

bool IFConstant( string token ) {
  Trace( " IFConstant" ) ;
  string peektoken ;
  
  if ( IfArith( token[0] ) ) return true ;
  if ( token == "true" || token == "false" ) {
    return true ;
  } // if()
  
  if ( token[0] == '\'' ) {
    return true ;
  } // if()
  
  if ( token[0] == '\"' ) return true ;
 
  return false ;
  // e.g., 35, 35.67, 'a', "Hi, there", true, false
  //       .35, 35., 0023
} // IFConstant()

bool Type_specifier( string token ) {
  Trace( "Type_specifier" ) ;
  if ( token == "int" ) return true ;
  else if ( token == "char" ) return true ;
  else if ( token == "float" ) return true ;
  else if ( token == "string" ) return true ;
  else if ( token == "bool" ) return true ;
  return false ;
} // Type_specifier()

bool Definition() {
  Trace( "Definition" ) ;
  vector<string> hasToken ;
  string token = "", peektoken = "" ;
  Peektoken( peektoken ) ;
  
  string id = "", type = "" ;
  string ifFun ;
  if ( peektoken == "void" ) {
    gIfFunction = true ;   // 開始是function
    TokenAndVector( token, hasToken ) ; // get void
    type = token ;
    TokenAndVector( token, hasToken ) ; // get id
    if ( IfIDENT( token[0] ) ) id = token ;
    else throw new Unexpected( token ) ;
    
    Peektoken( ifFun ) ;
    gIfFunction = true ;
    if ( IfIDENT( token[0] ) ) {
      if ( !Function_definition_without_ID( id, type ) ) {
        Peektoken( peektoken ) ;
        throw new Unexpected( peektoken ) ;
      } // if()
    } // if()
    
    else throw new Unexpected( token ) ;
    
  } // if()
  
  else if ( Type_specifier( peektoken ) ) {
    TokenAndVector( token, hasToken ) ; // get Type)specifier
    type = token ;

    TokenAndVector( token, hasToken ) ; // get id
    Peektoken( ifFun ) ;
    if ( IfIDENT( token[0] ) ) id = token ;
    else throw new Unexpected( token ) ;
    
    if ( ifFun == "(" ) {
      gIfFunction = true ;
      gFunctionToken.push_back( type ) ;
      gFunctionToken.push_back( id ) ;
    } // if()
    
    if ( !Function_definition_or_declarators( id, type ) ) {
      Peektoken( peektoken ) ;
      throw new Unexpected( peektoken ) ;
    } // if()
    
  } // else if()
  
  else if ( peektoken == "ListAllFunctions" ) {
    TokenAndVector( token, hasToken ) ; // get ListAllfunction
    TokenAndVector( token, hasToken ) ; // get (
    if ( token == "(" ) {
      TokenAndVector( token, hasToken ) ; // get )
      if ( token == ")" ) {
        TokenAndVector( token, hasToken ) ; // get ;
        if ( token == ";" )
          ListAllFunctions() ;
      } // if()
      
      else throw new Unexpected( token ) ;
    } // if()
    
    else throw new Unexpected( token ) ;
    cout << "Statement executed ..." << endl ;
  } // else if()
  
  else if ( peektoken == "ListAllVariables" ) {
    TokenAndVector( token, hasToken ) ; // get ListAllfunction
    TokenAndVector( token, hasToken ) ; // get (
    if ( token == "(" ) {
      TokenAndVector( token, hasToken ) ; // get )
      if ( token == ")" ) {
        TokenAndVector( token, hasToken ) ; // get ;
        if ( token == ";" ) ListAllVariables() ;
      } // if()
      
      else throw new Unexpected( token ) ;
    } // if()
    
    else throw new Unexpected( token ) ;
    cout << "Statement executed ..." << endl ;
  } // else if()
  
  else if ( peektoken == "ListFunction" ) {
    TokenAndVector( token, hasToken ) ; // get ListAllfunction
    TokenAndVector( token, hasToken ) ; // get (
    if ( token == "(" ) {
      TokenAndVector( token, hasToken ) ; // get id
      string swapid ;
      for ( int k = 0 ; k < token.size() ; k ++ ) {
        if ( token[k] != '"' ) swapid += token[k] ;
      } // for()
      
      //      cout << swapid << endl ;
      id = swapid ;
      TokenAndVector( token, hasToken ) ; // get )
      if ( token == ")" ) {
        TokenAndVector( token, hasToken ) ; // get ;
        if ( token == ";" ) {
          if ( DefinedIDorFunction( id ) )
            ListFunction( id ) ;
          else throw new Undefined( id ) ;
        } // if()
      } // if()
      
      else throw new Unexpected( token ) ;
    } // if()
    
    else throw new Unexpected( token ) ;
    cout << "Statement executed ..." << endl ;
  } // else if()
  
  else if ( peektoken == "ListVariable" ) {
    TokenAndVector( token, hasToken ) ; // get ListVariable
    // cout << "hello" << endl ;
    
    TokenAndVector( token, hasToken ) ; // get (
    if ( token == "(" ) {
      TokenAndVector( token, hasToken ) ; // get id
      string swapid ;
      for ( int k = 0 ; k < token.size() ; k ++ ) {
        if ( token[k] != '"' ) swapid += token[k] ;
      } // for()
      
      id = swapid ;
      TokenAndVector( token, hasToken ) ; // get )
      if ( token == ")" ) {
        TokenAndVector( token, hasToken ) ; // get ;
        if ( token == ";" )  {
          if ( DefinedIDorFunction( id ) ) {
            ListVariable( id ) ;
          } // if()
          
          else throw new Undefined( id ) ;
        } // if ()
      } // if()
      
      else throw new Unexpected( token ) ;
    } // if()
    
    else throw new Unexpected( token ) ;
    
    cout << "Statement executed ..." << endl ;
  } // else if()
  
  else if ( peektoken == "Done" ) {
    TokenAndVector( token, hasToken ) ; // get Done
    TokenAndVector( token, hasToken ) ; // get Done
    if ( token == "(" ) {
      TokenAndVector( token, hasToken ) ; // get Done
      if ( token == ")" ) {
        TokenAndVector( token, hasToken ) ; // get ;
        if ( token == ";" )
          throw new ExitOurC( "Done()" ) ;
        else throw new Unexpected( token ) ;
      } // if()
    } // if()
    
    else throw new Unexpected( token ) ;
  } // else if()
  
  else return false ;
  
  if ( ifFun == "(" ) {
    cout << "> " ;
    bool hasDef = false ;
    for ( int i = 0 ; i < gDefinedFunction.size() ; i ++ ) {
      if ( gDefinedFunction[i].id == id ) hasDef = true ;
    } // for()
    
    if ( hasDef )
      cout << "New definition of " << id << "()" << " entered ..." << endl ;
    else
      cout << "Definition of " << id << "()" << " entered ..." << endl ;
    PushNewFunction( id ) ;
  } // if ()
  else {
    if ( gidpoll.size() != 0 ) cout << "> " ;
    bool hasDef = false ;
    for ( int i = 0 ; i < gidpoll.size() ; i ++ ) {
      hasDef = false ;
      for ( int k = 0 ; k < gAllVarDefine.size() ; k ++ ) {
        if ( gAllVarDefine[k].id == gidpoll[i] ) hasDef = true ;
      } // for()
      
      if ( hasDef )
        cout << "New definition of " << gidpoll[i] << " entered ..." << endl ;
      else
        cout << "Definition of " << gidpoll[i] << " entered ..." << endl ;
      PushNewAddress( gidpoll[i], type, gconstpoll[i] ) ;
    } // for()
  } // else
  
  return true ;
  // :           VOID Identifier function_definition_without_ID
  // | type_specifier Identifier function_definition_or_declarators
} // Definition()

bool Rest_of_declarators( string id, string type  ) {
  Trace( "Rest_of_declarators " ) ;
  vector<string> hasToken ;
  string token = "", peektoken = "" ;
  bool hasConstant = false ;
  Peektoken( peektoken ) ;
  if ( peektoken == "[" ) {
    TokenAndVector( token, hasToken ) ; // get "["
    TokenAndVector( token, hasToken ) ; // get constant ;
    if ( IFConstant( token ) ) {
      gconstpoll.push_back( atoi( token.c_str() ) ) ;
    } // if()
    else throw new Unexpected( token ) ;
    TokenAndVector( token, hasToken ) ; // get "]"
    if ( token == "]" ) ;
    else throw new Unexpected( token ) ;
    hasConstant = true ;
  } // if()
  
  else gconstpoll.push_back( 0 ) ;
  bool stop = false ;
  Peektoken( peektoken ) ;
  vector<bool> array ;
  gidpoll.push_back( id ) ;
  while ( !stop ) {
    Peektoken( peektoken ) ;
    if ( peektoken != "," ) stop = true ;
    else {
      TokenAndVector( token, hasToken ) ; // get ","
      TokenAndVector( token, hasToken ) ; // get ID
      if ( IfIDENT( token[0] ) ) gidpoll.push_back( token ) ;
      else throw new Unexpected( token ) ;
      if ( !IfIDENT( token[0] ) )  throw new Unexpected( token ) ;
      Peektoken( peektoken ) ;
      if ( peektoken == "[" ) {
        TokenAndVector( token, hasToken ) ; // get "["
        TokenAndVector( token, hasToken ) ; // get constant ;
        if ( IFConstant( token ) ) {
          gconstpoll.push_back( atoi( token.c_str() ) ) ;
        } // if()
        
        else throw new Unexpected( token ) ;
        TokenAndVector( token, hasToken ) ; // get "]"
        if ( token == "]" ) ;
        else throw new Unexpected( token ) ;
      } // if()
      
      else gconstpoll.push_back( 0 ) ;
    } // else ()
  } // while()
  
  if ( peektoken != ";" ) throw new Unexpected( peektoken ) ;
  TokenAndVector( token, hasToken ) ;  // get ";"
  return true ;
  // : [ '[' Constant ']' ]
  // { ',' Identifier [ '[' Constant ']' ] } ';'
} // Rest_of_declarators()

bool Compound_statement( ) {
  Trace( "Compound_statement()" ) ;
  vector<string> hasToken ;
  string token = "", peektoken = "" ;
  Peektoken( peektoken ) ;
  bool stop = false, statementCorrect = false, declarationCorrect = false ;
  vector<AllVar> swap ;
  if ( peektoken == "{" ) {
    gwhileFunction.push_back( swap ) ;
    glikeFunction = true ;
    gIfFunction = true ;
    TokenAndVector( token, hasToken ) ; // get "{"
    while ( !stop ) {
      Peektoken( peektoken ) ;
      if ( !Declaration() ) {
        Peektoken( peektoken ) ;
        if ( peektoken == "}" ) stop = true ;
        else Statement() ;
        
        Peektoken( peektoken ) ;
        if ( peektoken == "}" ) stop = true ;
      } // if()
    } // while()
    
    TokenAndVector( token, hasToken ) ;
    if ( token != "}" ) throw new Unexpected( token );
    gwhileFunction.pop_back() ;
    // cout << "done" << endl ;
  } // if()
  
  else return false ;
  return true ;
  // : '{' { declaration | statement } '}'
} // Compound_statement()

bool Formal_parameter_list() {
  Trace( "Formal_parameter_list()" ) ;
  vector<string> hasToken ;
  string token = "", peektoken = "" ;
  string type, id, size  ;
  int swapSize = 0 ;
  Peektoken( peektoken ) ;
  if ( Type_specifier( peektoken ) ) {
    TokenAndVector( token, hasToken ) ; // get Type_specifier
    type = token ;
    Peektoken( peektoken ) ;
    if ( peektoken == "&" ) {
      TokenAndVector( token, hasToken ) ;  // get "&"
    } // if()
    
    Peektoken( peektoken ) ;
    if ( IfIDENT( peektoken[0] ) ) {
      TokenAndVector( token, hasToken ) ; // get id
      id = token ;
      Peektoken( peektoken ) ;
      if ( peektoken == "[" ) {
        TokenAndVector( token, hasToken ) ; // get "["
        TokenAndVector( token, hasToken ) ; // get constant ;
        size = token ;
        if ( IFConstant( token ) ) {
        } // if()
        else throw new Unexpected( token ) ;
        TokenAndVector( token, hasToken ) ; // get "]"
        if ( token == "]" ) ;
        else throw new Unexpected( token ) ;
      } // if()
    } // if()
    
    else throw new Unexpected( peektoken ) ;
    gidpoll.push_back( id ) ;
    swapSize = atoi( size.c_str() ) ;
    vector<AllVar> swap ;
    gwhileFunction.push_back( swap ) ;
    gIfFunction = true ;
    PushNewAddress( id, type, swapSize ) ;
  } // if()
  
  else return false ;
  bool stop = false ;
  while ( !stop ) {
    Peektoken( peektoken ) ;
    if ( peektoken != "," ) stop = true ;
    else {
      TokenAndVector( token, hasToken ) ; // get ","
      TokenAndVector( token, hasToken ) ; // get Type_specifier
      if ( !Type_specifier( token ) ) throw new Unexpected( token ) ;
      Peektoken( peektoken ) ;
      if ( peektoken == "&" ) {
        TokenAndVector( token, hasToken ) ;  // get "&"
      } // if()
      
      TokenAndVector( token, hasToken ) ; // get id
      if ( !IfIDENT( token[0] ) )  throw new Unexpected( token ) ;
      id = token ;
      Peektoken( peektoken ) ;
      if ( peektoken == "[" ) {
        TokenAndVector( token, hasToken ) ; // get "["
        TokenAndVector( token, hasToken ) ; // get constant ;
        if ( ! IFConstant( token ) ) throw new Unexpected( token ) ;
        size = token ;
        TokenAndVector( token, hasToken ) ; // get "]"
        if ( token == "]" ) ;
        else throw new Unexpected( token ) ;
      } // if()
    } // else
    
    gidpoll.push_back( id ) ;
    swapSize = atoi( size.c_str() ) ;
    PushNewAddress( id, type, swapSize ) ;
  } // while()
  
  
  return true ;
  // : type_specifier [ '&' ] Identifier [ '[' Constant ']' ]
  // { ',' type_specifier [ '&' ] Identifier [ '[' Constant ']' ] }
} // Formal_parameter_list()

bool Declaration() {
  Trace( "Declaration()" ) ;
  vector<string> hasToken ;
  string token = "", peektoken = "" ;
  string id = "", type = "" ;
  Peektoken( peektoken ) ;
  if ( Type_specifier( peektoken ) ) {
    gIfFunction = true ;
    TokenAndVector( token, hasToken ) ; // get type
    type = token ;
    TokenAndVector( token, hasToken ) ; // get id
    if ( IfIDENT( token[0] ) ) id = token ;
    else throw new Unexpected( token ) ;
    id = token ;

    if ( IfIDENT( token[0] ) ) {
      if ( Rest_of_declarators( id, type ) ) {
        PushNewAddress( id, type, 0 ) ;
        return true ;
      } // if()
      
      return false ;
    } // if()
    
    else throw new Unexpected( token ) ;
  } // if()
  
  else {
    return false ;
  } // else
  // : type_specifier Identifier rest_of_declarators
} // Declaration()

bool Function_definition_or_declarators( string id, string type  ) {
  Trace( "Function_definition_or_declarators()" ) ;
  string peektoken ;
  Peektoken( peektoken ) ;
  if ( Function_definition_without_ID( id, type ) ) return true ;
  else if ( Rest_of_declarators( id, type ) ) return true ;
  else return false ;
  //  : function_definition_without_ID
  // | rest_of_declarators
} // Function_definition_or_declarators()

bool Function_definition_without_ID( string id, string type ) {
  Trace( "Function_definition_without_ID()" ) ;
  vector<string> hasToken ;
  string token = "", peektoken = "" ;
  bool formal_parameterCorrect = false ;
  Peektoken( peektoken ) ;
  if ( peektoken == "(" ) {
    TokenAndVector( token, hasToken ) ;  // get "("
    Peektoken( peektoken ) ;
    if ( peektoken == "void" ) {
      TokenAndVector( token, hasToken ) ;
    } // if()
    
    else if ( Formal_parameter_list() )  formal_parameterCorrect = true ;
    TokenAndVector( token, hasToken ) ; // get ")"
    
    if ( token == ")" ) {
      if ( Compound_statement() ) return true ;
      else {
        Peektoken( peektoken ) ;
        throw new Unexpected( peektoken ) ;
      } // else
    } // if()
    
    else throw new Unexpected( token ) ;
  } // if()
  
  else return false ;
  
  // return true ;
  // : '(' [ VOID | formal_parameter_list ] ')' compound_statement
  
} // Function_definition_without_ID()

bool Basic_expression() {
  Trace( "Basic_expression() " ) ;
  vector<string> hasToken ;
  string token = "", peektoken = "" ;
  string id = "" ; // 存放id
  Peektoken( peektoken ) ;
  if ( IFConstant( peektoken ) || peektoken == "(" ) {
    TokenAndVector( token, hasToken ) ; // get Constant || "("
    if ( token == "(" ) {
      Peektoken( peektoken ) ;
      if ( peektoken == "if" || peektoken == "while" || peektoken == "else"
           || peektoken == "do" || peektoken == "return" )
        throw new Unexpected( peektoken ) ;
      if ( !Expression() ) throw new Unexpected( peektoken ) ;
      TokenAndVector( token, hasToken ) ; // get ")"
      if ( token != ")" ) throw new Unexpected( token  ) ;
    } // if()
    
    Peektoken( peektoken ) ;
    if ( peektoken == ";" ) return true ;
    if ( !Romce_and_romloe() ) throw new Unexpected( peektoken ) ;
  } // if()
  
  else if ( IfIDENT( peektoken[0] ) ) {
    TokenAndVector( token, hasToken ) ; // get id
    id = token ;
    if ( !DefinedIDorFunctionTWO( id ) ) throw new Undefined( id ) ;
    
    if ( !Rest_of_Identifier_started_basic_exp() ) {
      Peektoken( peektoken ) ;
      throw new Unexpected( peektoken ) ;
    } // if()
  } // if()
  
  else if ( peektoken == "++" || peektoken == "--" ) {
    TokenAndVector( token, hasToken ) ; // get "++" || "--"
    TokenAndVector( token, hasToken ) ; // get id
    if ( IfIDENT( token[0] ) ) {
      id = token ;
      if ( !DefinedIDorFunctionTWO( id ) ) throw new Undefined( id ) ;
      if ( !Rest_of_PPMM_Identifier_started_basic_exp() ) {
        Peektoken( peektoken ) ;
        throw new Unexpected( peektoken ) ;
      } // if()
    } // if()
    
    else {
      Peektoken( peektoken ) ;
      throw new Unexpected( peektoken ) ;
    } // else
  } // else if()
  
  else if ( Sign( peektoken ) ) {
    TokenAndVector( token, hasToken ) ; // get sign
    bool stop = false ;
    while ( !stop ) {
      Peektoken( peektoken ) ;
      if ( !Sign( peektoken ) ) stop = true ;
      else TokenAndVector( token, hasToken ) ; // get sign
    } // while()
    
    if ( !Signed_unary_exp() ) {
      Peektoken( peektoken ) ;
      throw new Unexpected( peektoken ) ;
    } // if()
    
    if ( !Romce_and_romloe() ) {
      Peektoken( peektoken ) ;
      throw new Unexpected( peektoken ) ;
    } // if()
  } // else if()
  
  
  else return false ;
  return true ;
  //  basic_expression
  //  : Identifier rest_of_Identifier_started_basic_exp
  //  | ( PP | MM ) Identifier rest_of_PPMM_Identifier_started_basic_exp
  //  | sign { sign } signed_unary_exp romce_and_romloe
  //  | ( Constant | '(' expression ')' ) romce_and_romloe
} // Basic_expression()

bool Rest_of_PPMM_Identifier_started_basic_exp() {
  Trace( "Rest_of_PPMM_Identifier_started_basic_exp() " ) ;
  vector<string> hasToken ;
  string token = "", peektoken = "" ;
  Peektoken( peektoken ) ;
  if ( peektoken == "[" ) {
    TokenAndVector( token, hasToken ) ;  // get "["
    if ( !Expression() ) {
      Peektoken( peektoken ) ;
      throw new Unexpected( peektoken ) ;
    } // if()
    
    Peektoken( peektoken ) ;
    if ( peektoken != "]" ) throw new Unexpected( peektoken ) ;
  } // if ()
  
  if ( !Romce_and_romloe() ) {
    Peektoken( peektoken ) ;
    throw new Unexpected( peektoken ) ;
  } // if()
  
  return false ;
  // : [ '[' expression ']' ] romce_and_romloe
} // Rest_of_PPMM_Identifier_started_basic_exp()

bool Rest_of_Identifier_started_basic_exp() {
  Trace( "Rest_of_Identifier_started_basic_exp " ) ;
  vector<string> hasToken ;
  string token = "", peektoken = "" ;
  Peektoken( peektoken ) ;
  if ( peektoken == "[" ) {
    TokenAndVector( token, hasToken ) ;  // get "["
    if ( !Expression() ) {
      Peektoken( peektoken ) ;
      throw new Unexpected( peektoken ) ;
    } // if()
    
    Peektoken( peektoken ) ;
    if ( peektoken == "]" ) TokenAndVector( token, hasToken ) ;  // get "]"
  } // if()
  
  Peektoken( peektoken ) ;
  if ( Assignment_operator( peektoken ) ) {
    TokenAndVector( token, hasToken ) ;  // get Assignment
    if ( !Basic_expression() ) {
      Peektoken( peektoken ) ;
      throw new Unexpected( peektoken ) ;
    } // if()
  } // if()
  
  else if ( peektoken == "++" || peektoken == "--" ) {
    TokenAndVector( token, hasToken ) ; // get "++ | -- "
    if ( !Romce_and_romloe() ) {
      Peektoken( peektoken ) ;
      throw new Unexpected( peektoken ) ;
    } // if()
  } // else if ()
  
  else if ( peektoken == "(" ) {
    TokenAndVector( token, hasToken ) ; // get "("
    Actual_parameter_list()  ;
    Peektoken( peektoken ) ;
    if ( peektoken == ")" )  TokenAndVector( token, hasToken ) ; // get ")"
    else throw new Unexpected( peektoken ) ;
    if ( !Romce_and_romloe() ) {
      Peektoken( peektoken ) ;
      throw new Unexpected( peektoken ) ;
    } // if()
  } // else if()
  
  else if ( IFConstant( peektoken ) ) throw new Unexpected( peektoken ) ;
  else if ( Romce_and_romloe() ) {
    return true ;
  } // else if()
  
  else return false ;
  return true ;
  // : [ '[' expression ']' ]
  // ( assignment_operator basic_expression
  // |
  // [ PP | MM ] romce_and_romloe
  // )
  // | '(' [ actual_parameter_list ] ')' romce_and_romloe
} // Rest_of_Identifier_started_basic_exp()

bool Actual_parameter_list() {
  Trace( "Actual_parameter_list()" ) ;
  vector<string> hasToken ;
  string token = "", peektoken = "" ;
  bool stop = false ;
  Basic_expression() ;
  while ( !stop ) {
    Peektoken( peektoken ) ;
    if ( peektoken == "," ) TokenAndVector( token, hasToken ) ; // get ","
    else stop = true ;
    Peektoken( peektoken ) ;
    if ( !stop && !Basic_expression() ) {
      Peektoken( peektoken ) ;
      throw new Unexpected( peektoken ) ;
    } // if()
  } // while()
  
  return true ;
  // : basic_expression { ',' basic_expression }
} // Actual_parameter_list()

bool Rest_of_maybe_logical_OR_exp() {
  Trace( "Rest_of_maybe_logical_OR_exp()" ) ;
  vector<string> hasToken ;
  string token = "", peektoken = "" ;
  bool stop = false ;
  Rest_of_maybe_logical_AND_exp() ;
  while ( !stop ) {
    Peektoken( peektoken ) ;
    if ( peektoken == "||" ) TokenAndVector( token, hasToken ) ; // get "||"
    else stop = true ;
    Peektoken( peektoken ) ;
    if ( !stop && !Maybe_logical_AND_exp() ) {
      Peektoken( peektoken ) ;
      throw new Unexpected( peektoken ) ;
    }  // if()
  } // while()
  
  return true ;
  // : rest_of_maybe_logical_AND_exp { OR maybe_logical_AND_exp }
} // Rest_of_maybe_logical_OR_exp()

bool Maybe_logical_AND_exp() {
  Trace( "Maybe_logical_logical_AND_exp()" ) ;
  vector<string> hasToken ;
  string token = "", peektoken = "" ;
  bool stop = false ;
  Maybe_bit_OR_exp() ;
  while ( !stop ) {
    Peektoken( peektoken ) ;
    if ( peektoken == "&&" ) TokenAndVector( token, hasToken ) ; // get "&&"
    else  stop = true ;
    Peektoken( peektoken ) ;
    if ( !stop && !Maybe_bit_OR_exp() ) {
      Peektoken( peektoken ) ;
      throw new Unexpected( peektoken ) ;
    } // if()
  } // while()
  
  return true ;
  // : maybe_bit_OR_exp { AND maybe_bit_OR_exp }
} // Maybe_logical_AND_exp()

bool Rest_of_maybe_logical_AND_exp() {
  Trace( "Rest_of_maybe_logical_AND_exp()" ) ;
  vector<string> hasToken ;
  string token = "", peektoken = "" ;
  bool stop = false ;
  Rest_of_maybe_bit_OR_exp() ;
  while ( !stop ) {
    Peektoken( peektoken ) ;
    if ( peektoken == "&&" ) TokenAndVector( token, hasToken ) ; // get "&&"
    else stop = true ;
    Peektoken( peektoken ) ;
    if ( !stop && !Maybe_bit_OR_exp() ) {
      Peektoken( peektoken ) ;
      throw new Unexpected( peektoken ) ;
    } // if()
  } // while()
  
  return true ;
  // : rest_of_maybe_bit_OR_exp { AND maybe_bit_OR_exp }
} // Rest_of_maybe_logical_AND_exp()


bool Maybe_bit_OR_exp() {
  Trace( "Maybe_bit_OR_exp()" ) ;
  vector<string> hasToken ;
  string token = "", peektoken = "" ;
  bool stop = false ;
  Maybe_bit_ex_OR_exp()  ;
  while ( !stop ) {
    Peektoken( peektoken ) ;
    if ( peektoken == "|" )  TokenAndVector( token, hasToken ) ; // get "|"
    else stop = true ;
    Peektoken( peektoken ) ;
    if ( !stop && !Maybe_bit_ex_OR_exp() ) {
      Peektoken( peektoken ) ;
      throw new Unexpected( peektoken ) ;
    } // if()
  } // while()
  
  return true ;
  // : maybe_bit_ex_OR_exp { '|' maybe_bit_ex_OR_exp }
} // Maybe_bit_OR_exp()

bool Rest_of_maybe_bit_OR_exp() {
  Trace( "Rest_of_maybe_bit_OR_exp()" ) ;
  vector<string> hasToken ;
  string token = "", peektoken = "" ;
  bool stop = false ;
  Rest_of_maybe_bit_ex_OR_exp() ;
  while ( !stop ) {
    Peektoken( peektoken ) ;
    if ( peektoken == "|" )  TokenAndVector( token, hasToken ) ; // get "|"
    else stop = true ;
    Peektoken( peektoken ) ;
    if ( !stop && !Maybe_bit_ex_OR_exp() ) {
      Peektoken( peektoken ) ;
      throw new Unexpected( peektoken ) ;
    } // if()
  } // while()
  
  
  return true ;
  // : rest_of_maybe_bit_ex_OR_exp { '|' maybe_bit_ex_OR_exp }
} // Rest_of_maybe_bit_OR_exp()
//
bool Maybe_bit_ex_OR_exp() {
  Trace( "Maybe_bit_ex_OR_exp() " ) ;
  vector<string> hasToken ;
  string token = "", peektoken = "" ;
  bool stop = false ;
  Maybe_bit_AND_exp() ;
  while ( !stop ) {
    Peektoken( peektoken ) ;
    if ( peektoken == "^" )  TokenAndVector( token, hasToken ) ; // get "^"
    else stop = true ;
    Peektoken( peektoken ) ;
    if ( !stop && !Maybe_bit_AND_exp() ) {
      Peektoken( peektoken ) ;
      throw new Unexpected( peektoken ) ;
    } // if()
  } // while()
  
  return true ;
  // : maybe_bit_AND_exp { '^' maybe_bit_AND_exp }
} // Maybe_bit_ex_OR_exp()

bool Rest_of_maybe_bit_ex_OR_exp() {
  Trace( "Rest_of_maybe_bit_OR_exp()" ) ;
  vector<string> hasToken ;
  string token = "", peektoken = "" ;
  bool stop = false ;
  Rest_of_maybe_bit_AND_exp() ;
  while ( !stop ) {
    Peektoken( peektoken ) ;
    if ( peektoken == "^" )  TokenAndVector( token, hasToken ) ; // get "|"
    else stop = true ;
    Peektoken( peektoken ) ;
    if ( !stop && !Maybe_bit_AND_exp() ) {
      Peektoken( peektoken ) ;
      throw new Unexpected( peektoken ) ;
    } // if()
  } // while()
  
  return true ;
  // : rest_of_maybe_bit_AND_exp { '^' maybe_bit_AND_exp }
} // Rest_of_maybe_bit_ex_OR_exp()

bool Maybe_bit_AND_exp() {
  Trace( "Maybe_bit_AND_exp()" ) ;
  vector<string> hasToken ;
  string token = "", peektoken = "" ;
  bool stop = false ;
  Maybe_equality_exp() ;
  while ( !stop ) {
    Peektoken( peektoken ) ;
    if ( peektoken != "&" ) stop = true ;
    else TokenAndVector( token, hasToken ) ; // get "&"
    Peektoken( peektoken ) ;
    if ( !stop && !Maybe_equality_exp() ) {
      Peektoken( peektoken ) ;
      throw new Unexpected( peektoken ) ;
    } // if()
  } // while()
  
  return true ;
  // : maybe_equality_exp { '&' maybe_equality_exp }
} // Maybe_bit_AND_exp()

bool Rest_of_maybe_bit_AND_exp() {
  Trace( "Rest_of_maybe_bit_AND_exp()" ) ;
  vector<string> hasToken ;
  string token = "", peektoken = "" ;
  bool stop = false ;
  Rest_of_maybe_equality_exp() ;
  while ( !stop ) {
    Peektoken( peektoken ) ;
    if ( peektoken != "&" ) stop = true ;
    else TokenAndVector( token, hasToken ) ; // get "&"
    Peektoken( peektoken ) ;
    if ( !stop && !Maybe_equality_exp() ) {
      Peektoken( peektoken ) ;
      throw new Unexpected( peektoken ) ;
    } // if()
  } // while()
  
  return true ;
  // : rest_of_maybe_equality_exp { '&' maybe_equality_exp }
} // Rest_of_maybe_bit_AND_exp()
//
bool Maybe_equality_exp() {
  Trace( "Maybe_equality_exp()" ) ;
  vector<string> hasToken ;
  string token = "", peektoken = "" ;
  bool stop = false ;
  Maybe_relational_exp() ;
  while ( !stop ) {
    Peektoken( peektoken ) ;
    if ( peektoken != "==" && peektoken != "!=" ) stop = true ;
    else TokenAndVector( token, hasToken ) ; // get "==" | "!="
    Peektoken( peektoken ) ;
    if ( !stop && !Maybe_relational_exp() ) {
      Peektoken( peektoken ) ;
      throw new Unexpected( peektoken ) ;
    } // if()
  } // while()
  
  return true ;
  // : maybe_relational_exp
  // { ( EQ | NEQ ) maybe_relational_exp}
} // Maybe_equality_exp()

bool Rest_of_maybe_equality_exp() {
  Trace( "Rest_of_maybe_equality_exp()" ) ;
  vector<string> hasToken ;
  string token = "", peektoken = "" ;
  bool stop = false ;
  Rest_of_maybe_relational_exp() ;
  while ( !stop ) {
    Peektoken( peektoken ) ;
    if ( peektoken != "==" && peektoken != "!=" ) stop = true ;
    else TokenAndVector( token, hasToken ) ; // get "==" | "!="
    Peektoken( peektoken ) ;
    if ( !stop && !Maybe_relational_exp() ) {
      Peektoken( peektoken ) ;
      throw new Unexpected( peektoken ) ;
    } // if()
  } // while()
  
  return true ;
  // : rest_of_maybe_relational_exp
  // { ( EQ | NEQ ) maybe_relational_exp }
} // Rest_of_maybe_equality_exp()
//
bool Maybe_relational_exp() {
  Trace( "Maybe_relational_exp()" ) ;
  vector<string> hasToken ;
  string token = "", peektoken = "" ;
  bool stop = false ;
  Maybe_shift_exp() ;
  while ( !stop ) {
    Peektoken( peektoken ) ;
    if ( peektoken != "<" && peektoken != ">" &&
         peektoken != "<=" && peektoken != ">=" ) stop = true ;
    else TokenAndVector( token, hasToken ) ; // get "==" | "!="
    Peektoken( peektoken ) ;
    if ( !stop && !Maybe_relational_exp() ) {
      Peektoken( peektoken ) ;
      throw new Unexpected( peektoken ) ;
    } // if()
  } // while()
  
  return true ;
  // : maybe_shift_exp
  // { ( '<' | '>' | LE | GE ) maybe_shift_exp }
} // Maybe_relational_exp()

bool Rest_of_maybe_relational_exp() {
  Trace( "Rest_of_maybe_relational_exp()" ) ;
  vector<string> hasToken ;
  string token = "", peektoken = "" ;
  bool stop = false ;
  Rest_of_maybe_shift_exp() ;
  while ( !stop ) {
    Peektoken( peektoken ) ;
    if ( peektoken != "<" && peektoken != ">" &&
         peektoken != "<=" && peektoken != ">=" ) stop = true ;
    else TokenAndVector( token, hasToken ) ; // get ">" | "<" | ">=" | "<="
    Peektoken( peektoken ) ;
    if ( !stop && !Maybe_shift_exp() ) {
      Peektoken( peektoken ) ;
      throw new Unexpected( peektoken ) ;
    } // if()
  } // while()
  
  return true ;
  // : rest_of_maybe_shift_exp
  // { ( '<' | '>' | LE | GE ) maybe_shift_exp }
} // Rest_of_maybe_relational_exp()

bool Maybe_shift_exp() {
  Trace( "Maybe_shift_exp() " ) ;
  vector<string> hasToken ;
  string token = "", peektoken = "" ;
  bool stop = false ;
  Maybe_additive_exp() ;
  while ( !stop ) {
    Peektoken( peektoken ) ;
    if ( peektoken != "<<" && peektoken != ">>"  ) stop = true ;
    else {
      if ( gCinOrCout == 1 ) {
        if ( peektoken != ">>" ) throw new Unexpected( peektoken ) ;
      } // if()
      
      else if ( gCinOrCout == 2 )
        if ( peektoken != "<<" ) throw new Unexpected( peektoken ) ;
      TokenAndVector( token, hasToken ) ; // get ">>" | "<<"
    } // else
    
    Peektoken( peektoken ) ;
    if ( !stop &&  !Maybe_additive_exp() ) {
      Peektoken( peektoken ) ;
      throw new Unexpected( peektoken ) ;
    } // if()
  } // while()
  
  return true ;
  // : maybe_additive_exp { ( LS | RS ) maybe_additive_exp }
} // Maybe_shift_exp()

bool Rest_of_maybe_shift_exp() {
  Trace( "Rest_of_maybe_shift_exp() " ) ;
  vector<string> hasToken ;
  string token = "", peektoken = "" ;
  bool stop = false ;
  Rest_of_maybe_additive_exp() ;
  while ( !stop ) {
    Peektoken( peektoken ) ;
    if ( peektoken != "<<" && peektoken != ">>"  ) stop = true ;
    else {
      if ( gCinOrCout == 1 ) {
        if ( peektoken != ">>" ) throw new Unexpected( peektoken ) ;
      } // if()
      
      else if ( gCinOrCout == 2 )
        if ( peektoken != "<<" ) throw new Unexpected( peektoken ) ;
      TokenAndVector( token, hasToken ) ; // get ">>" | "<<"
      Peektoken( peektoken ) ;
      if ( peektoken == ";" ) throw new Unexpected( peektoken ) ;

    } // else
    
    if ( !stop &&  !Maybe_additive_exp() ) {
      Peektoken( peektoken ) ;
      throw new Unexpected( peektoken ) ;
    } // if()
  } // while()
  
  return true ;
  // : rest_of_maybe_additive_exp { ( LS | RS ) maybe_additive_exp }
} // Rest_of_maybe_shift_exp()
//
bool Maybe_additive_exp() {
  Trace( "Maybe_additive_exp() " ) ;
  vector<string> hasToken ;
  string token = "", peektoken = "" ;
  bool stop = false ;
  Maybe_mult_exp() ;
  while ( !stop ) {
    Peektoken( peektoken ) ;
    if ( peektoken != "+" && peektoken != "-"  ) stop = true ;
    else TokenAndVector( token, hasToken ) ; // get "+" | "-"
    Peektoken( peektoken ) ;
    if ( !stop &&  !Maybe_mult_exp() ) {
      Peektoken( peektoken ) ;
      throw new Unexpected( peektoken ) ;
    } // if()
  } // while()
  
  
  return true ;
  // :  maybe_mult_exp { ( '+' | '-' ) maybe_mult_exp }
} // Maybe_additive_exp()

bool Rest_of_maybe_additive_exp() {
  Trace( "Rest_of_maybe_additive_exp() " ) ;
  vector<string> hasToken ;
  string token = "", peektoken = "" ;
  bool stop = false ;
  Peektoken( peektoken ) ;
  Rest_of_maybe_mult_exp() ;
  while ( !stop ) {
    Peektoken( peektoken ) ;
    if ( peektoken != "+" && peektoken != "-"  ) stop = true ;
    else TokenAndVector( token, hasToken ) ; // get "+" | "-"
    Peektoken( peektoken ) ;
    if ( !stop &&  !Maybe_mult_exp() ) {
      Peektoken( peektoken ) ;
      throw new Unexpected( peektoken ) ;
    } // if()
  } // while()
  
  return true ;
  // : rest_of_maybe_mult_exp { ( '+' | '-' ) maybe_mult_exp }
} // Rest_of_maybe_additive_exp()
//
bool Maybe_mult_exp() {
  Trace( "Maybe_mult_exp() " ) ;
  vector<string> hasToken ;
  string token = "", peektoken = "" ;
  bool stop = false ;
  if ( Unary_exp() ) {
    Peektoken( peektoken ) ;
    if ( !Rest_of_maybe_mult_exp() ) throw new Unexpected( peektoken ) ;
    else return true ;
  } // if()
  
  else return false ;
  // : unary_exp rest_of_maybe_mult_exp
} // Maybe_mult_exp()

bool Rest_of_maybe_mult_exp() {
  Trace( "Rest_of_maybe_mult_exp() " ) ;
  vector<string> hasToken ;
  string token = "", peektoken = "" ;
  bool stop = false ;
  Peektoken( peektoken ) ;
  if ( ( peektoken != "*" && peektoken != "/" && peektoken != "%" )
       || peektoken == ";" ) return true ;
  else {
    while ( !stop ) {
      Peektoken( peektoken ) ;
      if ( peektoken != "*" && peektoken != "/" && peektoken != "%" ) stop = true ;
      else TokenAndVector( token, hasToken ) ; // get "*" | "/" | "%"
      Peektoken( peektoken ) ;
      if ( !stop && !Unary_exp() ) throw new Unexpected( peektoken ) ;
    } // while()
  } // else
  
  return true ;
  // : { ( '*' | '/' | '%' ) unary_exp }   could be empty !
} // Rest_of_maybe_mult_exp()
//
bool Unary_exp() {
  Trace( "Unary_exp() " ) ;
  vector<string> hasToken ;
  string token = "", peektoken = "" ;
  string id ;
  bool stop = false ;
  Peektoken( peektoken ) ;
  if ( Sign( peektoken ) ) {
    TokenAndVector( token, hasToken ) ; // get sign
    Peektoken( peektoken ) ;
    while ( !stop ) {
      if ( Sign( peektoken ) ) {
        TokenAndVector( token, hasToken ) ; // get sign
      } // if()
      
      else stop = true ;
      Peektoken( peektoken ) ;
    } // while()
    
    Peektoken( peektoken ) ;
    if ( !Signed_unary_exp() ) throw new Unexpected( peektoken ) ;
  } // if()
  
  else if ( Unsigned_unary_exp() )  return true ;
  else if ( peektoken == "++" || peektoken == "--" ) {
    TokenAndVector( token, hasToken ) ; // get sign
    Peektoken( peektoken ) ;
    if ( IfIDENT( peektoken[0] ) ) {
      TokenAndVector( token, hasToken ) ; // get ID
      id = token ;
      if ( !DefinedIDorFunctionTWO( id ) ) throw new Undefined( id ) ;
      Peektoken( peektoken ) ;
      if ( peektoken == "[" ) {
        TokenAndVector( token, hasToken ) ; // get [
        Peektoken( peektoken ) ;
        if ( !Expression() ) throw new Unexpected( peektoken ) ;
        Peektoken( peektoken ) ;
        if ( peektoken != "]" ) throw new Unexpected( peektoken ) ;
        else TokenAndVector( token, hasToken ) ; // get ]
      } // if()
    } // if()
    
    else throw new Unexpected( peektoken ) ;
  } // else if
  
  else return false ;
  return true ;
  // : sign { sign } signed_unary_exp
  // | unsigned_unary_exp
  // | ( PP | MM ) Identifier [ '[' expression ']' ]
} // Unary_exp()

bool Romce_and_romloe() {
  Trace( "Romce_and_romloe() " ) ;
  vector<string> hasToken ;
  string token = "", peektoken = "" ;
  bool stop = false ;
  if ( Rest_of_maybe_logical_OR_exp() ) {
    Peektoken( peektoken ) ;
    if ( peektoken == "?" ) {
      TokenAndVector( token, hasToken ) ; // get "?"
      Peektoken( peektoken ) ;
      if ( !Basic_expression() ) throw new Unexpected( peektoken ) ;
      Peektoken( peektoken ) ;
      if ( peektoken != ":" ) throw new Unexpected( peektoken ) ;
      TokenAndVector( token, hasToken ) ; // get ":"
      Peektoken( peektoken ) ;
      if ( !Basic_expression() ) throw new Unexpected( peektoken ) ;
    } // if()
  } // if()
  
  else return false ;
  return true ;
  // : rest_of_maybe_logical_OR_exp [ '?' basic_expression ':' basic_expression ]
} // Romce_and_romloe()

bool Unsigned_unary_exp() {
  Trace( "Unsigned_unary_exp() " ) ;
  vector<string> hasToken ;
  string token = "", peektoken = "" ;
  string id ;
  bool stop = false ;
  float value ;
  Peektoken( peektoken ) ;
  if ( IfIDENT( peektoken[0] ) ) {
    TokenAndVector( token, hasToken ) ; // get id
    id = token ;
    if ( !DefinedIDorFunctionTWO( id ) ) throw new Undefined( id ) ;
    Peektoken( peektoken ) ;
    if ( peektoken == "(" ) {
      TokenAndVector( token, hasToken ) ; // get "("
      Actual_parameter_list() ;
      Peektoken( peektoken ) ;
      if ( peektoken != ")" ) throw new Unexpected( peektoken ) ;
      TokenAndVector( token, hasToken ) ; // get ")"
    } // if()
    
    else if ( peektoken == "[" ) {
      TokenAndVector( token, hasToken ) ; // get "["
      Expression() ;
      Peektoken( peektoken ) ;
      if ( peektoken != "]" ) throw new Unexpected( peektoken ) ;
      TokenAndVector( token, hasToken ) ; // get ")"
      Peektoken( peektoken ) ;
      if ( peektoken == "++" || peektoken == "--" )
        TokenAndVector( token, hasToken ) ; // get ")"
    } // else if()
  } // if()
  
  else if ( IFConstant( peektoken ) ) {
    TokenAndVector( token, hasToken ) ; // get constant
  } // else if()
  
  else if ( peektoken == "(" ) {
    TokenAndVector( token, hasToken ) ; // get "("
    Peektoken( peektoken ) ;
    if ( !Expression() ) throw new Unexpected( peektoken ) ;
    Peektoken( peektoken ) ;
    if ( peektoken != ")" ) throw new Unexpected( peektoken ) ;
    TokenAndVector( token, hasToken ) ; // get ")"
  } // else if()
  
  else return false ;
  return true ;
  // : Identifier [ '(' [ actual_parameter_list ] ')'
  //              |
  //              [ '[' expression ']' ] [ ( PP | MM ) ]
  //              ]
  // | Constant
  // | '(' expression ')'
} // Unsigned_unary_exp()

bool Signed_unary_exp() {
  Trace( "Signed_unary_exp() " ) ;
  vector<string> hasToken ;
  string token = "", peektoken = "" ;
  string id ;
  bool stop = false ;
  Peektoken( peektoken ) ;
  if ( IfIDENT( peektoken[0] ) ) {
    TokenAndVector( token, hasToken ) ; // get id
    id = token ;
    if ( !DefinedIDorFunctionTWO( id ) ) throw new Undefined( id ) ;
    Peektoken( peektoken ) ;
    if ( peektoken == "(" ) {
      TokenAndVector( token, hasToken ) ; // get "("
      Actual_parameter_list() ;
      Peektoken( peektoken ) ;
      if ( peektoken != ")" ) throw new Unexpected( peektoken ) ;
      TokenAndVector( token, hasToken ) ; // get ")"
    } // if()
    
    else if ( peektoken == "[" ) {
      TokenAndVector( token, hasToken ) ; // get "["
      Expression() ;
      Peektoken( peektoken ) ;
      if ( peektoken != "]" ) throw new Unexpected( peektoken ) ;
      TokenAndVector( token, hasToken ) ; // get ")"
      Peektoken( peektoken ) ;
      
    } // else if()
  } // if()
  
  else if ( IFConstant( peektoken ) ) {
    TokenAndVector( token, hasToken ) ; // get constant
    
  } // else if()
  
  else if ( peektoken == "(" ) {
    TokenAndVector( token, hasToken ) ; // get "("
    Peektoken( peektoken ) ;
    if ( !Expression() ) throw new Unexpected( peektoken ) ;
    Peektoken( peektoken ) ;
    if ( peektoken != ")" ) throw new Unexpected( peektoken ) ;
    TokenAndVector( token, hasToken ) ; // get ")"
  } // else if()
  
  else return false ;
  return true ;
  // : Identifier [ '(' [ actual_parameter_list ] ')'
  //              |
  //              '[' expression ']'
  //              ]
  // | Constant
  // | '(' expression ')'
} // Signed_unary_exp()

bool Expression() {
  Trace( "Expresion " ) ;
  vector<string> hasToken ;
  string token = "", peektoken = "" ;
  bool stop = false, basic_expressionCorrect = false ;
  basic_expressionCorrect = Basic_expression() ;
  while ( !stop ) {
    Peektoken( peektoken ) ;
    if ( peektoken != "," ) stop = true ;
    else if ( !Basic_expression() ) {
      Peektoken( peektoken ) ;
      throw new Unexpected( peektoken ) ;
    } // if()
  } // while()
  
  if ( !basic_expressionCorrect ) return false ;
  return true ;
  //  expression
  //  : basic_expression { ',' basic_expression }
} // Expression()


bool Statement() {
  Trace( "Statement " ) ;
  vector<string> hasToken ;
  string token = "", peektoken = "" ;
  string type = "" ;
  Peektoken( peektoken ) ;
  if ( peektoken == ";" )  {
    TokenAndVector( token, hasToken ) ;  // get ";"

    return true ;
  } // if()
  
  else if ( peektoken == "return" ) {
    TokenAndVector( token, hasToken ) ; // get "retrun"
    Expression() ;
    TokenAndVector( token, hasToken ) ;
    if ( token != ";" ) throw new Unexpected( token ) ;
    return true ;
  } // else if()
  
  else if ( peektoken == "cout" ) {
    gCinOrCout = 2 ; // 2 is cout
    TokenAndVector( token, hasToken ) ; // get "cout"
    Peektoken( peektoken ) ;
    if ( peektoken == ">>" || peektoken == ";" ) throw new Unexpected( peektoken ) ;
    Rest_of_maybe_shift_exp() ;
    TokenAndVector( token, hasToken ) ;
    if ( token != ";" ) throw new Unexpected( token ) ;

    return true ;
  } // else if()
  
  else if ( peektoken == "cin" ) {
    TokenAndVector( token, hasToken ) ; // get "cin"
    gCinOrCout = 1 ; // 1 is cin
    Peektoken( peektoken ) ;
    if ( peektoken == "<<" || peektoken == ";" ) throw new Unexpected( peektoken ) ;
    Rest_of_maybe_shift_exp() ;
    TokenAndVector( token, hasToken ) ;
    if ( token != ";" ) throw new Unexpected( token ) ;
    return true ;
  } // else if()
  
  else if ( peektoken == "if" ) {
    // gIfFunction = true ;
    TokenAndVector( token, hasToken ) ; // get "if"
    type = token ;
    TokenAndVector( token, hasToken ) ; // get "("
    if ( token == "(" ) {
      if ( !Expression() ) {
        Peektoken( peektoken ) ;
        throw new Unexpected( peektoken ) ;
      } // if()
      
      TokenAndVector( token, hasToken ) ; // get ")"
      if ( Statement() ) {
        Peektoken( peektoken ) ;
        if ( peektoken == "else" ) {
          TokenAndVector( token, hasToken ) ; // get "else"
          if ( !Statement() ) {
            Peektoken( peektoken ) ;
            throw new Unexpected( peektoken ) ;
          } // if()
        } // if ()
      } // if ()
      
      else {
        Peektoken( peektoken ) ;
        throw new Unexpected( peektoken ) ;
      } // else
    } // if()
    
    else throw new Unexpected( token ) ;
    
    return true ;
    
  } // else if()
  
  else if ( peektoken == "while" ) {
    TokenAndVector( token, hasToken ) ; // get "while"
    type = token ;
    TokenAndVector( token, hasToken ) ; // get "("
    if ( token == "(" ) {
      if ( !Expression() ) {
        Peektoken( peektoken ) ;
        throw new Unexpected( peektoken ) ;
      } // if()
      
      TokenAndVector( token, hasToken ) ; // get ")"
      if ( token != ")" ) throw new Unexpected( token ) ;
     
      if ( !Statement() ) {
        Peektoken( peektoken ) ;
        throw new Unexpected( peektoken ) ;
      } // if ()
    } // if()
    
    else throw new Unexpected( token ) ;
    return true ;
  } // else if()
  
  else if ( peektoken == "do" ) {
    gIfFunction = true ;
    TokenAndVector( token, hasToken ) ; // get "do"
    type = token ;
    Peektoken( peektoken ) ;
    if ( !Statement() ) {
      Peektoken( peektoken ) ;
      throw new Unexpected( peektoken ) ;
    } // if ()
    
    TokenAndVector( token, hasToken ) ; // get "while"
    if ( token != "while" ) throw new Unexpected( token ) ;
    TokenAndVector( token, hasToken ) ; // get "("
    if ( token != "(" ) {
      Peektoken( peektoken ) ;
      throw new Unexpected( peektoken ) ;
    } // if()
    
    if ( !Expression() ) {
      Peektoken( peektoken ) ;
      throw new Unexpected( peektoken ) ;
    } // if()
    
    TokenAndVector( token, hasToken ) ; // get ")"
    if ( token != ")" ) {
      Peektoken( peektoken ) ;
      throw new Unexpected( peektoken ) ;
    } // if()
    
    TokenAndVector( token, hasToken ) ; // get ";"
    if ( token != ";" ) {
      Peektoken( peektoken ) ;
      throw new Unexpected( peektoken ) ;
    } // if()

    return true ;
  } // else if()
  
  else if ( Compound_statement() ) {
    return true ;
  } // else if()
  
  else if ( ( ( peektoken != "else" ) && IfIDENT( peektoken[0] ) ) ||
            IFConstant( peektoken ) || peektoken == "++"
            || peektoken == "--" || peektoken == "+" || peektoken == "-"
            || peektoken == "!" || peektoken == "(" ) {
    Expression()  ;
    Peektoken( peektoken ) ;
    if ( peektoken == ";" )
      TokenAndVector( token, hasToken ) ;  // get ";"
    else throw new Unexpected( peektoken ) ;

    return true ;
  } // else if()
  
  else {
    Peektoken( peektoken ) ;
    //    cout << "test" << endl ;
    throw new Unexpected( peektoken ) ;
  } // else
  // : ';'     // the null statement
  // | expression ';'   expression here should not be empty
  // | RETURN [ expression ] ';'
  // | compound_statement
  // | IF '(' expression ')' statement [ ELSE statement ]
  // | WHILE '(' expression ')' statement
  // | DO statement WHILE '(' expression ')' ';'
  return false ;
  
} // Statement()
 


bool User_input() {
  bool stop = false ;
  string peektoken ;
  
  if ( !Definition() ) {
    Statement() ;
    cout << "> Statement executed ..." << endl ;
    while ( !stop ) {
      gline = 0 ;
      gCinOrCout = 0 ; // 1 is cout
      glikeFunction = false ;
      gidpoll.clear() ;
      gFunctionToken.clear() ;
      gIfFunction = false ;
      gconstpoll.clear() ;
      gwhileFunction.clear() ;
      // Endline() ;
      if ( !Definition() ) {
        Statement() ;
        cout << "> Statement executed ..." << endl ;
      } // if()
    } // while()
  } // if()
  
  
  return false ;
  // : ( definition | statement ) { definition | statement }
} // User_input()

void Commond() {
  string token = "" ;
  bool stop = false, correct = false  ;
  float value = 0.0 ;
  string peektoken = "", pushToken = "" ;
  bool push = false ;
  int type = 0 ; // type = 1 NOT_ID  type = 2 boolen
  bool borS = false, define = false, boolorArith = false ;
  while ( !stop ) {
    try {
      //      cout << TokenID() << endl ;
      User_input() ;
      gline = 0 ;
    } // catch()
    
    catch ( Undefined * Undefined1 ) {
      cout << "> Line " << gline << " : " << Undefined1->mMsg  << endl ;
      gline = 0 ;
      Endline() ;
    } // catch()
    
    catch ( Unrecognized * Unrecognized1 ) {
      cout << "> Line " << gline << " : " << Unrecognized1->mMsg  << endl ;
      gline = 0 ;
      Endline() ;
    } // catch()
    
    catch ( Unexpected * Unexpected1 ) {
      cout << "> Line " << gline << " : "  << Unexpected1->mMsg  << endl ;
      gline = 0 ;
      Endline() ;
    } // catch()
    
    catch( ExitOurC * Exit1 ) {
      stop = true ;
      cout << "> Our-C exited ..." ;
    } // catch()
    catch ( const char * x  ) {
      cout << "> " << x << endl ;
    } // catch
    // 以下重置行 //////
    value = 0.0 ;
    type = 0 ;
    push = false ;
    boolorArith = false ;
    token.clear() ; // 清空此行
    gHasPoint = 0 ;
    gTokenPoint = 0 ;
    gCinOrCout = 0 ; // 1 is cout
    gIfFunction = false ;
    gidpoll.clear() ;
    gFunctionToken.clear() ;
    gIfFunction = false ;
    gconstpoll.clear() ;
    gwhileFunction.clear() ;
    while ( cin.peek() == ' ' || cin.peek() == '\n' || cin.peek() == '\t' ) {
      if ( cin.peek() == '\n' )
        gline ++ ;
      cin.get() ;
    } // while()
    
  } // while()
  
} // Commond()


int main()
{
  int startP = 0 ;
  cin >> startP ;
  cin.get() ;
  gline ++ ;
  cout << "Our-C running ..." << endl ;
  Commond() ;
  // cout << "> Program exits..." << endl ;
  return 0;
} // main()



