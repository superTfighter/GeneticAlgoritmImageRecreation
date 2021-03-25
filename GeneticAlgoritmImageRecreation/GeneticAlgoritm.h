#pragma once
#include <vector>
#include "GeneticEllipse.h"
#include <algorithm>
#include "ImageContainer.h"
#include "OpenCL.h"
#include <thread>
#include "GeneticImage.h"


class GeneticAlgoritm
{
public:
    GeneticAlgoritm();

    int generation;

    void CalculateFitness(ImageContainer GoalImageContainer);   
    
    void Mate();
    void InitPopulation();
  
    void Swap();
    void SortByFitness();
    GeneticImage getBestImage();

private:
    const int GA_POPSIZE = 1000;
    const float GA_ELITRATE = 0.25f;
    const float GA_MUTATIONRATE = 0.25f;

    std::vector<GeneticImage> population;
    std::vector<GeneticImage> buffer;

    //OpenCL CLAlgoritm;

    void Crossover(GeneticImage &i);

    void Mutate(GeneticImage& i);

    // Parallel Functions
    void Elitism(int esize);

    void ParallelInitPopulation(int start, int end,std::vector<GeneticImage>& population, std::vector<GeneticImage>& buffer);
    void ParallelCrossoverAndMutate(const int esize, int start, int end, std::vector<GeneticImage>& population, std::vector<GeneticImage>& buffer);

    // + GPU
    void ParallelCalcFitness(int start, int end, std::vector<GeneticImage>& population, ImageContainer GoalImageContainer);
};
