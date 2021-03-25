#include "GeneticAlgoritm.h"

GeneticAlgoritm::GeneticAlgoritm()
{
    generation = 0;

    population.clear();
    buffer.clear();
    population.reserve(GA_POPSIZE);
    buffer.reserve(GA_POPSIZE);
}

void GeneticAlgoritm::InitPopulation()
{   
    population.clear();
    population.clear();
    buffer.clear();
    population.reserve(GA_POPSIZE);
    buffer.reserve(GA_POPSIZE);

    std::vector<std::thread> thread_pool;
    int numberOfThreads = std::thread::hardware_concurrency();
    thread_pool.reserve(numberOfThreads);
    int blockSize = GA_POPSIZE / numberOfThreads;

    for (size_t i = 0; i < numberOfThreads; i++)
    {
        if(i == numberOfThreads - 1 )
        {
           
            thread_pool.push_back(std::thread(&GeneticAlgoritm::ParallelInitPopulation, this, blockSize * i, GA_POPSIZE, std::ref(population), std::ref(buffer)));
        }
        else
        {
            thread_pool.push_back(std::thread(&GeneticAlgoritm::ParallelInitPopulation, this, blockSize * i, blockSize * (i + 1), std::ref(population), std::ref(buffer)));
        }
    }

    for (size_t i = 0; i < thread_pool.size(); i++)
    {
        if(thread_pool[i].joinable())
            thread_pool[i].join();
    }

    //for (int i = 0; i < GA_POPSIZE; i++)
    //{
    //    GeneticImage e;        
    //    
    //    population.push_back(e);
    //    buffer.push_back(e);
    //}

}

void GeneticAlgoritm::CalculateFitness(ImageContainer GoalImageContainer)
{
    std::vector<std::thread> thread_pool;
    int numberOfThreads = std::thread::hardware_concurrency();
    thread_pool.reserve(numberOfThreads);
    int blockSize = GA_POPSIZE / numberOfThreads;

    for (size_t i = 0; i < numberOfThreads; i++)
    {
        if (i == numberOfThreads - 1)
        {
            thread_pool.push_back(std::thread(&GeneticAlgoritm::ParallelCalcFitness, this, blockSize * i, GA_POPSIZE, std::ref(population), GoalImageContainer));

        }
        else
        {
            thread_pool.push_back(std::thread(&GeneticAlgoritm::ParallelCalcFitness, this, blockSize * i, blockSize * (i + 1), std::ref(population), GoalImageContainer));
        }
    }

    for (size_t i = 0; i < thread_pool.size(); i++)
    {
        if (thread_pool[i].joinable())
            thread_pool[i].join();
    }

    //for (int i = 0; i < GA_POPSIZE; i++)
    //{     
    //    population[i].Fitness = 0;

    //    //ON CPU
    //    for (int j = 0; j < population[i].imageContainer.width * population[i].imageContainer.height * 4; j++)
    //    {
    //        population[i].Fitness += abs((float)GoalImageContainer.getPixelData()[j] - (float)population[i].imageContainer.getPixelData()[j]);
    //    }

    //    
    //    //ON GPU
    //   /* std::vector<float> fitness = CLAlgoritm.FitnessCalculation(population[i].imageContainer.getPixelData(), GoalImageContainer.getPixelData(), GeneticEllipse::pictureSize.x, GeneticEllipse::pictureSize.y);
    //    
    //    for (int j = 0; j < GeneticEllipse::pictureSize.x * GeneticEllipse::pictureSize.y * 4; j++)
    //    {
    //        population[i].Fitness += fitness[j];
    //    }*/
    //}
}

void GeneticAlgoritm::SortByFitness()
{
    //SORT VECTOR DESCENDING
    std::sort(population.begin(), population.end());

    std::cout << population[0].Fitness << " : " << population[population.size() - 1].Fitness << std::endl;
}

GeneticImage GeneticAlgoritm::getBestImage()
{
    return population[0]; 
}

void GeneticAlgoritm::Mate()
{
    const int esize = (int)(GA_POPSIZE * GA_ELITRATE);
    
    std::vector<std::thread> thread_pool;
    int numberOfThreads = std::thread::hardware_concurrency();
    thread_pool.reserve(numberOfThreads);

    thread_pool.push_back(std::thread(&GeneticAlgoritm::Elitism, this, esize));
    
    //Elitism(esize);

    int blockSize = (GA_POPSIZE - esize) / (numberOfThreads - 1);


    for (size_t i = 0; i < numberOfThreads - 1; i++)
    {

        if( i == (numberOfThreads - 1) - 1)
        {
            thread_pool.push_back(std::thread(&GeneticAlgoritm::ParallelCrossoverAndMutate, this, esize, blockSize * i, (GA_POPSIZE - esize), std::ref(population), std::ref(buffer)));
        }
        else
        {
            thread_pool.push_back(std::thread(&GeneticAlgoritm::ParallelCrossoverAndMutate, this, esize, blockSize * i, blockSize * (i +1), std::ref(population), std::ref(buffer)));
        }

    }

    for (size_t i = 0; i < thread_pool.size(); i++)
    {
        if (thread_pool[i].joinable())
            thread_pool[i].join();
    }

     
    //for (int i = esize; i < GA_POPSIZE; i++)
    //{
    //    Crossover(buffer[i]);

    //    //Random number betweeen 0 and 1
    //    float chance = (float)(rand() % 101) / 100;

    //    if (chance <= GA_MUTATIONRATE)
    //        Mutate(buffer[i]);
    //}
}

void GeneticAlgoritm::Swap()
{
    population.swap(buffer);
}

void GeneticAlgoritm::Elitism(int esize)
{
    for (int i = 0; i < esize; i++)
    {
        buffer[i] = population[i];
    }
}

void GeneticAlgoritm::Crossover(GeneticImage &i)
{
    int parent1i = rand() % (GA_POPSIZE);
    int parent2i = rand() % (GA_POPSIZE);
    int crossoveri = rand() % ELLIPSE_NUMBER;

    GeneticImage p1Chromosomes = population[parent1i];
    GeneticImage p2Chromosomes = population[parent2i];

    i.Crossover(p1Chromosomes, p2Chromosomes, crossoveri);
}

void GeneticAlgoritm::Mutate(GeneticImage &i)
{
    i.Mutate();
}

void GeneticAlgoritm::ParallelInitPopulation(int start, int end, std::vector<GeneticImage>& population, std::vector<GeneticImage>& buffer)
{
    for (int i = start; i < end; i++)
    {
        GeneticImage image;

        population.push_back(image);
        buffer.push_back(image);
    }

}

void GeneticAlgoritm::ParallelCrossoverAndMutate(const int esize,int start, int end, std::vector<GeneticImage>& population, std::vector<GeneticImage>& buffer)
{

    for (size_t i = start; i < end; i++)
    {
        int parent1i = 0;
        int parent2i = 0;

        while(parent1i == parent2i)
        {
            parent1i = rand() % esize;
            parent2i = rand() % esize;
        }

        int crossoveri = rand() % ELLIPSE_NUMBER;

        GeneticImage p1Chromosomes = population[parent1i];
        GeneticImage p2Chromosomes = population[parent2i];

        buffer[i].Crossover(p1Chromosomes, p2Chromosomes, crossoveri);

        //Random number betweeen 0 and 1
        float chance = (float)(rand() % 101) / 100;

        if (chance <= GA_MUTATIONRATE)
            Mutate(buffer[i]);
    }

}

void GeneticAlgoritm::ParallelCalcFitness(int start, int end, std::vector<GeneticImage>& population, ImageContainer GoalImageContainer)
{

    for (int i = start; i < end; i++)
    {
        population[i].Fitness = 0;

        for (int j = 0; j < population[i].imageContainer.width * population[i].imageContainer.height * 4; j++)
        {
            population[i].Fitness += abs((float)GoalImageContainer.getPixelData()[j] - (float)population[i].imageContainer.getPixelData()[j]);
        }

    }
  
}
