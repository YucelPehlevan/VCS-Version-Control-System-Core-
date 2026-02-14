#pragma once
#include <string>
struct info {
	int version;
	std::string date;
	std::string message;

	void set(int number_of_versions,std::string date, std::string& message) {
		version = number_of_versions;
		this->date = date;
		this->message = message;
	}
};

