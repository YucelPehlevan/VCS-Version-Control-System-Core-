#pragma once
#include <string>
struct info {
	int version;
	std::string date;
	std::string message;

	void set(int version_number,std::string date, std::string& message) {
		version = version_number;
		this->date = date;
		this->message = message;
	}
};

