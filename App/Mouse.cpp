#include "Mouse.h"


void Mouse::ReadMazeFromTxt(std::string s) {

    std::ifstream file;
    file.open(s);

    if (file) {
        int v;
        for (int y = 0; y < m_mazeHeight; y++) {
            for (int x = 0; x < m_mazeWidth; x++) {
                file >> v;

                m_maze[y * m_mazeWidth + x] = v;

            }
        }
    }

}


void Mouse::RightWallFollow() {
    // 0W0S0E0N
    // 1 4 16 64
    int pathLength = 0;
    auto adjustDirection = [](int value) {
        if (value > 8) {
            return 1;
        }
        else if (value < 1) {
            return 8;
        }
        else {
            return value;
        }
        };
    int x = 0, y = 0;

    while (x != m_mazeWidth - 1 || y != m_mazeHeight - 1) {

        int right = adjustDirection(orientation * 2);
        int left = adjustDirection(orientation / 2);

        if (right & m_maze[m_mazeWidth * y + x]) {
            orientation = adjustDirection(orientation * 2);
            NextCell(orientation, x, y);

        }
        else if (orientation & m_maze[m_mazeWidth * y + x]) {
            NextCell(orientation, x, y);
        }
        else if (left & m_maze[m_mazeWidth * y + x]) {
            orientation = adjustDirection(orientation / 2);
            NextCell(orientation, x, y);
        }
        else {
            orientation = adjustDirection(orientation * 2);
            orientation = adjustDirection(orientation * 2);
            NextCell(orientation, x, y);
        }


        // path = DrawPath(x, y, sf::Color::Red);
        // window.draw(path);
         //window.display();
        // sf::sleep(sf::seconds(0.1));
         //window.clear();
       //  std::cout << "orientation " << orientation << " X " << x << " Y " << y << '\n';
        pathLength++;
    }

    std::cout << "Path length: " << pathLength << '\n';
}

void Mouse::NextCell(int orientation, int& x, int& y) {



    // North is new cell
    if (orientation & CELL_PATH_N)
        y--;
    // East
    if (orientation & CELL_PATH_E)
        x++;
    // South 
    if (orientation & CELL_PATH_S)
        y++;
    // West 
    if (orientation & CELL_PATH_W)
        x--;


}


void Mouse::CreateMaze(int x, int y, int distance) {
    std::vector < int > neighbours;

    if (m_distance[m_mazeWidth * y + x].second == true) return;

    m_distance[m_mazeWidth * y + x].first = distance;
    m_distance[m_mazeWidth * y + x].second = true;
    distance++;
    // std::cout << distance << ' ' << x << ' ' << y <<'\n';
    // North neighbour

    if (y > 0 && (m_maze[m_mazeWidth * y + x] & CELL_PATH_N))
        CreateMaze(x, y - 1, distance);
    // East
    if (x < m_mazeWidth - 1 && (m_maze[m_mazeWidth * y + x] & CELL_PATH_E))
        CreateMaze(x + 1, y, distance);
    // South
    if (y < m_mazeHeight - 1 && (m_maze[m_mazeWidth * y + x] & CELL_PATH_S))
        CreateMaze(x, y + 1, distance);
    // West
    if (x > 0 && (m_maze[m_mazeWidth * y + x] & CELL_PATH_W))
        CreateMaze(x - 1, y, distance);
}

void Mouse::PrintFloodFill() {

    for (int y = 0; y < m_mazeHeight; y++) {
        for (int x = 0; x < m_mazeWidth; x++) {
            std::cout << m_distance[m_mazeWidth * y + x].first << ' ';
        }
        std::cout << '\n';
    }
}

void Mouse::FloodFill() {
    int pathLength = 0;
    int x = 0, y = 0;
 //   std::cout << "PATH: \n";
   // m_shortestPath.push_back({ x,y });

    std::vector<int> visited;
    for (int i = 0; i < m_mazeWidth * m_mazeHeight; ++i) {
        visited.push_back(0);
    }

    while (x != m_mazeWidth - 1 || y != m_mazeHeight - 1) {
    
      //  std::cout << "X " << x << " Y " << y << "\n";

       


        //N
        if (m_distance[m_mazeWidth * (y - 1) + x].first == pathLength+1 && visited[m_mazeWidth * (y - 1) + x]==0) {
          //  std::cout << "N" << '\n';
            y--;
            m_shortestPath.push_back({ x,y });
            m_shortestPathDirections.push_back('N');
            pathLength++;
            visited[m_mazeWidth * y + x] = 1;
        }
        // E
        else if (m_distance[m_mazeWidth * y + x + 1].first == pathLength+1 && visited[m_mazeWidth * y + x + 1] == 0) {
         //   std::cout << "E" << '\n';
            x++;
            m_shortestPath.push_back({ x,y });
            m_shortestPathDirections.push_back('E');
            pathLength++;
            visited[m_mazeWidth * y + x] = 1;
        }
        // S
        else if (m_distance[m_mazeWidth * (y + 1) + x].first == pathLength+1 && visited[m_mazeWidth * (y + 1) + x] == 0) {
         //  std::cout << "S" << '\n';
            y++;
            m_shortestPath.push_back({ x,y });
            m_shortestPathDirections.push_back('S');
            pathLength++;
            visited[m_mazeWidth * y + x] = 1;
        }
        // W
        else if (m_distance[m_mazeWidth * y + x - 1].first == pathLength+1 && visited[m_mazeWidth * y + x - 1] == 0) {
         //   std::cout << "W" << '\n';
            x--;
            m_shortestPath.push_back({ x,y });
            m_shortestPathDirections.push_back('W');
            pathLength++;
            visited[m_mazeWidth * y + x] = 1;
        }
        else {
            
          
            m_shortestPath.pop_back();
            //auto [&x, &y] = m_shortestPath.back();
            x = m_shortestPath.back().first;
            y = m_shortestPath.back().second;
           
        //    std::cout << "FROM VECTOR X " << x << " Y " << y << "\n";
            pathLength --;
        }


        if (x == m_mazeWidth - 1 && y == m_mazeHeight - 1) {
          //  std::cout << "X " << x << " Y " << y << "\n";
           // std::cout << "FLOODFILL FOUND\n";

            for (int i = 0; i < m_shortestPath.size(); i++) {
                std::cout << i << ". " <<  "X " << m_shortestPath[i].first << " Y " << m_shortestPath[i].second << "\n";
            }

            return;
        }

    
      //  std::cout << "Path length: " << pathLength << '\n';


    }

    std::cout << "Path length: " << pathLength << '\n';

}


void Mouse::DFSsearch() {
    std::stack<std::pair<int, int>> s;

    //bool visited[7][3];
   
    std::vector<bool> visited;
    for (int i = 0; i < m_mazeWidth*m_mazeHeight; ++i) {
        visited.push_back(0);
    }

    int x = 0, y = 0;
    s.push({ x,y });
    while (!s.empty()) {
      
        auto [x, y] = s.top();
        visited[m_mazeWidth*y + x] = 1;
        s.pop();
       
        if (x == m_mazeWidth - 1 && y == m_mazeHeight - 1) {
           std::cout << "X " << x << " Y " << y << "\n";
             std::cout << "DFS FOUND\n";

            for (int y = 0; y < m_mazeHeight; y++) {
                for (int x = 0; x < m_mazeWidth; x++) {
                    std::cout << visited[m_mazeWidth * y + x] << " ";
                }
                std::cout << "\n";
            } 

            return;
        }
      
        std::cout << "DFS X " << x << " Y " << y << "\n";
     
        // North
        if (y>0 &&(m_maze[m_mazeWidth * y + x] & CELL_PATH_N) && visited[m_mazeWidth*(y-1) + x] == 0) {
            s.push({ x,y-1 });
           // std::cout << m_maze[m_mazeWidth * y + x] << " PUSHED N" << (m_maze[m_mazeWidth * y + x] & CELL_PATH_N) << "\n";
        }
        // East
        if (x < m_mazeWidth - 1 && (m_maze[m_mazeWidth * y + x] & CELL_PATH_E) && visited[m_mazeWidth*y + x + 1]==0) {
            s.push({ x+1,y });
           // std::cout << m_maze[m_mazeWidth * y + x] <<  " PUSHED E"<< (m_maze[m_mazeWidth * y + x] & CELL_PATH_E)  <<"\n";
         
        }
        // South
        if (y < m_mazeHeight - 1 && (m_maze[m_mazeWidth * y + x] & CELL_PATH_S) && visited[m_mazeWidth*(y+1) + x]==0) {
            s.push({ x,y+1 });
            //std::cout << m_maze[m_mazeWidth * y + x] << " PUSHED S" << (m_maze[m_mazeWidth * y + x] & CELL_PATH_S) << "\n";
        }
        // West
        if (x>0 && (m_maze[m_mazeWidth * y + x] & CELL_PATH_W) && visited[m_mazeWidth*y + x-1]==0) {
            s.push({ x-1,y });
           // std::cout << m_maze[m_mazeWidth * y + x] << " PUSHED W" << (m_maze[m_mazeWidth * y + x] & CELL_PATH_W) << "\n";
        }
     

    

    }

  
    std::cout << "END OF STACKA\n";

   
}


void Mouse::BFSsearch() {
    std::queue<std::pair<int, int>> q;

   

    std::vector<bool> visited;
    for (int i = 0; i < m_mazeWidth * m_mazeHeight; ++i) {
        visited.push_back(0);
    }

    int x = 0, y = 0;
    q.push({ x,y });
    while (!q.empty()) {

        auto [x, y] = q.front();
        visited[m_mazeWidth * y + x] = 1;
        q.pop();

        if (x == m_mazeWidth - 1 && y == m_mazeHeight - 1) {
               std::cout << "X " << x << " Y " << y << "\n";
               std::cout << "BFS FOUND\n";

            for (int y = 0; y < m_mazeHeight; y++) {
                for (int x = 0; x < m_mazeWidth; x++) {
                    std::cout << visited[m_mazeWidth * y + x] << " ";
                }
                std::cout << "\n";
            }

            return;
        }

          std::cout << "BFS X " << x << " Y " << y << "\n";

          // North
        if (y > 0 && (m_maze[m_mazeWidth * y + x] & CELL_PATH_N) && visited[m_mazeWidth * (y - 1) + x] == 0) {
            q.push({ x,y - 1 });
            // std::cout << m_maze[m_mazeWidth * y + x] << " PUSHED N" << (m_maze[m_mazeWidth * y + x] & CELL_PATH_N) << "\n";
        }
        // East
        if (x < m_mazeWidth - 1 && (m_maze[m_mazeWidth * y + x] & CELL_PATH_E) && visited[m_mazeWidth * y + x + 1] == 0) {
            q.push({ x + 1,y });
            // std::cout << m_maze[m_mazeWidth * y + x] <<  " PUSHED E"<< (m_maze[m_mazeWidth * y + x] & CELL_PATH_E)  <<"\n";

        }
        // South
        if (y < m_mazeHeight - 1 && (m_maze[m_mazeWidth * y + x] & CELL_PATH_S) && visited[m_mazeWidth * (y + 1) + x] == 0) {
            q.push({ x,y + 1 });
            //std::cout << m_maze[m_mazeWidth * y + x] << " PUSHED S" << (m_maze[m_mazeWidth * y + x] & CELL_PATH_S) << "\n";
        }
        // West
        if (x > 0 && (m_maze[m_mazeWidth * y + x] & CELL_PATH_W) && visited[m_mazeWidth * y + x - 1] == 0) {
            q.push({ x - 1,y });
            // std::cout << m_maze[m_mazeWidth * y + x] << " PUSHED W" << (m_maze[m_mazeWidth * y + x] & CELL_PATH_W) << "\n";
        }




    }


    std::cout << "END OF QUEUE\n";


}

void Mouse::ShortestPath() {
    sf::VertexArray path;
    int prev_x=0, prev_y = 0;

    // DRAWING
    path = DrawPath(0, 0, sf::Color::Red);
    window.draw(path);
    // DRAWING

    for (int i= 0;i<m_shortestPath.size();i++) {
        auto [x, y] = m_shortestPath[i];
        char d =m_shortestPathDirections[i];

        // DRAWING
        path = DrawPath(x, y, sf::Color::Green);
        window.draw(path);
        path = BreakTheWall({ x,y }, { prev_x,prev_y }, sf::Color::Green);
        window.draw(path);
        prev_x = x;
        prev_y = y;
        // DRAWING

      //  std::cout << "DIRECTION  " << d << " TO X " << x << " Y " << y << "\n";
       
    }
    path = DrawPath(m_mazeWidth-1, m_mazeHeight-1, sf::Color::Red);
    window.draw(path);
}