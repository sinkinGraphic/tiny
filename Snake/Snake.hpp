#pragma once

#include "Eigen/Eigen"
#include <vector>

class Snake
{
public:

public:
    std::vector<Eigen::Vector2i>& GetBodyCoord() { return BodyCoord; };

    std::vector<Eigen::Vector2i> BodyCoord;
};