#include "GeneticEllipse.h"

sf::Vector2f GeneticEllipse::pictureSize = sf::Vector2f(0, 0);

GeneticEllipse GeneticEllipse::getRandomEllipse()
{
    return GeneticEllipse(randVector2f(GeneticEllipse::pictureSize), randVector2f(GeneticEllipse::pictureSize / 2.f), randfloat(360), randColor());
}

GeneticEllipse::GeneticEllipse()
{
}

GeneticEllipse::GeneticEllipse(Vector2f const &_position, Vector2f const &_size, float const &_angle, Color const &_color)
{
    position = _position;
    size = _size / 2.f;
    angle = _angle;
    color = _color;

    computeFocus();
}

void GeneticEllipse::computeFocus()
{
    a = max(size.x, size.y);
    float c = sqrt(size.x * size.x - size.y * size.y);
    F1 = position + Vector2f(c * cos(rad(angle)), c * sin(rad(angle)));
    F2 = position - Vector2f(c * cos(rad(angle)), c * sin(rad(angle)));
}

void GeneticEllipse::move(Vector2f _translation)
{
    position += _translation;
    computeFocus();
}

void GeneticEllipse::rotate(float _angle)
{
    angle += _angle;
    computeFocus();
}

void GeneticEllipse::scale(Vector2f _factor)
{
    size += _factor;
    computeFocus();
}

void GeneticEllipse::addToImage(Image &image)
{
    for (int i(max(0.f, position.x - a)); i < min((float)image.getSize().x, position.x + a); i++)
    {
        for (int j(max(0.f, position.y - a)); j < min((float)image.getSize().y, position.y + a); j++)
        {
            if (Distance(F1, Vector2f(i, j)) + Distance(F2, Vector2f(i, j)) < 2 * size.x)
            {
                image.setPixel(i, j, sf::Color(color.a / 255.f * color.r + (1 - color.a / 255.f) * image.getPixel(i, j).r, color.a / 255.f * color.g + (1 - color.a / 255.f) * image.getPixel(i, j).g, color.a / 255.f * color.b + (1 - color.a / 255.f) * image.getPixel(i, j).b, 255));
            }
        }
    }
}

std::string GeneticEllipse::toString()
{
    return std::to_string(position.x) + ";" + std::to_string(position.y) + ";" + std::to_string(size.x) + ";" + std::to_string(size.y) + ";" + std::to_string(angle) + ";" + std::to_string(color.toInteger());
}

std::vector<float> GeneticEllipse::GetChromosomeVector()
{
    return std::vector<float>{position.x, position.y, size.x, size.y, angle};
}

void GeneticEllipse::setChromosome(std::vector<float> ChromosomeVector)
{
    this->position = sf::Vector2f(ChromosomeVector[0], ChromosomeVector[1]);
    this->size = sf::Vector2f(ChromosomeVector[2], ChromosomeVector[3]);
    this->angle = ChromosomeVector[4];

    computeFocus();
}
