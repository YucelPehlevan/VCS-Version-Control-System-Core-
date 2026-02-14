#pragma once
#include "info.h"
#include <fstream>
#include <iostream>
#include <filesystem>
#include <chrono>
#include <sstream> 
#include <iomanip> 
#include <ctime>
#include <vector>
using namespace std::filesystem;

class VCS{
private:
	int max_version_number;
	std::string file_path;
	info version_info;
	path directory_path;
	std::vector<int> versions;

	path create_directory_path(const std::string&);
	void make_info_file(std::ofstream&);
	void clear_files(std::ifstream& , std::ifstream&);
	bool check_versions(std::ifstream& , std::ifstream&);
	int find_max_version();
	std::string set_time();
	bool search_version(int);

public:
	VCS(const std::string&);
	void save();
	void list();
	void go_to_version(int);
	void show_status();
	void delete_version(int);
};

