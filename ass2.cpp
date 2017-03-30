/***********************************************************************************
 * CSCI251/851 - Assignment 2
 * ass2.cpp - Contains function definitions for phone database program
 * Nabin, nt433 and the date last modified here.
 *
 ***********************************************************************************/

#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <algorithm>
using namespace std;

// ============== Constants ==========================================

const char cDataFileName[] = "phone.txt";
const int cMaxRecs = 250;  // Max records in database
const int cMaxNChars = 20; // Max chars in name
const int cMaxLChars = 30; // Max chars in location
const int cMaxPChars = 8;  // Max chars in phone no


// ============= User Defined types ==================================

struct PhoneRecord {
	char PhoneNo[cMaxPChars + 1];
	char FamilyName[cMaxNChars + 1];
	char GivenName[cMaxNChars + 1];
	int StreetNo;
	char StreetName[cMaxLChars + 1];
	char Suburb[cMaxLChars + 1];
	int PostCode;
};


// ============= Global Data =========================================

PhoneRecord *gPhoneRecs[cMaxRecs];
int gNumRecs = 0;


// ============= Private Function Prototypes =========================

void DisplayRecord(int i); // Displays record i on screen
bool ShowNext();//Prompt user for show next records
void MakeUpper(string &str); // Takes a string input and turns it into upper case
//char* GetPhoneNo();


// ============= Public Function Definitions =========================

void ReadFile()
{
	//Load Phone records from text datafile into array
	ifstream fin;
	fin.open(cDataFileName);
	if (!fin.good())
	{
		cerr << "Could not open data file!\n";
		exit(1);
	}
	gNumRecs = 0;
	int i;
	for (i = 0; i < cMaxRecs; i++)
	{
		gPhoneRecs[i] = new PhoneRecord();
		fin >> gPhoneRecs[i]->PhoneNo;
		if (fin.fail()) break; // then eof
		fin >> gPhoneRecs[i]->FamilyName;
		fin >> gPhoneRecs[i]->GivenName;
		fin >> gPhoneRecs[i]->StreetNo;
		fin.ignore();
		fin.getline(gPhoneRecs[i]->StreetName, cMaxLChars + 1);
		fin.getline(gPhoneRecs[i]->Suburb, cMaxLChars + 1);
		fin >> gPhoneRecs[i]->PostCode;
	}
	gNumRecs = i;
	fin.close();
	cout << "\nThere are " << gNumRecs << " records in the Phone database\n";
}

void DisplayRecords()
{
	// Displays records one at a time
	for (int i = 0; i < gNumRecs; i++)
	{
		DisplayRecord(i);
		if (!ShowNext()) {
			return;
		}
	}
}

void AddRecord()
{
	int i = gNumRecs;
	// Adds a new record to the database array
	if (gNumRecs < cMaxRecs) {
		cout << "Adding a new Record to Database..." << endl;
		PhoneRecord *newRec = new PhoneRecord();
		*newRec->PhoneNo = GetPhoneNo();
	}
	else {
		cout << "Cannot add more records. Db Size full."
	}

	// if DB not full
	//      create new PhoneRecord 
	//      get data from user and assign to new record's fields
	//      add new record to array
	//      increment gNumRecs
	//      append new record to the DB file
	//      print "record added to DB" message
	// else
	//      print "Error: DB Full!" message
}

void SearchRecords()
{// Searches database array for phone number 

	cout << "SearchRecord()- Not yet implemented\n";

	// get phone no. from user
	// search array for record
	// if found 
	//     display record
	// else
	//     print "record not found!" message
}

void CleanUp()
{// Deletes all dynamic data in gPhoneRecs array

	cout << "CleanUp()- Not yet implemented\n";

	cout << "\n\t*** Thanks for using the Phone DB ***\n";
}
// ============= Private Functions Definitions =========================

void DisplayRecord(int i)
{
	// Displays record i on screen
	cout << endl;
	cout << setw(12) << left << gPhoneRecs[i]->PhoneNo;
	cout << gPhoneRecs[i]->GivenName << " " << gPhoneRecs[i]->FamilyName << ",";
	cout << gPhoneRecs[i]->StreetNo << " " << gPhoneRecs[i]->StreetName << ",";
	cout << gPhoneRecs[i]->Suburb << ", " << gPhoneRecs[i]->PostCode << endl;
	cout << endl;
}

//Prompt for show more records.
bool ShowNext()
{
	cout << endl;
	cout << "Display next Record? (y/n): ";
	char Ans;
	cin >> Ans;
	if (toupper(Ans) == 'Y') {
		return true;
	}
	else {
		return false;
	}
}

// Takes a string input and turns it into upper case
void MakeUpper(string &str) {
	transform(str.begin(), str.end(), str.begin(), ::toupper);
}

void SetPhoneNo(char)
{
	char var[10];
	
	cout << "Enter Phone Number";
	cin >> var;
	return var;
}
