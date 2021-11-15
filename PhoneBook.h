#pragma once
#include <iostream>
#include <string>
#include <optional>
#include <vector>
#include <tuple>
#include <fstream>
#include <sstream>
#include <assert.h>
#include <algorithm> 
#include <iomanip>

struct Person
{
	std::string _lastName;
	std::string _firstName;
	std::optional<std::string> _middleName;
};

std::ostream& operator<<(std::ostream& out, const Person& p);
bool operator==(const Person& p1, const Person& p2);
bool operator<(const Person& p1, const Person& p2);


struct PhoneNumber
{
	int _countryCode;
	int _cityCode;
	std::string _number;
	std::optional<int> _additionalNumber;
};

std::ostream& operator<<(std::ostream& out, const PhoneNumber& pN);
bool operator<(const PhoneNumber& pN1, const PhoneNumber& pN2);

class PhoneBook
{
private:
	std::vector<std::pair<Person, PhoneNumber>> _phoneBook;
public:
	PhoneBook(std::ifstream& file);

	void SortByName();
	void SortByPhone();
	void ChangePhoneNumber(const Person& p, const PhoneNumber& pN);

	std::tuple<std::string, PhoneNumber> GetPhoneNumber(const std::string& lastName);

	friend std::ostream& operator<<(std::ostream& out, const PhoneBook& book);
};