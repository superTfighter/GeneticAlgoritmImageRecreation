#include "Display.h"

Display::Display(std::string path, GeneticAlgoritm* _algoritm, sf::RenderWindow* _window)
{
	GoalImage.loadFromFile(path);
	GeneticEllipse::pictureSize = Vector2f(GoalImage.getSize().x, GoalImage.getSize().y);

	CopyImage.create(GoalImage.getSize().x, GoalImage.getSize().y, sf::Color::Black);
	GoalContainer = ImageContainer(GoalImage);

	window = _window;
	algoritm = _algoritm;
	
	GoalImageTexture.loadFromImage(GoalImage);
	GoalImageSprite.setTexture(GoalImageTexture);
	

	if (!font.loadFromFile("font.ttf"))
	{
		throw "Font cannot be loaded!";
	}
	else
	{
		GenerationText.setCharacterSize(15);
		EllapsedTimeText.setCharacterSize(15);

		GenerationText.setFont(font);
		EllapsedTimeText.setFont(font);
	}

	DisplayLoadingWindow();

	algoritm->InitPopulation();

	setCopySprite();
	handleResizeEvent();

	simulationStartTime = std::chrono::steady_clock::now();
}


void Display::DoWork()
{
	algoritm->CalculateFitness(GoalContainer);
	algoritm->SortByFitness();
	algoritm->Mate();
	algoritm->Swap();
    
	algoritm->generation++;
}

//Only this is when we want to Render an actual sf::Image
void Display::Render()
{
	setCopySprite();
	setTexts();

	window->clear(sf::Color::Black);

	window->draw(GoalImageSprite);
	window->draw(CopyImageSprite);

	window->draw(GenerationText);
	window->draw(EllapsedTimeText);

	window->setActive();
	window->display();
}

void Display::HandleEvents()
{
	sf::Event event;

	while (window->pollEvent(event))
	{
		// Request for closing the window
		if (event.type == sf::Event::Closed)
			window->close();
		else if (event.type == sf::Event::Resized)
			handleResizeEvent();
	}

}

void Display::DisplayLoadingWindow()
{
	sf::Text loadingText;

	loadingText.setFont(font);
	loadingText.setCharacterSize(30);
	loadingText.setString("Loading...");
	loadingText.setPosition((this->window->getView().getSize().x / 2) - loadingText.getGlobalBounds().width, (this->window->getView().getSize().y / 2) - loadingText.getGlobalBounds().height);

	window->clear(sf::Color::Black);
	window->draw(loadingText);

	window->setActive();
	window->display();
	


}

void Display::setCopySprite()
{
	CopyImage.copy(algoritm->getBestImage().image, 0, 0, sf::IntRect{ 0,0,0,0 }, true);
	
	CopyImageTexture.loadFromImage(CopyImage);
	CopyImageSprite.setTexture(CopyImageTexture);
}

void Display::setImageScale(sf::Sprite& sprite)
{
	float aspectRatio = (float)sprite.getTexture()->getSize().x / (float)sprite.getTexture()->getSize().y;
	bool scalingDone = false;

	float yScale = (static_cast<float>(window->getView().getSize().y / 2.0f) / sprite.getTexture()->getSize().y);
	float xScale = ((static_cast<float>(window->getView().getSize().y / 2.0f) / sprite.getTexture()->getSize().y)) * aspectRatio;//((static_cast<float>(window->getView().getSize().x)) / sprite.getTexture()->getSize().x)*aspectRatio;

	while(!scalingDone)
	{
		if(xScale* sprite.getTexture()->getSize().x > static_cast<float>(window->getView().getSize().x))
		{
			xScale = xScale / 2;
			yScale = yScale / 2;
		}
		else
		{
			scalingDone = true;
		}

	}

	sprite.setScale(xScale,yScale);
}

void Display::handleResizeEvent()
{
	setImageScale(GoalImageSprite);
	GoalImageSprite.setPosition(window->getView().getSize().x / 2 - (GoalImageSprite.getGlobalBounds().width / 2), window->getView().getSize().y / 2);
	
	setImageScale(CopyImageSprite);
	CopyImageSprite.setPosition(window->getView().getSize().x / 2 - (CopyImageSprite.getGlobalBounds().width / 2), 0);
}

void Display::setTexts()
{
	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();

	GenerationText.setString("Generation: " + std::to_string(algoritm->generation));
	EllapsedTimeText.setString("Elapsed time: " + std::to_string(std::chrono::duration_cast<std::chrono::seconds> (now - simulationStartTime).count()) + " seconds");

	GenerationText.setPosition(window->getView().getSize().x - GenerationText.getGlobalBounds().width, 0);
	EllapsedTimeText.setPosition(window->getView().getSize().x - EllapsedTimeText.getGlobalBounds().width, 15);
}
