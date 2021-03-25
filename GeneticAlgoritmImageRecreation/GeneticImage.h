#pragma once
#include <vector>
#include "GeneticEllipse.h"
#include "SFML/Graphics/Image.hpp"
#include "ImageContainer.h"

#define ELLIPSE_NUMBER 10


class GeneticImage
{

public:	

    GeneticImage();
	
    // Represents the difference
    int Fitness;

	ImageContainer imageContainer;   
    sf::Image image;

    void Crossover(GeneticImage& parent1, GeneticImage parent2, int crossoverIndex);

    void Mutate();

    bool operator<(const GeneticImage& str) const
    {
        return (Fitness < str.Fitness);
    }

    bool operator>(const GeneticImage& str) const
    {
        return (Fitness > str.Fitness);
    }

private:
    std::vector<GeneticEllipse> ellipses;


};

