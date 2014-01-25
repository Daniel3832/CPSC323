//========================================
//Name: Daniel Jordan
//Email: daniel_jordan@csu.fullerton.edu
//Class: CPSC 323
//Assignment: Project 1: Lexical analysis 
//========================================
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
						
