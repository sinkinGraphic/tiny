#pragma once

#include <functional>
#include "Eigen/Eigen"

class Candy
{
public:
    using OnCandyEaten = std::function<void()>;

    void SetOnCandyEaten(OnCandyEaten Handle) { OnEatenHandle = Handle; };

public:
    Eigen::Vector2i Pos;

private:
    OnCandyEaten OnEatenHandle;
};