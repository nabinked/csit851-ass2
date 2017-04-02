/***********************************************************************************
 * CSCI251/851 - Assignment 2
 * ass2.cpp - Contains function definitions for phone database program
 * Nabin, nt433 and the date last modified here.
 *
 ***********************************************************************************/

#include <algorithm>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include <string>
#include <iomanip>
#include <sstream>
#include "ass2.h"
using namespace std;

// ============== Constants ==========================================

const char cDataFileName[] = "phone.txt";
const char cInputFileName[] = "input.txt";
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
string GetPhoneNo();
bool ValidPhoneNum(string &str);

string GetName(string type);
bool ValidName(string &name);

int GetStreetNo();
bool ValidStreetNo(int &streetNo);

string GetLocation(string type);
bool ValidLocation(string &stName);

int GetPostCode();
bool ValidPostCode(int &pc);

void WriteFile(PhoneRecord *rec);
void WriteRecord(PhoneRecord *rec, ofstream &fout);
void WriteFile();

int SearchRecord();
//Utilities
bool ShowNext();//Prompt user for show next records
void MakeUpper(string &str); // Takes a string input and turns it into upper case
void MakeFirstLetterUpper(string &str);
void ClearCinForInvalidInput(istream &cin, string inputPrompt);
void ClearInput(istream & cin);
bool IsAllAlpha(string &str);
int IntLength(int in);





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
	cout << "\nThere are " << gNumRecs << " records in the Phone database\n" << endl;;
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
		strncpy(newRec->PhoneNo, GetPhoneNo().c_str(), cMaxPChars);
		strncpy(newRec->GivenName, GetName("Given").c_str(), cMaxNChars);
		strncpy(newRec->FamilyName, GetName("Family").c_str(), cMaxNChars);
		newRec->StreetNo = GetStreetNo();
		strncpy(newRec->StreetName, GetLocation("Street Name").c_str(), cMaxLChars);
		strncpy(newRec->Suburb, GetLocation("Suburb Name").c_str(), cMaxLChars);
		newRec->PostCode = GetPostCode();

		gPhoneRecs[i] = newRec;
		gNumRecs++;
		WriteFile(newRec);
		cout << "A new record has been added to the database." << endl;
		cout << "...there are " << gNumRecs << " records in the database." << endl;

	}
	else {
		cout << "Cannot add more records. Db Size full." << endl;
	}
}

void SearchRecords()
{// Searches database array for phone number 

	int i = SearchRecord();
	if (i > -1) {
		DisplayRecord(i);
	}
	else {
		cout << "Record not found." << endl;
	}
}

void CleanUp()
{
	for (size_t i = 0; i < gNumRecs; i++)
	{
		delete gPhoneRecs[i];
	}
	cout << "\n\t*** Thanks for using the Phone DB ***\n";
}
void EraseRecord()
{
	int i = SearchRecord();
	if (i > -1) {
		string studentNum = gPhoneRecs[i]->PhoneNo;
		for (; i < gNumRecs; ++i)
		{
			gPhoneRecs[i] = gPhoneRecs[i + 1]; // copy next element left
		}
		delete gPhoneRecs[gNumRecs];
		gNumRecs--;

		//Write new records to db file.
		WriteFile();

		cout << studentNum << " has been erased from the database." << endl;
		cout << "...there are " << gNumRecs << " records in the database." << endl;
	}
	else {
		cout << "Record not found." << endl;
	}

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

string GetPhoneNo()
{
	string input;
	cout << "Enter Phone Number: ";
	while (!(cin >> input)) {
		ClearCinForInvalidInput(cin, "Phone Number");
	};

	if (ValidPhoneNum(input)) {
		ClearInput(cin);
		return input;
	}
	else {
		return GetPhoneNo();
	}
}

bool ValidPhoneNum(string &phoneNum)
{
	//Check for length
	if (phoneNum.length() != 8) {
		cout << "Phone number should be 8 digits long." << endl;
		return false;
	}
	//Check all numeric
	for (size_t i = 0; i < phoneNum.length(); i++)
	{
		if (!isdigit(phoneNum[i])) {
			cout << "Phone number should be all digits." << endl;
			return false;
		}
	}
	return true;
}

//Get Name from cin
string GetName(string type) {
	cout << "Enter " << type + " Name :";
	string input;
	while (!(cin >> input)) {
		ClearCinForInvalidInput(cin, type + " Name");
	};
	if (ValidName(input)) {
		MakeFirstLetterUpper(input);
		ClearInput(cin);
		return input;
	}
	else {
		return GetName(type);
	}
}

//Validate name
bool ValidName(string &name) {
	if (name.length() < 1 || name.length() > cMaxNChars) {
		cout << "Name length is not valid." << endl;
		return false;
	}
	if (!IsAllAlpha(name)) {
		cout << "Name should be all alphabet." << endl;
		return false;
	}
	return true;
}

int GetStreetNo()
{
	int input;
	cout << "Enter Street No: ";
	while (!(cin >> input)) {
		ClearCinForInvalidInput(cin, "Street No");
	};
	if (ValidStreetNo(input)) {
		ClearInput(cin);
		return input;
	}
	else {
		return GetStreetNo();
	}

}

bool ValidStreetNo(int &streetNumber)
{
	if (streetNumber < 1) {
		cout << "Street No cannot be less than 1" << endl;
		return false;
	}
	return true;
}

string GetLocation(string type) {
	string input;
	cout << "Enter " + type + ": ";
	getline(cin, input);
	if (ValidLocation(input)) {
		MakeFirstLetterUpper(input);
		return input;
	}
	else {
		return GetLocation(type);
	}
}

bool ValidLocation(string &location) {
	if (location.length() < 1 || location.length() > cMaxLChars) {
		cout << "location length not valid. Either too short or too long" << endl;
		return false;
	}
	if (!IsAllAlpha(location)) {
		cout << "location should only have alphabets." << endl;
		return false;
	}
	return true;
}

int GetPostCode()
{
	int input;
	cout << "Enter PostCode: ";
	while (!(cin >> input))
	{
		ClearCinForInvalidInput(cin, "PostCode");
	}
	if (ValidPostCode(input)) {
		ClearInput(cin);
		return input;
	}
	else {
		return GetPostCode();
	}
}

bool ValidPostCode(int &pc) {
	if (pc < 1000) {
		cout << "Post code is not valid. Must be greater than or equal to 1000." << endl;;
		return false;
	}
	if (IntLength(pc) != 4) {
		cout << "Post code must be 4 digits long" << endl;
		return false;
	}
	return true;
}


// Writes student record array to text data file
void WriteFile(PhoneRecord *rec)
{
	ofstream fout;
	fout.open(cDataFileName, ios::app);
	if (fout.fail()) {
		cout << "Could not open output file!\n" << endl;
		exit(1);
	}
	WriteRecord(rec, fout);
	fout.close();
}

//Write a record to filestream
void WriteRecord(PhoneRecord *rec, ofstream &fout)
{
	fout << rec->PhoneNo << endl;
	fout << rec->GivenName << endl;
	fout << rec->FamilyName << endl;
	fout << rec->StreetNo << endl;
	fout << rec->StreetName << endl;
	fout << rec->Suburb << endl;
	fout << rec->PostCode << endl;
}

void WriteFile()
{
	ofstream fout;
	fout.open(cDataFileName);
	if (fout.fail()) {
		cout << "Could not open output file!\n";
		exit(1);
	}
	for (int i = 0; i < gNumRecs; i++) {
		WriteRecord(gPhoneRecs[i], fout);
	}
	fout.close();
}

int SearchRecord() {
	string input = GetPhoneNo();

	for (size_t i = 0; i < gNumRecs; i++)
	{
		string str(gPhoneRecs[i]->PhoneNo);
		if (input == str) {
			return i;
		}
	}
	return -1;
}

//***************************Utilities*********************************//

//Valid Input from cin

void ClearCinForInvalidInput(istream &cin, string inputPrompt) {
	ClearInput(cin);
	cout << "Invalid input.Try again." << endl;
	cout << "Enter " + inputPrompt << ": ";
}

void ClearInput(istream &cin) {
	cin.clear();
	cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

//Make first letter uppercase
void MakeFirstLetterUpper(string &str)
{
	str[0] = toupper(str[0]);
	for (size_t i = 1; i < str.length(); ++i)
		str[i] = tolower(str[i]);
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

//Check all alpha
bool IsAllAlpha(string &str) {

	for (size_t i = 0; i < str.length(); i++)
	{
		if (str[i] != ' ') {
			if (!isalpha(str[i])) {
				return false;
			}
		}
	}
	return true;
}

int IntLength(int in) {
	int length = 1;
	while (in /= 10)
	{
		length++;
	}
	return length;
}
