/********************************************************
* CSCI251/851 - Assignment 2
* main.cpp - contains main program + menu function.
* Nabin, nt433 and the date last modified here.
*
*********************************************************/
#include <iostream>
#include <cctype>
#include "ass2.h"
using namespace std;

char Menu();

int main()
{
	// Option for redirecting input from input.txt 
	// Use this when you demo your program to your 
	// tutor as explained oin the specs.
	char RedirectInput[20];
	cout << "Redirect input? (y/n): ";
	cin.getline(RedirectInput,20);
	if(RedirectInput[0]=='y'){ // then get input from input.txt...
		FILE *fp = freopen ("input.txt","r",stdin);    
		if(fp==NULL){cerr<<"\"input.txt\" not found!\n"; exit(1);}
	}

	bool Quit = false;
	do
	{
		char Cmd = Menu();
		switch(Cmd)
		{
			case 'r':
				ReadFile();
				break;
			case 'd':
				DisplayRecords();
				break;
			case 'a':
				AddRecord();
				break;
			case 's':
				SearchRecords();
				break;
			case 'q':
				Quit = true;
				break;
			default:
				cerr << "Invalid command!\n";
		}
	} while(!Quit);

	CleanUp();
	if(RedirectInput[0]=='y') fclose (stdin);    // close redirected input from input.txt
	return 0;
}

char Menu()
{
	char Cmd;
	cout << "*********************************"<< endl;
	cout << "*     Phone Records Database    *"<< endl;
	cout << "*     (r)ead data file          *"<< endl;
	cout << "*     (d)isplay records         *"<< endl;
	cout << "*     (a)dd record to DB        *"<< endl;
	cout << "*     (s)earch records          *"<< endl;
	cout << "*     (q)uit                    *"<< endl;
	cout << "*********************************"<< endl;
	cout << "Command: ";
	cin  >> Cmd;
	cin.ignore();  // eat up '\n'
	Cmd = tolower(Cmd);
	return Cmd;
}

