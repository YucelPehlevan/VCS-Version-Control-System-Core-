#pragma once
#include <string>
struct info {
	int version;
	int date = 2026;
	std::string message;

	void set(int version_number, std::string& message) {
		version = version_number;
		this->message = message;
	}
};

