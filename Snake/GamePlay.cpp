#include "GamePlay.h"
#include <iostream>

void GamePlay::InitGame(const GameSettings &InGameSettings)
{
    Settings = InGameSettings;
    SetScreenResolution(Settings.BoardSize.x(), Settings.BoardSize.y());
}

void GamePlay::ProcessInput(EKeyBoardKeys Key)
{
    const Eigen::Vector2i Key2Direction[4] = {
            Eigen::Vector2i(0, -1),
            Eigen::Vector2i(-1, 0),
            Eigen::Vector2i(0, 1),
            Eigen::Vector2i(1, 0)
    };
    Eigen::Vector2i NewDirection = Key2Direction[static_cast<int>(Key)];
    CurrentDirection = CurrentDirection == NewDirection * -1 ? CurrentDirection : NewDirection;
}

void GamePlay::SetScreenResolution(const int X, const int Y)
{
    Buffer.resize(X * Y);
    EntityOnBoard.resize(X * Y);
    RenderBuffer.resize(X * Y * Settings.GridSize * Settings.GridSize, Eigen::Vector3f());
    BoardSize.x() = X;
    BoardSize.y() = Y;

    InitBorder();
    InitDrawStyle();
    mSnake.BodyCoord.emplace_back(Eigen::Vector2i(X / 2, Y / 2));
    mSnake.BodyCoord.emplace_back(Eigen::Vector2i(X / 2, Y / 2));
    CreateNewCandy();
}

EGameState GamePlay::GetGameState()
{
    return State;
}

void GamePlay::Step()
{
    mSnake.BodyCoord[0] += CurrentDirection;
    if (mSnake.CheckCandy(mCandy.Pos))
    {
        mSnake.BodyCoord.emplace_back(Eigen::Vector2i());
        CreateNewCandy();
    }
    auto Size = mSnake.BodyCoord.size();
    for (size_t i = Size - 1 ; i > 0 ; --i)
    {
        mSnake.BodyCoord[i] = mSnake.BodyCoord[i - 1];
    }
    if (IsCollideWithBorderOrSelf())
    {
        std::cout << "Collision " << std::endl;
        State = EGameState::Error;
        return;
    }
    FlushBuffer();
}

void GamePlay::InitDrawStyle()
{
    DrawStyle.clear();
    DrawStyle.emplace(EEntityType::Border, std::vector<Eigen::Vector3f>());
    DrawStyle.emplace(EEntityType::SnakeBody, std::vector<Eigen::Vector3f>());
    DrawStyle.emplace(EEntityType::SnakeHead, std::vector<Eigen::Vector3f>());
    DrawStyle.emplace(EEntityType::Candy, std::vector<Eigen::Vector3f>());

    const Eigen::Vector3f BackGroundColor = Eigen::Vector3f(157.f, 173.f, 132.f) / 255.f;

    const auto Size = Settings.GridSize * Settings.GridSize;
    constexpr int8_t SeamWidth = 1, OutLineWidth = 3, WhiteLineWidth = 3;
    //Candy Style
    DrawStyle[EEntityType::Candy].resize(Settings.GridSize * Settings.GridSize, Eigen::Vector3f());
    for (int i = SeamWidth + OutLineWidth ; i < Settings.GridSize - SeamWidth - OutLineWidth ; ++i)
    {
        for (int j = SeamWidth + OutLineWidth ; j < Settings.GridSize - SeamWidth - OutLineWidth ; ++j)
        {
            bool A = (i < SeamWidth + OutLineWidth + WhiteLineWidth || i > Settings.GridSize - SeamWidth - OutLineWidth - WhiteLineWidth - 1);
            bool B = (j < SeamWidth + OutLineWidth + WhiteLineWidth || j > Settings.GridSize - SeamWidth - OutLineWidth - WhiteLineWidth - 1);
            if (!A && !B)
            {
                continue;
            }
            DrawStyle[EEntityType::Candy][ScreenCoordToIndex(Eigen::Vector2i(i, j), Eigen::Vector2i(Settings.GridSize, Settings.GridSize))] = BackGroundColor;
        }
    }

    //Border Style
    const Eigen::Vector3f BorderColor = BackGroundColor;
    DrawStyle[EEntityType::Border].resize(Size, BorderColor);

    constexpr int BorderWidth=6;
}

void GamePlay::CreateNewCandy()
{
    auto NewCandyPosition = GetCandySpawnPosition();
    mCandy.Pos = NewCandyPosition;
}

void GamePlay::FlushBuffer()
{
    int X = BoardSize.x(), Y = BoardSize.y();
    const Eigen::Vector3f BackGroundColor = Eigen::Vector3f(157.f, 173.f, 132.f) / 255.f;
    Buffer.clear();
    Buffer.resize(X * Y, BackGroundColor);
    EntityOnBoard.clear();
    EntityOnBoard.resize(X * Y, EEntityType::None);
    InitBorder();
    RenderBuffer.resize(X * Y * Settings.GridSize * Settings.GridSize, BackGroundColor);

    //draw Snake
    for (int i = 0 ; i != mSnake.BodyCoord.size() ; ++i)
    {
        Buffer[ScreenCoordToIndex(mSnake.BodyCoord[i], BoardSize)] = Eigen::Vector3f(1.f, 0.f, 0.f);
        EntityOnBoard[ScreenCoordToIndex(mSnake.BodyCoord[i], BoardSize)] = i == 0 || i == 1 ? EEntityType::SnakeHead : EEntityType::SnakeBody;
    }

    //draw candy
    Buffer[ScreenCoordToIndex(mCandy.Pos, BoardSize)] = Eigen::Vector3f(0.f, 1.f, 0.f);
    EntityOnBoard[ScreenCoordToIndex(mCandy.Pos, BoardSize)] = EEntityType::Candy;
    DrawRenderBuffer();
}

void GamePlay::InitBorder()
{
    const Eigen::Vector3f BorderColor = Eigen::Vector3f(1.f, 1.f, 1.f);

    for (int i = 0 ; i != BoardSize.y() ; ++i)
    {
        Buffer[ScreenCoordToIndex(Eigen::Vector2i(0, i), BoardSize)] = BorderColor;
        Buffer[ScreenCoordToIndex(Eigen::Vector2i(BoardSize.x() - 1, i), BoardSize)] = BorderColor;
        EntityOnBoard[ScreenCoordToIndex(Eigen::Vector2i(0, i), BoardSize)] = EEntityType::Border;
        EntityOnBoard[ScreenCoordToIndex(Eigen::Vector2i(BoardSize.x() - 1, i), BoardSize)] = EEntityType::Border;

        Border.emplace_back(Eigen::Vector2i(0, i));
        Border.emplace_back(Eigen::Vector2i(BoardSize.x() - 1, i));
    }
    for (int i = 0 ; i != BoardSize.x() ; ++i)
    {
        Buffer[ScreenCoordToIndex(Eigen::Vector2i(i, 0), BoardSize)] = BorderColor;
        Buffer[ScreenCoordToIndex(Eigen::Vector2i(i, BoardSize.y() - 1), BoardSize)] = BorderColor;
        EntityOnBoard[ScreenCoordToIndex(Eigen::Vector2i(i, 0), BoardSize)] = EEntityType::Border;
        EntityOnBoard[ScreenCoordToIndex(Eigen::Vector2i(i, BoardSize.y() - 1), BoardSize)] = EEntityType::Border;
        Border.emplace_back(Eigen::Vector2i(i, 0));
        Border.emplace_back(Eigen::Vector2i(i, BoardSize.y() - 1));
    }
}

const std::vector<Eigen::Vector3f> &GamePlay::GetEntityDrawStyle(EEntityType EntityType)
{
    return DrawStyle[EntityType];
}

void GamePlay::DrawEntity(const int X, const int Y, EEntityType Entity)
{
    const int GridSize = Settings.GridSize;
    const std::vector<Eigen::Vector3f> &EntityStyle = GetEntityDrawStyle(Entity);
    if (Entity != EEntityType::Candy)
    {
        return;
    }
    for (int i = X * GridSize, a = 0 ; i != X * GridSize + GridSize ; ++i, ++a)
    {
        for (int j = Y * GridSize, b = 0 ; j != Y * GridSize + GridSize ; ++j, ++b)
        {
            RenderBuffer[ScreenCoordToIndex(Eigen::Vector2i(i, j), Eigen::Vector2i(Settings.BoardSize.x() * Settings.GridSize, Settings.BoardSize.y() * Settings.GridSize))] =
                    EntityStyle[ScreenCoordToIndex(Eigen::Vector2i(a, b), Eigen::Vector2i(GridSize, GridSize))];

        }
    }
}

void GamePlay::DrawRenderBuffer()
{
    for (int i = 0 ; i != BoardSize.x() ; ++i)
    {
        for (int j = 0 ; j != BoardSize.y() ; ++j)
        {
            DrawGrid(i, j, Buffer[ScreenCoordToIndex(Eigen::Vector2i(i, j), BoardSize)]);
            DrawEntity(i, j, EntityOnBoard[ScreenCoordToIndex(Eigen::Vector2i(i, j), BoardSize)]);
        }
    }
}

void GamePlay::DrawGrid(int X, int Y, const Eigen::Vector3f &Color)
{
    const int GridSize = Settings.GridSize;
    for (int i = X * GridSize ; i != X * GridSize + GridSize ; ++i)
    {
        for (int j = Y * GridSize ; j != Y * GridSize + GridSize ; ++j)
        {
            RenderBuffer[ScreenCoordToIndex(Eigen::Vector2i(i, j), Eigen::Vector2i(Settings.BoardSize.x() * Settings.GridSize, Settings.BoardSize.y() * Settings.GridSize))] = Color;
        }
    }
}

int GamePlay::ScreenCoordToIndex(const Eigen::Vector2i &ScreenCoord, const Eigen::Vector2i &InScreenRes)
{
    return ScreenCoord.y() * InScreenRes.x() + ScreenCoord.x();
}

bool GamePlay::IsCollideWithBorderOrSelf()
{
    //Check collision with border
    for (const auto &rCoord : Border)
    {
        if (mSnake.BodyCoord[1] == rCoord)
        {
            return true;
        }
    }

    //check collision with self
    for (size_t Index = 2 ; Index < mSnake.BodyCoord.size() ; ++Index)
    {
        //only head can collide with body
        if (mSnake.BodyCoord[1] == mSnake.BodyCoord[Index])
        {
            return true;
        }
    }

    return false;
}

void GamePlay::RestartGame()
{
}

template<typename ElementType>
auto Find(const std::vector<ElementType> &Container, const ElementType &Element)
{
    auto First = Container.begin();
    auto End = Container.end();
    while (First != End)
    {
        if (*First == Element)
        {
            break;
        }
        ++First;
    }
    return First;
}

Eigen::Vector2i GamePlay::GetCandySpawnPosition()
{
    Eigen::Vector2i SpawnPosition;
    std::vector<Eigen::Vector2i> AvailableSpawnPoint;
    for (int i = 0 ; i != Settings.BoardSize.x() ; ++i)
    {
        for (int j = 0 ; j != Settings.BoardSize.y() ; ++j)
        {
            AvailableSpawnPoint.emplace_back(Eigen::Vector2i(i, j));
        }
    }

    //exclude border
    for (const auto &rBoder : Border)
    {
        auto Iter = Find(AvailableSpawnPoint, rBoder);
        if (Iter != AvailableSpawnPoint.end())
        {
            AvailableSpawnPoint.erase(Iter);
        }
    }

    //exclude snake
    for (const auto &rSnake : mSnake.BodyCoord)
    {
        auto Iter = Find(AvailableSpawnPoint, rSnake);
        if (Iter != AvailableSpawnPoint.end())
        {
            AvailableSpawnPoint.erase(Iter);
        }
    }
    auto Size = rand() % AvailableSpawnPoint.size();
    std::cout << Size << std::endl;
    SpawnPosition = AvailableSpawnPoint[Size];
    return SpawnPosition;
}