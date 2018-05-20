//
// Created by dawid on 20.05.18.
//


#include "EvoAlgo/EvolutionaryAlgorithm.h"

void EvolutionaryAlgorithm::EvaluateCurrentGenarationAndEvolve(std::vector<double> fitness)
{
    currentGenerationFitness = fitness;
    auto bestChromos = selectBestChromosomes();
    assert(bestChromos.size() == howManySelected);
    currentGeneration = crossoverChromosomes(bestChromos);
    mutateCurrentGeneration();
}

const std::vector<Chromosome>& EvolutionaryAlgorithm::GetCurrentGeneration() const
{
    return currentGeneration;
}

std::vector<Chromosome> EvolutionaryAlgorithm::selectBestChromosomes()
{
    std::vector<std::pair<Chromosome, double>> chromosWithFitness(howManySelected);

    for (size_t i = 0; i < chromosWithFitness.size(); ++i)
    {
        chromosWithFitness.emplace_back(std::make_pair(currentGeneration[i], currentGenerationFitness[i]));
    }

    std::vector<Chromosome> toReturn(howManySelected);

    std::partial_sort(chromosWithFitness.begin(), chromosWithFitness.begin() + howManySelected,
                      chromosWithFitness.end(),
                      [](const std::pair<Chromosome, double>& left, const std::pair<Chromosome, double>& right)
                      {
                          return left.second < right.second;
                      });

    for (size_t i = 0; i < howManySelected; ++i)
    {
        toReturn.emplace_back(chromosWithFitness[i].first);
    }

    return toReturn;
}

std::vector<Chromosome> EvolutionaryAlgorithm::crossoverChromosomes(std::vector<Chromosome>& chromos)
{
    return std::vector<Chromosome>();
}

void EvolutionaryAlgorithm::mutateCurrentGeneration()
{

}
