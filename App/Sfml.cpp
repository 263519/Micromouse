#include "Sfml.h"
sf::VertexArray DrawPath(float x, float y, sf::Color color) {
    sf::VertexArray path(sf::Quads, 4);
    x *= 18;
    y *= 18;

    sf::Color c(210, 206, 69);
    if (color == sf::Color::Yellow)
        color = c;
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

sf::VertexArray BreakTheWall(std::pair<int, int> A, std::pair<int, int> N, sf::Color color) {
    sf::VertexArray path(sf::Quads, 4);
    bool x_sign = (A.first == N.first);
    bool y_sign = (A.second == N.second);
    int x = std::min(A.first, N.first);
    int y = std::min(A.second, N.second);
    // std::cout << x_sign << " X\n";
    // std::cout << y_sign << " Y\n";

    path = DrawPath(y_sign * 0.4 + x, x_sign * 0.4 + y, color);
    return path;

}