//
// Created by dawid on 20.05.18.
//

#ifndef PROJECT_RANDOMNUMBERGENERATOR_H
#define PROJECT_RANDOMNUMBERGENERATOR_H

#include <random>
#include <algorithm>

class RandomNumberGenerator
{
public:
    static RandomNumberGenerator& Instance()
    {
        static RandomNumberGenerator singleton;
        return singleton;
    }

    double GetFromNormalDist(double mean = 0.0, double deviation = 1.0)
    {
        std::normal_distribution<> dist(mean, deviation);
        return dist(rng);
    }

    int GetIntFromUniformDist(int min, int max)
    {
        std::uniform_int_distribution<int> dist(min, max);
        return dist(rng);
    }

    double GetDoubleFromUniformDist(double min, double max)
    {
        std::uniform_real_distribution<double> dist(min, max);
        return dist(rng);
    }

private:
    RandomNumberGenerator()
    {
        static std::random_device rd;
        rng = std::mt19937(rd());
    }

    RandomNumberGenerator(const RandomNumberGenerator&) = delete;

    void operator=(RandomNumberGenerator const&)= delete;

    std::mt19937 rng;
};

#endif //PROJECT_RANDOMNUMBERGENERATOR_H
