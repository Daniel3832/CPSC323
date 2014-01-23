//=========================================
//Name: Daniel Jordan
//Email: daniel_jordan@csu.fullerton.edu
//Class: CPSC 323
//Assignment: Project 1: Lexical analysis 
//=========================================
#include <iostream>
#include<fstream>
#include <string>
#include <iomanip>
using namespace std;
int getFileSize(char * INPUT_FILE);
char * const INPUT_FILE = "input.txt";
char * const OUTPUT_FILE = "output.txt";
enum STATES{STARTING, IDENTIFIER, END_IDENTIFIER, INTEGER, END_INTEGER, REAL, END_REAL, ERROR};
const string keywords[] = {"do", "while", "if", "fi", "else", "real",  "begin", "end", "read", "write", "return", "function", "int", "bool"};
int main()	
{	
	STATES state = STARTING;
	//Calculate file size of source file
	int fileSize = getFileSize(INPUT_FILE);
	//Allocate size of source file to char array
	char * token = new char[fileSize];
	//Input file
	fstream input(INPUT_FILE,ios::in);
	//Strings to temporarily hold each scanned character 
	string tempIdentifier, tempInteger, tempReal;
	bool isKeyword;
	//Fill array with source file
	for(int i=0; i<fileSize; i++)
		token[i] = input.get();
	input.close();
	//Output file
	fstream output(OUTPUT_FILE, ios::out);
	for(int i=0; i<fileSize; i++)
	{
		cout << token[i];
	}
	cout << "\n\nToken" << setw(20) << "Lexeme\n\n";
	output << "Token" << setw(20) << "Lexeme\n\n";


	for(int i=0; i<fileSize; i++)
	{
		do{
			switch(state)
			{
				/*
				STARTING STATE
				*/
				case STARTING:
					//First character of identifier must be alphabetic
					if(isalpha(token[i]))
					{
						tempIdentifier+=token[i];
						state = IDENTIFIER;
					}
					else if(isdigit(token[i]))
					{
						tempInteger+=token[i];
						state = INTEGER;
					}
					else if(token[i] =='.')
						state = ERROR;
					else if(token[i] == ' ')
						state = STARTING;
					else if(token[i] == '(')
					{
						state = STARTING;
						cout << "Separator" << setw(13) << token[i] << endl;
						output << "Separator" << setw(13) << token[i] << endl;
					}
					else if(token[i] == ')')
					{
						state = STARTING;
						cout << "Separator" << setw(13) << token[i] << endl;
						output << "Separator" << setw(13) << token[i] << endl;
					}
					else if(token[i] == '{')
					{
						state = STARTING;
						cout << "Separator" << setw(13) << token[i] << endl;
						output << "Separator" << setw(13) << token[i] << endl;
					}
					else if(token[i] == '}')
					{
						state = STARTING;
						cout << "Separator" << setw(13) << token[i] << endl;
						output << "Separator" << setw(13) << token[i] << endl;
					}
					else if(token[i] == ';')
					{
						state = STARTING;
						cout << "Separator" << setw(13) << token[i] << endl;
						output << "Separator" << setw(13) << token[i] << endl;
					}
					else if(token[i] == '<')
					{
						if(token[i+1] == '=')
						{
							cout << "Operator" << setw(14) << token[i] << token[i+1] << endl;
							output << "Operator" << setw(14) << token[i] << token[i+1] << endl;
						}
						else
						{
							cout << "Operator" << setw(14) << token[i] << endl;
							output << "Operator" << setw(14) << token[i] << token[i+1] << endl;
						}
						state = STARTING;
					}
					else if(token[i] == '>')
					{
						if(token[i+1] == '=')
						{
							cout << "Operator" << setw(14) << token[i] << token[i+1] << endl;
							output << "Operator" << setw(14) << token[i] << token[i+1] << endl;
						}
						else
						{
							cout << "Operator" << setw(14) << token[i] << endl;
							output << "Operator" << setw(14) << token[i] << token[i+1] << endl;
						}
						state = STARTING;
					}
					else if(token[i] == '/')
					{
						state = STARTING;
						cout << "Operator" << setw(14) << token[i] << endl;
						output << "Operator" << setw(14) << token[i] << endl;
					}
					else if(token[i] == '+')
					{
						state = STARTING;
						cout << "Operator" << setw(14) << token[i] << endl;
						output << "Operator" << setw(14) << token[i] << endl;
					}
					else if(token[i] == '*')
					{
						state = STARTING;
						cout << "Operator" << setw(14) << token[i] << endl;
						output << "Operator" << setw(14) << token[i] << endl;
					}
					else if(token[i] == '-')
					{
						state = STARTING;
						cout << "Operator" << setw(14) << token[i] << endl;
						output << "Operator" << setw(14) << token[i] << endl;
					}
					else if(token[i] == ':')
					{
						state = STARTING;
						if(token[i+1] == '=')
						{
							cout << "Operator" << setw(14) << token[i] << token[i+1] << endl;
							output << "Operator" << setw(14) << token[i] << token[i+1] << endl;
						}
					}
					else if(token[i] == '=')
						state = STARTING;
					break;
				/*
				IDENTIFIER STATE
				*/
				case IDENTIFIER:
					//Underscore _ is allowed once in IDENTIFIER state. Not allowed for first or last character
					if(isalpha(token[i]) || token[i] == '_')
					{
						tempIdentifier+=token[i];
						//Switches to ERROR state, if last character in identifier is _
						if(token[i] == '_' && isalnum(token[i+1])==0)
							state = ERROR;
						else
							state = IDENTIFIER;
					}
					else if(isdigit(token[i]))
					{
						tempIdentifier+=token[i];
						state = IDENTIFIER;
					}
					else if(token[i] =='.')
						state = ERROR;
					else if(token[i] == ' ')
						state = END_IDENTIFIER;
					else if(token[i] == '(')
						state = END_IDENTIFIER;
					else if(token[i] == ')')
						state = END_IDENTIFIER;
					else if(token[i] == '{')
						state = END_IDENTIFIER;
					else if(token[i] == '}')
						state = END_IDENTIFIER;
					else if(token[i] == ';')
						state = END_IDENTIFIER;
					else if(token[i] == '<')
						state = END_IDENTIFIER;
					else if(token[i] == '>')
						state = END_IDENTIFIER;
					else if(token[i] == '/')
						state = END_IDENTIFIER;
					else if(token[i] == '+')
						state = END_IDENTIFIER;
					else if(token[i] == '*')
						state = END_IDENTIFIER;
					else if(token[i] == '-')
						state = END_IDENTIFIER;
					else if(token[i] == ':')
						state = END_IDENTIFIER;
					else if(token[i] == '=')
						state = END_IDENTIFIER;
					break;
				/*
				END IDENTIFIER STATE
				*/
				case END_IDENTIFIER:
					//Loops through keyword array to see if identifier is a keyword
					for(int k=0; k<14; k++)
					{
						if(tempIdentifier == keywords[k])
						{
							isKeyword = true;
							//Loop stops once keyword is found
							break;
						}
						else
							isKeyword = false;
					}
					if(isKeyword == true)
					{
						cout << "Keyword" << setw(17) << tempIdentifier << endl;
						output << "Keyword" << setw(17) << tempIdentifier << endl;
					}
					else
					{
						cout << "Identifier" << setw(14) << tempIdentifier << endl;
						output << "Identifier" << setw(14) << tempIdentifier << endl;
					}
					//Clear identifier after its printed
					tempIdentifier.clear();
					state = STARTING;
					if(token[i] == '(')
					{
						cout << "Separator" << setw(13) << token[i] << endl;
						output << "Separator" << setw(13) << token[i] << endl;
					}
					else if(token[i] == ')')
					{
						cout << "Separator" << setw(13) << token[i] << endl;
						output << "Separator" << setw(13) << token[i] << endl;
					}
					else if(token[i] == '{')
					{
						cout << "Separator" << setw(13) << token[i] << endl;
						output << "Separator" << setw(13) << token[i] << endl;
					}
					else if(token[i] == '}')
					{
						cout << "Separator" << setw(13) << token[i] << endl;
						output << "Separator" << setw(13) << token[i] << endl;
					}
					else if(token[i] == ';')
					{
						cout << "Separator" << setw(13) << token[i] << endl;
						output << "Separator" << setw(13) << token[i] << endl;
					}
					else if(token[i] == '<')
					{
						if(token[i+1] == '=')
						{
							cout << "Operator" << setw(14) << token[i] << token[i+1] << endl;
							output << "Operator" << setw(14) << token[i] << token[i+1] << endl;
						}
						else
						{
							cout << "Operator" << setw(14) << token[i] << endl;
							output << "Operator" << setw(14) << token[i] << endl;
						}
					}
					else if(token[i] == '>')
					{
						if(token[i+1] == '=')
						{
							cout << "Operator" << setw(14) << token[i] << token[i+1] << endl;
							output << "Operator" << setw(14) << token[i] << token[i+1] << endl;
						}
						else
						{
							cout << "Operator" << setw(14) << token[i] << endl;
							output << "Operator" << setw(14) << token[i] << endl;
						}					}
					else if(token[i] == '/')
					{
						cout << "Operator" << setw(14) << token[i] << endl;
						output << "Operator" << setw(14) << token[i] << endl;
					}
					else if(token[i] == '+')
					{
						cout << "Operator" << setw(14) << token[i] << endl;
						output << "Operator" << setw(14) << token[i] << endl;
					}
					else if(token[i] == '*')
					{
						cout << "Operator" << setw(14) << token[i] << endl;
						output << "Operator" << setw(14) << token[i] << endl;
					}
					else if(token[i] == '-')
					{
						cout << "Operator" << setw(14) << token[i] << endl;
						output << "Operator" << setw(14) << token[i] << endl;
					}
					else if(token[i] == ':')
					{
						if(token[i+1] == '=')
						{
							cout << "Operator" << setw(14) << token[i] << token[i+1] << endl;	
							output << "Operator" << setw(14) << token[i] << token[i+1] << endl;	
						}
					}
					break;
				/*
				INTEGER STATE
				*/
				case INTEGER:
					//Transitions to error state if letter is entered from integer state
					if(isalpha(token[i]))
					{
						tempInteger+=token[i];
						//Waits until string finishes before transitioning to error state
						if(isalnum(token[i+1])==0)
							state = ERROR;
						else
							state = INTEGER;
					}
					else if(isdigit(token[i]))
					{
						tempInteger+=token[i];
						state = INTEGER;
					}
					else if(token[i] =='.')
					{
						tempReal+=tempInteger+=".";
						state = REAL;
						//Clear integer value before transition to real number
						tempInteger.clear();
					}
					else if(token[i] == ' ')
						state = END_INTEGER;
					else if(token[i] == '(')
						state = END_INTEGER;
					else if(token[i] == ')')
						state = END_INTEGER;
					else if(token[i] == '{')
						state = END_INTEGER;
					else if(token[i] == '}')
						state = END_INTEGER;
					else if(token[i] == ';')
						state = END_INTEGER;
					else if(token[i] == '<')
						state = END_INTEGER;
					else if(token[i] == '>')
						state = END_INTEGER;
					else if(token[i] == '/')
						state = END_INTEGER;
					else if(token[i] == '+')
						state = END_INTEGER;
					else if(token[i] == '*')
						state = END_INTEGER;
					else if(token[i] == '-')
						state = END_INTEGER;
					else if(token[i] == ':')
						state = END_INTEGER;
					break;
				/*
				END INTEGER STATE
				*/
				case END_INTEGER:
					cout << "Integer" << setw(17) << tempInteger << endl;
					output << "Integer" << setw(17) << tempInteger << endl;
					//Clear interger after its printed
					tempInteger.clear();
					state = STARTING;
					if(token[i] == '(')
					{
						cout << "Separator" << setw(13) << token[i] << endl;
						output << "Separator" << setw(13) << token[i] << endl;
					}
					else if(token[i] == ')')
					{
						cout << "Separator" << setw(13) << token[i] << endl;
						output << "Separator" << setw(13) << token[i] << endl;
					}
					else if(token[i] == '{')
					{
						cout << "Separator" << setw(13) << token[i] << endl;
						output << "Separator" << setw(13) << token[i] << endl;
					}
					else if(token[i] == '}')
					{
						cout << "Separator" << setw(13) << token[i] << endl;
						output << "Separator" << setw(13) << token[i] << endl;
					}
					else if(token[i] == ';')
					{
						cout << "Separator" << setw(13) << token[i] << endl;
						output << "Separator" << setw(13) << token[i] << endl;
					}
					else if(token[i] == '<')
					{
						if(token[i+1] == '=')
						{
							cout << "Operator" << setw(14) << token[i] << token[i+1] << endl;
							output << "Operator" << setw(14) << token[i] << token[i+1] << endl;
						}
						else
						{
							cout << "Operator" << setw(14) << token[i] << endl;
							output << "Operator" << setw(14) << token[i] << endl;
						}
					}
					else if(token[i] == '>')
					{
						if(token[i+1] == '=')
						{
							cout << "Operator" << setw(14) << token[i] << token[i+1] << endl;
							output << "Operator" << setw(14) << token[i] << token[i+1] << endl;
						}
						else
						{
							cout << "Operator" << setw(14) << token[i] << endl;
							output << "Operator" << setw(14) << token[i] << endl;
						}
					}
					else if(token[i] == '/')
					{
						cout << "Operator" << setw(14) << token[i] << endl;
						output << "Operator" << setw(14) << token[i] << endl;
					}
					else if(token[i] == '+')
					{
						cout << "Operator" << setw(14) << token[i] << endl;
						output << "Operator" << setw(14) << token[i] << endl;
					}
					else if(token[i] == '*')
					{
						cout << "Operator" << setw(14) << token[i] << endl;
						output << "Operator" << setw(14) << token[i] << endl;
					}
					else if(token[i] == '-')
					{
						cout << "Operator" << setw(14) << token[i] << endl;
						output << "Operator" << setw(14) << token[i] << endl;
					}
					else if(token[i] == ':')
					{
						if(token[i+1] == '=')
						{
							cout << "Operator" << setw(14) << token[i] << token[i+1] << endl;
							output << "Operator" << setw(14) << token[i] << token[i+1] << endl;
						}
					}
					break;
				/* 
				REAL NUMBERS STATE
				*/
				case REAL:
					if(isalpha(token[i]))
					{
						tempIdentifier+=token[i];
						state = END_REAL;
					}
					else if(isdigit(token[i]))
					{
						tempReal+=token[i];
						state = REAL;
					}
					else if(token[i] =='.')
					{
						tempReal+=token[i];
						state = ERROR;
					}
					else if(token[i] == ' ')
						state = END_REAL;
					else if(token[i] == '(')
						state = END_REAL;
					else if(token[i] == ')')
						state = END_REAL;
					else if(token[i] == '{')
						state = END_REAL;
					else if(token[i] == '}')
						state = END_REAL;
					else if(token[i] == ';')
						state = END_REAL;
					else if(token[i] == '<')
						state = END_REAL;
					else if(token[i] == '>')
						state = END_REAL;
					else if(token[i] == '/')
						state = END_REAL;
					else if(token[i] == '+')
						state = END_REAL;
					else if(token[i] == '*')
						state = END_REAL;
					else if(token[i] == '-')
						state = END_REAL;
					else if(token[i] == ':')
						state = END_REAL;
					break;
				/*
				END REAL NUMBERS STATE
				*/
				case END_REAL:
					cout << "Real" << setw(20) << tempReal << endl;
					output << "Real" << setw(20) << tempReal << endl;
					//Clear real number after its printed
					tempReal.clear();
					state = STARTING;
					if(token[i] == '(')
					{
						cout << "Separator" << setw(13) << token[i] << endl;
						output << "Separator" << setw(13) << token[i] << endl;
					}
					else if(token[i] == ')')
					{
						cout << "Separator" << setw(13) << token[i] << endl;
						output << "Separator" << setw(13) << token[i] << endl;
					}
					else if(token[i] == '{')
					{
						cout << "Separator" << setw(13) << token[i] << endl;
						output << "Separator" << setw(13) << token[i] << endl;
					}
					else if(token[i] == '}')
					{
						cout << "Separator" << setw(13) << token[i] << endl;
						output << "Separator" << setw(13) << token[i] << endl;
					}
					else if(token[i] == ';')
					{
						cout << "Separator" << setw(13) << token[i] << endl;
						output << "Separator" << setw(13) << token[i] << endl;
					}
					else if(token[i] == '<')
					{
						if(token[i+1] == '=')
						{
							cout << "Operator" << setw(14) << token[i] << token[i+1] << endl;
							output << "Operator" << setw(14) << token[i] << token[i+1] << endl;
						}
						else
						{
							cout << "Operator" << setw(14) << token[i] << endl;
							output << "Operator" << setw(14) << token[i] << endl;
						}
					}
					else if(token[i] == '>')
					{
						if(token[i+1] == '=')
						{
							cout << "Operator" << setw(14) << token[i] << token[i+1] << endl;
							output << "Operator" << setw(14) << token[i] << token[i+1] << endl;
						}
						else
						{
							cout << "Operator" << setw(14) << token[i] << endl;
							output << "Operator" << setw(14) << token[i] << endl;
						}
					}
					else if(token[i] == '/')
					{
						cout << "Operator" << setw(14) << token[i] << endl;
						output << "Operator" << setw(14) << token[i] << endl;
					}
					else if(token[i] == '+')
					{
						cout << "Operator" << setw(14) << token[i] << endl;
						output << "Operator" << setw(14) << token[i] << endl;
					}
					else if(token[i] == '*')
					{
						cout << "Operator" << setw(14) << token[i] << endl;
						output << "Operator" << setw(14) << token[i] << endl;
					}
					else if(token[i] == '-')
					{
						cout << "Operator" << setw(14) << token[i] << endl;
						output << "Operator" << setw(14) << token[i] << endl;
					}
					else if(token[i] == ':')
					{
						if(token[i+1] == '=')
						{
							cout << "Operator" << setw(14) << token[i] << token[i+1] << endl;	
							output << "Operator" << setw(14) << token[i] << token[i+1] << endl;	
						}
					}
					break;
				/*
				END REAL NUMBERS STATE
				*/
				case ERROR:
					if(tempIdentifier!="")
					{
						cout << "ERROR" << setw(19) << tempIdentifier << endl;
						output << "ERROR" << setw(19) << tempIdentifier << endl;
						tempIdentifier.clear();
						state = STARTING;
					}
					if(tempInteger!="")
					{
						cout << "ERROR" << setw(18) << tempInteger << endl;
						output << "ERROR" << setw(18) << tempInteger << endl;
						tempInteger.clear();
						state = STARTING;
					}
					if(tempReal!="")
					{
						cout << "ERROR" << setw(20) << tempReal << endl;
						output << "ERROR" << setw(20) << tempReal << endl;
						tempReal.clear();
						state = STARTING;
					}
					break;
			}
			}while(state == END_IDENTIFIER || state== END_INTEGER || state==END_REAL); 
		}
      output.close();
	  delete[] token;
	  system("pause");
}
//==============================================================

//====== getFileSize() =========================================
int getFileSize(char * INPUT_FILE)
{
	int fileSize;
	fstream file(INPUT_FILE,ios::in|ios::out);
	file.seekg (0, ios::end);
	fileSize = file.tellg();
	file.seekg(ios::beg);
	return fileSize;
}

//OUTPUT


/////////////////////////////////////////////
// CASE 1 - 11 lexemes
/////////////////////////////////////////////
/*
while (fahr < upper)  do a := 23.00;

Token            Lexeme

Keyword            while
Separator            (
Identifier          fahr
Operator             <
Identifier         upper
Separator            )
Keyword               do
Identifier             a
Operator             :=
Real               23.00
Separator            ;
Press any key to continue . . .
*/


////////////////////////////////////////////
// CASE 2 - 17 lexemes
/////////////////////////////////////////////
/*
function convert (fahr)
begin
           return 5.5*(fahr-32.1)/9;
end   

Token            Lexeme

Keyword         function
Identifier       convert
Separator            (
Identifier          fahr
Separator            )
Keyword            begin
Keyword           return
Real                 5.5
Operator             *
Separator            (
Identifier          fahr
Operator             -
Real                32.1
Separator            )
Operator             /
Integer                9
Separator            ;
Press any key to continue . . .
*/

////////////////////////////////////////////
// CASE 3 - 30 lexemes 
/////////////////////////////////////////////
/*
do{
if(fahr < upper)
{
        do a := 23.00;
}
else
{
        read(upper);
}
}while(foo < fahr);         

Token            Lexeme

Keyword               do
Separator            {
Keyword               if
Separator            (
Identifier          fahr
Operator             <
Identifier         upper
Separator            )
Separator            {
Keyword               do
Identifier             a
Operator             :=
Real               23.00
Separator            ;
Separator            }
Keyword             else
Separator            {
Keyword             read
Separator            (
Identifier         upper
Separator            )
Separator            ;
Separator            }
Separator            }
Keyword            while
Separator            (
Identifier           foo
Operator             <
Identifier          fahr
Separator            )
Separator            ;
Press any key to continue . . .
*/
