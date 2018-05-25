//
// Created by dawid on 20.05.18.
//


#include "EvoAlgo/EvolutionaryAlgorithm.h"

void EvolutionaryAlgorithm::EvaluateCurrentGenarationAndEvolve(std::vector<float> fitness)
{
    currentGenerationFitness = fitness;
    auto bestChromos = selectBestChromosomes();
    assert(bestChromos.size() == howManySelected);
    currentGeneration = crossoverChromosomesForNewGeneration(bestChromos);
    mutateCurrentGeneration();
    currentGenerationFitness.clear();
}

const std::vector<Chromosome>& EvolutionaryAlgorithm::GetCurrentGeneration() const
{
    return currentGeneration;
}

std::vector<Chromosome> EvolutionaryAlgorithm::selectBestChromosomes()
{
    std::vector<std::pair<Chromosome, float>> chromosWithFitness;

    for (size_t i = 0; i < currentGeneration.size(); ++i)
    {
        chromosWithFitness.emplace_back(std::make_pair(currentGeneration[i], currentGenerationFitness[i]));
    }

    std::vector<Chromosome> toReturn;

    std::partial_sort(chromosWithFitness.begin(), chromosWithFitness.begin() + howManySelected,
                      chromosWithFitness.end(),
                      [](const std::pair<Chromosome, float>& left, const std::pair<Chromosome, float>& right)
                      {
                          return left.second > right.second;
                      });

    for (size_t i = 0; i < howManySelected; ++i)
    {
        toReturn.emplace_back(chromosWithFitness[i].first);
    }

    return toReturn;
}

std::vector<Chromosome> EvolutionaryAlgorithm::crossoverChromosomesForNewGeneration(std::vector<Chromosome>& chromos)
{
    std::vector<Chromosome> toReturn;

    while (toReturn.size() < generationSize)
    {
        int first = RandomNumberGenerator::Instance().GetIntFromUniformDist(0, chromos.size() - 1);
        int second;
        do
        {
            second = RandomNumberGenerator::Instance().GetIntFromUniformDist(0, chromos.size() - 1);
        } while (first == second);

        toReturn.emplace_back(crossoverChromos(chromos[first], chromos[second]));
    }

    return toReturn;
}

void EvolutionaryAlgorithm::mutateCurrentGeneration()
{
    std::vector<Chromosome> newGeneration = currentGeneration;
    currentGeneration.clear();

    for (const auto& chromo:newGeneration)
    {
        currentGeneration.emplace_back(mutateChromo(chromo));
    }
}

Chromosome EvolutionaryAlgorithm::crossoverChromos(const Chromosome& first, const Chromosome& second)
{
    std::vector<std::pair<float, float>> newBodyVertices;
    std::vector<int> newWheelVertices;
    std::vector<float> newWheelSizes;

    int cutAt = RandomNumberGenerator::Instance().GetIntFromUniformDist(0, Chromosome::VERTICES_NUMBER);

    newBodyVertices.insert(newBodyVertices.begin(), first.getBodyVertices().begin(),
                           first.getBodyVertices().begin() + cutAt);
    newBodyVertices.insert(newBodyVertices.end(), second.getBodyVertices().begin() + cutAt,
                           second.getBodyVertices().end());

    cutAt = RandomNumberGenerator::Instance().GetIntFromUniformDist(0, Chromosome::WHEELS_NUMBER);

    newWheelVertices.insert(newWheelVertices.begin(), first.getWheelVertices().begin(),
                            first.getWheelVertices().begin() + cutAt);
    newWheelVertices.insert(newWheelVertices.end(), second.getWheelVertices().begin() + cutAt,
                            second.getWheelVertices().end());

    cutAt = RandomNumberGenerator::Instance().GetIntFromUniformDist(0, Chromosome::WHEELS_NUMBER);

    newWheelSizes.insert(newWheelSizes.begin(), first.getWheelSizes().begin(), first.getWheelSizes().begin() + cutAt);
    newWheelSizes.insert(newWheelSizes.end(), second.getWheelSizes().begin() + cutAt, second.getWheelSizes().end());

    return Chromosome(newBodyVertices, newWheelVertices, newWheelSizes);
}

Chromosome EvolutionaryAlgorithm::mutateChromo(const Chromosome& chromo)
{
    std::vector<std::pair<float, float>> newBodyVertices;
    std::vector<int> newWheelVertices;
    std::vector<float> newWheelSizes;

    for (auto const& vertex:chromo.getBodyVertices())
    {
        if (RandomNumberGenerator::Instance().GetDoubleFromUniformDist(0.0, 1.0) < mutationProb)
        {
            float v1 = vertex.first +
                        RandomNumberGenerator::Instance().GetFromNormalDist(0.0, Chromosome::BODY_VERTEX_MAX / 2);
            float v2 = vertex.second +
                        RandomNumberGenerator::Instance().GetFromNormalDist(0.0, Chromosome::BODY_VERTEX_MAX / 2);

            v1 = boost::algorithm::clamp(v1, -Chromosome::BODY_VERTEX_MAX, Chromosome::BODY_VERTEX_MAX);
            v2 = boost::algorithm::clamp(v2, -Chromosome::BODY_VERTEX_MAX, Chromosome::BODY_VERTEX_MAX);

            newBodyVertices.emplace_back(std::make_pair(v1, v2));
        }
        else
        {
            newBodyVertices.emplace_back(vertex);
        }
    }

    for (const auto& vertex:chromo.getWheelVertices())
    {
        if (RandomNumberGenerator::Instance().GetDoubleFromUniformDist(0.0, 1.0) < mutationProb)
        {
            int newWheelVertex = vertex + static_cast<int>(RandomNumberGenerator::Instance().GetFromNormalDist(0,
                                                                                                               Chromosome::VERTICES_NUMBER /
                                                                                                               2));

            newWheelVertex = boost::algorithm::clamp(newWheelVertex, 0, Chromosome::WHEELS_NUMBER - 1);

            newWheelVertices.emplace_back(newWheelVertex);
        }
        else
        {
            newWheelVertices.emplace_back(vertex);
        }
    }

    for (const auto& size:chromo.getWheelSizes())
    {
        if (RandomNumberGenerator::Instance().GetDoubleFromUniformDist(0.0, 1.0) < mutationProb)
        {
            float newSize = size + RandomNumberGenerator::Instance().GetFromNormalDist(0,
                                                                                        Chromosome::WHEEL_SIZE_MAX /
                                                                                        2);

            newSize = boost::algorithm::clamp(newSize, 0.0, Chromosome::WHEEL_SIZE_MAX);

            newWheelSizes.emplace_back(newSize);
        }
        else
        {
            newWheelSizes.emplace_back(size);
        }
    }


    return Chromosome(newBodyVertices, newWheelVertices, newWheelSizes);
}

EvolutionaryAlgorithm::EvolutionaryAlgorithm(size_t genSize, size_t selectionSize, float mutationProbability)
        : generationSize{genSize},
          howManySelected{selectionSize},
          mutationProb(mutationProbability),
          currentGeneration(
                  generatePopulation())
{
}

std::vector<Chromosome> EvolutionaryAlgorithm::generatePopulation()
{
    std::vector<Chromosome> toReturn;

    while (toReturn.size() < generationSize)
    {
        toReturn.emplace_back(generateRandomChromosme());
    }

    return toReturn;
}

Chromosome EvolutionaryAlgorithm::generateRandomChromosme()
{
    std::vector<std::pair<float, float>> newBodyVertices;
    std::vector<int> newWheelVertices;
    std::vector<float> newWheelSizes;

    for (int i = 0; i < Chromosome::VERTICES_NUMBER; ++i)
    {
        float v1 = RandomNumberGenerator::Instance().GetDoubleFromUniformDist(-Chromosome::BODY_VERTEX_MAX,
                                                                               Chromosome::BODY_VERTEX_MAX);
        float v2 = RandomNumberGenerator::Instance().GetDoubleFromUniformDist(-Chromosome::BODY_VERTEX_MAX,
                                                                               Chromosome::BODY_VERTEX_MAX);

        newBodyVertices.emplace_back(std::make_pair(v1, v2));
    }

    for (int i = 0; i < Chromosome::WHEELS_NUMBER; ++i)
    {
        float size = RandomNumberGenerator::Instance().GetDoubleFromUniformDist(0.0, Chromosome::WHEEL_SIZE_MAX);

        int vertex = RandomNumberGenerator::Instance().GetIntFromUniformDist(0, Chromosome::VERTICES_NUMBER - 1);

        newWheelVertices.emplace_back(vertex);
        newWheelSizes.emplace_back(size);
    }

    return Chromosome(newBodyVertices, newWheelVertices, newWheelSizes);
}
