//
// Created by louis on 13/11/16.
//

#ifndef SIMULATION_UTILS_H
#define SIMULATION_UTILS_H

#include <thread>
#include <chrono>
#include <fstream>
#include <sstream>
#include <vector>
#include <glm/glm.hpp>
#include <sys/stat.h>

#ifdef _WIN32
#include <direct.h>
typedef unsigned char uint;
#endif

inline void createDirectory(std::string directory) {
#ifdef _WIN32
    mkdir(directory.c_str());
#else
    mkdir(directory.c_str(), 0700);
#endif
}

inline std::string readText(const std::string & filename) {
    std::ifstream file(filename);
    std::string contents = "";
    std::string line;

    if (file.is_open()) {
        while (getline(file, line)) {
            contents += line + "\n";
        }
        file.close();
    }
    else {
        throw std::runtime_error("File not found : " + filename);
    }

    return contents;
}

inline void sleep(float ms) {
    std::this_thread::sleep_for(std::chrono::microseconds((int) (ms * 1000)));
}

inline glm::vec3 randBrightColor() {
    float r = (float) rand() / RAND_MAX;
    float g = (float) rand() / RAND_MAX;
    float b = (float) rand() / RAND_MAX;
    float max = r >= g && r >= b ? r : (g >= r && g >= b ? g : b);
    float min = r <= g && r <= b ? r : (g <= r && g <= b ? g : b);
    if (max == min) {
        max = 1;
        min = 0;
    }
    r = (r - min) / (max - min);
    g = (g - min) / (max - min);
    b = (b - min) / (max - min);
    return glm::vec3(r, g, b);
}

inline void printCommandDict(std::string commands[][2], int commandCount,
                             int commandSize = 20, int descriptionSize = 200) {

    for (int i = 0 ; i < commandCount ; i++) {
        std::string command = commands[i][0];
        std::string description = commands[i][1];

        int numSpaces = commandSize - (int) command.size();
        if (numSpaces < 3) {
            numSpaces = 3;
        }

        std::string spaces(numSpaces, ' ');
        //TODO descriptionSize
        std::cout << "\t" << command << spaces << ": " << description << std::endl;
    }
}

inline std::vector<std::string> split(const std::string & splitted, const char delim, bool trimEmpty = false) {
    std::vector<std::string> result;
    std::stringstream ss;
    ss.str(splitted);
    std::string line;

    while (getline(ss, line, delim)) {
        if (trimEmpty && line.size() == 0) continue;
        result.push_back(line);
    }
    return result;
}

inline std::string trimSpaces(const std::string & item) {
    int startIndex = 0, endIndex = item.size();
    for (int i = 0 ; i < item.size() ; i++) {
        if (item[i] == ' ' || item[i] == '\t') {
            startIndex++;
        }
        else {
            break;
        }
    }
    for (int i = endIndex - 1 ; i >= startIndex ; i++) {
        if (item[i] == ' ' || item[i] == '\t') {
            endIndex--;
        }
        else {
            break;
        }
    }
    return item.substr(startIndex, endIndex - startIndex);
}

inline bool startsWith(const std::string & item, const std::string & prefix) {
    if (item.size() < prefix.size()) {
        return false;
    }

    //C'est pas drôle
    for (int c = 0 ; c < prefix.size() ; c++) {
        //vous avez ri ?
        if (item[c] != prefix[c]) {
            return false;
        }
    }

    return true;
}

inline bool endsWith(const std::string & item, const std::string & suffix) {
	if (item.size() < suffix.size()) {
		return false;
	}

	for (int i = item.size(), j = suffix.size(); j >= 0; i--, j--) {
		if (item[i] != suffix[j]) {
			return false;
		}
	}

	return true;
}

#endif // SIMULATION_UTILS_H