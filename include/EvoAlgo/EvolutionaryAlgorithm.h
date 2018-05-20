//
// Created by dawid on 20.05.18.
//

#ifndef PROJECT_EVOLUTIONARYALGORITHM_H
#define PROJECT_EVOLUTIONARYALGORITHM_H

#include <boost/assert.hpp>
#include <algorithm>
#include "Chromosome.h"

class EvolutionaryAlgorithm
{
public:
    EvolutionaryAlgorithm();

    void EvaluateCurrentGenarationAndEvolve(std::vector<double> fitness);

    const std::vector<Chromosome>& GetCurrentGeneration() const;

private:
    const size_t generationSize;
    const size_t howManySelected;
    std::vector<Chromosome>& currentGeneration;
    std::vector<double>& currentGenerationFitness;

    std::vector<Chromosome> selectBestChromosomes();

    std::vector<Chromosome> crossoverChromosomes(std::vector<Chromosome>& chromos);

    void mutateCurrentGeneration();
};


#endif //PROJECT_EVOLUTIONARYALGORITHM_H
