/*
	Steven Steele
	May 8, 2018

	Rental Manager
*/

#ifndef RENTAL_H
#define RENTAL_H

#define _CRT_SECURE_NO_WARNINGS
#include <vector>
#include <set>
#include <string>
#include <utility>
#include <time.h>
#include <iostream>
#include <fstream>

using namespace std;

struct Time
{
	friend bool operator<(const Time& t1, const Time& t2);

	Time(int month, int day, int year) : month_(month), day_(day), year_(year) {};

	int month_;
	int day_;
	int year_;
};

struct Item
{
	friend bool operator<(const Item& item1, const Item& item2);

	Item(string barcode, string description) :
		barcode_(barcode), description_(description) {};

	static set<Item> items_;
	static void createItem();

	string barcode_;
	string description_;
};


class Customer
{
	friend bool operator<(const Customer& cust1, const Customer& cust2);
	friend bool operator==(const Customer& cust1, const Customer& cust2);
public:
	static vector<Customer> customers_;

	Customer() {};
	Customer(string lastName, string firstName, string phoneNumber) :
		lastName_(lastName), firstName_(firstName), phoneNumber_(phoneNumber) {};
	Customer(string lastName, string firstName, string phoneNumber, set<pair<Item, Time>> rented) :
		lastName_(lastName), firstName_(firstName), phoneNumber_(phoneNumber), rented_(rented) {};

	string getLastName() const { return lastName_; };
	string getFirstName() const { return firstName_; };
	string getPhoneNumber() const { return phoneNumber_; };
	set<pair<Item, Time>> getRented() const { return rented_; };

	void setLastName(const string& lastName) { lastName_ = lastName; };
	void setFirstName(const string& firstName) { firstName_ = firstName; };
	void setPhoneNumber(const string &phoneNumber) { phoneNumber_ = phoneNumber; };
	void setVector(set<pair<Item, Time>> rented) { rented_ = rented; };

	void addItem(const Item& item);
	static void createCustomer();
	void modify();
	void checkout();
	void returnItem();
	void changeInfo();
	void viewCheckedOut() const;
	void erase();
private:
	string lastName_;
	string firstName_;
	string phoneNumber_;
	set<pair<Item, Time>> rented_;
};

void findName();

void load();
void save();

#endif