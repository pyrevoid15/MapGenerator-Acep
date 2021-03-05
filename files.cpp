#include "files.h"

std::string files::file_in(const char* filename) {
	std::fstream f(filename, std::ios_base::in);
	std::string s = "";

	if (f.is_open()) {
		std::cout << "OPEN!\n";

		char cc;
		while (f.get(cc)) {
			//std::cout << cc << '\n';
			s += cc;
		}

		f.close();
		return s;
	}
	else {
		std::cout << "NON-OPEN!\n";
		return 0;
	}
}

int files::file_out(const char* filename, const char* content) {
	std::fstream f(filename, std::ios_base::out);

	if (f.is_open()) {
		std::cout << "OPEN!\n";

		for (char cc : std::string(content)) {
			f.put(cc);
			//std::cout << cc << '\n';
		}

		f.close();
		return 1;
	}
	else {
		std::cout << "NON-OPEN!\n";
		return 0;
	}


}