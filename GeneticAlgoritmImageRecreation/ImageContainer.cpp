#include "ImageContainer.h"

ImageContainer::ImageContainer()
{
    width = 0;
    height = 0;
}

ImageContainer::ImageContainer(sf::Image& image)
{
    width = image.getSize().x;
    height = image.getSize().y;

    pixelArray = image.getPixelsPtr();

}

ImageContainer::~ImageContainer()
{
    //delete(pixelArray);
}

const sf::Uint8* ImageContainer::getPixelData()
{
    return pixelArray;
}
