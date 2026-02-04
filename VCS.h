#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>
using namespace std::filesystem;

class VCS{
private:
	int version_number;
	std::string file_path;

	path create_directory_name(const std::string& object_path) {
		std::string file_name;
		for (char c : object_path) {
			if (c == '.')
				break;
			file_name += c;
		}
		path directory_path = "versions_of_" + file_name;
		return directory_path;
	}
	
public:
	VCS(const std::string& object_path);

	void save();

	void list();
};

