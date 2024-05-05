#include "Core/Core.h"
#include <SFML/Graphics.hpp>
#include "../Maze.h"
#include "../Mouse.h"
#include "../Sfml.h" // own sfml.h,  not library


//  https://www.youtube.com/watch?v=Y37-gB83HKE&t=1015s

int main()
{
    srand(time(NULL));
    // 18 pixels box x 32 of them
    // 3 pixels wall thickenss
    // 12 pixels path width 


    int dim_x = 25, dim_y = 20;
    sf::RenderWindow window(sf::VideoMode(dim_x * 18 + shift, dim_y * 18 + shift, 32), "MAZE");
    Maze* m = new Maze(dim_x, dim_y, window);
    Mouse* n = new Mouse(dim_x, dim_y, window);




   // while (window.isOpen())
    //{
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

        m->Iterative();
       // m->AldousBroder();
        m->ToTxt();
        m->ReadMazeFromTxt("maze.txt");

       
        n->ReadMazeFromTxt("maze.txt");
       // n->RightWallFollow();
        n->CreateMaze(0, 0, 0);
      //  n->PrintFloodFill();
       n->FloodFill();

        n->DFSsearch();
      
       n->ShortestPath();
       // sf::sleep(sf::seconds(200.0));
       // 
       window.display();
       sf::sleep(sf::seconds(200.0));

   // }


    delete m;
    return 0;
}