#include "Core/Core.h"
#include <SFML/Graphics.hpp>
#include <queue>
#include <stack>
#include <iostream>
#include <fstream>



sf::VertexArray path;
constexpr int shift = 6;
//  https://www.youtube.com/watch?v=Y37-gB83HKE&t=1015s
class Maze {
private:
    int m_mazeHeight;
    int m_mazeWidth;
    int* m_maze;

    sf::RenderWindow& window;

    enum {
        CELL_PATH_N = 0x01,
        CELL_PATH_E = 0x02,
        CELL_PATH_S = 0x04,
        CELL_PATH_W = 0x08,
        CELL_VISITED = 0x10,
    };

    // Algorithm variables
    std::stack<std::pair<int, int>> m_stack;
    int  m_nVisitedCells;

public:
    Maze(sf::RenderWindow& window) : window(window) {
        m_mazeHeight = 32;
        m_mazeWidth = 32;
        m_maze = new int[m_mazeHeight * m_mazeWidth];
        memset(m_maze, 0x00, m_mazeHeight * m_mazeWidth * sizeof(int));

        m_stack.push(std::make_pair(0, 0));
        m_maze[0] = CELL_VISITED;
        m_nVisitedCells = 1;

    }

    Maze(int x, int y, sf::RenderWindow& window) : m_mazeHeight(y), m_mazeWidth(x), window(window) {

        m_maze = new int[m_mazeHeight * m_mazeWidth];
        memset(m_maze, 0x00, m_mazeHeight * m_mazeWidth * sizeof(int));

        m_stack.push(std::make_pair(0, 0));
        m_maze[0] = CELL_VISITED;
        m_nVisitedCells = 1;
    }

    const std::pair<int, int> get_Dimension() {
        return { m_mazeWidth, m_mazeHeight };
    }


    // GENERATING MAZE ALGORIHMS
    void RecursiveBacktracking();
    void Iterative();
    

    // TXT
    void ToTxt();
    void ReadMazeFromTxt(std::string s);

    void GeneratingDFS1();
    void SelectNewCell(std::vector<int> neighbours);
    void CheckNeigbours(std::vector<int> &neigbours);

};


class Mouse{
private:
    int m_mazeHeight;
    int m_mazeWidth;
    int* m_maze;

    // https://forbot.pl/blog/roboty-micromouse-5-metod-przeszukiwania-labiryntu-id17354
    // 0W0S0E0N
    uint8_t orientation = 0b100;

    sf::RenderWindow& window;

    enum {
        CELL_PATH_N = 0x01,
        CELL_PATH_E = 0x02,
        CELL_PATH_S = 0x04,
        CELL_PATH_W = 0x08,
        CELL_VISITED = 0x10,
    };

public:
    Mouse(sf::RenderWindow& window) : window(window) {
        m_mazeHeight = 32;
        m_mazeWidth = 32;
        m_maze = new int[m_mazeHeight * m_mazeWidth];
        memset(m_maze, 0x00, m_mazeHeight * m_mazeWidth * sizeof(int));

    }

    Mouse(int x, int y, sf::RenderWindow& window) : m_mazeHeight(y), m_mazeWidth(x), window(window) {

        m_maze = new int[m_mazeHeight * m_mazeWidth];
        memset(m_maze, 0x00, m_mazeHeight * m_mazeWidth * sizeof(int));

 
    }


    void RightWallFollow();

    // TXT
    void ReadMazeFromTxt(std::string s);


};

sf::VertexArray DrawPath(float x, float y) {
    sf::VertexArray path(sf::Quads, 4);
    x *= 18;
    y *= 18;
    sf::Color color(210,206,69);
    path[0].position = sf::Vector2f(x + shift, y + shift);
    path[1].position = sf::Vector2f(x + 12.f + shift, y + shift);
    path[2].position = sf::Vector2f(x + 12.f + shift, y + 12.f + shift);
    path[3].position = sf::Vector2f(x + shift, y + 12.f + shift);
    path[0].color = color;
    path[1].color = color;
    path[2].color = color;
    path[3].color = color;

    return path;

}

sf::VertexArray BreakTheWall(std::pair<int, int> A, std::pair<int, int> N) {
    sf::VertexArray path(sf::Quads, 4);
    bool x_sign = (A.first == N.first);
    bool y_sign = (A.second == N.second);
    int x = std::min(A.first, N.first);
    int y = std::min(A.second, N.second);
    // std::cout << x_sign << " X\n";
    // std::cout << y_sign << " Y\n";

    path = DrawPath(y_sign * 0.4 + x, x_sign * 0.4 + y);
    return path;

}


void Maze::GeneratingDFS1() {
    /*

    sf::VertexArray path;
    std::stack<std::pair<int, int>> s;
    // 1. Choose the initial cell, mark it as visited and push it to the stack
    int x = 0, y = 0;
 
    s.push({ x,y });

    m_maze[x * m_mazeWidth + y] |= CELL_VISITED;

    path = DrawPath(x, y);
    window.draw(path);
 

    while (!s.empty()) {
    //for(int i=0; i<20; i++){
        //visited[x][y] = true;
        x = s.top().first;
        y = s.top().second;
        s.pop();
        path = DrawPath(x, y);
        window.draw(path);
        std::cout << "X: " << x << "\n";
        std::cout << "Y: " << y << "\n";
        //printNeighbour({x, y});
        if (1) {
            // Push the current cell to the stack
            s.push({ x,y });
            // Choose one of the unvisited neighbours
            //path = DrawPath(x, y);
           // window.draw(path);
           // std::cout << "ItERATRIR " << iterator << "\n";
            auto [new_x, new_y] = SelectAdjacentCellFirstRight({ x,y });
            // Remove the wall between the current cell and the chosen cell
            path = BreakTheWall({ x,y }, { new_x, new_y });
            window.draw(path);
          
            // Mark the chosen cell as visited and push it to the stack
            m_maze[new_x * m_mazeWidth + new_y] |= CELL_VISITED;
            s.push({ new_x,new_y });
        }
       std::cout << "KONIEC IFA SPRAWDZAJACEGO" << "\n";
       

    }
    std::cout << "KONIEC STOSU" << "\n";
    */
}

void Maze::SelectNewCell(std::vector<int> neighbours) {
    auto offset = [&](int x, int y) {
        return (m_stack.top().second + y) * m_mazeWidth + (m_stack.top().first + x);
        };

    auto [x, y] = m_stack.top();

    int next_cell_dir = neighbours[rand() % neighbours.size()];
    std::cout << next_cell_dir << '\n';
    switch (next_cell_dir) {
    case 0: //  North
        m_maze[offset(0, -1)] |= CELL_VISITED | CELL_PATH_S;
        m_maze[offset(0, 0)] |= CELL_PATH_N;
        path = BreakTheWall({ x, y }, std::make_pair(m_stack.top().first + 0, (m_stack.top().second - 1)));
        window.draw(path);
        m_stack.push(std::make_pair(m_stack.top().first + 0, (m_stack.top().second - 1)));

        break;

    case 1: //  East
        m_maze[offset(+1, 0)] |= CELL_VISITED | CELL_PATH_W;
        m_maze[offset(0, 0)] |= CELL_PATH_E;
        path = BreakTheWall({ x, y }, std::make_pair(m_stack.top().first + 1, (m_stack.top().second + 0)));
        window.draw(path);
        m_stack.push(std::make_pair(m_stack.top().first + 1, (m_stack.top().second + 0)));

        break;
    case 2: // South
        m_maze[offset(0, +1)] |= CELL_VISITED | CELL_PATH_N;
        m_maze[offset(0, 0)] |= CELL_PATH_S;
        path = BreakTheWall({ x, y }, std::make_pair(m_stack.top().first + 0, (m_stack.top().second + 1)));
        window.draw(path);
        m_stack.push(std::make_pair(m_stack.top().first + 0, (m_stack.top().second + 1)));

        break;
    case 3: // West
        m_maze[offset(-1, 0)] |= CELL_VISITED | CELL_PATH_E;
        m_maze[offset(0, 0)] |= CELL_PATH_W;
        path = BreakTheWall({ x, y }, std::make_pair(m_stack.top().first - 1, (m_stack.top().second + 0)));
        window.draw(path);
        m_stack.push(std::make_pair(m_stack.top().first - 1, (m_stack.top().second + 0)));

        break;
    }
}

void Maze::ToTxt() {

    std::ofstream file;
    file.open("maze.txt", std::ios::out);
    if (file) {
   
        for (int i = 0; i < m_mazeHeight; i++) {
            for (int j = 0; j < m_mazeWidth; j++) {
                file << (m_maze[m_mazeWidth*i + j]&0xf) << ' ';
            }
            file << '\n';
        }
    }
    else {
        std::cout << "Failed opening a file???" << '\n';
    }


    file.close();
}

void Maze::CheckNeigbours(std::vector<int>& neighbours) {
    auto offset = [&](int x, int y) {
        return (m_stack.top().second + y) * m_mazeWidth + (m_stack.top().first + x);
        };
    // North neighbour
    if (m_stack.top().second > 0 && (m_maze[offset(0, -1)] & CELL_VISITED) == 0)
        neighbours.push_back(0);
    // East neigbour
    if (m_stack.top().first < m_mazeWidth - 1 && (m_maze[offset(1, 0)] & CELL_VISITED) == 0)
        neighbours.push_back(1);
    // South
    if (m_stack.top().second < m_mazeHeight - 1 && (m_maze[offset(0, 1)] & CELL_VISITED) == 0)
        neighbours.push_back(2);
    // West
    if (m_stack.top().first > 0 && (m_maze[offset(-1, 0)] & CELL_VISITED) == 0)
        neighbours.push_back(3);
}


void Maze::RecursiveBacktracking() {
    
    std::vector<int> neigbours;
   
    CheckNeigbours(neigbours);

    // Do Maze algorithm
   while (m_nVisitedCells < m_mazeHeight * m_mazeWidth) {
        // set of unvisited neighbours
        std::vector<int> neighbours;
        auto [x, y] = m_stack.top();
        path = DrawPath(x, y);
        window.draw(path);

        CheckNeigbours(neighbours);

        if (!neighbours.empty()) {
            SelectNewCell(neighbours);
            m_nVisitedCells++;
        }
        else {
            m_stack.pop();
        }
     }

   auto [x, y] = m_stack.top();
   path = DrawPath(x, y);
   window.draw(path);
}

void Maze::Iterative() {


    while (!m_stack.empty()) {

        std::vector<int> neigbours;
        auto [x, y] = m_stack.top();
        path = DrawPath(x, y);
        window.draw(path);
        CheckNeigbours(neigbours);
        m_stack.pop();

        if (!neigbours.empty()) {
            m_stack.push({ x,y });
            SelectNewCell(neigbours);
         }



    }
}

void Maze::ReadMazeFromTxt(std::string s) {

    auto offset = [&](int x, int y) {
        return (m_stack.top().second + y) * m_mazeWidth + (m_stack.top().first + x);
        };

    std::ifstream file;
    file.open(s);

    if ( file) {
        int v;
        for(int y =0 ; y < m_mazeHeight  ; y ++){
            for(int x=0; x< m_mazeWidth; x++){
                file >> v;
                path = DrawPath(x,y);
                window.draw(path);

                // North
                if (y > 0 && (v & CELL_PATH_N) ) {
                    path = BreakTheWall({ x,y }, { x, y - 1 });
                    window.draw(path);
                    std::cout <<x << ' ' << y << ". N\n";
                }

                // East
                if (x < m_mazeWidth - 1 && (v & CELL_PATH_E)) {
                    path = BreakTheWall({ x, y }, { x + 1, y });
                    window.draw(path);
                    std::cout << x << ' ' << y << ". E\n";
                }

                // South 
                if (y < m_mazeHeight - 1 && (v & CELL_PATH_S)) {
                    path = BreakTheWall({ x, y }, { x, y + 1 });
                    window.draw(path);
                    std::cout << x<< ' ' << y << ". S\n";
                }

                // West neighbour
                if (x > 0 && (v & CELL_PATH_W) ) {
                    path = BreakTheWall({ x, y }, { x - 1, y });
                    window.draw(path);
                    std::cout << x << ' ' << y << ". W\n";
                }
                
            }
        }
    }
}



void Mouse::ReadMazeFromTxt(std::string s) {

  
    std::ifstream file;
    file.open(s);

    if (file) {
        int v;
        for (int y = 0; y < m_mazeHeight; y++) {
            for (int x = 0; x < m_mazeWidth; x++) {
                file >> v;
              

                // North
                if (y > 0 && (v & CELL_PATH_N)) {
                    m_maze[y * m_mazeWidth + x] |= CELL_PATH_N;
                }

                // East
                if (x < m_mazeWidth - 1 && (v & CELL_PATH_E)) {
                    m_maze[y * m_mazeWidth + x] |= CELL_PATH_E;
                }

                // South 
                if (y < m_mazeHeight - 1 && (v & CELL_PATH_S)) {
                    m_maze[y * m_mazeWidth + x] |= CELL_PATH_S;
                }

                // West neighbour
                if (x > 0 && (v & CELL_PATH_W)) {
                    m_maze[y * m_mazeWidth + x] |= CELL_PATH_W;
                }

            }
        }
    }
}


void Mouse::RightWallFollow() {


}


int main()
{
    srand(time(NULL));
    // 18 pixels box x 32 of them
    // 3 pixels wall thickenss
    // 12 pixels path width 


    int dim_x = 50, dim_y = 50;
    sf::RenderWindow window(sf::VideoMode(dim_x * 18 + shift, dim_y * 18 + shift, 32), "MAZE");
    Maze* m = new Maze(dim_x, dim_y, window);
    Mouse* n = new Mouse(dim_x, dim_y, window);




    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();


     m->Iterative();
     m->ToTxt();
     m->ReadMazeFromTxt("maze.txt");

        window.display();


        sf::sleep(sf::seconds(200.0));
    }


    delete m;
    return 0;
}