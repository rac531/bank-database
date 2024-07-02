/*
Name: Ricardo Cardenas, 5007471477, CS302-1004, ASSIGNMENT 6
Description: The program reads the contents of a file and stores them
using a custom hashMap. The program then prompts the user multiple 
choices then outputs different data from the hashMap depending on what
the user requests.
Input: User input, data.csv file.
Output: Requested data
*/

#include "hashMap.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>

using namespace std;

struct bankType
{
	struct transactionType
	{
		transactionType(bool t, double a)
		{
			type = t;
			amount = a;
		}

		bool type;
		double amount;
	};

	bankType(string first, string last, string acctNo,
		double balance,unsigned int pin)
	{
		name = last + ", " + first;
		this->balance = balance;
		this->pin = pin;
		this->acctNo = acctNo;
		locked = false;
	}

	bankType()
	{
		name = "";
		balance = 0;
		pin = 0;
		locked = false;
	}

	string name;
	string acctNo;
	double balance;
	unsigned int pin;
	bool locked;

	vector<transactionType> transactions;
};

string lowercaseString(std::string word);

int main()
{
	cout << fixed << setprecision(2);
	hashMap< string, hashMap< string, bankType > > bank;
	hashMap< string, int> locations;
    
	// read the data file and stores in hashmap
	ifstream in;
	string line;
	in.open("data.csv");
	getline(in, line);
	while(in.peek() != EOF){
		getline(in, line);
		string firstName = lowercaseString(line.substr(0, line.find(",")));
		line.erase(0, line.find(",") + 1);
		string lastName = lowercaseString(line.substr(0, line.find(",")));
		line.erase(0, line.find(",") + 1);
		string location = lowercaseString(line.substr(0, line.find(",")));
		line.erase(0, line.find(",") + 1);
		string acctNum = line.substr(0, line.find(","));
		line.erase(0, line.find(",") + 1);
		double balance = stod(line.substr(0, line.find(",")));
		line.erase(0, line.find(",") + 1);
		unsigned int pin = stoul(line);
		bank[location][acctNum.substr(acctNum.size() - 4, acctNum.size() - 1)] = bankType(firstName, lastName, acctNum, balance, pin);
		locations[location] = 1;
	}
	in.close();

	char userChar = 'y';
	string uBranch, uAcctnum, uPin, uInput;
	while(tolower(userChar) != 'n'){
		cout << "Please enter bank branch location: ";
		cin >> uBranch;
        if(cin.peek() != '\n'){
            string uBranch2;
            cin >> uBranch2;
            uBranch = uBranch + ' ' + uBranch2;
        }
		uBranch = lowercaseString(uBranch);

		if(locations[uBranch] == 1){
			cout << endl << "Enter last 4 digits of account: ";
			cin >> uAcctnum;
			if(bank[uBranch][uAcctnum].name != ""){
				if(bank[uBranch][uAcctnum].locked == false){
					int lockCount = 0;
					while(lockCount < 3){
						cout << endl << "Enter a pin: ";
						cin >> uPin;
						if(bank[uBranch][uAcctnum].pin == stoul(uPin))
							lockCount = 7;
						else{
							cout << endl << "Invalid pin" << endl;
							lockCount++;
						}
					}
					if(lockCount == 7){
					while(tolower(userChar) != 'e'){
						cout << endl << "(D)eposit" << endl
								  << "(W)ithdrawal" << endl
								  << "(V)iew account" << endl
								  << "(E)xit account" << endl
								  << "Enter choice: ";
						cin >> userChar;
						if(tolower(userChar) == 'd'){
							cout << endl << "Enter deposit amount: ";
							cin >> uInput;
							bank[uBranch][uAcctnum].balance += stod(uInput);
							bank[uBranch][uAcctnum].transactions.push_back(bankType::transactionType(true, stod(uInput)));
						}
						else if(tolower(userChar) == 'w'){
							cout << endl << "Enter withdrawal amount: ";
							cin >> uInput;
							if(stod(uInput) > bank[uBranch][uAcctnum].balance)
								cout << endl << "Not enough funds to make transaction" << endl;
							else{
								bank[uBranch][uAcctnum].balance -= stod(uInput);
								bank[uBranch][uAcctnum].transactions.push_back(bankType::transactionType(false, stod(uInput)));
							}
						}
						else if(tolower(userChar) == 'v'){
							cout << endl << "Name: " << bank[uBranch][uAcctnum].name << endl
									  << "Balance: $" << bank[uBranch][uAcctnum].balance << endl;
							if(bank[uBranch][uAcctnum].transactions.empty())
								cout << endl << "No transactions" << endl;
							else{
								cout << endl << "List of transactions" << endl;
								for(int i = 0; i < bank[uBranch][uAcctnum].transactions.size(); i++){
									if(bank[uBranch][uAcctnum].transactions[i].type == true)
										cout << endl << "Deposit amount $" << bank[uBranch][uAcctnum].transactions[i].amount << endl;
									else
										cout << endl << "Withdrawal amount $" << bank[uBranch][uAcctnum].transactions[i].amount << endl;
								}
							}
						}
						else if(tolower(userChar) == 'e')
							userChar = 'e';	// basically do nothing
						else
							cout << endl << "Invalid choice" << endl;
					}
					}
					else{
						cout << endl << "3 failed login attempts, account is now locked" << endl;
						bank[uBranch][uAcctnum].locked = true;
					}
				}
				else
					cout << endl << "Account has a lock" << endl;
			}
			else 
				cout << endl << "Account not found" << endl;
		}
		else
			cout << endl << "Bank branch not found " << endl;

		cout << "Continue? (Y/N): ";
		cin >> userChar;
        cout << endl;
	}

	return 0;
}


/*
 * function identifier: takes in string and converts it to lowercase
 * parameters: string word
 * return value: string lowercase word
 */
std::string lowercaseString(std::string word){
    for(int i = 0; i < word.length(); i++){
        word[i] = tolower(word[i]);
    }
    return word;
}