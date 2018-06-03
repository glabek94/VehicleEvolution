//
// Created by Dawid Glab and Michal Sokolski on 07.05.18.
//

#ifndef PROJECT_CHROMOSOME_H
#define PROJECT_CHROMOSOME_H

#include <iostream>
#include <vector>
#include <stdexcept>

/*! \class Chromosome
 *  \brief Class representing Vehicle's chromosome. Used by evolutionary algorithm
 *
 *  Chromosome consists of three parts: vertices defining body (chassis) of vehicle, vertices wheels are attached to and sizes of wheels.
 *
 */
class Chromosome {
public:
    static const size_t VERTICES_NUMBER;
    static const size_t WHEELS_NUMBER;
    static const float BODY_VERTEX_MIN;
    static const float BODY_VERTEX_MAX;
    static const float WHEEL_SIZE_MIN;
    static const float WHEEL_SIZE_MAX;

    /**
     * Default constructor. Required for std.
     */
    Chromosome();

    /**
     *
     * @param bodyVertices vector of vertices defining chassis of vehicle
     * @param wheelVertices vector of vertices wheels are attached to
     * @param wheelSizes vector of wheels sizes
     */
    Chromosome(std::vector<std::pair<float, float>>& bodyVertices,
               std::vector<int>& wheelVertices,
               std::vector<float>& wheelSizes);

    /**
     *
     * @return vector of vertices defining chassis of vehicle
     */
    const std::vector<std::pair<float, float>>& getBodyVertices() const;

/**
     *
     * @return vector of vertices wheels are attached to
     */
    const std::vector<int>& getWheelVertices() const;

    /**
     *
     * @return vector of wheels sizes
     */
    const std::vector<float>& getWheelSizes() const;

private:
    std::vector<std::pair<float, float>> bodyVertices;
    std::vector<int> wheelVertices;
    std::vector<float> wheelSizes;
};


#endif //PROJECT_CHROMOSOME_H
