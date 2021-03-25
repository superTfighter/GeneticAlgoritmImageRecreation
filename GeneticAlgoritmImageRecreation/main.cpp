#include <SFML/Graphics.hpp>
#include "Display.h"
#include "OpenCL.h"
#include <vector>
#include <thread>

using namespace std;

const int WindowWidth = 1280;
const int WindowHeight = 720;

bool running = true;

void doWork(Display& d)
{
    while(running)
    {
        d.DoWork();
    }
}

int main()
{
    srand(time(NULL));
    
     GeneticAlgoritm algoritm;

     sf::RenderWindow window(sf::VideoMode(WindowWidth, WindowHeight, 32), "Genetic Algoritm");

     Display d("ford.jpg",&algoritm,&window);

    std::thread workThread(doWork, std::ref(d));

     while (window.isOpen())
     {
        // d.DoWork();
         d.HandleEvents();
         d.Render();
     }

     running = false;

   workThread.join();

    return 0;
}