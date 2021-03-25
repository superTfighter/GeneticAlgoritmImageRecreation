#include "GeneticImage.h"

GeneticImage::GeneticImage()
{
    this->image.create(GeneticEllipse::pictureSize.x, GeneticEllipse::pictureSize.y, sf::Color::Black);
   
	for (size_t i = 0; i < ELLIPSE_NUMBER; i++)
	{
		GeneticEllipse ellipse;        
        ellipses.push_back(ellipse);
	}

    for (size_t i = 0; i < ELLIPSE_NUMBER; i++)
    {
        ellipses[i].addToImage(image);
    }

	imageContainer = ImageContainer(this->image);
	Fitness = 0;
}

void GeneticImage::Crossover(GeneticImage& parent1, GeneticImage parent2, int crossoverIndex)
{
    this->image.create(GeneticEllipse::pictureSize.x, GeneticEllipse::pictureSize.y, sf::Color::Black);

	for (size_t i = 0; i < ellipses.size(); i++)
	{
		if (i < crossoverIndex)
			ellipses[i] = parent1.ellipses[i];
		else
			ellipses[i] = parent2.ellipses[i];

        ellipses[i].addToImage(image);
	}

    imageContainer = ImageContainer(this->image);
}

void GeneticImage::Mutate()
{
    this->image.create(GeneticEllipse::pictureSize.x, GeneticEllipse::pictureSize.y, sf::Color::Black);

	int randomNumber = rand() % ellipses.size();

	for (size_t i = 0; i < randomNumber; i++)
	{
      	ellipses[i] = GeneticEllipse();
        ellipses[i].addToImage(this->image);
	}

	for (size_t i = randomNumber; i < ellipses.size(); i++)
	{
		ellipses[i].addToImage(this->image);
	}

    imageContainer = ImageContainer(this->image);
}
