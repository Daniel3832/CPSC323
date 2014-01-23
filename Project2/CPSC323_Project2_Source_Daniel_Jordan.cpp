//=========================================
//Name: Daniel Jordan
//Email: daniel_jordan@csu.fullerton.edu
//Class: CPSC 323
//Assignment: Project 2: Syntax analysis 
//=========================================
#include <iostream>
#include<fstream>
#include <string>
#include <iomanip>
#include <deque>
#include <stack>
using namespace std;
void initQueue(deque<char> & queue);
string lexer(deque<char> & queue);
string tokenParser(string lexeme);
bool isTerminal(string token);
void reverseSTR(string production, stack<string> & reverse);
string removeSpaces(string lexeme);
string searchTable(string t, string lexeme);
char * const INPUT_FILE = "input.txt";
char * const OUTPUT_FILE = "output.txt";
enum STATES{STARTING, IDENTIFIER, END_IDENTIFIER, INTEGER, END_INTEGER, REAL, END_REAL, ERROR};
const string keywords[] = {"do", "while", "if", "fi", "else", "real",  "begin", "end", "read", "write", "return", "function", "int", "bool", "true", "false"};
const string operators[] = {"<", ">", "<=", ">=", "*", "/", "-", "+", ":="};
const string separators[] = {"(", ")", "{", "}", ";","$", "$$", ",", ":"};
const string terminals[] = {"+", "-", "*", "/", "=", "/=", "<", ">", "(", ")", "{", "}", ",", ";", "if", "fi", "else", "true", "false", "int", "boolean", "real", "INTEGER", "REAL", "IDENTIFIER", "function", "begin", "end", "return", "write", "read", "while", "do", "$$","$", ":=",":"};
const string nonTerminals[] = {"<Rat12SU>", "<Expression>", "<ExpressionPrime>", "<Term>", "<TermPrime>", "<Factor>", "<Primary>", "<PrimaryPrime>", "<If>", "<IfPrime>", "<OptDeclarationList>", "<OptFunctionDefinitions>", "<Function>",
			   "<FunctionDefinitions>", "<FunctionDefinitionsPrime>", "<DeclarationList>", "<DeclarationListPrime>", "<Declaration>", "<Qualifier>", "<ID>", "<IDPrime>", "<Body>", "<StatementList>", "<StatementListPrime>",
			   "<Statement>", "<Compound>", "<Assign>", "<Return>", "<Write>", "<Read>", "<While>", "<Condition>", "<Relop>", "<Parameter>","<Empty>"};
//Parse Table 
const string table[35][37]={
{"", "", "","","","","","","","","", "", "","","","","","","","","", "", "","","","","","","","","","","","$$ <OptFunctionDefinitions> $$ <OptDeclarationList> <StatementList> $$","","",""},//<Rat12SU>
{"","<Term> <ExpressionPrime>","","","","","","","<Term> <ExpressionPrime>","","","","","","","","","<Term> <ExpressionPrime>","<Term> <ExpressionPrime>","","","","<Term> <ExpressionPrime>","<Term> <ExpressionPrime>","<Term> <ExpressionPrime>","",""},//<Expression>
{"+ <Term> <ExpressionPrime>","- <Term> <ExpressionPrime>","<Empty>","<TermPrime>","","<Empty>","<Empty>", "<Empty>","","<Empty>","","","","<Empty>","","","","","","","","","","","<Empty>","","","","","","","","","","","",""},//<Expression Prime>
{"","<Factor> <TermPrime>","","","","","","","<Factor> <TermPrime>","","","","","","","","","<Factor> <TermPrime>","<Factor> <TermPrime>","","","","<Factor> <TermPrime>","<Factor> <TermPrime>","<Factor> <TermPrime>","","","","","","","","","","","",""},//<Term>
{"<Empty>", "<Empty>", "* <Factor>","/ <Factor>","","","<Empty>","<Empty>","","<Empty>","","","","<Empty>","","","","","","","","","","","<Empty>","","","","","","","","","","","",""},//<Term Prime>
{"","- <Primary>","","","","","","","<Primary>","","","","","","","","","<Primary>","<Primary>","","","","<Primary>","<Primary>","<Primary>", "","","","","","","","","","","",""},//<Factor>
{"","","","","","","","","( <Expression> )","","","","","","","","","true","false","","","","INTEGER","REAL","<ID> <PrimaryPrime>","","","","","","","","","","","",""},//<Primary>
{"<Empty>","<Empty>","<Empty>","<Empty>","","","<Empty>","<Empty>","( <ID> )","<Empty>","","","","<Empty>","","","","","","","","","","","<Empty>","","","","","","","","","","",""},//<Primary Prime>
{"","","","","","","","","","","","","","","if( <Condition> ) <Statement>","","","","","","","","","","","","","","","","","","","","","",""},//<IF>
{"","","","","","","","","","","","","","","","fi","else<statement>fi","","","","","","","","","","","","","","","","","","","",""},//<IF Prime>
{"","","","","","","","","","","<Empty>","","","","<Empty>","","","","","<DeclarationList>","<DeclarationList>","<DeclarationList>","","","<Empty>","","<Body>","","<Empty>","<Empty>","<Empty>","<Empty>","","","",""},//<Opt Declaration List>
{"","","","","","","","","","","","","","","","","","","","","","","","","","<FunctionDefinitions>","","","","","","","","<Empty>","","",""},//<Opt Function Definitions>
{"","","","","","","","","","","","","","","","","","","","","","","","","","function IDENTIFIER ( <Parameter> ) <OptDeclarationList> <Body>","","","","","","","","","","",""},//<Function>
{"","","","","","","","","","","","","","","","","","","","","","","","","","<Function> <FunctionDefinitionsPrime>","","","","","","","","","","",""},//<Function Definitions>
{"","","","","","","","","","","","","","","","","","","","","","","","","","<FunctionDefinitions>","","","","","","","","<Empty>","<Empty>","",""},//<Function Definitions Prime>
{"","","","","","","","","","","","","","","","","","","","<Declaration> <DeclarationListPrime> ;","<Declaration> <DeclarationListPrime> ;","<Declaration> <DeclarationListPrime> ;","","","","","","","","","","","","","","",""},//<Declaration List>
{"","","","","","","","","","","<Empty>","","","<Empty>","<Empty>","","","","","<DeclarationList>","<DeclarationList>","<DeclarationList>","","","<Empty>","","<Empty>","","<Empty>","<Empty>","<Empty>","<Empty>","","","","",""},//<Declaration List Prime>
{"","","","","","","","","","","","","","","","","","","","<Qualifier> <ID>","<Qualifier> <ID>","<Qualifier> <ID>","","","","","","","","","","","","","","",""},//<Declaration>
{"","","","","","","","","","","","","","","","","","","","int","boolean","real","","","","","","","","","","","","","","",""},//<Qualifier>
{"","","","","","","","","","","","","","","","","","","","","","","","","IDENTIFIER <IDPrime>","","","","","","","","","","","",""},//<ID>
{"<Empty>","<Empty>","<Empty>","<Empty>","","","<Empty>","<Empty>","<Empty>","<Empty>","","",", <ID>","<Empty>","","","","","","","","","","","<Empty>","","","","","","","","","","","",""},//<ID Prime>
{"","","","","","","","","","","","","","","","","","","","","","","","","","","begin <StatementList> end","","","","","","","<Empty>","","",""},//<Body>
{"","","","","","","","","","","<Statement> <StatementListPrime>","","","","<Statement> <StatementListPrime>","","","","","","","","","","<Statement> <StatementListPrime>","","","","<Statement> <StatementListPrime>","<Statement> <StatementListPrime>","<Statement> <StatementListPrime>","<Statement> <StatementListPrime>","","","","",""},//<Statement List>
{"","","","","","","","","","","<StatementList>","<Empty>","","","<StatementList>","","","","","","","","","","<StatementList>","","","<Empty>","<StatementList>","<StatementList>","<StatementList>","<StatementList>","","<Empty>","<Empty>","",""},//<Statement List Prime>
{"","","","","","","","","","","<Compound>","","","","<If>","","","","","","","","","","<Assign>","","","","<Return>","<Write>","<Read>","<While>","","","","",""},//<Statement>
{"","","","","","","","","","","{ <StatementList> }", "","","","","","","","","","","","","","","","","","","","","","","","","",""},//<Compound>
{"","","","","","","","","","","","","","","","","","","","","","","","","IDENTIFIER := <Expression> ;","","","","","","","","","","","<Expression>",""},//<Assign>
{"","","","","","","","","","","","","","","","","","","","","","","","","","","","","return <Expression> ;","","","","","","","",""},//<Return>
{"","","","","","","","","","","","","","","","","","","","","","","","","","","","","","write ( <Expression> ) ;","","","","","","",""},//<Write>
{"","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","read ( <ID> ) ;","","","","","",""},//<Read>
{"","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","while ( <Condition> ) do <Statement>","","","","",""},//<While>
{"","<Expression> <Relop> <Expression>","","","","","","","","<Empty>","","","","","","","","","","","","","<Expression> <Relop> <Expression>","<Expression> <Relop> <Expression>","<Expression> <Relop> <Expression>","","","","","","","","","","","",""},//<Condition>
{"","","","","","/=","<",">","","","","","","","","","","","","","","","","","","","","","","","","","","","","",""},//<Relop>
{"","","","","","","","","","","","","","","","","","","","","","","","","IDENTIFIER : <Qualifier>","","","","","","","","","","","",""},//<Parameter>
{"epsilon","epsilon","epsilon","epsilon","epsilon","epsilon","epsilon","epsilon","epsilon","epsilon","epsilon","epsilon","epsilon","epsilon","epsilon","epsilon","epsilon","epsilon","epsilon","epsilon","epsilon","epsilon","epsilon","epsilon","epsilon","epsilon","epsilon","epsilon","epsilon","epsilon","epsilon","epsilon","epsilon","epsilon","epsilon","epsilon","epsilon"},//<Empty>
};
//===========================================================================
//
//====== main() ============================================================
int main()	
{	
	deque<char> queue;
	deque<string> lexQueue;
	stack<string> reverse;
	//Initialize stack, and push dollar sign and start symbol
	stack<string> stack; stack.push("$"); stack.push("<Rat12SU>");
	string lexeme, token, t; //Lexeme, token, top of stack
	initQueue(queue);
	fstream output(OUTPUT_FILE, ios::out);
	int counter=0;
	int lexCounter = 0;
	//Run lexer on char queue, and push analyzed lexemes onto new string queue
	while(queue.size()>1)
	{
		lexeme = lexer(queue);
		lexQueue.push_back(lexeme);
	}	
	cout << "\n\nToken" << setw(20) << "Lexeme\n\n";
	while(!stack.empty())
	{
		t = stack.top();
		lexeme = lexQueue.front();
		token = tokenParser(lexeme);
		if(token == "Identifier") lexeme = "IDENTIFIER";
		else if(token == "Integer") lexeme = "INTEGER";
		else if(token == "Real") lexeme = "REAL";
		if(counter==0)
		{
			cout << "-----------------------------------------------------------------------------------\n";
			cout << "Token: " << token << " Lexeme: " << lexeme << endl << endl;
	
			output << "-----------------------------------------------------------------------------------\n";
			output << "Token: " << token << " Lexeme: " << lexeme << endl << endl;

			lexCounter++;
		}
		if(isTerminal(t))
		{
			if(t==lexeme)
			{
				cout << "-----------------------------------------------------------------------------------\n\n";
				output << "-----------------------------------------------------------------------------------\n\n";
				stack.pop();
				lexQueue.pop_front();
				counter=0;
			}
			else
			{
				cout << "\nERROR after token #" << lexCounter-1 << endl;
				output << "\nERROR after token #" << lexCounter-1 << endl;
				break;
			}
		}
		else
		{
			cout << t << "::=" << searchTable(t, lexeme) << endl;
			output << t << "::=" << searchTable(t, lexeme) << endl;
			if(searchTable(t, lexeme) !="")
			{
				stack.pop();
				string production = searchTable(t, lexeme);
				reverseSTR(production, reverse);
				while(!reverse.empty())
				{
					if(reverse.top()!="epsilon")
						stack.push(reverse.top());
					reverse.pop();	
				}
				counter++;
			}
			else
			{
				cout << "\nERROR after token #" << lexCounter-1 << endl;
				output << "\nERROR after token #" << lexCounter-1 << endl;	
				break;
			}
		}

	}
	output.close();
	system("pause");
}
//===========================================================================
// Description: Function pops the top  element of an stl queue. An FSM with 
// 8 states is implimented through the form of a looped switch. First 
// character is analyzed, and states  are transitioned accordingly. In the 
// case of a compound opperator, the second character is looked at. If it 
// matches /the pattern of those opperators, a second character is popped 
// and the opperator is returned as two characters. This loop repeats until 
// the stack is empty.
//====== lexer() ============================================================
string lexer(deque<char> & queue)
{
	STATES state = STARTING;
	string tempIdentifier, tempInteger, tempReal;
	char token, tokenNext;
	while(!queue.empty())
	{
		token = queue.front();
		queue.pop_front();
		if(!queue.empty())
			tokenNext = queue.front();
		do{
			switch(state)
			{
				/*
				STARTING STATE
				*/
				case STARTING:
					//First character of identifier must be alphabetic
					if(isalpha(token) || token=='_')
					{
						tempIdentifier+=token;
						state = IDENTIFIER;
					}
					else if(isdigit(token))
					{
						tempInteger+=token;
						state = INTEGER;
					}
					else if(token =='.')
						state = ERROR;
					else if(token == ' ')
						state = STARTING;
					else if(token == '(')
					{
						state = STARTING;
						return "(";
					}
					else if(token == '$' && queue.empty())
						return "$";
					else if(token == '$')
					{
						if(tokenNext == '$')
						{
							queue.pop_front();
							return "$$";
						}
						else
							return "$";
						state = STARTING;
					}
					else if(token == ')')
					{
						state = STARTING;
						return ")";
					}
					else if(token == ',')
					{
						state = STARTING;
						return ",";
					}
					else if(token == '{')
					{
						state = STARTING;
						return "{";
					}
					else if(token == '}')
					{
						state = STARTING;
						return "}";						
					}
					else if(token == ';')
					{
						state = STARTING;
						return ";";
					}
					else if(token == '<')
					{
						state = STARTING;
						return "<";
					}
					else if(token == '>')
					{
						state = STARTING;
						return ">";
					}
					else if(token == '/')
					{
						state = STARTING;
						if(tokenNext == '=')
						{
							queue.pop_front();
							return "/=";
						}
						else
							return "/";
					}
					else if(token == '+')
					{
						state = STARTING;
						return "+";
					}
					else if(token == '*')
					{
						state = STARTING;
						return "*";
					}
					else if(token == '-')
					{
						state = STARTING;
						return "-";
					}
					else if(token == ':')
					{
						state = STARTING;
						if(tokenNext == '=')
						{
							queue.pop_front();
							return ":=";
						}
						else
							return ":";
					}
					else if(token == '=')
					{
						state = STARTING;
						return "=";
					}
					break;
				/*
				IDENTIFIER STATE
				*/
				case IDENTIFIER:
					if(isalpha(token) || token == '_')
					{
						tempIdentifier+=token;
						//Switches to ERROR state, if last character in identifier is _
						if(token == '_' && isalnum(tokenNext)==0)
							state = ERROR;
						else
							state = IDENTIFIER;
					}
					else if(isdigit(token))
					{
						tempIdentifier+=token;
						state = IDENTIFIER;
					}
					if(queue.empty())
						state = END_IDENTIFIER;
					else if(token == ' ')
						state = END_IDENTIFIER;
					else if(token == '(')
						state = END_IDENTIFIER;
					else if(token == ')')
						state = END_IDENTIFIER;
					else if(token == '{')
						state = END_IDENTIFIER;
					else if(token == '}')
						state = END_IDENTIFIER;
					else if(token == ';')
						state = END_IDENTIFIER;
					else if(token == ',')
						state = END_IDENTIFIER;
					else if(token == '$')
						state = END_IDENTIFIER;
					else if(token == '<')
						state = END_IDENTIFIER;
					else if(token == '>')
						state = END_IDENTIFIER;
					else if(token == '/')
						state = END_IDENTIFIER;
					else if(token == '+')
						state = END_IDENTIFIER;
					else if(token == '*')
						state = END_IDENTIFIER;
					else if(token == '-')
						state = END_IDENTIFIER;
					else if(token == ':')
						state = END_IDENTIFIER;
					break;
				/*
				END IDENTIFIER STATE
				*/
				case END_IDENTIFIER:
					if(tempIdentifier!="")
					{
						string identifier = tempIdentifier;
						state = STARTING;
						tempIdentifier.clear();						
						queue.push_front(token);
						return removeSpaces(identifier);
					}
				/*
				INTEGER STATE
				*/
				case INTEGER:
					//Transitions to error state if letter is entered from integer state
					if(isalpha(token))
					{
						tempInteger+=token;
						//Waits until string finishes before transitioning to error state
						if(isalnum(tokenNext)==0)
							state = ERROR;
						else
							state = INTEGER;
					}
					else if(isdigit(token))
					{
						tempInteger+=token;
						state = INTEGER;
					}
					else if(token =='.')
					{
						tempReal+=tempInteger+=".";
						state = REAL;
						//Clear integer value before transition to real number
						tempInteger.clear();
					}
					if(token == ' ')
						state = END_INTEGER;
					else if(token == '(')
						state = END_INTEGER;
					else if(token == ')')
						state = END_INTEGER;
					else if(token == '{')
						state = END_INTEGER;
					else if(token == '}')
						state = END_INTEGER;
					else if(token == ';')
						state = END_INTEGER;
					else if(token == ',')
						state = END_INTEGER;
					else if(token == '$')
						state = END_INTEGER;
					else if(token == '<')
						state = END_INTEGER;
					else if(token == '>')
						state = END_INTEGER;
					else if(token == '/')
						state = END_INTEGER;
					else if(token == '+')
						state = END_INTEGER;
					else if(token == '*')
						state = END_INTEGER;
					else if(token == '-')
						state = END_INTEGER;
					else if(token == ':')
						state = END_INTEGER;
					break;
				/*
				END INTEGER STATE
				*/
				case END_INTEGER:
					if(tempInteger!="")
					{
						string integer = tempInteger;
						state = STARTING;
						tempInteger.clear();						
						queue.push_front(token);
						return removeSpaces(integer);
					}
					break;
				/* 
				REAL NUMBERS STATE
				*/
				case REAL:
					if(isalpha(token))
					{
						tempIdentifier+=token;
						state = END_REAL;
					}
					else if(isdigit(token))
					{
						tempReal+=token;
						state = REAL;
					}
					else if(token =='.')
					{
						tempReal+=token;
						state = ERROR;
					}
					if(tokenNext == ' ')
						state = END_REAL;
					else if(tokenNext == '(')
						state = END_REAL;
					else if(tokenNext == ')')
						state = END_REAL;
					else if(tokenNext == '{')
						state = END_REAL;
					else if(tokenNext == '}')
						state = END_REAL;
					else if(tokenNext == ';')
						state = END_REAL;
					else if(tokenNext == '$')
						state = END_REAL;
					else if(tokenNext == ',')
						state = END_REAL;
					else if(tokenNext == '<')
						state = END_REAL;
					else if(tokenNext == '>')
						state = END_REAL;
					else if(tokenNext == '/')
						state = END_REAL;
					else if(tokenNext == '+')
						state = END_REAL;
					else if(tokenNext == '*')
						state = END_REAL;
					else if(tokenNext == '-')
						state = END_REAL;
					else if(tokenNext == ':')
						state = END_REAL;
					break;
				/*
				END REAL NUMBERS STATE
				*/
				case END_REAL:
					state = STARTING;
					return removeSpaces(tempReal);
					break;
				/*
				ERROR STATE
				*/
				case ERROR:
					state = STARTING;
					if(tempIdentifier!="")
						tempIdentifier.clear();
					if(tempInteger!="")
						tempInteger.clear();
					if(tempReal!="")
						tempReal.clear();
					break;
			}
		}while(state == END_IDENTIFIER || state == END_INTEGER || state==END_REAL); 
	}
}
//==============================================================
// Description: Opens input file, and pushes each character
// onto an STL queue. 
//====== initQueue() ===========================================
void initQueue(deque<char> & queue)
{
		fstream input(INPUT_FILE,ios::in);
		while(input.good())
			queue.push_back(input.get());
		queue.push_back('$');
		input.close();
}
//==============================================================
// Description: Lexeme is taken and compared with 3 different 
// arrays, to find predetermined keywords, operators, and separators.
// If the lexeme starts with a digit, the remaining characters
// are checked for a decimal. If none of the checks are satisfied,
// the lexeme must be an error. 
//====== tokenParser() =========================================
string tokenParser(string lexeme)
{
	//Keywords and identifiers
	//First letter must be letter or _, and last character cannot be _
	if((isalpha(lexeme[0])||lexeme[0] == '_') && lexeme[lexeme.length()-1]!='_')
	{
		for(int i=0; i<16; i++)
		{
			if(lexeme == keywords[i]) 
				return "Keyword";
		}
		return "Identifier";
	}
	//Operators
	for(int i=0; i<9; i++)
	{
		if(lexeme == operators[i]) 
		{

			return "Operator";
		}
	}
	//separators
	for(int i=0; i<9; i++)
	{
		if(lexeme == separators[i]) 
			return "Separator";
	}
	if(isdigit(lexeme[0]))
	{
		for(int i=0; i<lexeme.length(); i++)
		{
			if(lexeme[i] == '.')
				return "Real";				
		}
		return "Integer";
	}
	return "ERROR";
}
//==============================================================
// Description: Function runs through array of 37 input tokens
// to check if string is a token symbol. 
//====== isTerminal() ==========================================
bool isTerminal(string token)
{
	for(int i=0; i<37; i++)
	{
		if(token == terminals[i])
			return true;
	} 
	return false;
}
//==============================================================
//  Description: Performs a check on all strings returned from 
// lexer, just to be sure no additional white spaces are included
//====== removeSpaces() ========================================
string removeSpaces(string lexeme)
{
	string temp;
	for(int i=0; i<lexeme.length(); i++)
	{
		if(!isspace(lexeme[i]))
			temp+=lexeme[i];
	} 
	return temp;
}
//==============================================================
// Description: Function accepts a non-terminal symbol, and lexeme
// from main. A pre-defined array of non-terminals is scanned,
// to find the symbol's row position in the parse table. Once
// the row is found, the column containing the input lexeme is 
// searched for in a similar array of input tokens. The XY position
// in the parse table is then returned. 
//====== searchTable() =========================================
string searchTable(string t, string lexeme)
{
	int row, column;
	for(int i=0; i<35; i++)
	{
		//Search for row beloning to Non-Terminal t  
		if(nonTerminals[i] == t)
			row = i;
	}
	//Search corresponding row for input token 
	for(int i=0; i<37; i++)
	{
		if(terminals[i] == lexeme)
			column = i;
	}
	return table[row][column];
}
//==============================================================
//  Description: Function accepts string and an stl stack. 
//  String is scanned character by character for spaces, to 
// separate each word. Once a full word is found, it gets pushed
// onto the stack. When the main driver pops each element, they 
// will be in reversed order. 
//====== reverseSTR() ==========================================
void reverseSTR(string production, stack<string> & reverse)
{
	string temp;
	for(int i=0; i<production.length(); i++)
	{
		if((production[i] == ' ') && (i!=0))
		{
			reverse.push(temp);
			temp.clear();
		}
		else
		{
			if(i==production.length()-1)
			{
				temp+=production[i];
				reverse.push(temp);
				temp.clear();
			}
			else
				temp+=production[i];
		}
	}
}