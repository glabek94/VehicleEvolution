//
// Created by Dawid Glab and Michal Sokolski on 20.05.18.
//

#ifndef PROJECT_EVOLUTIONARYALGORITHM_H
#define PROJECT_EVOLUTIONARYALGORITHM_H

#include <RandomNumberGenerator.h>
#include <boost/algorithm/clamp.hpp>
#include <boost/assert.hpp>
#include <algorithm>
#include "Chromosome.h"

/*! \class EvolutionaryAlgorithm
 *  \brief Class representing evolutionary algorithm
 *
 *  This implementation of evolutionary algorithm use Michalewicz crossover operator. Mutation are completely random.
 *
 */
class EvolutionaryAlgorithm {
public:
    /**
     *
     * @param genSize size of generation
     * @param selectionSize how many chromosomes from population are selected to crossover
     * @param mutationProbability probability threshold of mutation
     */
    EvolutionaryAlgorithm(size_t genSize, size_t selectionSize, float mutationProbability);

    /**
     *
     * @param fitness vector of values of fitness for current generation. i-th value is fitness for i-th chromosome from current generation
     */
    void EvaluateCurrentGenarationAndEvolve(std::vector<float> fitness);

    /**
     *
     * @return vector of chromosomes from current generation
     */
    const std::vector<Chromosome>& GetCurrentGeneration() const;

private:
    const size_t generationSize;
    const size_t howManySelected;
    float mutationProb;
    std::vector<Chromosome> currentGeneration;
    std::vector<float> currentGenerationFitness;

    std::vector<Chromosome> selectBestChromosomes();

    std::vector<Chromosome> crossoverChromosomesForNewGeneration(std::vector<Chromosome>& chromos);

    std::vector<Chromosome> crossoverChromos(const Chromosome& first, const Chromosome& second);

    Chromosome mutateChromo(const Chromosome& chromo);

    void mutateCurrentGeneration();

    std::vector<Chromosome> generatePopulation();

    Chromosome generateRandomChromosme();
};


#endif //PROJECT_EVOLUTIONARYALGORITHM_H
