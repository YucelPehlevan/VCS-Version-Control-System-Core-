#include "VCS.h"

// Private helper functions
path VCS::create_directory_path(const std::string& object_path) {
	path directory_path = "versions_of_" + path(object_path).filename().string();
	return directory_path;
}

void VCS::make_info_file(std::ofstream& info_file) {
	std::string date = set_time();
	std::string message;
	std::cout << "Please enter your message for your version if you want: ";
	std::getline(std::cin, message);
	if (message.empty())
		message = "changes saved";
	version_info.set(max_version_number,date, message);
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

int VCS::find_max_version() {
	if (versions.empty())
		return 0;
	int max_element = versions.at(0);
	int vector_size = versions.size();
	for (int i = 1; i < vector_size; i++) {
		if (versions.at(i) > max_element)
			max_element = versions.at(i);
	}
	return max_element;
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

bool VCS::search_version(int target_version){
	auto it = std::find(versions.begin(), versions.end(), target_version);
	return (it != versions.end());
}

// Fundamental VCS functions
VCS::VCS(const std::string& object_path){
	file_path = object_path;
	directory_path = create_directory_path(file_path);
	max_version_number = 0;
	versions = {};
	if (!exists(directory_path)) {
		create_directory(directory_path);
		std::cout << "The directory created: " << directory_path << "\n";
	}
	else {
		directory_iterator iterator(directory_path);
		for (const auto& dir_entry : iterator) {
			std::string version_string = dir_entry.path().filename().string();
			if (version_string[0] == 'v' && version_string.find("_info") == std::string::npos) {
				int version_no = std::stoi(version_string.substr(1));
				versions.push_back(version_no);
			}
		}
		if (!versions.empty()) {
			std::sort(versions.begin(), versions.end());
			max_version_number = versions.back();
		}
	}
}

void VCS::save() {
	std::ifstream original_file(file_path);
	path latest_version_path = directory_path / ("v" + std::to_string(max_version_number));
	std::ifstream latest_version_file(latest_version_path);
	if (versions.size() != 0 && latest_version_file.is_open() && original_file.is_open()) {
		if (check_versions(latest_version_file, original_file)) {
			std::cerr << "The latest version and original files are same so nothing to save\n";
			original_file.close();
			latest_version_file.close();
			return;
		}
	}
	path version_path = directory_path / ("v" + std::to_string(++max_version_number));
	path info_path = directory_path / ("v" + std::to_string(max_version_number) + "_info");
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
	versions.push_back(max_version_number);
	std::cout << "v" << max_version_number << " has saved successfully\n";
}

void VCS::list() {
	if (directory_path.empty()) {
		std::cerr << "This path does not have any version so cannot list versions\n";
		return;
	}
	std::cout << "The number of versions: " << versions.size() << "\n";
	std::string version_info;
	for (int version : versions) {
			path info_path = directory_path / ("v" + std::to_string(version) + "_info");
			std::ifstream info_file(info_path);
			if (!info_file.is_open())
				std::cerr << "v" << version << " is corrupted";
			while (std::getline(info_file, version_info)) {
				std::string key, value;
				bool is_first_colon_found = false;
				key.reserve(16);

				int version_info_size = version_info.length();
				for (int j = 0; j < version_info_size; j++) {
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

void VCS::go_to_version(int version){
	if (!search_version(version)) {
		std::cerr << "This version does not exist\n";
		return;
	}
	std::string version_name = "v" + std::to_string(version);
	path version_path = directory_path / version_name;
	std::ifstream version_file(version_path);
	std::ofstream original_file(file_path);
	if (version_file.is_open() && original_file.is_open()) { 
		std::string line;
		while (std::getline(version_file, line))
			original_file << line <<"\n";
		version_file.clear();
		version_file.seekg(0);
		version_file.close();
		original_file.close();
		std::cout << "now you are in v" << version << "\n";	
	}
	else 
		std::cerr << "This version cannot opened\n";
}

void VCS::show_status(){
	std::ifstream original_file(file_path);
	path latest_version_path = directory_path / ("v" + std::to_string(max_version_number));
	std::ifstream latest_version_file(latest_version_path);

	if (!(latest_version_file.is_open() && original_file.is_open())) {
		std::cerr << "The files could not opened \n";
		return;
	}
	if (check_versions(latest_version_file, original_file))
		std::cout<<"The latest version and original files are same so nothing to save\n";
	else {
		path original_path = file_path;
		int original_file_size = file_size(original_path);
		int latest_version_file_size = file_size(latest_version_path);
		if (original_file_size > latest_version_file_size)
			std::cout << "[+] The original file is bigger so there can be some add ups";
		else if (original_file_size == latest_version_file_size)
			std::cout << "[~] file sizes are same but there can be changes in original file";
		else
			std::cout << "[-] Latest version file is bigger so there can be some deletions in original file";
	}
}

void VCS::delete_version(int version){
	if (!search_version(version)) {
		std::cerr << "This version does not exist so cannot delete\n";
		return;
	}
	path version_path = directory_path / ("v" + std::to_string(version));
	path info_path = directory_path / ("v" + std::to_string(version) + "_info");

	bool version_deleted = remove(version_path);
	if (!version_deleted) {
		std::cerr << "The version file could not deleted\n";
		return;
	}
	std::cout << "v" << version << " has deleted successfully\n";
	
	bool info_deleted = remove(info_path);
	if (!info_deleted) {
		std::cerr << "info file could not deleted so it will be a garbage you can manually delete\n";
		info_path.filename() = "v" + std::to_string(version) + "_info_garbage";
	}

	auto it = std::find(versions.begin(), versions.end(), version);
	versions.erase(it);
	if (versions.empty())
		max_version_number = 0;
	else 
		max_version_number = find_max_version();
}

