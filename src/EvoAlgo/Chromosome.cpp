//
// Created by dawid on 07.05.18.
//

#include "EvoAlgo/Chromosome.h"

Chromosome::Chromosome() : bodyVertices{VERTICES_NUMBER}, wheelVertices{WHEELS_NUMBER}, wheelSizes{WHEELS_NUMBER}
{
}

Chromosome::Chromosome(std::vector<std::pair<double, double>>& bodyVertices, std::vector<int>& wheelVertices,
                       std::vector<double>& wheelSizes) : Chromosome{}
{
    if(bodyVertices.size() != VERTICES_NUMBER)
    {
        throw std::runtime_error("Improper number of vertices");
    }

    if(wheelVertices.size() != WHEELS_NUMBER || wheelSizes.size() != WHEELS_NUMBER)
    {
        throw std::runtime_error("Improper number of wheels");
    }

    this->bodyVertices = bodyVertices;
    this->wheelVertices = wheelVertices;
    this->wheelSizes = wheelSizes;
}
