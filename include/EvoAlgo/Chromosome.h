//
// Created by dawid on 07.05.18.
//

#ifndef PROJECT_CHROMOSOME_H
#define PROJECT_CHROMOSOME_H

#include <iostream>
#include <vector>

class Chromosome
{
public:
    static constexpr int VERTICES_NUMBER = 6;
    static constexpr int WHEELS_NUMBER = 2;
    static constexpr double BODY_VERTEX_MAX = 1.0;
    static constexpr double WHEEL_SIZE_MAX = 1.0;

    Chromosome();
    Chromosome(std::vector<std::pair<double, double>>& bodyVertices,
               std::vector<int>& wheelVertices,
               std::vector<double>& wheelSizes);

    const std::vector<std::pair<double, double>>& getBodyVertices() const;

    const std::vector<int>& getWheelVertices() const;

    const std::vector<double>& getWheelSizes() const;

private:
    std::vector<std::pair<double, double>> bodyVertices;
    std::vector<int> wheelVertices;
    std::vector<double> wheelSizes;
};


#endif //PROJECT_CHROMOSOME_H
