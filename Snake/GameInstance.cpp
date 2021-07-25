#include "GameInstance.h"
#include <chrono>
#include <iostream>
#include <map>
#include "opencv2/opencv.hpp"

GameSettings Settings;

void GameInstance::Start()
{
    Settings.BoardSize = Eigen::Vector2i(12, 12);
    Settings.GridSize = 50;
    GamePlayLogic.InitGame(Settings);
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
    auto GameState = GamePlayLogic.GetGameState();
    if (GameState == EGameState::Paused)
    {
        return;
    }
    if (GameState == EGameState::Error)
    {
        //todo,GameError
        return;
    }
    Seconds += DeltaSeconds;
    if (Seconds > 0.5f)
    {
        if (bHasInput == false)
        {
            GamePlayLogic.Step();
            Seconds = 0.f;
        }
        else
        {
            bHasInput = false;
            Seconds = .2f;
        }
    }

    static std::map<int, EKeyBoardKeys> Key2Type = {
            std::pair<int, EKeyBoardKeys>(119, EKeyBoardKeys::W),
            std::pair<int, EKeyBoardKeys>(97, EKeyBoardKeys::A),
            std::pair<int, EKeyBoardKeys>(115, EKeyBoardKeys::S),
            std::pair<int, EKeyBoardKeys>(100, EKeyBoardKeys::D)
    };

    int Key;
    cv::Mat img(Settings.BoardSize.x() * Settings.GridSize, Settings.BoardSize.y() * Settings.GridSize, CV_32FC3, GamePlayLogic.GetRenderBuffer().data());
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