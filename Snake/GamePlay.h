#pragma once

#include <vector>
#include "Eigen/Eigen"
#include "Snake.hpp"
#include "Candy.hpp"

enum class EKeyBoradKeys : uint8_t
{
    W,
    A,
    S,
    D
};

struct GameSettings
{
    Eigen::Vector2i BoardSize;
    int GridSize;
};

class GamePlay
{
public:

    void InitGame(const GameSettings& InGameSettings);

    void ProcessInput(EKeyBoradKeys Key);

    void SetScreenResolution(const int X,const int Y);

    void CreateNewCandy();

    std::vector<Eigen::Vector3f>& GetRenderBuffer() { return RenderBuffer; }

    void Step();

private:
    void InitBorder();
    
private:
    void FlushBuffer();
    
    inline int ScreenCoordToIndex(const Eigen::Vector2i& ScreenCoord,const Eigen::Vector2i& InScreenRes);

    void DrawRenderBuffer();

    void DrawGrid(const int X,const int Y,const Eigen::Vector3f& Color);

    Eigen::Vector2i CurrentDirection = Eigen::Vector2i(1, 0);
private:
    std::vector<Eigen::Vector3f> Buffer;
    std::vector<Eigen::Vector3f> RenderBuffer;
    Eigen::Vector2i ScreenRes;

    Candy mCandy;
    Snake mSnake;

    GameSettings Settings;
};