#include "VCS.h"

VCS::VCS(const std::string& object_path){
	file_path = object_path;
	path directory_path = create_directory_path(file_path);
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
	std::ifstream file(file_path);
	path directory_path = create_directory_path(file_path);
	path version_path = directory_path / ("v" + std::to_string(++version_number));
	path info_path = directory_path / ("v" + std::to_string(version_number) + "_info");
	std::ofstream version_file(version_path);
	std::ofstream info_file(info_path);

	if (!(file.is_open() && version_file.is_open() && info_file.is_open())) {
		std::cerr <<"The files could not created";
		return;
	}

	std::string line;
	while (std::getline(file, line)) {
		version_file << line << "\n";
	}
	make_info_file(info_file);
	version_file.close();
	file.close();
	info_file.close();
}

void VCS::list(){
	path directory_path = create_directory_path(file_path);
	if (directory_path.empty()) {
		std::cerr << "This path does not have any version so cannot list versions";
		return;
	}

	directory_iterator iterator(directory_path);
	std::string version_info;
	std::string file_name;
	int i = 0;
	for (const auto& dir_entry : iterator) {
		file_name = dir_entry.path().filename().string();
		if (file_name.substr(2) == "_info") {
			i++;
			path info_path = directory_path / ("v" + std::to_string(i) + "_info");
			std::ifstream info_file(info_path);
			while (std::getline(info_file, version_info)) {
				std::string key, value;
				key.reserve(16);
				
				for (int j = 0; j < version_info.length(); j++) {
					if (version_info[j] == ':') {
						key.assign(version_info, 0, j);
						value.assign(version_info.begin() + j + 2, version_info.end());
					}
				}
				if (key == "version")
					std::cout << "v" + value << " | ";
				else if(key == "message")
					std::cout << value;
				else
					std::cout << value << " | ";
			}
			std::cout << "\n";
		}

	}
}
