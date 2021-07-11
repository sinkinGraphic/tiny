#pragma once

#include "Eigen/Eigen"
#include <vector>

class Snake
{
public:
    bool CheckCandy(const Eigen::Vector2i& CandyPos)
    {
        return BodyCoord[0] == CandyPos;
    }

public:
    std::vector<Eigen::Vector2i>& GetBodyCoord() { return BodyCoord; };

    std::vector<Eigen::Vector2i> BodyCoord;
};