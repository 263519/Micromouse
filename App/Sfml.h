#pragma once
#include <SFML/Graphics.hpp>

constexpr int shift = 6;

sf::VertexArray DrawPath(float x, float y, sf::Color color);
sf::VertexArray BreakTheWall(std::pair<int, int> A, std::pair<int, int> N, sf::Color color);