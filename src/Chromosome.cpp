//
// Created by dawid on 07.05.18.
//

#include "Chromosome.h"

const int Chromosome::VERTICES_NUMBER(6);
const int Chromosome::WHEELS_NUMBER(2);
const float Chromosome::BODY_VERTEX_MIN(0.0);
const float Chromosome::BODY_VERTEX_MAX(4.0);
const float Chromosome::WHEEL_SIZE_MIN(0.25);
const float Chromosome::WHEEL_SIZE_MAX(1.5);

Chromosome::Chromosome() : bodyVertices{VERTICES_NUMBER}, wheelVertices{WHEELS_NUMBER}, wheelSizes{WHEELS_NUMBER}
{
}

Chromosome::Chromosome(std::vector<std::pair<float, float>>& bodyVertices, std::vector<int>& wheelVertices,
                       std::vector<float>& wheelSizes) : Chromosome{}
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

const std::vector<std::pair<float, float>>& Chromosome::getBodyVertices() const
{
    return bodyVertices;
}

const std::vector<int>& Chromosome::getWheelVertices() const
{
    return wheelVertices;
}

const std::vector<float>& Chromosome::getWheelSizes() const
{
    return wheelSizes;
}
