#pragma once
#include "Sfml.h"
#include <queue>
#include <stack>
#include <iostream>
#include <fstream>



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


    void SelectNewCell(std::vector<int> neighbours);
    void CheckNeigbours(std::vector<int>& neigbours);
   

};
