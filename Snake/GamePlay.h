#pragma once

#include <vector>
#include "Eigen/Eigen"
#include "Snake.hpp"
#include "Candy.hpp"
#include <map>

enum class EKeyBoardKeys : uint8_t
{
    W,
    A,
    S,
    D
};

enum class EEntityType : uint8_t
{
    SnakeHead,
    SnakeBody,
    Candy,
    Border,
    None
};
enum class EGameState : uint8_t
{
    Normal,
    Paused,
    Error
};

struct GameSettings
{
    Eigen::Vector2i BoardSize;
    int GridSize;
};

class GamePlay
{
public:
    void InitGame(const GameSettings &InGameSettings);

    void ProcessInput(EKeyBoardKeys Key);

    void SetScreenResolution(const int X, const int Y);

    void CreateNewCandy();

    std::vector<Eigen::Vector3f> &GetRenderBuffer()
    {
        return RenderBuffer;
    }

    void Step();

    EGameState GetGameState();

private:
    void InitBorder();

private:
    void FlushBuffer();

    inline int ScreenCoordToIndex(const Eigen::Vector2i &ScreenCoord, const Eigen::Vector2i &InScreenRes);

    void DrawRenderBuffer();

    void DrawGrid(const int X, const int Y, const Eigen::Vector3f &Color);

    void DrawEntity(const int X, const int Y, EEntityType Entity);

    void InitDrawStyle();

    const std::vector<Eigen::Vector3f> &GetEntityDrawStyle(EEntityType EntityType);

    Eigen::Vector2i CurrentDirection = Eigen::Vector2i(1, 0);

private:
    bool IsCollideWithBorderOrSelf();

    void RestartGame();

    Eigen::Vector2i GetCandySpawnPosition();

    std::vector<Eigen::Vector2i> Border;

private:
    std::vector<Eigen::Vector3f> Buffer;
    std::vector<EEntityType> EntityOnBoard;
    std::vector<Eigen::Vector3f> RenderBuffer;
    Eigen::Vector2i BoardSize;

    Candy mCandy;
    Snake mSnake;

    GameSettings Settings;
    EGameState State;
    std::map<EEntityType, std::vector<Eigen::Vector3f> > DrawStyle;
};