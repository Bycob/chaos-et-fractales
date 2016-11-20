//
// Created by louis on 13/11/16.
//

#ifndef SIMULATION_UTILS_H
#define SIMULATION_UTILS_H

#include <thread>
#include <chrono>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>
#include <sys/stat.h>
#include <direct.h>

inline void createDirectory(std::string directory) {
#ifdef _WIN32
    mkdir(directory.c_str());
#else
    mkdir(directory.c_str(), 0700);
#endif
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
    return item.substr(startIndex, endIndex);
}

#endif // SIMULATION_UTILS_H