#include "Core/Core.h"
#include <SFML/Graphics.hpp>
#include <queue>
#include <stack>
#include <iostream>


sf::VertexArray path;

//  https://www.youtube.com/watch?v=Y37-gB83HKE&t=1015s
class Maze {
private:
    int m_mazeHeight;
    int m_mazeWidth;
    int* m_maze;

    sf::RenderWindow &window;

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

    Maze(int x, int y, sf::RenderWindow& window) : m_mazeHeight(x), m_mazeWidth(y), window(window) {

        m_maze = new int[m_mazeHeight * m_mazeWidth];
        memset(m_maze, 0x00, m_mazeHeight * m_mazeWidth * sizeof(int));

        m_stack.push(std::make_pair(0, 0));
        m_maze[0] = CELL_VISITED;
        m_nVisitedCells = 1;
    }

    const std::pair<int, int> get_Dimension() {
        return { m_mazeWidth, m_mazeHeight };
    }

    void RecursiveBacktracking();
    std::pair<int, int > SelectAdjacentCell1(std::pair<int, int> current_cell);
    std::pair<int, int > SelectAdjacentCellFirstRight(std::pair<int, int> current_cell);
    void GeneratingDFS1();
    void printNeighbour(std::pair<int, int> current_cell);
    bool IsAnyNotVisited(std::pair<int, int> current_cell);
};

sf::VertexArray DrawPath(float x, float y) {
    sf::VertexArray path(sf::Quads, 4);
    x *= 18;
    y *= 18;
    path[0].position = sf::Vector2f(x + 3.f, y + 3.f);
    path[1].position = sf::Vector2f(x + 12.f + 3.f, y + 3.f);
    path[2].position = sf::Vector2f(x + 12.f + 3.f, y + 12.f + 3.f);
    path[3].position = sf::Vector2f(x + 3.f, y + 12.f + 3.f);
    path[0].color = sf::Color::Blue;
    path[1].color = sf::Color::Blue;
    path[2].color = sf::Color::Blue;
    path[3].color = sf::Color::Blue;

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
    /*

    int x = std::min(A.first, N.first);
    int y = std::min(A.second, N.second);
    x *= 18;
    y *= 18;
    //int x_change_sign = N.first - A.first;
    //int y_change_sign = N.second - A.second;
    path[0].position = sf::Vector2f(x + 3.f + 16.f, y + 3.f + 16.f);
    path[1].position = sf::Vector2f(x + 16.f + x_sign*6.f + 3.f, y + 3.f + y_sign*6.f);
    path[2].position = sf::Vector2f(x + 16.f + x_sign*6.f + 3.f, y + 16.f + y_sign * 6.f + 3.f);
    path[3].position = sf::Vector2f(x + 3.f + 16.f, y + 16.f + 3.f + y_sign *6.f);
    path[0].color = sf::Color::Blue;
    path[1].color = sf::Color::Blue;
    path[2].color = sf::Color::Blue;
    path[3].color = sf::Color::Blue;




    path[0].position = sf::Vector2f(x, y);
    path[1].position = sf::Vector2f(x + (210.f* x_change_sign), y);
    path[2].position = sf::Vector2f(x + (210.f * x_change_sign), y + (210.f * y_change_sign));
    path[3].position = sf::Vector2f(x, y + (210.f *y_change_sign));
    path[0].color = sf::Color::Blue;
    path[1].color = sf::Color::Blue;
    path[2].color = sf::Color::Blue;
    path[3].color = sf::Color::Blue;

   */

    return path;

}

/*
std::pair<int, int > SelectAdjacentCell(bool visited[DIMENSION][DIMENSION], std::queue<std::pair<int, int>>& q) {
    // srand((unsigned)time(NULL));

    auto [x, y] = q.back();
    int new_x = x, new_y = y;

    ///*
    while (visited[new_x + 1][new_y] && visited[new_x - 1][new_y] && visited[new_x][new_y + 1] && visited[new_x][new_y - 1]) {
        q.pop();
        new_x = q.front().first;
        new_y = q.front().second;
    }
   

    while (visited[new_x][new_y]) {
        int random = rand() % 4;


        if (random == 0 && x < 31)
            new_x++;
        if (random == 1 && x > 0)
            new_x--;
        if (random == 2 && y < 31)
            new_y++;
        if (random == 3 && y > 0)
            new_y--;

    }


    // New adjacent position to previous

   // std::cout << "X: " << x << "\n";
    //std::cout << "Y: " << y << "\n";


    return{ new_x,new_y };
}

*/



std::pair<int, int > Maze::SelectAdjacentCell1(std::pair<int, int> current_cell) {

    auto [x, y] = current_cell;
    auto [dim_x, dim_y] = get_Dimension();
    int chosen=1;

    do {
        int random = rand() % 4;
        if (random == 0 && x < dim_x-1 && !(m_maze[x*m_mazeWidth+y] & CELL_PATH_E)) {
            m_maze[x * m_mazeWidth + y] |= CELL_PATH_E;
            x += 1;
            chosen = 0;
           
            break;
        }
        else  if (random == 1 && x > 0 && !(m_maze[x * m_mazeWidth + y] & CELL_PATH_W)) {
            m_maze[x * m_mazeWidth + y] |= CELL_PATH_W;
            x -= 1;
            chosen = 0;
         
            break;
        }
        else  if (random == 2 && y < dim_y-1 && !(m_maze[x * m_mazeWidth + y] & CELL_PATH_N)) {
            m_maze[x * m_mazeWidth + y] |= CELL_PATH_N;
            y += 1;
            chosen = 0;
          
            break;
        }
        else  if (random == 3 && y > 0 && !(m_maze[x * m_mazeWidth + y] & CELL_PATH_S)) {
            m_maze[x * m_mazeWidth + y] |= CELL_PATH_S;
            y -= 1;
            chosen = 0;
           
            break;
        }

    } while (chosen);
    
    return { x,y };
}


std::pair<int, int > Maze::SelectAdjacentCellFirstRight(std::pair<int, int> current_cell) {

    auto [x, y] = current_cell;
    auto [dim_x, dim_y] = get_Dimension();
    int chosen = 1;


        if ( x < dim_x - 1 && !(m_maze[x * m_mazeWidth + y] & CELL_PATH_E)) {
            //m_maze[x * m_mazeWidth + y] |= CELL_PATH_E;
            x += 1;
           // m_maze[x * m_mazeWidth + y] |= CELL_PATH_W;

          

         
        }
        else  if (x > 0 && !(m_maze[x * m_mazeWidth + y] & CELL_PATH_W)) {
           // m_maze[x * m_mazeWidth + y] |= CELL_PATH_W;
            x -= 1;
           // m_maze[x * m_mazeWidth + y] |= CELL_PATH_E;
        

        
        }
        else  if (y < dim_y - 1 && !(m_maze[x * m_mazeWidth + y] & CELL_PATH_N)) {
           // m_maze[x * m_mazeWidth + y] |= CELL_PATH_N;
            y += 1;
           // m_maze[x * m_mazeWidth + y] |= CELL_PATH_S;
         

       
        }
        else  if ( y > 0 && !(m_maze[x * m_mazeWidth + y] & CELL_PATH_S)) {
           // m_maze[x * m_mazeWidth + y] |= CELL_PATH_S;
            y -= 1;
            //m_maze[x * m_mazeWidth + y] |= CELL_PATH_N;
       

           
        }

   

    return { x,y };
}

void Maze::printNeighbour(std::pair<int, int> current_cell) {
    auto [x, y] = current_cell;
    if (!(m_maze[x * m_mazeWidth + y] & CELL_PATH_N)) std::cout << "N DOSTEPNE\n";
    if (!(m_maze[x * m_mazeWidth + y] & CELL_PATH_S)) std::cout << "S DOSTEPNE\n";
    if (!(m_maze[x * m_mazeWidth + y] & CELL_PATH_E)) std::cout << "E DOSTEPNE\n";
    if (!(m_maze[x * m_mazeWidth + y] & CELL_PATH_W)) std::cout << "W DOSTEPNE\n";
}

bool Maze::IsAnyNotVisited(std::pair<int, int> current_cell){

    auto [x, y] = current_cell;
    auto [x_dim, y_dim] = get_Dimension();
    if (x < x_dim - 1 && y < y_dim - 1 && (m_maze[x * m_mazeWidth + y] & 0x0F) != 0x0F) {
        return 69;
    }
    



}
void Maze::GeneratingDFS1() {


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
        printNeighbour({x, y});
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
    
}




void Maze::RecursiveBacktracking() {
    auto offset = [&](int x, int y) {
        return (m_stack.top().second + y) * m_mazeWidth + (m_stack.top().first + x);
        };

  




    // Do Maze algorithm
   while (m_nVisitedCells < m_mazeHeight * m_mazeWidth) {
        // set of unvisited neighbours
        std::vector<int> neighbours;
        auto [x, y] = m_stack.top();
        path = DrawPath(x, y);
        window.draw(path);

        // North neighbour
        if (m_stack.top().second > 0 && (m_maze[offset(0, -1)] & CELL_VISITED) == 0)
            neighbours.push_back(0);
        // East neigbour
        if (m_stack.top().first < m_mazeWidth-1 && (m_maze[offset(1, 0)] & CELL_VISITED) == 0)
            neighbours.push_back(1);
        // South
        if (m_stack.top().second < m_mazeHeight - 1 && (m_maze[offset(0, 1)] & CELL_VISITED) == 0)
            neighbours.push_back(2);
        // West
        if (m_stack.top().first > 0 && (m_maze[offset(-1, 0)] & CELL_VISITED) == 0)
            neighbours.push_back(3);

        if (!neighbours.empty()) {
            int next_cell_dir = neighbours[rand() % neighbours.size()];
            std::cout << next_cell_dir << '\n';
            switch (next_cell_dir) {
            case 0: //  North
                m_maze[offset(0, -1)] |= CELL_VISITED | CELL_PATH_S;
                m_maze[offset(0, 0)] |= CELL_PATH_N;
                path = BreakTheWall({ x, y }, std::make_pair(m_stack.top().first + 0, (m_stack.top().second - 1)));
                window.draw(path);
                m_stack.push(std::make_pair(m_stack.top().first + 0, (m_stack.top().second  -1)));

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
                 m_stack.push(std::make_pair(m_stack.top().first + 0 , (m_stack.top().second + 1)));
             
                 break;
             case 3: // West
                 m_maze[offset(-1, 0)] |= CELL_VISITED | CELL_PATH_S;
                 m_maze[offset(0, 0)] |= CELL_PATH_W;
                 path = BreakTheWall({ x, y }, std::make_pair(m_stack.top().first - 1, (m_stack.top().second + 0)));
                 window.draw(path);
                 m_stack.push(std::make_pair(m_stack.top().first - 1, (m_stack.top().second + 0)));
               
                 break;
            }
           
            m_nVisitedCells++;
        
       
        }
        else {
            m_stack.pop();
        
        }
        //RecursiveBacktracking();
      
     }

    

    
}

int main()
{
    srand(time(NULL));
    // 18 pixels box x 32 of them
    // 3 pixels wall thickenss
    // 12 pixels path width 
  

    int dim_x = 32, dim_y = 32;
    sf::RenderWindow window(sf::VideoMode(dim_x*18, dim_y*18), "MAZE");
    Maze* m = new Maze(dim_x, dim_y, window);




    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();


        m->RecursiveBacktracking();


       window.display();
       sf::sleep(sf::seconds(200.0));
    }


    delete m;
    return 0;
}