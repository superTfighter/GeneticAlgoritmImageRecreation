#pragma once
#include <SFML/Graphics.hpp>
#include "Utility.h"
#include "math.h"

class GeneticEllipse
{
public:
    static sf::Vector2f pictureSize;
    static GeneticEllipse getRandomEllipse();

    GeneticEllipse();
    GeneticEllipse(sf::Vector2f const &_position, sf::Vector2f const &_size, float const &_angle, sf::Color const &_color);
    
    void computeFocus();
    void move(sf::Vector2f _translation);
    void rotate(float _angle);
    void scale(sf::Vector2f _factor);
    Color color;
    int Fitness;

    void addToImage(sf::Image &image);

    //Returns a chromosome vector => position.x,position.y,size.x,size.y,angle,color as int
    std::vector<float> GetChromosomeVector();
    void setChromosome(std::vector<float> ChromosomeVector);

    std::string toString();

    bool operator<(const GeneticEllipse &str) const
    {
        return (Fitness < str.Fitness);
    }

    bool operator>(const GeneticEllipse &str) const
    {
        return (Fitness > str.Fitness);
    }

private:
    Vector2f position;
    Vector2f size;
    float angle;

    Vector2f F1;
    Vector2f F2;

    float a;
};