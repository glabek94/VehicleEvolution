//
// Created by dawid on 07.05.18.
//

#ifndef PROJECT_CHROMOSOME_H
#define PROJECT_CHROMOSOME_H

#include <iostream>
#include <vector>
#include <stdexcept>

class Chromosome
{
public:
    static constexpr int VERTICES_NUMBER = 6;
    static constexpr int WHEELS_NUMBER = 2;
    static constexpr float BODY_VERTEX_MIN = 1.0;
    static constexpr float BODY_VERTEX_MAX = 4.0;
    static constexpr float WHEEL_SIZE_MIN = 0.25;
    static constexpr float WHEEL_SIZE_MAX = 1.0;

    Chromosome();
    Chromosome(std::vector<std::pair<float, float>>& bodyVertices,
               std::vector<int>& wheelVertices,
               std::vector<float>& wheelSizes);

    const std::vector<std::pair<float, float>>& getBodyVertices() const;

    const std::vector<int>& getWheelVertices() const;

    const std::vector<float>& getWheelSizes() const;

private:
    std::vector<std::pair<float, float>> bodyVertices;
    std::vector<int> wheelVertices;
    std::vector<float> wheelSizes;
};


#endif //PROJECT_CHROMOSOME_H
