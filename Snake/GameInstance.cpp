#include "GameInstance.h"
#include <chrono>
#include <iostream>
#include <map>
#include "opencv2/opencv.hpp"

void GameInstance::Start()
{
    GamePlayLogic.SetScreenResolution(50, 50);
    float DeltaSeconds = 0.f;

    while (true)
    {
        auto StartTime = std::chrono::high_resolution_clock::now();
        Tick(DeltaSeconds);
        auto EndTime = std::chrono::high_resolution_clock::now();
        DeltaSeconds = std::chrono::duration_cast<std::chrono::milliseconds>(EndTime - StartTime).count() / 1000.f;
    }
}

void GameInstance::Tick(float DeltaSeconds)
{
    static float Seconds = 0.f;
    static bool bHasInput = false;
    Seconds += DeltaSeconds;
    if (Seconds > 0.5f)
    {
        if (bHasInput == false)
        {
            GamePlayLogic.Step();
        }
        else
        {
            bHasInput = false;
        }
        Seconds = 0.f;
    }

    static std::map<int, EKeyBoradKeys> Key2Type = {
        std::pair<int, EKeyBoradKeys>(119, EKeyBoradKeys::W),
        std::pair<int, EKeyBoradKeys>(97, EKeyBoradKeys::A),
        std::pair<int, EKeyBoradKeys>(115, EKeyBoradKeys::S),
        std::pair<int, EKeyBoradKeys>(100, EKeyBoradKeys::D)};

    int Key;
    cv::Mat img(1000, 1000, CV_32FC3, GamePlayLogic.GetRenderBuffer().data());
    cv::imshow("Snake", img);
    Key = cv::waitKey(30);
    auto Iter = Key2Type.find(Key);
    if (Iter != Key2Type.end())
    {
        bHasInput = true;
        GamePlayLogic.ProcessInput(Iter->second);
        GamePlayLogic.Step();
    }
}