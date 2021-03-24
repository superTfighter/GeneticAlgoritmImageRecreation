#include "GeneticAlgoritm.h"

GeneticAlgoritm::GeneticAlgoritm()
{
    generation = 0;
}

void GeneticAlgoritm::InitPopulation(std::vector<GeneticEllipse> &population, std::vector<GeneticEllipse> &buffer)
{
    population.clear();
    buffer.clear();

    population.reserve(GA_POPSIZE);
    buffer.reserve(GA_POPSIZE);

    //unsigned int numThreads = std::thread::hardware_concurrency();
    //std::vector<std::thread> thread_pool;

    //thread_pool.reserve(numThreads);

    //int blockSize = GA_POPSIZE / numThreads;

    //for (size_t i = 0; i < numThreads; i++)
    //{

    //    if (i == numThreads - 1)
    //    {
    //        //Start,end,pop,buff
    //        thread_pool.push_back(std::thread(&GeneticAlgoritm::ParallelInitPopulation, this, i * blockSize, GA_POPSIZE, std::ref(population),std::ref(buffer)));
    //    }
    //    else
    //    {
    //        thread_pool.push_back(std::thread(&GeneticAlgoritm::ParallelInitPopulation, this, i * blockSize, (i + 1) * blockSize, std::ref(population), std::ref(buffer)));
    //    }

    //}

    //for (size_t i = 0; i < numThreads; i++)
    //{
    //    if(thread_pool[i].joinable())
    //    {
    //        try
    //        {
    //            thread_pool[i].join();
    //        }
    //        catch (const std::exception& e )
    //        {
    //            std::cout << e.what() << std::endl;
    //        }
    //        
    //    }
    //        
    //}

    for (int i = 0; i < GA_POPSIZE; i++)
    {
        GeneticEllipse e = GeneticEllipse::getRandomEllipse();

        population.push_back(e);
        buffer.push_back(GeneticEllipse());
    }
}

void GeneticAlgoritm::CalculateFitness(std::vector<GeneticEllipse>& population, ImageContainer GoalImageContainer, ImageContainer BaseImageContainer)
{
    //unsigned int numThreads = std::thread::hardware_concurrency();
    //std::vector<std::thread> thread_pool;

    //thread_pool.reserve(numThreads);

    //int blockSize = GA_POPSIZE / numThreads;

    //for (size_t i = 0; i < numThreads; i++)
    //{

    //    if (i == numThreads - 1)
    //    {
    //        //Start,end,pop,buff
    //        thread_pool.push_back(std::thread(&GeneticAlgoritm::ParallelCalcFitness, this, i * blockSize, GA_POPSIZE, std::ref(population),GoalImageContainer,BaseImageContainer));
    //    }
    //    else
    //    {
    //        thread_pool.push_back(std::thread(&GeneticAlgoritm::ParallelCalcFitness, this, i * blockSize, (i + 1)  * blockSize, std::ref(population), GoalImageContainer, BaseImageContainer));
    //    }

    //}

    //for (size_t i = 0; i < numThreads; i++)
    //{
    //    if (thread_pool[i].joinable())
    //    {
    //        try
    //        {
    //            thread_pool[i].join();
    //        }
    //        catch (const std::exception& e)
    //        {
    //            std::cout << e.what() << std::endl;
    //        }

    //    }

    //}

     //INIT STATE
    sf::Image CopyImage;

    for (int i = 0; i < GA_POPSIZE; i++)
    {
        CopyImage.create(GeneticEllipse::pictureSize.x, GeneticEllipse::pictureSize.y, BaseImageContainer.getPixelData());
        population[i].addToImage(CopyImage);

        ImageContainer CopyImageContainer(CopyImage);
        
        population[i].Fitness = 0;

        //ON CPU
        //for (int j = 0; j < CopyImageContainer.width * CopyImageContainer.height * 4; j++)
        //{
        //    population[i].Fitness += abs((float)GoalImageContainer.getPixelData()[j] - (float)CopyImageContainer.getPixelData()[j]);
        //}

        
        //ON GPU
        std::vector<float> fitness = CLAlgoritm.FitnessCalculation(CopyImageContainer.getPixelData(), GoalImageContainer.getPixelData(), CopyImageContainer.width, CopyImageContainer.height);
        
        for (int j = 0; j < CopyImageContainer.width * CopyImageContainer.height * 4; j++)
        {
            population[i].Fitness += fitness[j];
        }
    }
}

void GeneticAlgoritm::SortByFitness(std::vector<GeneticEllipse> &population)
{
    //SORT VECTOR DESCENDING
    std::sort(population.begin(), population.end());
}

void GeneticAlgoritm::Mate(std::vector<GeneticEllipse> &population, std::vector<GeneticEllipse> &buffer)
{
    const int esize = (int)(GA_POPSIZE * GA_ELITRATE);

    Elitism(population, buffer, esize);

    for (int i = esize; i < GA_POPSIZE; i++)
    {
        Crossover(buffer[i], population);

        //Random number betweeen 0 and 1
        float chance = (float)(rand() % 101) / 100;

        if (chance <= GA_MUTATIONRATE)
            Mutate(buffer[i]);
    }

    //unsigned int numThreads = std::thread::hardware_concurrency();
    //std::vector<std::thread> thread_pool;
    //thread_pool.reserve(numThreads);

    //thread_pool.push_back(std::thread(&GeneticAlgoritm::Elitism, this, std::ref(population), std::ref(buffer),esize));

    //int blockSize = (GA_POPSIZE - esize) / (numThreads -1);

    //for (size_t i = 1; i < numThreads; i++)
    //{
    //    if (i == numThreads - 1)
    //    {
    //        //Start,end,pop,buff
    //        thread_pool.push_back(std::thread(&GeneticAlgoritm::ParallelCrossoverAndMutate, this, i * blockSize, GA_POPSIZE - esize, std::ref(population), std::ref(buffer)));
    //    }
    //    else
    //    {
    //        thread_pool.push_back(std::thread(&GeneticAlgoritm::ParallelCrossoverAndMutate, this, i * blockSize, (i + 1) * blockSize, std::ref(population), std::ref(buffer)));
    //    }
    //}   

    //for (size_t i = 0; i < numThreads; i++)
    //{
    //    thread_pool[i].join();
    //}
}

void GeneticAlgoritm::Swap(std::vector<GeneticEllipse> &population, std::vector<GeneticEllipse> &buffer)
{
    population.swap(buffer);
}

void GeneticAlgoritm::Elitism(std::vector<GeneticEllipse> &population, std::vector<GeneticEllipse> &buffer, int esize)
{
    for (int i = 0; i < esize; i++)
    {
        buffer[i] = population[i];
    }
}

void GeneticAlgoritm::Crossover(GeneticEllipse &c, std::vector<GeneticEllipse> &population)
{
    int parent1i = rand() % (GA_POPSIZE);
    int parent2i = rand() % (GA_POPSIZE);

    int crossoveri = rand() % 6;

    std::vector<float> p1Chromosomes = population[parent1i].GetChromosomeVector();
    std::vector<float> p2Chromosomes = population[parent2i].GetChromosomeVector();

    std::vector<float> newChromosomes = {0, 0, 0, 0, 0};

    for (int i = 0; i < 5; i++)
    {
        if (i < crossoveri)
        {
            newChromosomes[i] = p1Chromosomes[i];
        }
        else
        {
            newChromosomes[i] = p2Chromosomes[i];
        }
    }

    if(crossoveri == 5)
    {
        c.color = population[parent1i].color;

    }
    else
    {
        c.color = population[parent2i].color;
    }

    c.setChromosome(newChromosomes);
}

void GeneticAlgoritm::Mutate(GeneticEllipse &c)
{
    int mutatei = rand() % 6;

    switch (mutatei)
    {
    case 0:
        c.move(randVector2f(Vector2f(-5, -5), Vector2f(5, 5)));
        break;
    case 1:
        c.move(randVector2f(Vector2f(-5, -5), Vector2f(5, 5)));
        break;
    case 2:
        c.scale(randVector2f(Vector2f(-5, -5), Vector2f(5, 5)));
        break;
    case 3:
        c.scale(randVector2f(Vector2f(-5, -5), Vector2f(5, 5)));
        break;
    case 4:
        c.rotate(randfloat(-5, 5));
        break;
    case 5:

        c.color.r += randfloat(-10, 10);
        c.color.g += randfloat(-10, 10);
        c.color.b += randfloat(-10, 10);
        c.color.a += randfloat(-10, 10);
        break;
    }
}

void GeneticAlgoritm::ParallelInitPopulation(int start, int end, std::vector<GeneticEllipse>& population, std::vector<GeneticEllipse>& buffer)
{
    for (int i = start; i < end; i++)
    {
        GeneticEllipse e = GeneticEllipse::getRandomEllipse();

        population.push_back(e);
        buffer.push_back(GeneticEllipse());
    }

}

void GeneticAlgoritm::ParallelCrossoverAndMutate(int start, int end, std::vector<GeneticEllipse>& population, std::vector<GeneticEllipse>& buffer)
{
    for (int i = start; i < end; i++)
    {
        Crossover(buffer[i], population);

        //Random number betweeen 0 and 1
        float chance = (float)(rand() % 101) / 100;

        if (chance <= GA_MUTATIONRATE)
            Mutate(buffer[i]);
    }

}

void GeneticAlgoritm::ParallelCalcFitness(int start, int end, std::vector<GeneticEllipse>& population, ImageContainer GoalImageContainer, ImageContainer BaseImageContainer)
{
   
    //INIT STATE
    sf::Image CopyImage;

    for (int i = start; i < end; i++)
    {
        population[i].Fitness = 0;
        CopyImage.create(GeneticEllipse::pictureSize.x, GeneticEllipse::pictureSize.y, BaseImageContainer.getPixelData());
        population[i].addToImage(CopyImage);
        ImageContainer CopyImageContainer(CopyImage);
                
        //ON GPU
        std::vector<float> fitness = CLAlgoritm.FitnessCalculation(CopyImageContainer.getPixelData(), GoalImageContainer.getPixelData(), CopyImageContainer.width, CopyImageContainer.height);
        
        for (int j = 0; j < CopyImageContainer.width * CopyImageContainer.height * 4; j++)
        {
            population[i].Fitness += fitness[j];
        }
    }
}
