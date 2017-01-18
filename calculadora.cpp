//
// This is example code from Chapter 7.2 "Input and output" of
// "Programming -- Principles and Practice Using C++" by Bjarne Stroustrup
//
/*
	Statement :
		Declaration
		Expression
		Print
		pow
		let -> #
		Quit
		surt
	Print :
		;
	Quit:
		quit
		surt
	Calculation Statement
	Expression :
		Term
		Expression + Term
		Expression - Term
	Term :
		Primary
		Term * Primary
		Term / Primary
		Term % Primary
		Term ! Primary
		
		
	Primary :
		Number
		( Expression ) 
		- Primary
		+ Primary
		sqrt Primary
	Number :
		Floating-point-literal
-------------------------------------------------------------
-> Input comes from cin through the Token_stream called td <-
-------------------------------------------------------------
*/

#include<iostream>
#include<stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <cmath>
#include <string>
#include <sstream>
#include<std_lib_facilities.h>

using namespace std;

#define debug 1
//------------------------------------------------------------------------------
//class Variable;

class Symbol_Table{
  
public:
  string constant;
  string name;
  double value;
  double val;
  
  double get_value(string s);
  void set_value(string s, double d);
  bool is_declared(string var);
  double define_name(string var, double val);
  double define_name_const(string var, double val);
  
  
  /*Variable*/
  Symbol_Table(string nm, double val)
		:name(nm), value(val){ }
  /*Variable*/
  Symbol_Table(string constat1, string nm, double val)
		:constant(constat1), name(nm), value(val){ }
	    
};

/*#####################Cambiat per Symbol_Table########################*/
/*class Variable
{
public:
	string constant;
	string name;
	double value;
	
	Variable(string nm, double val)
		:name(nm), value(val){ }
	Variable(string constat1, string nm, double val)
		:constant(constat1), name(nm), value(val){ }
};
*/
class Token {
public:
    char kind;        // what kind of token
    double value;     // for numbers: a value
    string name;
   
    Token(char ch)    // make a Token from a char
        :kind{ch} {}
    Token(char ch, double val)     // make a Token from a char and a double
        :kind{ch}, value{val} {}	
    Token(char ch, string n)
	:kind{ch}, name{n} {}
};

//------------------------------------------------------------------------------

class Token_stream {
public:
    Token_stream();   // make a Token_stream that reads from cin
    Token get();      // get a Token (get() is defined elsewhere)
    void putback(Token t);    // put a Token back
    void ignore(char c);
//    void putback2(Token t, Token t2);
//    void clean_up_mess();
private:
    bool full{false};        // is there a Token in the buffer?
    Token buffer;     // here is where we keep a Token put back using putback()
};

//------------------------------------------------------------------------------

const char number ='8'; //global!! //8 <- és número
const char quit = 'e';
const char print = ';';
const string prompt = "|=>";
const char  result = '='; //used to indicate that what follows is a result
//vector<Variable> var_table; //cambiat per Symbol_Table
vector<Symbol_Table> var_table;


Token_stream ts;        // provides get() and putback()


// The constructor just sets full to indicate that the buffer is empty:
Token_stream::Token_stream()
:full(false), buffer(0)    // no Token in buffer
{
}


void Token_stream::ignore(char c)
  //c representa el tipus de Token
  {
          //primera mirada al buffer?:
          if(full && c==buffer.kind){
                  full=false;
                  return;
          }
          full=false;
          //ara busca introduccions
          char ch=0;
          while(cin>>ch){
		  //cout << " c=" <<  endl;
                  if(ch==c) return;
	  }
  }



void clean_up_mess()
{
	ts.ignore(print); //pagina 216-217 del llibre, just al cambiar de pàgina
}



//------------------------------------------------------------------------------

double get_value(string s)// return the Value of a variable named s
{
      cout << "s = " << s<< endl;
      for(const /*Variable*/ Symbol_Table& v: var_table)
                if(v.name == s) return v.value;
        error("get: undefined variable ",s);//, s);

}

void set_value(string s, double d)
// set the Variable named s to d
{
for (/*Variable*/Symbol_Table& v : var_table)
	if (v.name == s) {
		if(debug==1)cout << "Estic dins del setter Variable 2" << endl;
		v.value = d;
		return;
}
error("set: undefined variable ",s);//, s);
} 	

void set_value(string c, string s, double d)
// set the Variable named s to d
{
for (/*Variable*/Symbol_Table& v : var_table)
	if (v.name == s) {
		if(debug==1)cout << "Estic dins del setter Variable 3" << endl;
		v.value = d;
		return;
}
error("set: undefined variable ",s);//, s);
} 	


bool is_declared(string var)
  // is var already in var_table ?
  {
	for (const /*Variable*/Symbol_Table& v : var_table)
        	if (v.name == var) return true;

        return false;
  }

double define_name(string var, double val)
  // add (var,val) to var_table
  {
	if (is_declared(var)){
			      //error(var," declared twice");
	  		      char c='1';
			      do{
				cout << "Var name already exists, do you want overwrite value ?(s,S,n,N)" ;
				cin >> c;
			      }while(c!='s' && c!='n' && c!='S' && c!='N');
			      if(c!='s' && c!='S'){
					  error(var," declared twice");
			      }
			      else{
					  var_table.push_back(/*Variable*/Symbol_Table(var,val)); //falta el setter ... pagina 218
			    }
	
	
	}
	else{
	      var_table.push_back(/*Variable*/Symbol_Table(var,val));
	}
        return val;
  }

double define_name_const(string var, double val)
  // add (var,val) to var_table
  {
	if (is_declared(var)){
		error(var," declared twice");
			      }
	else{
		var_table.push_back(/*Variable*/Symbol_Table(var,val)); //falta el setter ... pagina 218
	}
	return val;
  }
  
  
  
// The putback() member function puts its argument back into the Token_stream's buffer:
void Token_stream::putback(Token t)
{
    if (full) { 
		error("putback() into a full buffer\nNO HI HA RES AL BUFFER??\n");
			//return (-3);
		}
    buffer = t;       // copy t to buffer
    full = true;      // buffer is now full
}

//------------------------------------------------------------------------------
const char name = 'a';
const char let = '#';
const string decllet = "#";

const char arrel = 'r';	
const string declsqrt = "sqrt";

const char pows='p';
const string declpow = "pow";

const char surt='s';
const string declsurt="surt";
const char quit1='q';
const string declquit="quit";
const char exit1='e';
const string declexit="exit";

const char constat='c';
const string declconstat1="declara";
// name token
// declaration token
// declaration keyword

Token Token_stream::get() //buscar espais i retorns de carro '\n' amb la llibreria isspace(ch) -> cert si espai
//lectura de dades des del teclat i composició del Token
{
    if (full) {       // do we already have a Token ready?
        // remove token from buffer
        full=false;
        return buffer;
    }

    char ch;
    cin >> ch;    // note that >> skips whitespace (spsmace, newline, tab, etc.)
//    cout << " ch=" << ch << endl;
    switch (ch) {
		case quit:
		case print:    // for "print"
		case result: 
		case '%':
		case '!': 
		case '{': 
		case '}': 
		case '(': 
		case ')': 
		case '+': 
		case '-': 
		case '*': 
		case '/':
		case ',':
		case '#':
		//case '_':
	        	return Token(ch);        // let each character represent itself
	    case '.':
	    case '0': case '1': case '2': case '3': case '4':
	    case '5': case '6': case '7': case '8': case '9':
	        {
//		    cout << "ch = " << ch << endl;
	            cin.putback(ch);         // put digit back into the input stream
	            double val;
	            cin >> val;              // read a floating-point number
//		    cout << "val = " << val << endl << " number = " << number << endl;
	            return Token(number,val);   // let '8' represent "a number"
	        }
 	   default:
		if(isalpha(ch)){ //si es let, val l, entro aquí.
			string s;
			//if(debug==1)cout << "s(abans) = " <<  s << endl;
			s += ch;
			//if(debug==1)cout << "s(després) = " << s << endl;
			while(cin.get(ch) && (isalpha(ch) || isdigit(ch) || ch=='_')){ // comparar aquí si ch == _ (underscore), no ?
					 s+=ch;
/*					if(debug==1){
						cout << endl << "carrega" << endl;
						cout << endl << " ch = " << ch << endl << " s = " << s << endl << "decllet = " << decllet << endl << " s = " << s << endl;
						cout << "s = " << s << endl;
						}
*/
			}
			cin.putback(ch);
			//cout << "ch = " << ch;
			if(s==declsurt){
			      if(debug==1)cout << "SURT?" << endl;
			      return Token{surt};
			}
			if(s==declquit){
			      if(debug==1)cout << "QUIT?" << endl;
			      return Token{quit1};
			}
			if(s==declexit){
			      if(debug==1)cout << "EXIT?" << endl;
			      return Token{exit1};
			}
			if(s==decllet){//let
					if(debug==1) {
							cout << endl << "decllet" << endl;
							cout << endl << "surto token{let}" << endl<< " let = " << let << endl << " ch =" << ch << endl << " s = " << s << endl << "decllet=" << decllet << endl;
					}
					
					 return Token{let};
			}
			if(s==declsqrt){//arrel
					/*if(debug==1){
							cout << endl << "declsqrt" << endl;
							 cout << "faig arrel ?(get)" << endl;
					}
					*/
					return Token{arrel};
			}
			if(s==declpow) {//pow
					if(debug==1){
							/*cout << endl << "declpow" << endl;
							cout << "faig la potència?" << endl;
							cout << "falta llegir la segona variable per fer l'exponencial, com ho faig ?" << endl;
							*/
							
					}
					return Token{pows};
					//float i;
					//llegir 
					//expression 
					//o
					//primary
					//o
					//term	
					//com ho faig ? com ho puc fer?
					//return (0); //Token{pows};
			}
			if (s==declconstat1) {
					if(debug==1)cout << endl << " Declara VARIABLE? (crear clase i metodes pk no es pugui sobreescriure?)" << endl;
					return Token{constat};
			}						
			return Token{name,s};
		}
		//cout << "ch =" << ch << endl;
	        error("Bad token");
	
    }
}

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

double expression();    // declaration so that primary() can call expression()

//------------------------------------------------------------------------------
// deal with numbers and parentheses
double primary()
{
    Token t = ts.get();
    switch (t.kind) {
    case '{':
	{
            double d = expression();
            t = ts.get();
            if (t.kind != '}') error("'}' expected");
            return d;
        }
    case'(':    // handle '(' expression ')'
        {
            double d = expression();
            t = ts.get();
            if (t.kind != ')') error("')' expected");
            return d;
        }
    case number:             // we use '8' to represent a number
        return t.value;  // return the number's value
    case '-':
	return - primary();
    case '+':case '=': 
	return primary();
    case 'r':
	double valor; 
	double resultat;
	valor = primary();
	if(valor <0) {
		cout << " D'aquest valor, " << valor << ", NO es pot calcular l'arrel REAL" << endl;
		return (0);
	}
	else {
		//if(debug==1)cout << " valor = " << valor; 
		resultat=sqrt(valor);	
		//cout << endl << " El resultat de l'arrel de "<< valor << " és " ; // << resultat << endl;
		return (resultat);
	}
    case 'a': //si 'a', recull valor!!!!
	if(debug==1)cout << "get_value(t.kind) = " << get_value(t.name) << endl;
	return get_value(t.name);

    /*case 'p': // forçar pow(x,i) i integer ? :w
     * 
	double valore;
	double valorp;
	valorp=primary();
	valore=expression();
	cout << endl << "fa la potencia ara ? (primary)ÉS EL BOO??, t.value =" << t.value << "valorp = " << valorp << endl;
*/
    default:
	//if(debug==1)cout << "t.value = " << t.value << endl << "t.kind =" << t.kind << endl;
        error("primary expected");
    }
}

//------------------------------------------------------------------------------
int factorial(int left)
{
//	if(debug)cout  << " dins de factorial ?\n" ;
if (left!=1 && left!=0){
//		if(debug) cout << "\n factorial de  " << left << " * factorial(" << left-1 << ") = " << left*factorial(left-1) ;
                return( left* factorial(left-1));


	}
	else {
		//if(debug) cout << "left = " << left << "\n";
		if(left==0) return(1);
		//if(debug) cout << "\nfactorial  de 1 " << endl;
                return(1);
		}

}


double sqrt1( double val){ //arrel
	double prova;
	if(val >1){
		prova =sqrt(val);
		if(debug==1)cout << "prova = " << prova << endl;
	}	
	else{
		cout << "\n Error!!!!!!!!!!!!! valor no vàlid" << endl;
//		cout << "\n val = " << val << "\n sqrt(val) = " << sqrt(val) << endl;
	}

return(1);
}

// deal with *, /, and %
double term()
{
    double left = primary();
    double valor;
    Token t = ts.get();        // get the next token from token stream

    while(true) {
        switch (t.kind) {
	case '!':
//	    if(debug)cout << "\nfactorial ?\n left = " << factorial((int)left) << "\n";
	    valor=left;
	    left=factorial(left);
	    cout << "El factorial de " << (int)valor << " és " << (int)left << "\nIntro > ";
	    t = ts.get(); 
 	    break;
        case '*':
            left *= primary();
            t = ts.get();
            break;
        case '/':
            {
                double d = primary();
                if (d == 0) error("divide by zero");
                left /= d;
                t = ts.get();
                break;
            }
	case '%':
	{
		//pagina 209 del llibre pdf, mirar narrow_cast
		//int i1 = narrow_cast<int>(left); //no incloc narrow_cast, mirar-ho bé ... com fer-ho
		//int i2 = narrow_cast<int>(primary());
		try{
			 
		  //pàgina 208 del llibre pdf pregunta 7. What does narrow_cast do?
		    int i1 = narrow_cast<int>(left);
		    int i2 = narrow_cast<int>(primary());
		    if (i2 == 0) error("%: divide by zero");
		    left = i1 % i2;
		    t = ts.get();
		    break;
		   
		/*	double d = primary();
			if(d==0) error("divide by zero");
//			if(i2==0) error("% : divide by zero");
			left = fmod(left,d);//i1 % i2;
			t = ts.get();
			break;
		*/	
		}
		catch (exception& e){
			cout << "Aquí hi ha un error ? pot ser ? e.what = " << e.what() << endl;
		}
	}
        default:
            ts.putback(t);     // put t back into the token stream
            return left;
        }
    }
}

//------------------------------------------------------------------------------

// deal with + and -
double expression()
{
    double left = term();      // read and evaluate a Term
    Token t = ts.get();        // get the next token from token stream

    while(true) {
        switch(t.kind) {
        case '+':
            left += term();    // evaluate Term and add
            t = ts.get();
            break;
        case '-':
            left -= term();    // evaluate Term and subtract or change sign
            t = ts.get();
            break;
        default:
            ts.putback(t);     // put t back into the token stream
            return left;       // finally: no more + or -: return the answer
        }
    }
}

double define_name(string var, double val);

double calcul_pow()
{

  try{
    Token t= ts.get();//pow
    Token t2 = ts.get(); //valor1

    double d=t2.value; //Read first value
    Token t3 = ts.get(); //coma 

    
    //Token t4 = ts.get(); //valor2 //forçar integer
    //float i=(int)t4.value;
    try{
      Token t4=ts.get(); //valor2 //forçar integer ?
      int i= (int)t4.value;     
      if(debug==1)cout << "i = " << i << endl << " d=" << d << endl;
      for (int j=1; j<i; j++) d=d*t2.value;
      Token t5 = ts.get();//)  
      
      return(d);
    }
    catch(exception& e){
      cout << "no és un enter ?" << endl;
    }
  
    
    return(0);
    
  }	
  catch(exception& e){
    cout << "Error de conversió" << endl;
    return(0);
  }
}

double declaration()
// assume we have seen "let”
// handle : name = expression
// declare a variable called "name ” with the initial value "expression”
{
Token t = ts.get();
//cout << "DEBUGt.kind = " << t.kind << endl << " name =" << name << endl; //kind value i name
cout << "t.kind = " << t.kind << endl << "t.value = " << t.value << endl << "t.name = " << t.name << endl;
if (t.kind != name) error ("name expected in declaration");
string var_name = t.name;


Token t2 = ts.get();
if ((t2.kind != '=')&&(t2.kind != '_')) error("_ or = missing in declaration of ", var_name);//aqui és on es llegeix el '_', és aquí on l'he de permetre.
//cout << "si?" << endl;
double d = expression();
cout << " d= " << d << endl;
define_name(var_name,d);
if(debug==1) cout << "var_name = " << var_name << "\n d =" << d << endl;
set_value(var_name,d);
return d;
}

double declaration_constants()
// assume we have seen "let”
// handle : name = expression
// declare a variable called "name ” with the initial value "expression”
{
Token t = ts.get();
cout << "t.kind = " << t.kind << endl << " constat = " <<constat << endl; //<< " t.value = " << t.value << "t.name = " << t.name;
//if( t.kind != constat ) error ("constant expected in declaration");

Token t1 = ts.get();
//cout << "DEBUGt.kind = " << t.kind << endl << " name =" << name << endl; //kind value i name
cout << "t1.kind = " << t1.kind << endl ; //<< "t1.value = " << t1.value << endl << "t1.name = " << t1.name << endl;
//if (t.kind != constat || t1.kind != name) error ("name expected in declaration or constant expected in declaration");
string var_name = t1.name;


Token t2 = ts.get();
if ((t2.kind != '=')&&(t2.kind != '_')) error("_ or = missing in declaration of ", var_name);//aqui és on es llegeix el '_', és aquí on l'he de permetre.
//cout << "si?" << endl;
double d = expression();
cout << " d= " << d << endl;
define_name_const(var_name,d);
if(debug==1) cout << "var_name = " << var_name << "\n d =" << d << endl;
set_value(var_name,d);
return d;
}

double statement()
{
  Token t = ts.get();
  switch (t.kind) {
	 case constat:
		 return declaration_constants();
         case let:
                 return declaration();
	 case pows:
		// cout << "statement pow" << endl << "t.kind = "<< t.kind << endl;
		return 	calcul_pow();
	//	cout << "ts.putback(t) = " << ts.putback(t) << endl;
//		return (ts.putback(t)); 
         default:
		//cout << "passa ?" << endl ;
		ts.putback(t);
                return expression();
  }
}



void calculate()
{

	while (cin) 
		try{
	       		cout << prompt;          // print prompt
	        	Token t = ts.get();
       			while(t.kind == print) t = ts.get();
			
		        if(t.kind == quit1){
//				keep_window_open();
//				return (0);
				if(debug==1)cout << "AQUI ara hauria de sortir per  \"QUIT\"" << endl;
				return; 
	        	}
			if(t.kind== surt){
			   if(debug==1)cout << "AQUI ara hauria de sortir per \"SURT\"" << endl;
			   return;
			}
			if(t.kind== exit1){
			  if(debug==1)cout << "AQUI ara hauria de sortir per \"EXIT\"" << endl;
			   return;
			}
			if(debug==1)cout << "t.kind = " << t.kind << endl;
			ts.putback(t);
   		     	cout << result << statement() << endl; //expression() << endl;
		}
		catch(exception& e){ //si hi ha error d'introducció, surt aquí ...
//			cout << endl << "surto aqui, oi, pel let ??" << endl ;
			cerr << e.what() << endl;
			clean_up_mess();
		}
}

//------------------------------------------------------------------------------

int main()
try
{
	//predefined names :
	define_name("pi", 3.1415926535);
	define_name("ne", 2.7182818284);
	define_name("k", 1000);
	if(debug==1)cout << "\npàgina 226 del pdf,  TEMA 7, Exercises 5"<< endl;
	if(debug==1)cout << "Exercises" << endl << "5. Modify Token_stream::get() to return Token(print) when it sees a newline. This implies looking for whitespace characters and treating newline ('\n') specially. You might find the standard library function isspace(ch), which returns true if ch is a whitespace character, useful." << endl;
	/*
	 * 
	 http://www.cplusplus.com/doc/tutorial/classes/
	 http://www.cplusplus.com/doc/tutorial/templates/
	 http://www.cplusplus.com/doc/tutorial/classes2/
	 http://www.cplusplus.com/doc/tutorial/inheritance/
	 http://www.cplusplus.com/doc/tutorial/polymorphism/
	 */
	calculate();
	if(debug==1)cout << "Fora calculate" << endl;
	keep_window_open();
	  return 0;
}
catch (exception& e){
    cerr << "error: " << e.what() << endl;
    keep_window_open("~~");
    return (1);
}
catch (...) {
    cerr << "Oops: unknown exception!\n";
    keep_window_open("~~");
    return (2);
}