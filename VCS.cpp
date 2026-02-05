#include "VCS.h"

// Private helper functions
path VCS::create_directory_path(const std::string& object_path) {
	std::string file_name;
	for (char c : object_path) {
		if (c == '.')
			break;
		file_name += c;
	}
	path directory_path = "versions_of_" + file_name;
	return directory_path;
}

void VCS::make_info_file(std::ofstream& info_file) {
	std::string date = set_time();
	std::string message;
	std::cout << "Please enter your message for your version if you want: ";
	std::getline(std::cin, message);
	if (message.empty())
		message = "changes saved";
	version_info.set(version_number,date, message);
	info_file << "version: " << version_info.version << "\n"
		<< "date: " << version_info.date << "\n"
		<< "message: " << version_info.message;
}

void VCS::clear_files(std::ifstream& file1, std::ifstream& file2) {
	file1.clear();
	file2.clear();
	file1.seekg(0);
	file2.seekg(0);
}

bool VCS::check_versions(std::ifstream& file1,std::ifstream& file2){
	std::string line1,line2;
	while (std::getline(file1, line1)) {
		if (std::getline(file2, line2)) {
			if (line1 != line2) {
				clear_files(file1, file2);
				return false;
			}
		}
		else {
			clear_files(file1, file2);
			return false;
		}
	}
	if (std::getline(file2, line2)) {
		clear_files(file1, file2);
		return false;
	}
	clear_files(file1, file2);
	return true;
}

std::string VCS::set_time() {
	auto now = std::chrono::system_clock::now();
	std::time_t now_c = std::chrono::system_clock::to_time_t(now);

	std::tm local_time{};
	localtime_s(&local_time, &now_c); // Windows

	std::stringstream ss;
	ss << std::setfill('0')
		<< std::setw(2) << local_time.tm_mday << "/"
		<< std::setw(2) << (local_time.tm_mon + 1) << "/"
		<< (local_time.tm_year + 1900) << " "
		<< std::setw(2) << local_time.tm_hour << ":"
		<< std::setw(2) << local_time.tm_min << ":"
		<< std::setw(2) << local_time.tm_sec;
	return ss.str();
}

// Fundamental VCS functions
VCS::VCS(const std::string& object_path){
	file_path = object_path;
	directory_path = create_directory_path(file_path);
	version_number = 0;
	if (!exists(directory_path)) {
		create_directory(directory_path);
		std::cout << "The directory created: " << directory_path << "\n";
	}
	else {
		directory_iterator iterator(directory_path);
		for (const auto& dir_entry : iterator)
			version_number++;
		version_number /= 2;
	}
}

void VCS::save() {
	std::ifstream original_file(file_path);
	path latest_version_path = directory_path / ("v" + std::to_string(version_number));
	std::ifstream latest_version_file(latest_version_path);
	if (version_number != 0 && latest_version_file.is_open() && original_file.is_open()) {
		if (check_versions(latest_version_file, original_file)) {
			std::cerr << "The latest version and original files are same so nothing to save\n";
			original_file.close();
			latest_version_file.close();
			return;
		}
	}
	path version_path = directory_path / ("v" + std::to_string(++version_number));
	path info_path = directory_path / ("v" + std::to_string(version_number) + "_info");
	std::ofstream version_file(version_path);
	std::ofstream info_file(info_path);

	if (!(version_file.is_open() && info_file.is_open())) {
		std::cerr <<"The files could not created";
		return;
	}

	std::string line;
	while (std::getline(original_file, line)) {
		version_file << line << "\n";
	}
	make_info_file(info_file);
	version_file.close();
	original_file.close();
	info_file.close();
	latest_version_file.close();
	std::cout << "v" << version_number << " has saved successfully\n";
}

void VCS::list() {
	if (directory_path.empty()) {
		std::cerr << "This path does not have any version so cannot list versions\n";
		return;
	}

	directory_iterator iterator(directory_path);
	std::string version_info;
	std::string file_name;
	int i = 0;
	for (const auto& dir_entry : iterator) {
		file_name = dir_entry.path().filename().string();
		if (file_name.find('_') != std::string::npos) {
			if (file_name.substr(file_name.find('_')) == "_info") {
				i++;
				path info_path = directory_path / ("v" + std::to_string(i) + "_info");
				std::ifstream info_file(info_path);
				while (std::getline(info_file, version_info)) {
					std::string key, value;
					bool is_first_colon_found = false;
					key.reserve(16);

					for (int j = 0; j < version_info.length(); j++) {
						if (version_info[j] == ':' && !is_first_colon_found) {
							key.assign(version_info, 0, j);
							value.assign(version_info.begin() + j + 2, version_info.end());
							is_first_colon_found = true;
						}
					}
					if (key == "version")
						std::cout << "v" + value << " | ";
					else if (key == "message")
						std::cout << value;
					else
						std::cout << value << " | ";
				}
				std::cout << "\n";
			}
		}
	}
}

void VCS::go_to_version(int version){
	if (version > version_number) {
		std::cerr << "This version does not exist\n";
		return;
	}
	std::string version_name = "v" + std::to_string(version);
	path version_path = directory_path / version_name;
	std::ifstream version_file(version_path);
	std::ofstream original_file(file_path);
	directory_iterator iterator(directory_path);
	if (version_file.is_open()) {
		for (const auto& dir_entry : iterator) {
			if (dir_entry.path().filename().string() == version_name) {
				std::string line;
				while (std::getline(version_file, line))
					original_file << line <<"\n";
				version_file.clear();
				version_file.seekg(0);
				version_file.close();
				original_file.close();
				std::cout << "now you are in v" << version << "\n";
				break;
			}
		}
	}
	else 
		std::cerr << "This version cannot opened\n";
		
}