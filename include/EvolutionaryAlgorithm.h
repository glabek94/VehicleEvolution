//
// Created by dawid on 20.05.18.
//

#ifndef PROJECT_EVOLUTIONARYALGORITHM_H
#define PROJECT_EVOLUTIONARYALGORITHM_H

#include <RandomNumberGenerator.h>
#include <boost/algorithm/clamp.hpp>
#include <boost/assert.hpp>
#include <algorithm>
#include "Chromosome.h"

class EvolutionaryAlgorithm {
public:
    EvolutionaryAlgorithm(size_t genSize, size_t selectionSize, float mutationProbability);

    void EvaluateCurrentGenarationAndEvolve(std::vector<float> fitness);

    const std::vector<Chromosome> &GetCurrentGeneration() const;

private:
    const size_t generationSize;
    const size_t howManySelected;
    float mutationProb;
    std::vector<Chromosome> currentGeneration;
    std::vector<float> currentGenerationFitness;

    std::vector<Chromosome> selectBestChromosomes();

    std::vector<Chromosome> crossoverChromosomesForNewGeneration(std::vector<Chromosome> &chromos);

    Chromosome crossoverChromos(const Chromosome &first, const Chromosome &second);

    std::vector<Chromosome> crossoverChromosMichalewicz(const Chromosome &first, const Chromosome &second);

    Chromosome mutateChromo(const Chromosome &chromo);

    void mutateCurrentGeneration();

    std::vector<Chromosome> generatePopulation();

    Chromosome generateRandomChromosme();
};


#endif //PROJECT_EVOLUTIONARYALGORITHM_H
