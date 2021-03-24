#pragma once
#include <SFML/Graphics/Image.hpp>
#include <algorithm>
#include <iterator>
#include <iostream>


class ImageContainer
{
public:
	ImageContainer();
	ImageContainer(sf::Image& image);

	~ImageContainer();

	// Return a continous array of pixels, 4 data/pixel(RGBA)
	const sf::Uint8* getPixelData();
	
	int width;
	int height;


private:
	const sf::Uint8* pixelArray;
};
