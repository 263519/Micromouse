#pragma once
#include "Sfml.h"
#include <queue>
#include <stack>
#include <iostream>
#include <fstream>
#pragma warning(disable:6385)

class Mouse {
private:
    int m_mazeHeight;
    int m_mazeWidth;
    int* m_maze;
    std::pair<int, bool>* m_distance;
    std::vector <std::pair<int, int>> m_shortestPath;
    std::vector <char> m_shortestPathDirections;

    // https://forbot.pl/blog/roboty-micromouse-5-metod-przeszukiwania-labiryntu-id17354
    // 0W0S0E0N
    int orientation = 0x02;

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
        m_distance = new std::pair<int, bool>[m_mazeHeight * m_mazeWidth];
        memset(m_distance, 0x00, m_mazeHeight * m_mazeWidth * sizeof(std::pair<int, bool>));

    }

    Mouse(int x, int y, sf::RenderWindow& window) : m_mazeHeight(y), m_mazeWidth(x), window(window) {

        m_maze = new int[m_mazeHeight * m_mazeWidth];
        memset(m_maze, 0x00, m_mazeHeight * m_mazeWidth * sizeof(int));
        m_distance = new std::pair<int, bool>[m_mazeHeight * m_mazeWidth];
        memset(m_distance, 0x00, m_mazeHeight * m_mazeWidth * sizeof(std::pair<int, bool>));


    }


    void RightWallFollow();
    void NextCell(int orientation, int& x, int& y);
    void CreateMaze(int x, int y, int distance);
    void PrintFloodFill();
    void FloodFill();
    void DFSsearch();
    void BFSsearch();
    void ShortestPath();
    // TXT
    void ReadMazeFromTxt(std::string s);


};