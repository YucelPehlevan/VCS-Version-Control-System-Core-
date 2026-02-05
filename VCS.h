#pragma once
#include "info.h"
#include <fstream>
#include <iostream>
#include <filesystem>
#include <chrono>
#include <sstream> 
#include <iomanip> 
#include <ctime>
using namespace std::filesystem;

class VCS{
private:
	int version_number;
	std::string file_path;
	info version_info;
	path directory_path;

	path create_directory_path(const std::string&);
	void make_info_file(std::ofstream&);
	void clear_files(std::ifstream& , std::ifstream&);
	bool check_versions(std::ifstream& , std::ifstream&);
	std::string set_time();

public:
	VCS(const std::string&);
	void save();
	void list();
	void go_to_version(int);
};

