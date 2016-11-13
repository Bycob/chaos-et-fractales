//
// Created by louis on 13/11/16.
//

#include <thread>
#include <chrono>
#include <glm/glm.hpp>

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


