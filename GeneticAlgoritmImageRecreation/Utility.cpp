#include "Utility.h"
#include "math.h"

Vector2f randVector2f(Vector2f const &among)
{
	return Vector2f((float)(rand() / (float)RAND_MAX) * among.x, (float)(rand() / (float)RAND_MAX) * among.y);
}

Vector2f randVector2f(Vector2f const &min, Vector2f const &max)
{
	return Vector2f((float)(rand() / (float)RAND_MAX) * (max.x - min.x) + min.x, (float)(rand() / (float)RAND_MAX) * (max.y - min.y) + min.y);
}

float randfloat(float const &among)
{
	return (float)rand() / RAND_MAX * among;
}

float randfloat(float const &min, float const &max)
{
	return (float)rand() / RAND_MAX * (max - min) + min;
}

Color randColor()
{
	return sf::Color((float)rand() / RAND_MAX * 255, (float)rand() / RAND_MAX * 255, (float)rand() / RAND_MAX * 255, (float)rand() / RAND_MAX * 255);
}

Color randColor(Color base, float const &max, float const &min)
{
	return sf::Color(base.r + (int)randfloat(min, max), base.g + (int)randfloat(min, max), base.b + (int)randfloat(min, max), base.a + (int)randfloat(min, max));
}

Image createCloneImage(Image base)
{
	sf::Image toReturn;
	toReturn.create(base.getSize().x, base.getSize().y, sf::Color::Black);

	for (int x = 0; x < base.getSize().x; x++)
	{
		for (int y = 0; y < base.getSize().y; y++)
		{
			toReturn.setPixel(x, y, Color(base.getPixel(x, y).r, base.getPixel(x, y).g, base.getPixel(x, y).b));
		}
	}

	return toReturn;
}

void coutVector2f(Vector2f const &u)
{
	cout << u.x << " ; " << u.y << endl;
}

float rad(float deg)
{
	return deg * 3.14159265359 / 180.f;
}

float Distance(Vector2f const &u, Vector2f const &v)
{
	return sqrt(pow(u.x - v.x, 2) + pow(u.y - v.y, 2));
}

void drawPoint(Vector2f const &p, float const &radius, RenderWindow &window, Color const &color)
{
	CircleShape c(radius);
	c.setFillColor(color);
	c.setPosition(p);
	c.setOrigin(radius, radius);
	window.draw(c);
}

void parallelSum(std::vector<float>& sumArray, int startIndex, int endIndex)
{
	for(int i = startIndex; i < endIndex; i++)
	{
		if(i != startIndex)
		{
			sumArray[i] += sumArray[i - 1];
		}
	}
}