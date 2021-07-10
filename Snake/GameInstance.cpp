#include "GameInstance.h"
#include <chrono>
#include <iostream>
#include <thread>

void GameInstance::Start()
{
    float DeltaSeconds = 0.f;
    while (1)
    {
        auto StartTime = std::chrono::high_resolution_clock::now();
        Tick(DeltaSeconds);
        auto EndTime = std::chrono::high_resolution_clock::now();
        DeltaSeconds = std::chrono::duration_cast<std::chrono::milliseconds>(EndTime - StartTime).count() / 1000.f;
    }
}

void GameInstance::Tick(float DeltaSeconds)
{
    
}