/*
Steven Steele
May 8, 2018

Rental Manager
*/

#include "rental.h"

vector<Customer> Customer::customers_;
set<Item> Item::items_;

bool operator<(const Time& t1, const Time& t2)
{
	return (t1.year_ < t2.year_ && t1.month_ < t2.month_ && t1.day_ < t2.day_);
}

bool operator==(const Customer& cust1, const Customer& cust2)
{
	if (cust1.getLastName() == cust2.getLastName())
		if (cust1.getFirstName() == cust2.getFirstName())
			if (cust1.getPhoneNumber() == cust2.getPhoneNumber())
				return true;
	return false;
}

bool operator<(const Item& item1, const Item& item2)
{
	return (item1.barcode_ < item2.barcode_);
}

void Item::createItem()
{
	cout << "Please scan item:\n";
	string barcode;
	getline(cin, barcode);

	bool original = true;
	set<Item>::iterator iter = Item::items_.begin();
	while (iter != Item::items_.end())
	{
		if (barcode == iter->barcode_)
		{
			original = false;
			break;
		}
		++iter;
	}

	if (original)
	{
		cout << "Describe the item (EX: Yoga Mat or Shoes):\n";
		string description;
		getline(cin, description);

		Item newItem(barcode, description);
		items_.insert(newItem);
	}
	else
		cout << "Item " << iter->description_ << " already exists.\n";
}

bool operator<(const Customer& cust1, const Customer& cust2)
{
	return(cust1.getLastName() < cust2.getLastName() && cust1.getFirstName() < cust2.getFirstName()
		&& cust1.getPhoneNumber() < cust2.getPhoneNumber());
}

void Customer::addItem(const Item& item)
{
	time_t t = time(NULL);
	struct tm *time = localtime(&t);
	int month = time->tm_mon + 1;
	int day = time->tm_mday;
	int year = time->tm_year + 1900;
	Time properTime(month, day, year);
	pair<Item, Time> newPair(item, properTime);
	rented_.insert(newPair);
}

void Customer::createCustomer()
{
	string lastName;
	cout << "Customer's last name:\n";
	getline(cin, lastName);

	string firstName;
	cout << "Customer's first name:\n";
	getline(cin, firstName);

	string phoneNumber;
	cout << "Customer's phone number(no dashes):\n";
	getline(cin, phoneNumber);

	Customer newCustomer = Customer(lastName, firstName, phoneNumber);

	bool unique = true;
	for (auto a : customers_)
	{
		if (a == newCustomer)
		{
			unique = false;
			break;
		}
	}
	if (unique)
		Customer::customers_.push_back(newCustomer);
	else
		cout << "CUSTOMER ALREADY EXISTS\n";
}

void save()
{
	ofstream itemFile;
	itemFile.open("items.txt");
	
	for (auto a : Item::items_)
	{
		itemFile << a.barcode_ << ' ' << a.description_ << endl;
	}
	itemFile << "END";

	itemFile.close();

	ofstream customerFile;
	customerFile.open("customers.txt");

	set<pair<Item, Time>> rentedCopy;
	for (auto a : Customer::customers_)
	{
		rentedCopy = a.getRented();
		customerFile << a.getLastName() << ' ' << a.getFirstName() << ' ' << a.getPhoneNumber() << ' ';
		customerFile << rentedCopy.size() << ' ';
		for (auto b : rentedCopy)
		{
			customerFile << b.first.barcode_ << ' ';
			customerFile << b.first.description_ << ' ';
			customerFile << b.second.month_ << ' ';
			customerFile << b.second.day_ << ' ';
			customerFile << b.second.year_ << ' ';
		}
		customerFile << endl;
	}
	customerFile << "END";

	customerFile.close();
}

void load()
{
	string barcode;
	string description;
	ifstream itemIn;
	itemIn.open("items.txt");
	itemIn >> barcode;
	while (barcode != "END")
	{
		itemIn >> description;
		Item item(barcode, description);
		Item::items_.insert(item);

		itemIn >> barcode;
	}
	itemIn.close();

	ifstream in;
	in.open("customers.txt");

	string lastName;
	string firstName;
	string phoneNumber;
	int n;
	string Barcode;
	string Description;
	int Month;
	int Day;
	int Year;
	set<pair<Item, Time>> Set;

	in >> lastName;
	while (lastName != "END")
	{
		Set.clear();
		in >> firstName;
		in >> phoneNumber;
		in >> n;
		for (int i = 0; i < n; ++i)
		{
			in >> Barcode;
			in >> Description;
			in >> Month;
			in >> Day;
			in >> Year;

			Item item(Barcode, Description);
			Time time(Month, Day, Year);
			pair<Item, Time> Pair(item, time);
			Set.insert(Pair);
		}

		Customer customer(lastName, firstName, phoneNumber, Set);
		Customer::customers_.push_back(customer);

		in >> lastName;
	}

	in.close();
}

void Customer::modify()
{
	cout << "    1. Check out an item\n"
		 << "    2. Return an item\n"
		 << "    3. Change name/phone number\n"
		 << "    4. View items already checked out\n"
		 << "    5. Delete user\n"
		 << "    0. Done\n";
	string choice;
	getline(cin, choice);

	while (choice != "0")
	{
		if (choice == "1")
		{
			checkout();
		}
		else if (choice == "2")
		{
			returnItem();
		}
		else if (choice == "3")
		{
			changeInfo();
		}
		else if (choice == "4")
		{
			viewCheckedOut();
		}
		else if (choice == "5")
		{
			cout << "Are you sure? y/n\n";
			string deleteMe;
			getline(cin, deleteMe);
			if (deleteMe == "y" || deleteMe == "Y")
			{
				erase();
				break;
			}
		}
		else
		{
			cout << "Not a valid choice\n";
		}

		cout << "    1. Check out an item\n"
			 << "    2. Return an item\n"
			 << "    3. Change name/phone number\n"
			 << "    4. View items already checked out\n"
			 << "    5. Delete user\n"
			 << "    0. Done\n";
		getline(cin, choice);
	}
}

void Customer::checkout()
{
	cout << "Please scan item\n";
	string barcode;
	getline(cin, barcode);

	bool found = false;
	set<Item>::const_iterator iter = Item::items_.cbegin();
	while (iter != Item::items_.cend())
	{
		if (iter->barcode_ == barcode)
		{
			found = true;
			break;
		}
		++iter;
	}

	if (!found)
	{
		cout << "Item is not in the system" << endl;
		cout << "Add now?\n";
		string answer;
		getline(cin, answer);
		if (answer == "y" || answer == "Y")
		{
			Item::createItem();
		}
	}
	else
	{
		addItem(*iter);
		cout << iter->description_ << " checked out to " << getFirstName() << endl;
	}
}

void Customer::returnItem()
{
	cout << "Scan item you would like to return:\n";
	string barcode; 
	getline(cin, barcode);

	set<pair<Item, Time>>::const_iterator iter = rented_.cbegin();
	set<pair<Item, Time>>::const_iterator end = rented_.cend();
	bool found = false;

	while (iter != end)
	{
		if (iter->first.barcode_ == barcode)
		{
			found = true; 
			break;
		}
		++iter;
	}

	if (found)
	{
		cout << iter->first.description_ << " returned from " << getFirstName()
			<< "'s account." << endl
			<< "Original date checked out: " << iter->second.month_ << "/" << iter->second.day_
			<< "/" << iter->second.year_ << endl;
	}
	else
	{
		cout << "Item not checked out to " << getFirstName() << endl;
	}

	cout << endl;
}

void Customer::changeInfo()
{
	cout << "1. Change name\n"
		 << "2. Change phone number\n"
		 << "0. Quit\n\n";
	string choice;
	getline(cin, choice);
	while (choice != "0")
	{
		if (choice == "1")
		{
			cout << "1. Change last name: " << getLastName() << endl
				 << "2. Change first name : " << getFirstName() << endl
				 << "0. Quit\n";
			string choice1;
			getline(cin, choice1);
			cout << "\n\n";
			while (choice1 != "0")
			{
				if (choice1 == "1")
				{
					cout << "What would you like to change last name: " << getLastName() << " to? (enter 0 to quit)\n";
					string newLastName;
					getline(cin, newLastName);
					if (newLastName != "0")
					{
						setLastName(newLastName);
						cout << "New last name: " << getLastName() << endl;
					}
				}
				else if (choice1 == "2")
				{
					cout << "What would you like to change first name: " << getFirstName() << " to? (enter 0 to quit)\n";
					string newFirstName;
					getline(cin, newFirstName);
					if (newFirstName != "0")
					{
						setFirstName(newFirstName);
						cout << "New first name: " << getFirstName() << endl;
					}
				}
				else
					cout << "INVALID SELECTION\n";

				cout << "1. Change last name: " << getLastName() << endl
					<< "2. Change first name : " << getFirstName() << endl
					<< "0. Quit\n";
				getline(cin, choice1);
				cout << "\n\n";
			}
		}
		else if (choice == "2")
		{
			cout << "What would you like to change phone number: " << getPhoneNumber() << " to? (enter 0 to quit)\n";
			string newPhoneNumber;
			getline(cin, newPhoneNumber);
			while (newPhoneNumber != "0")
			{
				setPhoneNumber(newPhoneNumber);
				cout << "New phone number: " << getPhoneNumber() << " . Is this correct? y/n\n";
				string choice2;
				getline(cin, choice2);
				if (choice2 == "y" || choice2 == "Y")
					break;
				cout << "What would you like to change phone number: " << getPhoneNumber() << " to? (enter 0 to quit)\n";
				getline(cin, newPhoneNumber);
			}
		}
		cout << "1. Change name\n"
			<< "2. Change phone number\n"
			<< "0. Quit\n\n";
		getline(cin, choice);
		cout << "\n\n";
	}
}

void Customer::viewCheckedOut() const
{
	cout << "\n\nAlready checked out by " << getFirstName() << ":\n";
	for (auto a : rented_)
	{
		cout << a.first.barcode_ << " " << a.first.description_
			<< " since " << a.second.month_ << "/" << a.second.day_
			<< "/" << a.second.year_ << endl;
	}
	cout << "\n\n";
}

void Customer::erase()
{
	vector<Customer>::iterator iter = customers_.begin();

	bool found = false;
	while (iter != customers_.end())
	{
		if (*this == *iter)
		{
			found = true;
			customers_.erase(iter);
			break;
		}

		++iter;
	}
}

void findName()
{
	cout << "\n\n\n";
	string lastName;
	cout << "Enter customer's last name:\n";
	getline(cin, lastName);

	vector<Customer>::iterator iter = Customer::customers_.begin();
	vector<vector<Customer>::iterator> custVec;
	while (iter != Customer::customers_.end())
	{
		if (lastName == iter->getLastName())
			custVec.push_back(iter);
		++iter;
	}

	if (custVec.size() == 0)
	{
		string input;
		cout << "No customer found under last name: " << lastName << ".\n"
			<< "\nWould you like to create a new account? (y/n)\n";
		getline(cin, input);
		if (input == "y" || input == "Y")
		{
			Customer::createCustomer();
		}
	}
	else
	{
		cout << "Customers with matching last names:\n";
		int count = 1;
		for (int i = 0; i < custVec.size(); ++i)
		{
			cout << "    " << count++ << ". " << custVec[i]->getLastName() << ", " << custVec[i]->getFirstName()
				<< " - " << custVec[i]->getPhoneNumber() << endl;
		}

		cout << "Enter the number of the correct person (Enter 0 to quit):\n";
		int choice;
		cin >> choice;
		cin.clear();
		cin.ignore();
		while (cin.fail() || choice > custVec.size())
		{
			cout << "NOT VALID. PLEASE ENTER SINGLE OR DOUBLE DIGIT NUMBER PRECEDING THE CORRECT CHOICE\n";
			cin >> choice;
			cin.clear();
			cin.ignore();
		}

		if (choice != 0)
		{
			cout << custVec[choice - 1]->getLastName() << ", " << custVec[choice - 1]->getFirstName()
				<< " - " << custVec[choice - 1]->getPhoneNumber() << endl;
			custVec[choice - 1]->modify();
		}
	}
}