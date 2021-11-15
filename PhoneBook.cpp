#include "PhoneBook.h"
using namespace std;
std::ostream& operator<<(std::ostream& out, const Person& p)
{
	if (p._middleName.has_value())
	{
		out << std::setw(12) << p._lastName << " " << std::setw(10) << p._firstName << " " << std::setw(15) << p._middleName.value();
	}
	else
	{
		out << std::setw(12) << p._lastName << " " << std::setw(10) << p._firstName << std::setw(16) << " ";
	}
	return out;
}

bool operator==(const Person& p1, const Person& p2)
{
	return std::tie(p1._lastName, p1._firstName, p1._middleName) == std::tie(p2._lastName, p2._firstName, p2._middleName);
}

bool operator<(const Person& p1, const Person& p2)
{
	return (std::tie(p1._lastName, p1._firstName, p1._middleName) < std::tie(p2._lastName, p2._firstName, p2._middleName));
}

std::ostream& operator<<(std::ostream& out, const PhoneNumber& pN)
{
	if (pN._additionalNumber.has_value())
	{
		out << "+" << pN._countryCode << "(" << pN._cityCode << ")" << pN._number << " " << pN._additionalNumber.value();
	}
	else
	{
		out << "+" << pN._countryCode << "(" << pN._cityCode << ")" << pN._number;
	}
	return out;
}

bool operator<(const PhoneNumber& pN1, const PhoneNumber& pN2)
{
	if (pN1._additionalNumber != std::nullopt)
	{
		return (std::tie(pN1._countryCode, pN1._cityCode, pN1._number, pN1._additionalNumber) < std::tie(pN2._countryCode, pN2._cityCode, pN2._number, pN2._additionalNumber));
	}
	else
	{
		return (std::tie(pN1._countryCode, pN1._cityCode, pN1._number) < std::tie(pN2._countryCode, pN2._cityCode, pN2._number));
	}
}

std::ostream& operator<<(std::ostream& out, const PhoneBook& book)
{
	for (size_t i = 0; i < book._phoneBook.size(); i++)
	{
		out << book._phoneBook[i].first << "    " << book._phoneBook[i].second << std::endl;
	}
	return out;
}

PhoneBook::PhoneBook(std::ifstream& file)
{
	if (file.is_open())
	{
		std::vector<Person> persons;
		std::vector<PhoneNumber> numbers;


		std::string tmp;
		while (std::getline(file, tmp))
		{
			std::istringstream iss(tmp);
			Person p;
			PhoneNumber pN;
			iss >> p._lastName >> p._firstName;
			if (iss.getline(&tmp[0], 15, ' '))
			{
				if (iss.peek() != '+')
				{
					iss >> p._middleName.emplace();
				}
			}
			iss >> pN._countryCode >> pN._cityCode >> pN._number;

			if (iss.getline(&tmp[0], 15, ' '))
			{
				if (iss.peek() != '\n')
				{
					iss >> pN._additionalNumber.emplace();
				}
			}
			persons.push_back(p);
			numbers.push_back(pN);
		}

		for (int i = 0; i < persons.size(); ++i)
		{
			_phoneBook.push_back(std::make_pair(persons[i], numbers[i]));
		}


	}
	else
	{
		std::cout << "can't open file" << std::endl;
		return;
	}
}

void PhoneBook::SortByName()
{
	std::sort(_phoneBook.begin(), _phoneBook.end(), [](const auto& left, const auto& right)
		{
			return left.first < right.first;
		}
	);
}


void PhoneBook::SortByPhone()
{
	std::sort(_phoneBook.begin(), _phoneBook.end(), [](const auto& left, const auto& right)
		{
			return left.second < right.second;
		}
	);
}

void PhoneBook::ChangePhoneNumber(const Person& p, const PhoneNumber& pN)
{
	auto findName = [&](const auto& book)
	{
		return book.first == p;
	};
	auto fName = std::find_if(_phoneBook.begin(), _phoneBook.end(), findName);
	if (fName != std::end(_phoneBook))
	{
		fName->second = pN;
	}
}

std::tuple<std::string, PhoneNumber> PhoneBook::GetPhoneNumber(const std::string& lastName)
{
	std::string str;
	PhoneNumber pN;
	int count = 0;
	auto isEqual = [&](const auto& pBook) mutable
	{
		if (pBook.first._lastName == lastName)
		{
			count++;
			pN = pBook.second;
		}

	};
	std::for_each(_phoneBook.begin(), _phoneBook.end(), isEqual);
	if (count == 0)
	{
		str = "Person with Last Name '" + lastName + "' Not found";
	}
	else if (count == 1)
	{
		str.erase();
	}
	else
	{
		str = "Found more than 1 Person with Last Name '" + lastName + "'";
	}
	return std::make_tuple(str, pN);
}