#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Image.hpp>
#include "ImageContainer.h"
#include "GeneticAlgoritm.h"
#include "GeneticEllipse.h"
#include "Utility.h"
#include<iostream>
#include<string>
#include<fstream>
#include <chrono>


class Display
{
public:

    Display(std::string path);
    Display(std::string path,sf::RenderWindow *_window);
    void Render();
    void HandleEvents();

    void DoWork();

private:

    // Render properties
    sf::RenderWindow *window;
    sf::Image GoalImage;
    sf::Sprite GoalImageSprite;
    sf::Texture GoalImageTexture;
    
    sf::Image BaseImage;
    sf::Image CopyImage;
	sf::Sprite CopyImageSprite;
	sf::Texture CopyImageTexture;

    sf::Font font; // Need to be loaded in!!

    sf::Text GenerationText;
    sf::Text EllapsedTimeText;

    ImageContainer GoalContainer;
    ImageContainer BaseContainer;

    std::chrono::steady_clock::time_point simulationStartTime;


    // Genetic Algoritm properties
    std::vector<GeneticEllipse> population;
    std::vector<GeneticEllipse> buffer;
    
    GeneticAlgoritm algoritm;

    void setCopySprite();
    void setImageScale(sf::Sprite& sprite);
    void handleResizeEvent();

    void setTexts();
};