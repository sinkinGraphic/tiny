#include "GamePlay.h"

void GamePlay::ProcessInput(EKeyBoradKeys Key)
{
    const Eigen::Vector2i Key2Direction[4] = {
        Eigen::Vector2i(0, -1),
        Eigen::Vector2i(-1, 0),
        Eigen::Vector2i(0, 1),
        Eigen::Vector2i(1, 0)};
    Eigen::Vector2i NewDirection = Key2Direction[static_cast<int>(Key)];
    CurrentDirection = CurrentDirection == NewDirection * -1 ? CurrentDirection : NewDirection;
}

void GamePlay::SetScreenResolution(const int X, const int Y)
{
    Buffer.resize(X * Y);
    RenderBuffer.resize(X * Y * 20 * 20, Eigen::Vector3f());
    ScreenRes.x() = X;
    ScreenRes.y() = Y;

    InitBorder();

    DrawRenderBuffer();

    mSnake.BodyCoord.push_back(Eigen::Vector2i(X / 2, Y / 2));
}

void GamePlay::Step()
{
    auto Size = mSnake.BodyCoord.size();
    for (size_t i = Size; i != 1; --i)
    {
        mSnake.BodyCoord[i] = mSnake.BodyCoord[i - 1];
    }
    mSnake.BodyCoord[0] += CurrentDirection;
    FlushBuffer();
}

void GamePlay::CreateNewCandy()
{
}

void GamePlay::FlushBuffer()
{
    int X=ScreenRes.x(),Y=ScreenRes.y();
    Buffer.clear();
    Buffer.resize(X * Y,Eigen::Vector3f(0.f,0.f,0.f));
    InitBorder();
    RenderBuffer.resize(X * Y * 20 * 20, Eigen::Vector3f());

    for(int i=0;i!=mSnake.BodyCoord.size();++i)
    {
        Buffer[ScreenCoordToIndex(mSnake.BodyCoord[i],ScreenRes)]=Eigen::Vector3f(1.f,0.f,0.f);
    }
    DrawRenderBuffer();
}

void GamePlay::InitBorder()
{
    const Eigen::Vector3f BorderColor = Eigen::Vector3f(1.f, 1.f, 1.f);

    for (int i = 0; i != ScreenRes.y(); ++i)
    {
        Buffer[ScreenCoordToIndex(Eigen::Vector2i(0, i), ScreenRes)] = BorderColor;
        Buffer[ScreenCoordToIndex(Eigen::Vector2i(ScreenRes.x() - 1, i), ScreenRes)] = BorderColor;
    }
    for (int i = 0; i != ScreenRes.x(); ++i)
    {
        Buffer[ScreenCoordToIndex(Eigen::Vector2i(i, 0), ScreenRes)] = BorderColor;
        Buffer[ScreenCoordToIndex(Eigen::Vector2i(i, ScreenRes.y() - 1), ScreenRes)] = BorderColor;
    }
}

void GamePlay::DrawRenderBuffer()
{
    for (int i = 0; i != ScreenRes.x(); ++i)
    {
        for (int j = 0; j != ScreenRes.y(); ++j)
        {
            DrawGrid(i, j, Buffer[ScreenCoordToIndex(Eigen::Vector2i(i, j), ScreenRes)]);
        }
    }
}

void GamePlay::DrawGrid(int X, int Y, const Eigen::Vector3f &Color)
{
    constexpr int GridSize = 20;
    for (int i = X * GridSize; i != X * GridSize + GridSize; ++i)
    {
        for (int j = Y * GridSize; j != Y * GridSize + GridSize; ++j)
        {
            RenderBuffer[ScreenCoordToIndex(Eigen::Vector2i(i, j), Eigen::Vector2i(1000, 1000))] = Color;
        }
    }
}

int GamePlay::ScreenCoordToIndex(const Eigen::Vector2i &ScreenCoord, const Eigen::Vector2i &InScreenRes)
{
    return ScreenCoord.y() * InScreenRes.x() + ScreenCoord.x();
}