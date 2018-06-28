/*
Steven Steele
May 8, 2018

Rental Manager
*/

#include "rental.h"
#include <algorithm>

void clear()
{
	cin.clear();
	cin.ignore(20, '\n');
}

void intro()
{
	cout << "          **********Rental Manager**********\n\n"
		<< "Enter number of option you would like to select:\n"
		<< "--------------------------------------------------\n"
		<< "1. Search for customer\n"
		<< "2. Create new customer account\n"
		<< "3. View list of all customers\n"
		<< "4. Create new item\n"
		<< "5. View all items\n"
		<< "0. Save and quit\n\n";
}

int main()
{
	intro();
	load();
	string n;
	getline(cin, n);

	while (n != "0")
	{
		if (n == "1") 
		{
			findName();
		}
		else if (n == "2") 
		{
			Customer::createCustomer();
		}
		else if (n == "3")
		{
			for (auto a : Customer::customers_)
			{
				cout << a.getLastName() << ", " << a.getFirstName() << " - " << a.getPhoneNumber() << endl;
			}
		}
		else if (n == "4")
		{
			Item::createItem();
		}
		else if (n == "5")
		{
			cout << endl;
			for (auto a : Item::items_)
				cout << a.barcode_ << " " << a.description_ << endl;
		}
		else if (n == "42")
		{
			cout << "CREATED BY STEVEN STEELE FOR THE BENEFIT OF MAN AND MICHAEL\n";
		}
		else
		{
			cout << "Not a valid selection\n";
		}

		cout << "\n\n\n\n\n";
		intro();
		save();
		sort(Customer::customers_.begin(), Customer::customers_.end());
		getline(cin, n);
	}

	return 0;
}