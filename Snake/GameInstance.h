#pragma once
#include "GamePlay.h"

class GameInstance
{
public:
    void Start();

private:
    void Tick(float DeltaSeconds);


private:
    GamePlay GamePlayLogic;
};