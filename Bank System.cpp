#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <iomanip>
#include <cstdlib>

using namespace std;

void ShowMainMenue();
void ShowTransactionsMeune();

const string ClientsFileName = "Clients.txt";

struct stClient
{
	string AccountNumber = "";
	string PinCode = "";
	string Name = "";
	string Phone = "";
	float AccountBalance = 0;
	bool MarkForDelete = false;
};

vector <string> SplitString(string S1, string Delim)
{
	vector <string> vString;

	string sWord = "";
	int pos = 0;

	while ((pos = S1.find(Delim)) != string::npos)
	{
		sWord = S1.substr(0, pos);

		if (sWord != "")
		{
			vString.push_back(sWord);
		}

		S1.erase(0, pos + Delim.length());
	}

	if (S1 != "")
	{
		vString.push_back(S1);
	}

	return vString;
}

stClient ConvertLineRecord(string Line, string Seperator = "#//#")
{
	stClient Client;
	vector <string> vClientData;

	vClientData = SplitString(Line, Seperator);

	Client.AccountNumber = vClientData[0];
	Client.PinCode = vClientData[1];
	Client.Name = vClientData[2];
	Client.Phone = vClientData[3];
	Client.AccountBalance = stof(vClientData[4]);

	return Client;
}

string ConvertRecordToLine(stClient Client, string Seperator = "#//#")
{
	string stClientRecord = "";

	stClientRecord += Client.AccountNumber + Seperator;
	stClientRecord += Client.PinCode + Seperator;
	stClientRecord += Client.Name + Seperator;
	stClientRecord += Client.Phone + Seperator;
	stClientRecord += to_string(Client.AccountBalance);

	return stClientRecord;
}

vector <stClient> LoadCleintsDataFromFile(string FileName)
{
	vector <stClient> vClients;

	fstream MyFile;

	MyFile.open(FileName, ios::in);

	if (MyFile.is_open())
	{
		string Line;
		stClient Client;

		while (getline(MyFile, Line))
		{
			Client = ConvertLineRecord(Line);

			vClients.push_back(Client);
		}

		MyFile.close();
	}

	return vClients;
}

void AddDataLineToFile(string FileName, string stDataLine)
{
	fstream MyFile;

	MyFile.open(FileName, ios::out | ios::app);

	if (MyFile.is_open())
	{
		MyFile << stDataLine << endl;

		MyFile.close();
	}
}

vector <stClient> SaveCleintsDataToFile(string FileName, vector <stClient> vClients)
{
	fstream MyFile;

	MyFile.open(FileName, ios::out);

	if (MyFile.is_open())
	{
		string ClientLine;

		for (stClient& C : vClients)
		{
			if (C.MarkForDelete == false)
			{
				ClientLine = ConvertRecordToLine(C);

				MyFile << ClientLine << endl;
			}
		}

		MyFile.close();
	}

	return vClients;
}

vector <stClient> SaveCleintsDataToFile(string FileName, vector <stClient> vClients, string AccountNumber, stClient Client)
{
	fstream MyFile;

	MyFile.open(FileName, ios::out);

	if (MyFile.is_open())
	{
		string ClientLine;

		for (stClient& C : vClients)
		{
			ClientLine = ConvertRecordToLine(C);

			MyFile << ClientLine << endl;
		}

		MyFile.close();
	}

	return vClients;
}

void PrintClientCard(stClient Client)
{
	cout << "\nThe following are the client details:\n";
	cout << "-----------------------------------\n";
	cout << "Account Number : " << Client.AccountNumber << endl;
	cout << "Pin Code       : " << Client.PinCode << endl;
	cout << "Name           : " << Client.Name << endl;
	cout << "Phone          : " << Client.Phone << endl;
	cout << "Account Balance: " << Client.AccountBalance << endl;
	cout << "-----------------------------------\n";
}

void PrintClientRecord(stClient Client)
{
	cout << "| " << left << setw(15) << Client.AccountNumber;
	cout << "| " << left << setw(10) << Client.PinCode;
	cout << "| " << left << setw(40) << Client.Name;
	cout << "| " << left << setw(12) << Client.Phone;
	cout << "| " << left << setw(12) << Client.AccountBalance;
}

bool ClientExistsByAccountNumber(string AccountNumber, string FileName)
{
	vector <stClient> vClients;
	fstream MyFile;

	MyFile.open(FileName, ios::in);

	if (MyFile.is_open())
	{
		string Line;
		stClient Client;

		while (getline(MyFile, Line))
		{
			Client = ConvertLineRecord(Line);

			if (Client.AccountNumber == AccountNumber)
			{
				MyFile.close();

				return true;
			}

			vClients.push_back(Client);
		}

		MyFile.close();
	}



	return false;
}

stClient ReadNewClient()
{
	stClient Client;

	cout << "Enter Account Number? ";
	getline(cin >> ws, Client.AccountNumber);

	while (ClientExistsByAccountNumber(Client.AccountNumber, ClientsFileName))
	{
		cout << "Client white [" << Client.AccountNumber << "] already exists, Enter another Account Number? ";
		getline(cin >> ws, Client.AccountNumber);
	}

	cout << "Enter Pin Code? ";
	getline(cin, Client.PinCode);

	cout << "Enter Name? ";
	getline(cin, Client.Name);

	cout << "Enter Phone? ";
	getline(cin, Client.Phone);

	cout << "Enter Account Balance? ";
	cin >> Client.AccountBalance;

	return Client;
}

void AddNewClient()
{
	stClient Client = ReadNewClient();
	AddDataLineToFile(ClientsFileName, ConvertRecordToLine(Client));
}

void AddClients()
{
	char AddMore = 'Y';

	do
	{
		cout << "\nAdding New Client:\n\n";
		AddNewClient();

		cout << "\nClient Added Successfully, do you want to add more clients? Y/N? ";
		cin >> AddMore;

	} while (toupper(AddMore) == 'Y');
}

string ReadClientAccountNumber()
{
	string AccountNumber = "";

	cout << "\nPlease Enter Account Number? ";
	getline(cin >> ws, AccountNumber);

	return AccountNumber;
}

bool FindClientByAccountNumber(string AccountNumber, vector <stClient> vClients, stClient& Client)
{
	for (stClient C : vClients)
	{
		if (C.AccountNumber == AccountNumber)
		{
			Client = C;

			return true;
		}
	}

	return false;
}

bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector <stClient>& vClients)
{
	for (stClient& C : vClients)
	{
		if (C.AccountNumber == AccountNumber)
		{
			C.MarkForDelete = true;

			return true;
		}
	}

	return false;
}

bool DeleteClientByAccountNumber(string AccountNumber, vector <stClient>& vClients)
{
	stClient Client;
	char Answer = 'n';

	if (FindClientByAccountNumber(AccountNumber, vClients, Client))
	{
		PrintClientCard(Client);

		cout << "\n\nAre you sure you want delete this client? Y/N ? ";
		cin >> Answer;

		if (toupper(Answer) == 'Y')
		{
			MarkClientForDeleteByAccountNumber(AccountNumber, vClients);
			SaveCleintsDataToFile(ClientsFileName, vClients);

			vClients = LoadCleintsDataFromFile(ClientsFileName);

			cout << "\n\nClient Deleted Seuccessfully.\n";
		}

		return true;
	}
	else
	{
		cout << "\nClient with Account Number (" << AccountNumber << ") Not Found!\n";

		return false;
	}
}

stClient ChangeClientRecord(string AccountNumber)
{
	stClient Client;

	Client.AccountNumber = AccountNumber;

	cout << "\n\nEnter Pin Code? ";
	getline(cin >> ws, Client.PinCode);

	cout << "Enter Name? ";
	getline(cin, Client.Name);

	cout << "Enter Phone? ";
	getline(cin, Client.Phone);

	cout << "Enter Account Balance? ";
	cin >> Client.AccountBalance;

	return Client;
}

bool UpdateClientByAccountNumber(string AccountNumber, vector <stClient>& vClients)
{
	stClient Client;
	char Answer = 'n';

	if (FindClientByAccountNumber(AccountNumber, vClients, Client))
	{
		PrintClientCard(Client);

		cout << "\n\nAre you sure you want update this client? Y/N ? ";
		cin >> Answer;

		if (toupper(Answer) == 'Y')
		{
			for (stClient& C : vClients)
			{
				if (C.AccountNumber == AccountNumber)
				{
					C = ChangeClientRecord(AccountNumber);
					break;
				}
			}

			SaveCleintsDataToFile(ClientsFileName, vClients, AccountNumber, Client);

			cout << "\n\nClient Updated Seuccessfully.\n";
		}

		return true;
	}
	else
	{
		cout << "\nClient with Account Number (" << AccountNumber << ") Not Found!\n";

		return false;
	}
}

bool DepositBalanceToClientByAccountNumber(string AccountNumber, double Amount, vector <stClient>& vClients)
{
	char Answer = 'n';

	cout << "\n\nAre you sure you want pafrom this transaction? Y/N ? ";
	cin >> Answer;

	if (toupper(Answer) == 'Y')
	{
		for (stClient& C : vClients)
		{
			if (C.AccountNumber == AccountNumber)
			{
				C.AccountBalance += Amount;

				SaveCleintsDataToFile(ClientsFileName, vClients);

				cout << "\n\nDone Seuccessfully. New balance is: " << C.AccountBalance;

				return true;
			}
		}
	}

	return false;

}

void PrintClientRecordBalanceLine(stClient Client)
{
	cout << "| " << left << setw(15) << Client.AccountNumber;
	cout << "| " << left << setw(40) << Client.Name;
	cout << "| " << left << setw(12) << Client.AccountBalance;
}

void ShowAllClientsScreen()
{
	vector <stClient> vClients = LoadCleintsDataFromFile(ClientsFileName);

	cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client (s).";
	cout << "\n________________________________________________________________________________________________\n" << endl;
	cout << "| " << left << setw(15) << "Account Number";
	cout << "| " << left << setw(10) << "Pin Code";
	cout << "| " << left << setw(40) << "Client Name";
	cout << "| " << left << setw(12) << "Phone";
	cout << "| " << left << setw(12) << "Balance";
	cout << "\n________________________________________________________________________________________________\n" << endl;

	for (stClient& Client : vClients)
	{
		PrintClientRecord(Client);

		cout << endl;
	}

	cout << "________________________________________________________________________________________________\n" << endl;
}

void ShowAddNewClientScreen()
{
	cout << "\n-----------------------------------\n";
	cout << "\tAdd New Clients Screen\n";
	cout << "-----------------------------------\n";

	AddClients();
}

void ShowDeleteClientScreen()
{
	cout << "\n-----------------------------------\n";
	cout << "\tDelete Client Screen\n";
	cout << "-----------------------------------\n";

	vector <stClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
	string AccountNumber = ReadClientAccountNumber();

	DeleteClientByAccountNumber(AccountNumber, vClients);
}

void ShowUpdateClientInfoScreen()
{
	cout << "\n-----------------------------------\n";
	cout << "     Update Client Info Screen\n";
	cout << "-----------------------------------\n";

	vector <stClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
	string AccountNumber = ReadClientAccountNumber();

	UpdateClientByAccountNumber(AccountNumber, vClients);
}

void ShowFindClientScreen()
{
	cout << "\n-----------------------------------\n";
	cout << "\tFind Client Screen\n";
	cout << "-----------------------------------\n";

	vector <stClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
	stClient Client;
	string AccountNumber = ReadClientAccountNumber();

	if (FindClientByAccountNumber(AccountNumber, vClients, Client))
	{
		PrintClientCard(Client);
	}
	else
	{
		cout << "\nClient with Account Number[" << AccountNumber << "] is not found!\n";
	}
}

void ShowEndScreen()
{
	cout << "\n-----------------------------------\n";
	cout << "\tProgram Ends :-)\n";
	cout << "-----------------------------------\n";
}

void ShowDepositScreen()
{
	cout << "\n-----------------------------------\n";
	cout << "\tDeposit Screen\n";
	cout << "-----------------------------------\n";

	stClient Client;

	vector <stClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
	string AccountNumber = ReadClientAccountNumber();

	while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
	{
		cout << "\nClient with [" << AccountNumber << "] dose not exist.\n";

		AccountNumber = ReadClientAccountNumber();
	}

	PrintClientCard(Client);

	double Amount = 0;

	cout << "\nPlease enter deposit amount? ";
	cin >> Amount;

	DepositBalanceToClientByAccountNumber(AccountNumber, Amount, vClients);
}

void ShowWithDrawScreen()
{
	cout << "\n-----------------------------------\n";
	cout << "\tWithdraw Screen\n";
	cout << "-----------------------------------\n";

	stClient Client;

	vector <stClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
	string AccountNumber = ReadClientAccountNumber();

	while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
	{
		cout << "\nClient with [" << AccountNumber << "] dose not exist.\n";

		AccountNumber = ReadClientAccountNumber();
	}

	PrintClientCard(Client);

	double Amount = 0;

	cout << "\nPlease enter whitdraw amount? ";
	cin >> Amount;

	while (Amount > Client.AccountBalance)
	{
		cout << "\nAmount Exceeds the balance, you can  withdraw up to : " << Client.AccountBalance << endl;

		cout << "Please enter another amount? ";
		cin >> Amount;
	}

	DepositBalanceToClientByAccountNumber(AccountNumber, Amount * -1, vClients);
}

void ShowTotalBalances()
{
	vector <stClient> vClients = LoadCleintsDataFromFile(ClientsFileName);

	cout << "\n\t\t\t\t\tBalance List (" << vClients.size() << ") Client(s).";
	cout << "\n________________________________________________________________________________________________\n" << endl;
	cout << "| " << left << setw(15) << "Account Number";
	cout << "| " << left << setw(40) << "Client Name";
	cout << "| " << left << setw(12) << "Balance";
	cout << "\n________________________________________________________________________________________________\n" << endl;

	double TotalBalance = 0;

	if (vClients.size() == 0)
	{
		cout << "\t\t\t\tNo Clients Available In the System!\n" << endl;
	}
	else
	{
		for (stClient& Client : vClients)
		{
			PrintClientRecordBalanceLine(Client);
			TotalBalance += Client.AccountBalance;

			cout << endl;
		}
	}

	cout << "________________________________________________________________________________________________\n" << endl;

	cout << "\t\t\t\t\t   Total Balance = " << TotalBalance;
}

void ShowTotalBalancesScreen()
{
	ShowTotalBalances();
}

void GoBackToTransactionsMeune()
{
	cout << "\n\nPress any key to go back to Transactions Menue...";

	system("pause>0");

	ShowTransactionsMeune();
}

enum enTransactionsMunueOptions
{
	eDeposit = 1, eWithdraw = 2, eTotalBalance = 3, eShowMainMenue = 4
};

void PerfromTransactionsMunueOptions(enTransactionsMunueOptions TransactionsMunueOptions)
{
	switch (TransactionsMunueOptions)
	{
	case enTransactionsMunueOptions::eDeposit:
		system("cls");
		ShowDepositScreen();
		GoBackToTransactionsMeune();
		break;

	case enTransactionsMunueOptions::eWithdraw:
		system("cls");
		ShowWithDrawScreen();
		GoBackToTransactionsMeune();
		break;

	case enTransactionsMunueOptions::eTotalBalance:
		system("cls");
		ShowTotalBalancesScreen();
		GoBackToTransactionsMeune();
		break;

	case enTransactionsMunueOptions::eShowMainMenue:
		ShowMainMenue();
		break;
	}
}

short ReadTransactionsMunueOption()
{
	short Choice = 0;

	cout << "Choose what do you want to do? [1 to 4]? ";
	cin >> Choice;

	return Choice;
}

void ShowTransactionsMeune()
{
	system("cls");

	cout << "===========================================\n";
	cout << "\tTransactions Menue Screen\n";
	cout << "===========================================\n";
	cout << "\t[1] Deposit.\n";
	cout << "\t[2] Withdraw.\n";
	cout << "\t[3] Total Balance.\n";
	cout << "\t[4] Main Menue.\n";
	cout << "===========================================\n";

	PerfromTransactionsMunueOptions(enTransactionsMunueOptions(ReadTransactionsMunueOption()));
}

void GoBackToMainMeune()
{
	cout << "\n\nPress any key to go back to Main Menue...";

	system("pause>0");

	ShowMainMenue();
}

enum enMainMunueOptions
{
	eClientsList = 1, eAddNewClients = 2, eDeleteClient = 3, eUpdateClient = 4, eFindClient = 5, eShowTransactionsMenue = 6, eExit = 7
};

void PerfromMainMunueOptions(enMainMunueOptions MainMunueOption)
{
	switch (MainMunueOption)
	{
	case enMainMunueOptions::eClientsList:
		system("cls");
		ShowAllClientsScreen();
		GoBackToMainMeune();
		break;

	case enMainMunueOptions::eAddNewClients:
		system("cls");
		ShowAddNewClientScreen();
		GoBackToMainMeune();
		break;

	case enMainMunueOptions::eDeleteClient:
		system("cls");
		ShowDeleteClientScreen();
		GoBackToMainMeune();
		break;

	case enMainMunueOptions::eUpdateClient:
		system("cls");
		ShowUpdateClientInfoScreen();
		GoBackToMainMeune();
		break;

	case enMainMunueOptions::eFindClient:
		system("cls");
		ShowFindClientScreen();
		GoBackToMainMeune();
		break;

	case enMainMunueOptions::eShowTransactionsMenue:
		system("cls");
		ShowTransactionsMeune();
		break;

	case enMainMunueOptions::eExit:
		system("cls");
		ShowEndScreen();
		break;
	}
}

short ReadMainMunueOption()
{
	short Choice = 0;

	cout << "Choose what do you want to do? [1 to 7]? ";
	cin >> Choice;

	return Choice;
}

void ShowMainMenue()
{
	system("cls");

	cout << "===========================================\n";
	cout << "\t\tMain Menue Screen\n";
	cout << "===========================================\n";
	cout << "\t[1] Show Client List.\n";
	cout << "\t[2] Add New Client.\n";
	cout << "\t[3] Delete Client.\n";
	cout << "\t[4] Update Client Info.\n";
	cout << "\t[5] Find Client.\n";
	cout << "\t[6] Transactions.\n";
	cout << "\t[7] Exit.\n";
	cout << "===========================================\n";

	PerfromMainMunueOptions(enMainMunueOptions(ReadMainMunueOption()));
}

int main()
{
	ShowMainMenue();

	system("pause>0");

	return 0;
}