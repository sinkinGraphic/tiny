#pragma once

#include <functional>

class Candy
{
public:
    using OnCandyEaten = std::function<void()>;

    void SetOnCandyEaten(OnCandyEaten Handle) { OnEatenHandle = Handle; };

private:
    OnCandyEaten OnEatenHandle;
};