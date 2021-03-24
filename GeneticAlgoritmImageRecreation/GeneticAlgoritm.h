#pragma once
#include <vector>
#include "GeneticEllipse.h"
#include <algorithm>
#include "ImageContainer.h"
#include "OpenCL.h"
#include <thread>


class GeneticAlgoritm
{
public:
    GeneticAlgoritm();

    int generation;

    //Happening on GPU - Most computational demand
    void CalculateFitness(std::vector<GeneticEllipse> &population, ImageContainer GoalImageContainer, ImageContainer BaseImageContainer);
    
    void Mate(std::vector<GeneticEllipse> &population, std::vector<GeneticEllipse> &buffer);
    void InitPopulation(std::vector<GeneticEllipse>& population, std::vector<GeneticEllipse>& buffer);
  
    void Swap(std::vector<GeneticEllipse> &population, std::vector<GeneticEllipse> &buffer);
    void SortByFitness(std::vector<GeneticEllipse>& population);

private:
    const int GA_POPSIZE = 25;
    const float GA_ELITRATE = 0.10f;
    const float GA_MUTATIONRATE = 0.50f;

    OpenCL CLAlgoritm;

    void Crossover(GeneticEllipse &c, std::vector<GeneticEllipse> &population);
    void Mutate(GeneticEllipse &c);

    // Parallel Functions
    void Elitism(std::vector<GeneticEllipse>& population, std::vector<GeneticEllipse>& buffer, int esize);

    void ParallelInitPopulation(int start, int end,std::vector<GeneticEllipse>& population, std::vector<GeneticEllipse>& buffer);
    void ParallelCrossoverAndMutate(int start, int end, std::vector<GeneticEllipse>& population, std::vector<GeneticEllipse>& buffer);

    // + GPU
    void ParallelCalcFitness(int start, int end, std::vector<GeneticEllipse>& population,ImageContainer GoalImageContainer, ImageContainer BaseImageContainer);
};
