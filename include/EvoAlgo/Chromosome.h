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
    static const int VERTICES_NUMBER = 6;
    static const int WHEELS_NUMBER = 2;

    Chromosome(std::vector<std::pair<double, double>>& bodyVertices,
               std::vector<int>& wheelVertices,
               std::vector<double>& wheelSizes);

private:
    Chromosome();
    std::vector<std::pair<double, double>> bodyVertices;
    std::vector<int> wheelVertices;
    std::vector<double> wheelSizes;
};


#endif //PROJECT_CHROMOSOME_H
