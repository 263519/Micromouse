#include "Maze.h"
void Maze::SelectNewCell(std::vector<int> neighbours) {

    sf::VertexArray path;

    auto offset = [&](int x, int y) {
        return (m_stack.top().second + y) * m_mazeWidth + (m_stack.top().first + x);
        };

    auto [x, y] = m_stack.top();

    int next_cell_dir = neighbours[rand() % neighbours.size()];
    // std::cout << next_cell_dir << '\n';
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
                file << (m_maze[m_mazeWidth * i + j] & 0xf) << ' ';
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
    sf::VertexArray path;
    std::vector<int> neigbours;

    CheckNeigbours(neigbours);

    // Do Maze algorithm
    while (m_nVisitedCells < m_mazeHeight * m_mazeWidth) {
        // set of unvisited neighbours
        std::vector<int> neighbours;
        auto [x, y] = m_stack.top();
        path = DrawPath(x, y, sf::Color::Yellow);
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
    path = DrawPath(x, y, sf::Color::Yellow);
    window.draw(path);
}

void Maze::Iterative() {

    sf::VertexArray path;
    while (!m_stack.empty()) {

        std::vector<int> neigbours;
        auto [x, y] = m_stack.top();
        path = DrawPath(x, y, sf::Color::Yellow);
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
    sf::VertexArray path;
    auto offset = [&](int x, int y) {
        return (m_stack.top().second + y) * m_mazeWidth + (m_stack.top().first + x);
        };

    std::ifstream file;
    file.open(s);

    if (file) {
        int v;
        for (int y = 0; y < m_mazeHeight; y++) {
            for (int x = 0; x < m_mazeWidth; x++) {
                file >> v;
                path = DrawPath(x, y, sf::Color::Yellow);
                window.draw(path);

                // North
                if (y > 0 && (v & CELL_PATH_N)) {
                    path = BreakTheWall({ x,y }, { x, y - 1 });
                    window.draw(path);

                }

                // East
                if (x < m_mazeWidth - 1 && (v & CELL_PATH_E)) {
                    path = BreakTheWall({ x, y }, { x + 1, y });
                    window.draw(path);

                }

                // South 
                if (y < m_mazeHeight - 1 && (v & CELL_PATH_S)) {
                    path = BreakTheWall({ x, y }, { x, y + 1 });
                    window.draw(path);

                }

                // West neighbour
                if (x > 0 && (v & CELL_PATH_W)) {
                    path = BreakTheWall({ x, y }, { x - 1, y });
                    window.draw(path);

                }

            }
        }
    }
}

