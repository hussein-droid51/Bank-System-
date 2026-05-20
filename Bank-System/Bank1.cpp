#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

const string ClientsFileName = "Clients.txt";

void ShowMainMenu();


enum enMainMenuOption{showClientList=1,AddNewClient=2,DeleteClient=3,
	UpdateClientInfo=4,FindClient=5,Exit=6};

struct sMenu{
	enMainMenuOption ClientList;
	enMainMenuOption NewClient;
	enMainMenuOption DeleteClient;
	enMainMenuOption UpdateClient;
	enMainMenuOption FindClient;
	enMainMenuOption Exit;

};

struct sClient
{
	string AccountNumber;
	string PinCode;
	string Name;
	string Phone;
	double AccountBalance;
	bool MarkForDelete = false;
};

void AddDataLineToFile(string FileName, string stDataLine)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out | ios::app);// Read Mode Or write mode 
	if (MyFile.is_open())
	{
		MyFile << stDataLine << endl;

		MyFile.close();
	}
}



vector<string> SplitString(string S1, string Delim)
{
	vector<string> vString;
	short pos = 0;
	string sWord; 
	while ((pos = S1.find(Delim)) != std::string::npos)
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


sClient ConvertLineToRecord(string Line, string Seperator = "#//#")
{
	sClient Client;
	vector<string> vClientData;
	vClientData = SplitString(Line, Seperator);
	Client.AccountNumber = vClientData[0];
	Client.PinCode = vClientData[1];
	Client.Name = vClientData[2];
	Client.Phone = vClientData[3];
	Client.AccountBalance = stod(vClientData[4]);//cast string to double
	return Client;
}


string ConvertRecordToLine(sClient Client, string Seperator = "#//#")
{
	string stClientRecord = "";
	stClientRecord += Client.AccountNumber + Seperator;
	stClientRecord += Client.PinCode + Seperator;
	stClientRecord += Client.Name + Seperator;
	stClientRecord += Client.Phone + Seperator;
	stClientRecord += to_string(Client.AccountBalance);
	return stClientRecord;
}


vector <sClient> LoadCleintsDataFromFile(string FileName)
{
	vector <sClient> vClients;
	fstream MyFile;
	MyFile.open(FileName, ios::in);//read Mode
	if (MyFile.is_open())
	{
		string Line;
		sClient Client;
		while (getline(MyFile, Line))
		{
			Client = ConvertLineToRecord(Line);
			vClients.push_back(Client);
		}
		MyFile.close();
	}
	return vClients;
}


bool ClientExistsByAccountNumber(string AccountNumber, string FileName)
{
	vector<sClient>Vclient;
	fstream MyFile;
	MyFile.open(FileName, ios::in); //Read mode

	if (MyFile.is_open())
	{
		string Line;
		sClient client;
		while (getline(MyFile, Line))
		{
			client = ConvertLineToRecord(Line);
			if (client.AccountNumber == AccountNumber)
			{
				MyFile.close();
				return true;
			}
			Vclient.push_back(client);
		}
		MyFile.close();
	}
	return false;
}

sClient ReadNewClient()
{
	sClient client;

	cout << "Please Enter Account Number :" << endl;
	getline(cin >> ws, client.AccountNumber);

	while (ClientExistsByAccountNumber(client.AccountNumber, ClientsFileName))
	{
		cout << "Client with ["<<client.AccountNumber <<"] Already exists, Enter Another Account Number  : " << endl;
		getline(cin >> ws, client.AccountNumber);
	}


	cout << "Please Enter Pin Code :" << endl;
	getline(cin, client.PinCode);

	cout << "Please Enter Your Name :" << endl;
	getline(cin, client.Name);

	cout << "Please Enter Phone Number :" << endl;
	cin >> client.Phone;

	cout << "Please Enter Account Balance :" << endl;
	cin >> client.AccountBalance;

	return client;
}


void AddClient()
{
	cout << "======================================" << endl;
	cout << "\t\t Add New Clients Screen             " << endl;
	cout << "======================================" << endl;

	sClient client = ReadNewClient();
	AddDataLineToFile(ClientsFileName, ConvertRecordToLine(client));

}


void PrintClientCard(sClient Client)
{
	cout << "\nThe following are the client details:\n";
	cout << "=============================================\n";
	cout << "\nAccount Number : " << Client.AccountNumber;
	cout << "\nPin Code       : " << Client.PinCode;
	cout << "\nName           : " << Client.Name;
	cout << "\nPhone          : " << Client.Phone;
	cout << "\nAccount Balance: " << Client.AccountBalance;
	cout << "\n=============================================\n";

}

bool FindClientByAccountNumber(string AccountNumber, vector<sClient> vClients, sClient& Client)
{
	
	for (sClient C : vClients)
	{
		if (C.AccountNumber == AccountNumber)
		{
			Client = C;
			return true;
		}
	}
	return false;
}


bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector <sClient>& vClients)
{
	for (sClient& C : vClients)
	{
		if (C.AccountNumber == AccountNumber)
		{
			C.MarkForDelete = true;
			return true;
		}
	}
	return false;
}


vector <sClient> SaveCleintsDataToFile(string FileName, vector<sClient> vClients)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out);//overwrite
	string DataLine;
	if (MyFile.is_open())
	{
		for (sClient C : vClients)
		{
			if (C.MarkForDelete == false)
			{
				//we only write records that are not marked for delete
				DataLine = ConvertRecordToLine(C);
				MyFile << DataLine << endl;
			}
		}
		MyFile.close();
	}
	return vClients;
}


bool DeleteClientByAccountNumber(string AccountNumber, vector<sClient>& vClients)
{
	sClient Client;
	char Answer = 'n';
	if (FindClientByAccountNumber(AccountNumber, vClients,	Client))
	{
		PrintClientCard(Client);
		cout << "\n\nAre you sure you want delete this client? y/n ? ";
		cin >> Answer;
		if (Answer == 'y' || Answer == 'Y')
		{
			MarkClientForDeleteByAccountNumber(AccountNumber,vClients);

			SaveCleintsDataToFile(ClientsFileName, vClients);


			//Refresh Clients
			vClients = LoadCleintsDataFromFile(ClientsFileName);
			cout << "\n\nClient Deleted Successfully.";
			return true;
		}
	}
	else
	{
		cout << "\nClient with Account Number (" << AccountNumber
			<< ") is Not Found!";
		return false;
	}
}

vector<sClient> ReadClientDataFromFile(string FileName)
{
	vector<sClient>vClient;
	fstream MyFile;
	MyFile.open(FileName, ios::in);// Read Mood
	if (MyFile.is_open())
	{
		string line = " ";
		sClient client;

		while (getline(MyFile, line))
		{
			client = ConvertLineToRecord(line);
			vClient.push_back(client);

		}

		MyFile.close();
	}
	return vClient;
}

sClient ChangeClientRecord(string AccountNumber)
{
	sClient client;

	client.AccountNumber = AccountNumber;

	cout << "Enter your Pin Code ? " << endl;
	getline(cin >> ws, client.PinCode);

	cout << "Enter your Name ? " << endl;
	getline(cin, client.Name);

	cout << "Enter your Phone ? " << endl;
	getline(cin, client.Phone);

	cout << "Enter your Account Balance ? " << endl;
	cin >> client.AccountBalance;


	return client;
}


string ReadClientAccountNumber()
{
	string AccountNumber = "";
	cout << "\nPlease enter Account Number? ";
	cin >> AccountNumber;
	return AccountNumber;
}


bool UpdateClientByAccountNumber(string AccountNumber, vector<sClient>& vClients)
{
	sClient Client;
	char Answer = 'n';

	if (FindClientByAccountNumber(AccountNumber, vClients, Client))
	{
		PrintClientCard(Client);
		cout << "\n\nAre you sure you want Update this client? y/n ? ";
		cin >> Answer;
		if (Answer == 'y' || Answer == 'Y')
		{
			for (sClient& C : vClients)
			{
				if (C.AccountNumber == AccountNumber)
				{
					C = ChangeClientRecord(AccountNumber);
					break;
				}
			}


			SaveCleintsDataToFile(ClientsFileName, vClients);
			//Refresh Clients


			vClients = LoadCleintsDataFromFile(ClientsFileName);


			cout << "\n\nClient Updated Successfully.";

			return true;
		}
	}
	else
	{
		cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";

		return false;
	}
}

void PrintClientRecord(sClient Client)
{
	cout << "| " << setw(15) << left << Client.AccountNumber;
	cout << "| " << setw(10) << left << Client.PinCode;
	cout << "| " << setw(20) << left << Client.Name;
	cout << "| " << setw(12) << left << Client.Phone;
	cout << "| " << setw(12) << left << Client.AccountBalance;
}

void PrintAllClientsData(vector <sClient> vClients)
{
	cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ")Client(s)";
	cout << "\n___________________________________________________";
	cout << "_________________________________________\n" << endl;
	cout << "| " << left << setw(15) << "Account Number";
	cout << "| " << left << setw(10) << "Pin Code";
	cout << "| " << left << setw(20) << "Client Name";
	cout << "| " << left << setw(12) << "Phone";
	cout << "| " << left << setw(12) << "Balance";
	cout <<
		"\n________________________________________________";
	cout << "_________________________________________\n" << endl;
	if (vClients.size() == 0)
	{
		cout << "\t\t\t\t No Clients Avilable in the systems !\n"; 
	}
	else
	{

		for (sClient Client : vClients)
		{
			PrintClientRecord(Client);
			cout << endl;
		}
		cout << "\n______________________________________________________________________\n";
	}
}


enMainMenuOption ChoseOperation()
{
	short Number = 0;
	cout << "Choose what do you want to do  [1-6] ?" << endl;
	cin >> Number;
	return (enMainMenuOption)Number;
}

void ShowAllClientsScreens()
{

	vector<sClient> vclient = ReadClientDataFromFile(ClientsFileName);
	  PrintAllClientsData(vclient);

}

void ShowAddNewClientsScreen()
{
	
	char DoAgain = 'y';
	do
	{
		system("cls");
		AddClient();

		cout << "Client Add Successfully, Do you want to add more client  ? Y/N" << endl;
		cin >> DoAgain;

	} while (tolower(DoAgain) == 'y');


	
}

void ShowDeleteClientsScreen()
{
	cout << "\n-----------------------------------------------\n";
	cout << "\t Delete Clients Screen \n";
	cout << "-----------------------------------------------\n";

	vector <sClient> vClients =LoadCleintsDataFromFile(ClientsFileName);

	string AccountNumber = ReadClientAccountNumber();

	DeleteClientByAccountNumber(AccountNumber, vClients);

}

void ShowUpdateClientsScreen()
{
	cout << "\n-----------------------------------------------\n";
	cout << "\t Update Clients Screen \n";
	cout << "-----------------------------------------------\n";

	vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
	string AccountNumber = ReadClientAccountNumber();
	UpdateClientByAccountNumber(AccountNumber, vClients);

}

void FindClients()
{
	cout << "\n-----------------------------------------------\n";
	cout << "\t Find Clients Screen \n";
	cout << "-----------------------------------------------\n";


	sClient Client;
	vector<sClient> vClients= LoadCleintsDataFromFile(ClientsFileName);
	
	string AccountNumber = ReadClientAccountNumber();
	if (FindClientByAccountNumber(AccountNumber,vClients ,Client))
	{
		PrintClientCard(Client);
	}
	else
	{
		cout << "\nClient with Account Number (" << AccountNumber <<") is Not Found!";
	}

}

void ShowEndScreen()
{
	cout << "\n-----------------------------------------------\n";
	cout << "\t Program End :-) \n";
	cout << "-----------------------------------------------\n";


}


void GoBackToMainMenu()
{
	cout << "Press Any Key To go Back To Main Menu...";
	cout << system("pause > 0");
	ShowMainMenu();
}

void ClientOperation(enMainMenuOption MainMenuOption)
{
	switch (MainMenuOption)
	{
	case enMainMenuOption::showClientList:
		system("cls");
		ShowAllClientsScreens();
		GoBackToMainMenu();
		break;


	case enMainMenuOption::AddNewClient:
		system("cls");
		ShowAddNewClientsScreen();
		GoBackToMainMenu();
		break;
	case enMainMenuOption::DeleteClient:
		system("cls");
		ShowDeleteClientsScreen();
		GoBackToMainMenu();
			break;
			 
	case enMainMenuOption::UpdateClientInfo:
		system("cls");
		ShowUpdateClientsScreen();
		GoBackToMainMenu();
		break;

	case enMainMenuOption::FindClient:
		system("cls");
		FindClients();
		GoBackToMainMenu();
		break;

	case enMainMenuOption::Exit:
		system("cls");
		ShowEndScreen();




	}
}

void ShowMainMenu()
{
	system("cls");
	cout << "=========================================\n" ;
	cout << "\t\t Main Menu screen\n";
	cout << "=========================================\n";
	cout << "\t[1] Show Client List.\n";
	cout << "\t[2] Add New Client.\n";
	cout << "\t[3] Delete Client.\n";
	cout << "\t[4] Update Client Info.\n";
	cout << "\t[5] Find Client .\n";
	cout << "\t[6] Exit.\n";
	cout << "=========================================\n";
	ClientOperation(ChoseOperation());


}


int main()
{
	ShowMainMenu();
	cout << system("pause > 0");

}
