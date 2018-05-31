//
// Created by dawid on 07.05.18.
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
    static constexpr int VERTICES_NUMBER = 6;
    static constexpr int WHEELS_NUMBER = 2;
    static constexpr float BODY_VERTEX_MIN = 0.0;
    static constexpr float BODY_VERTEX_MAX = 4.0;
    static constexpr float WHEEL_SIZE_MIN = 0.25;
    static constexpr float WHEEL_SIZE_MAX = 1.5;

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
