//=========================================
//Name: Daniel Jordan
//Email: daniel_jordan@csu.fullerton.edu
//Class: CPSC 323
//Assignment: Project 3: IC Code Generator
//=========================================
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <deque>
#include <stack>
#include <sstream>
#include "List.h"
using namespace std;
char * const INPUT_FILE = "input.txt";
char * const OUTPUT_FILE = "output.txt";
void initQueue(deque<char> & queue);
void reverseSTR(string production, stack<string> & reverse);
void getInstructions(string lexeme, List & symbolTable, string lookahead, stack<string> & exprStack, int & address, string (&instructionTable)[300][3]);
void printInstructionTable(int address, string instructionTable[300][3]);
void insertTable(int & address, string op, int oprnd, string (&instructionTable)[300][3]);
void backPatch(int jumpAddress, string (&instructionTable)[300][3], stack<int> & jumpStack);
bool isTerminal(string token);
string lexer(deque<char> & queue);
string tokenParser(string lexeme);
string removeSpaces(string lexeme);
string searchTable(string t, string lexeme);
string getDataType(string t, string lexeme);
string toString(int number);
enum STATES{STARTING, IDENTIFIER, END_IDENTIFIER, INTEGER, END_INTEGER, REAL, END_REAL, ERROR};
const string keywords[] = {"do", "while", "if", "fi", "else", "",  "begin", "end", "read", "write", "return", "function", "int", "boolean", "true", "false"};
const string operators[] = {"<", ">", "<=", ">=", "*", "/", "-", "+", ":=" , "/="};
const string separators[] = {"(", ")", "{", "}", ";","$", "$$", ",", ":"};
const string terminals[] = {"+", "-", "*", "/", "=", "/=", "<", ">", "(", ")", "{", "}", ",", ";", "if", "fi", "else", "true", "false", "int", "boolean", "", "INTEGER", "REAL", "IDENTIFIER", "function", "begin", "end", "return", "write", "read", "while", "do", "$$","$", ":=",":","<=",">="};
const string nonTerminals[] = {"<Rat12SU>", "<Expression>", "<ExpressionPrime>", "<Term>", "<TermPrime>", "<Factor>", "<Primary>", "<PrimaryPrime>", "<If>", "<IfPrime>", "<OptDeclarationList>", "<OptFunctionDefinitions>", "<Function>",
			   "", "", "<DeclarationList>", "<DeclarationListPrime>", "<Declaration>", "<Qualifier>", "<ID>", "<IDPrime>", "<Body>", "<StatementList>", "<StatementListPrime>",
			   "<Statement>", "<Compound>", "<Assign>", "<Return>", "<Write>", "<Read>", "<While>", "<Condition>", "<Relop>", "<Parameter>","epsilon"};
//Instruction table holding the address, operation, and operand of up to 300 instructions
string instructionTable[300][3];
//Parse Table
const string table[35][39]={
{"", "", "","","","","","","","","", "", "","","","","","","","","", "", "","","","","","","","","","","","$$ <OptFunctionDefinitions> $$ <OptDeclarationList> <StatementList> $$","","","","",""},//<Rat12SU>
{"","<Term> <ExpressionPrime>","","","","","","","<Term> <ExpressionPrime>","","","","","","","","","<Term> <ExpressionPrime>","<Term> <ExpressionPrime>","","","","<Term> <ExpressionPrime>","<Term> <ExpressionPrime>","<Term> <ExpressionPrime>","","","",""},//<Expression>
{"+ <Term> <ExpressionPrime>","- <Term> <ExpressionPrime>","epsilon","<TermPrime>","epsilon","epsilon","epsilon", "epsilon","","epsilon","","","","epsilon","","","","","","","","","","","epsilon","","","","","","","","","","","","","epsilon","epsilon"},//<Expression Prime>
{"","<Factor> <TermPrime>","","","","","","","<Factor> <TermPrime>","","","","","","","","","<Factor> <TermPrime>","<Factor> <TermPrime>","","","","<Factor> <TermPrime>","<Factor> <TermPrime>","<Factor> <TermPrime>","","","","","","","","","","","","","",""},//<Term>
{"epsilon", "epsilon", "* <Factor>","/ <Factor>","epsilon","epsilon","epsilon","epsilon","","epsilon","","","","epsilon","","","","","","","","","","","epsilon","","","","","","","","","","","","","epsilon","epsilon"},//<Term Prime>
{"","- <Primary>","","","","","","","<Primary>","","","","","","","","","<Primary>","<Primary>","","","","<Primary>","<Primary>","<Primary>", "","","","","","","","","","","","","",""},//<Factor>
{"","","","","","","","","( <Expression> )","","","","","","","","","true","false","","","","INTEGER","REAL","<ID> <PrimaryPrime>","","","","","","","","","","","","","",""},//<Primary>
{"epsilon","epsilon","epsilon","epsilon","epsilon","epsilon","epsilon","epsilon","( <ID> )","epsilon","","","","epsilon","","","","","","","","","","","epsilon","","","","","","","","","","","","epsilon","epsilon"},//<Primary Prime>
{"","","","","","","","","","","","","","","if( <Condition> ) <Statement>","","","","","","","","","","","","","","","","","","","","","","","",""},//<IF>
{"","","","","","","","","","","","","","","","fi","else<statement>fi","","","","","","","","","","","","","","","","","","","","","",""},//<IF Prime>
{"","","","","","","","","","","epsilon","","","","epsilon","","","","","<DeclarationList>","<DeclarationList>","<DeclarationList>","","","epsilon","","<Body>","","epsilon","epsilon","epsilon","epsilon","","","","","",""},//<Opt Declaration List>
{"","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","epsilon","","","","",""},//<Opt Function Definitions>
{"","","","","","","","","","","","","","","","","","","","","","","","","","function IDENTIFIER ( <Parameter> ) <OptDeclarationList> <Body>","","","","","","","","","","","","",""},//<Function>
{"","","","","","","","","","","","","","","","","","","","","","","","","","<Function> <FunctionDefinitionsPrime>","","","","","","","","","","","","",""},//<Function Definitions> Entries removed for project 3
{"","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","epsilon","epsilon","","","",""},//<Function Definitions Prime>Entried removed for project 3
{"","","","","","","","","","","","","","","","","","","","<Declaration> ; <DeclarationListPrime>","<Declaration> ; <DeclarationListPrime>","<Declaration> ; <DeclarationListPrime>","","","","","","","","","","","","","","","","",""},//<Declaration List>
{"","","","","","","","","","","epsilon","","","<DeclarationListPrime>","epsilon","","","","","<DeclarationList>","<DeclarationList>","<DeclarationList>","","","epsilon","","epsilon","","epsilon","epsilon","epsilon","epsilon","","","","","","",""},//<Declaration List Prime>
{"","","","","","","","","","","","","","","","","","","","<Qualifier> <ID>","<Qualifier> <ID>","<Qualifier> <ID>","","","","","","","","","","","","","","","","",""},//<Declaration>
{"","","","","","","","","","","","","","","","","","","","int","boolean","real","","","","","","","","","","","","","","","","",""},//<Qualifier>
{"","","","","","","","","","","","","","","","","","","","","","","","","IDENTIFIER <IDPrime>","","","","","","","","","","","","","",""},//<ID>
{"epsilon","epsilon","epsilon","epsilon","epsilon","epsilon","epsilon","epsilon","epsilon","epsilon","","",", <ID>","epsilon","","","","","","","","","","","epsilon","","","","","","","","","","","","","epsilon","epsilon"},//<ID Prime>
{"","","","","","","","","","","","","","","","","","","","","","","","","","","begin <StatementList> end","","","","","","","epsilon","","","","",""},//<Body>
{"","","","","","","","","","","<Statement> <StatementListPrime>","","","","<Statement> <StatementListPrime>","","","","","","","","","","<Statement> <StatementListPrime>","","","","<Statement> <StatementListPrime>","<Statement> <StatementListPrime>","<Statement> <StatementListPrime>","<Statement> <StatementListPrime>","","","","","","",""},//<Statement List>
{"","","","","","","","","","","<StatementList>","epsilon","","","<StatementList>","","","","","","","","","","<StatementList>","","","epsilon","<StatementList>","<StatementList>","<StatementList>","<StatementList>","","epsilon","epsilon","","","",""},//<Statement List Prime>
{"","","","","","","","","","","<Compound>","","","","<If>","","","","","","","","","","<Assign>","","","","<Return>","<Write>","<Read>","<While>","","","","","","",""},//<Statement>
{"","","","","","","","","","","{ <StatementList> }", "","","","","","","","","","","","","","","","","","","","","","","","","","","",""},//<Compound>
{"","","","","","","","","","","","","","","","","","","","","","","","","IDENTIFIER := <Expression> ;","","","","","","","","","","","<Expression>","","",""},//<Assign>
{"","","","","","","","","","","","","","","","","","","","","","","","","","","","","return <Expression> ;","","","","","","","","","",""},//<Return>
{"","","","","","","","","","","","","","","","","","","","","","","","","","","","","","write ( <Expression> ) ;","","","","","","","","",""},//<Write>
{"","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","read ( <ID> ) ;","","","","","","","",""},//<Read>
{"","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","while ( <Condition> ) do <Statement>","","","","","","",""},//<While>
{"","<Expression> <Relop> <Expression>","","","","","","","","epsilon","","","","","","","","","","","","","<Expression> <Relop> <Expression>","<Expression> <Relop> <Expression>","<Expression> <Relop> <Expression>","","","","","","","","","","","","","",""},//<Condition>
{"","","","","=","/=","<",">","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","<=",">="},//<Relop>
{"","","","","","","","","","","","","","","","","","","","","","","","","IDENTIFIER : <Qualifier>","","","","","","","","","","","","","",""},//<Parameter>
{"epsilon","epsilon","epsilon","epsilon","epsilon","epsilon","epsilon","epsilon","epsilon","epsilon","epsilon","epsilon","epsilon","epsilon","epsilon","epsilon","epsilon","epsilon","epsilon","epsilon","epsilon","epsilon","epsilon","epsilon","epsilon","epsilon","epsilon","epsilon","epsilon","epsilon","epsilon","epsilon","epsilon","epsilon","epsilon","epsilon","epsilon","epsilon","epsilon"},//epsilon
};
//===========================================================================
//
//====== main() ============================================================
int main()
{
	fstream output(OUTPUT_FILE, ios::out);
	deque<char> queue;
	deque<string> lexQueue;
	stack<int> jumpStack;
	stack<string> stack, reverse, exprStack, condStack;
	stack.push("$"); stack.push("<Rat12SU>");
	List symbolTable;
	string lexeme, token, t, dataType, identifierName, save;
	int counter=0, lexCounter = 0, ML = 1000, address = 1, jumpAddress;
	//Read source file into queue
	initQueue(queue);
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
		save = lexeme;
		token = tokenParser(lexeme);
		//Save original lexeme name for symbol table
		identifierName = lexeme;
		if(token == "Identifier") lexeme = "IDENTIFIER";
		else if(token == "Integer") lexeme = "INTEGER";
		else if(token == "Real")
		{
			cout << "\nERROR after token #" << lexCounter << ". No reals allowed" << endl;
			output << "\nERROR after token #" << lexCounter << ". No reals allowed" << endl;
			break;
		}
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
				if(t == "$")
					insertTable(address, "POPS", -1, instructionTable);
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
			//Determine data type of identifiers, for symbol table
			if(t=="<Qualifier>")
				dataType = getDataType(t, lexeme);
			//Insert identifier into symbol table
			if(counter == 0)
			{
				if(lexeme == "IDENTIFIER")
				{
					if(lexQueue.at(1) == ":=")
						exprStack.push(identifierName);
					//If expression stack is not empty, program is still processing expression
					else if(!exprStack.empty())
						insertTable(address, "PUSHM", symbolTable.getMemoryLocation(identifierName),instructionTable);
					else if(!condStack.empty())
						insertTable(address, "PUSHM", symbolTable.getMemoryLocation(identifierName),instructionTable);
					else
					{
						//Check table for existing identifier name
						if(!symbolTable.duplicateID(identifierName))
						{
							symbolTable.insert(identifierName, ML, dataType);
							ML++;
						}
						else
						{
							if(lexQueue.at(1) == "+" || lexQueue.at(1) == "-" || lexQueue.at(1) == "+" || lexQueue.at(1) == "*" || lexQueue.at(1) == "/" || lexQueue.at(1) == "<" || lexQueue.at(1) == ">" || lexQueue.at(1) == "/="|| lexQueue.at(1) == ">="|| lexQueue.at(1) == "<=") 
								insertTable(address, "PUSHM", symbolTable.getMemoryLocation(identifierName),instructionTable);
							else
								insertTable(address, "POPM", symbolTable.getMemoryLocation(identifierName),instructionTable);
						}
					}
				}
				else if(lexeme == "INTEGER")
					insertTable(address, "PUSHI", atoi(identifierName.c_str()),instructionTable);
				else if(lexeme == "read" || lexeme == "write"  || lexeme == "return" )
					insertTable(address, "PUSHS", -1, instructionTable);
				else if(lexeme == "while" || lexeme == "if")
				{
					jumpAddress = address;
					insertTable(address, "LABEL", -1, instructionTable);
				}
				//End of a statement
				else if((!jumpStack.empty()&& exprStack.empty()) && (lexeme == "}" || lexeme == ";"))
				{
					insertTable(address, "JUMP", jumpAddress, instructionTable);
					backPatch(address, instructionTable, jumpStack);
				}
				//Skip over operators by pushing them into a temporary stack
				else if(lexeme == "+" || lexeme == "*" || lexeme == "-" || lexeme == "/")
					exprStack.push(lexeme);
				else if(lexeme == "<" || lexeme == ">" || lexeme == "=" || lexeme == "/=")
				{
					condStack.push(lexeme);
				}
				//End of the expression. Pop the operators and insert into table
				if(!exprStack.empty() && (lexeme == ";" || lexeme == ")"))
				{
					while(!exprStack.empty())
					{
						if(exprStack.top() == "+")
							insertTable(address, "ADD", -1,instructionTable);
						else if(exprStack.top() == "-")
							insertTable(address, "SUB", -1,instructionTable);
						else if(exprStack.top() == "*")
							insertTable(address, "MUL", -1,instructionTable);
						else if(exprStack.top() == "/")
							insertTable(address, "DIV", -1,instructionTable);
						else
							insertTable(address, "POPM", symbolTable.getMemoryLocation(exprStack.top()),instructionTable);
						exprStack.pop();
					}
				}
				else if(!condStack.empty() && lexeme == ")")
				{
					while(!condStack.empty())
					{
						if(condStack.top() == "<")
							insertTable(address, "LES", -1,instructionTable);
						else if(condStack.top() == ">")
							insertTable(address, "GRT", -1,instructionTable);
						else if(condStack.top() == "/=")
							insertTable(address, "NEQ", -1,instructionTable);
						else if(condStack.top() == "=")
							insertTable(address, "EQU", -1,instructionTable);
						condStack.pop();
						jumpStack.push(address);
						insertTable(address, "JUMPZ", -1,instructionTable);

					}
				}
			}
			cout << t << "::= " << searchTable(t, lexeme) << endl;
			output << t << "::= " << searchTable(t, lexeme) << endl;
			//Advances input when prime productions dont go to epsilon. Fixes infinite loop problem
			if(t==searchTable(t, lexeme))
			{
				lexQueue.pop_front();
				counter = 0;
			}
			else if(searchTable(t, lexeme) !="")
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
	printInstructionTable(address, instructionTable);
	symbolTable.displayTable();
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
					else if(token =='.') state = ERROR;
					else if(token == ' ') state = STARTING;
					else if(token == '$' && queue.empty()) return "$";
					else if(token == '('){ state = STARTING; return "("; }
					else if(token == ')') { state = STARTING; return ")"; }
					else if(token == ',') { state = STARTING; return ","; }
					else if(token == '{') { state = STARTING; return "{"; }
					else if(token == '}') { state = STARTING; return "}"; }
					else if(token == ';') { state = STARTING; return ";"; }
					else if(token == '+') { state = STARTING; return "+"; }
					else if(token == '*') { state = STARTING; return "*"; }
					else if(token == '-') { state = STARTING; return "-"; }
					else if(token == '=') { state = STARTING; return "="; }
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
					else if(token == '<')
					{
						state = STARTING;
						if(tokenNext == '=')
						{
							queue.pop_front();
							return "<=";
						}
						else
							return "<";
					}
					else if(token == '>')
					{
						state = STARTING;
						if(tokenNext == '=')
						{
							queue.pop_front();
							return ">=";
						}
						else
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
					if(queue.empty()) state = END_IDENTIFIER;
					else if(token == ' ') state = END_IDENTIFIER;
					else if(token == '(') state = END_IDENTIFIER;
					else if(token == ')') state = END_IDENTIFIER;
					else if(token == '{') state = END_IDENTIFIER;
					else if(token == '}') state = END_IDENTIFIER;
					else if(token == ';') state = END_IDENTIFIER;
					else if(token == ',') state = END_IDENTIFIER;
					else if(token == '$') state = END_IDENTIFIER;
					else if(token == '<') state = END_IDENTIFIER;
					else if(token == '>') state = END_IDENTIFIER;
					else if(token == '/') state = END_IDENTIFIER;
					else if(token == '+') state = END_IDENTIFIER;
					else if(token == '*') state = END_IDENTIFIER;
					else if(token == '-') state = END_IDENTIFIER;
					else if(token == ':') state = END_IDENTIFIER;
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
					if(token == ' ') state = END_INTEGER;
					else if(token == '(') state = END_INTEGER;
					else if(token == ')') state = END_INTEGER;
					else if(token == '{') state = END_INTEGER;
					else if(token == '}') state = END_INTEGER;
					else if(token == ';') state = END_INTEGER;
					else if(token == ',') state = END_INTEGER;
					else if(token == '$') state = END_INTEGER;
					else if(token == '<') state = END_INTEGER;
					else if(token == '>') state = END_INTEGER;
					else if(token == '/') state = END_INTEGER;
					else if(token == '+') state = END_INTEGER;
					else if(token == '*') state = END_INTEGER;
					else if(token == '-') state = END_INTEGER;
					else if(token == ':') state = END_INTEGER;
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
					if(tokenNext == ' ') state = END_REAL;
					else if(tokenNext == '(') state = END_REAL;
					else if(tokenNext == ')') state = END_REAL;
					else if(tokenNext == '{') state = END_REAL;
					else if(tokenNext == '}') state = END_REAL;
					else if(tokenNext == ';') state = END_REAL;
					else if(tokenNext == '$') state = END_REAL;
					else if(tokenNext == ',') state = END_REAL;
					else if(tokenNext == '<') state = END_REAL;
					else if(tokenNext == '>') state = END_REAL;
					else if(tokenNext == '/') state = END_REAL;
					else if(tokenNext == '+') state = END_REAL;
					else if(tokenNext == '*') state = END_REAL;
					else if(tokenNext == '-') state = END_REAL;
					else if(tokenNext == ':') state = END_REAL;
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
	for(int i=0; i<10; i++)
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
// Description: Function runs through array of 39 input tokens
// to check if string is a terminal symbol.
//====== isTerminal() ==========================================
bool isTerminal(string token)
{
	for(int i=0; i<39; i++)
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
	for(int i=0; i<39; i++)
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
//==============================================================
//  Description: Function returns the current position of the
//  parsing table, when parser is in a <Qualifier> state. This
//  will determine the datatype of any qualifier statement.
//====== reverseSTR() ==========================================
string getDataType(string t, string lexeme)
{
	if(searchTable(t, lexeme) == "int")
		return "Integer";
	else if(searchTable(t, lexeme) == "boolean")
		return "Boolean";
	else if(searchTable(t, lexeme) == "real")
		return "Real";
}
//==============================================================
//  Description: Inserts address, operation, and operand into
//  assembly table array. After inserting, address value is
//====== insertTable() =========================================
void insertTable(int & address, string op, int oprnd, string (&instructionTable)[300][3])
{
	instructionTable[address][0] = toString(address);
	instructionTable[address][1] = op;
	instructionTable[address][2] = toString(oprnd);
	address++;
}
//==============================================================
//  Description: 
//
//====== printInstructionTable() ===============================
void printInstructionTable(int address, string instructionTable[300][3])
{
	fstream output("output.txt", ios::app);
	cout << "\n--------------- Assembly Code Listing ---------------\n";
	output << "\n--------------- Assembly Code Listing ---------------\n";

	cout << "\nAddress\tOp\tOprnd\n";
	output << "\nAddress\tOp\tOprnd\n";

	for(int i=1; i< address; i++)
	{
		cout << instructionTable[i][0] << "\t" << instructionTable[i][1] << "\t";
		output << instructionTable[i][0] << "\t" << instructionTable[i][1] << "\t";
		if(instructionTable[i][2] == "-1")
		{
			if(instructionTable[i][1] =="PUSHI")
			{
				cout << instructionTable[i][2];
				output << instructionTable[i][2];
			}
		}
		else
		{
			cout << instructionTable[i][2];
			output << instructionTable[i][2];
		}
		cout << endl;
		output << endl;
	}
}
//==============================================================
//  Description: Function takes integer input and copies the
//  stream value into a string
//====== toString() ============================================
string toString(int number)
{
   stringstream ss;
   ss << number;
   return ss.str();
}
//==============================================================
//  Description: Function accepts a previous address value, 
//  and updates the operand with the top value of a stack
//====== backPatch() ===========================================
void backPatch(int jumpAddress, string (&instructionTable)[300][3], stack<int> & jumpStack)
{
	int address = jumpStack.top();
	jumpStack.pop();
	instructionTable[address][2].clear();
	instructionTable[address][2] = toString(jumpAddress);
}