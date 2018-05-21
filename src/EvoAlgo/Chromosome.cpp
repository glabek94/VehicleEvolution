//
// Created by dawid on 07.05.18.
//

#include "EvoAlgo/Chromosome.h"

constexpr double Chromosome::BODY_VERTEX_MAX;
constexpr double Chromosome::WHEEL_SIZE_MAX;

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

const std::vector<std::pair<double, double>>& Chromosome::getBodyVertices() const
{
    return bodyVertices;
}

const std::vector<int>& Chromosome::getWheelVertices() const
{
    return wheelVertices;
}

const std::vector<double>& Chromosome::getWheelSizes() const
{
    return wheelSizes;
}
