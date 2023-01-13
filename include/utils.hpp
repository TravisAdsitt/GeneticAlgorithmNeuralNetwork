// #include <SFML/Graphics.hpp>
// #include "config.hpp"
#include <cmath>
#include <vector>
#include <memory>

#ifndef UTILS_H
#define UTILS_H

std::default_random_engine generator;

float get_rand_float(float from, float to){
    std::uniform_real_distribution<float> distribution(from, to);
    return distribution(generator);
}

int get_rand_int(int from, int to){
    std::uniform_real_distribution<int> distribution(from, to);
    return distribution(generator);
}

bool get_rand_bool(float chance = 0.5){
    return get_rand_float(0.0, 1.0) < chance;
}

float dot_product(float* a, float* b, int length){
    float total = 0;

    for(int i = 0; i < length; i++){
        total += a[i] * b[i];
    }

    return total;
}

float sigmoid(float input){
    return 1 / (1 + exp(-input));
}

#endif