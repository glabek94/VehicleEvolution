//
// Created by dawid on 20.05.18.
//

#ifndef PROJECT_RANDOMNUMBERGENERATOR_H
#define PROJECT_RANDOMNUMBERGENERATOR_H

#include <random>
#include <algorithm>
#include <stdexcept>

class RandomNumberGenerator {
public:
    static RandomNumberGenerator &Instance() {
        static RandomNumberGenerator singleton;
        return singleton;
    }

    float GetFromNormalDist(float mean = 0.0, float deviation = 1.0) {
        std::normal_distribution<float> dist(mean, deviation);
        return dist(rng);
    }

    int GetIntFromUniformDist(int min, int max) {
        std::uniform_int_distribution<int> dist(min, max);
        return dist(rng);
    }

    float GetFloatFromUniformDist(float min, float max) {
        std::uniform_real_distribution<float> dist(min, max);
        return dist(rng);
    }

    float GetSignedFloatFromRangeUniform(float min, float max) {
        if (min >= max || min < 0 || max < 0) {
            throw std::out_of_range("Min and max must be positive and min must be less than max");
        }

        float toReturn = RandomNumberGenerator::Instance().GetFloatFromUniformDist(min, max);

        float coeff = RandomNumberGenerator::Instance().GetIntFromUniformDist(0, 1) == 0 ? -1.0f : 1.0f;

        return coeff*toReturn;
    }

private:
    RandomNumberGenerator() {
        static std::random_device rd;
        rng = std::mt19937(rd());
    }

    RandomNumberGenerator(const RandomNumberGenerator &) = delete;

    void operator=(RandomNumberGenerator const &)= delete;

    std::mt19937 rng;
};

#endif //PROJECT_RANDOMNUMBERGENERATOR_H
