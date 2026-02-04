#include "VCS.h"

VCS::VCS(const std::string& object_path){
	file_path = object_path;
	path directory_path = create_directory_name(file_path);
	version_number = 0;
	if (!exists(directory_path)) {
		create_directory(directory_path);
		std::cout << "The directory created: " << directory_path << "\n";
	}
	else {
		directory_iterator iterator(directory_path);
		for (const auto& dir_entry : iterator)
			version_number++;
	}
}

void VCS::save() {
	std::ifstream file(file_path);
	path directory_path = create_directory_name(file_path);
	path version_path =directory_path / ("v" + std::to_string(++version_number));
	std::ofstream version_file(version_path);
	if (!(file.is_open() && version_file.is_open())) {
		std::cerr <<"The file could not opened";
		return;
	}

	std::string line;
	while (std::getline(file, line)) {
		version_file << line << "\n";
	}
	version_file.close();
	file.close();
}

void VCS::list(){
	path directory_path = create_directory_name(file_path);
	if (!exists(directory_path)) {
		std::cerr << "This path does not exist so cannot list versions";
		return;
	}

	directory_iterator iterator(directory_path);
	for (const auto& dir_entry : iterator) {
		std::cout << dir_entry.path() << "-" << dir_entry.file_size()  << "bayt" << "\n";
	}
}
