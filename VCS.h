#pragma once
#include "info.h"
#include <fstream>
#include <iostream>
#include <filesystem>
using namespace std::filesystem;

class VCS{
private:
	int version_number;
	std::string file_path;
	info version_info;

	path create_directory_path(const std::string& object_path) {
		std::string file_name;
		for (char c : object_path) {
			if (c == '.')
				break;
			file_name += c;
		}
		path directory_path = "versions_of_" + file_name;
		return directory_path;
	}
	
	void make_info_file(std::ofstream& info_file) {
		std::string message;
		std::cout << "Please enter your message for your version if you want: ";
		std::getline(std::cin, message);
		if (message.empty())
			message = "changes saved";
		version_info.set(version_number, message);
		info_file << "version: " << version_info.version << "\n"
			<< "date: " << version_info.date << "\n"
			<< "message: " << version_info.message;
	}
public:
	VCS(const std::string& object_path);

	void save();

	void list();
};

