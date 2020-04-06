// MIT License
// 
// Copyright(c) 2020 Arthur Bacon and Kevin Dill
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this softwareand associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
// 
// The above copyright noticeand this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "Controller_AI_Rudra.h"
#include <cmath>

#include "/Game ai/New folder/CrashLoyal-master/Interface/src/Constants.h"
#include "/Game ai/New folder/CrashLoyal-master/Interface/src/EntityStats.h"
#include "/Game ai/New folder/CrashLoyal-master/Interface/src/iPlayer.h"
#include "/Game ai/New folder/CrashLoyal-master/Interface/src/Vec2.h"
#include "/Game ai/New folder/CrashLoyal-master/Game/src/Game.h"
#include "/Game ai/New folder/CrashLoyal-master/Game/src/Player.h"
#include "/Game ai/New folder/CrashLoyal-master/Game/src/Entity.h"

// X for giant spawn area

static const float GiantLeftSpawnX = (LEFT_BRIDGE_CENTER_X); 

static const float GiantRightSpawnRightX = (RIGHT_BRIDGE_CENTER_X );

// Y for giant spawn area
static const float GiantSpawnY = (RIVER_TOP_Y - 1.f);

// X for Knight Spawn

static const float KnightLeftSpawnX = (LEFT_BRIDGE_CENTER_X + BRIDGE_WIDTH + 1.f );

static const float KnightRightSpawnX = (RIGHT_BRIDGE_CENTER_X - BRIDGE_WIDTH - 1.f);

// Y for Knight Spawn

static const float KnightSpawnY = (RIVER_TOP_Y - 2.f);

// Archer spawn X
static const float ArcherLeftSpawnX = LEFT_BRIDGE_CENTER_X;
static const float ArcherRightSpawnX = RIGHT_BRIDGE_CENTER_X;

// Archer Spawn Y

static const float ArcherSpawnY = 0.f;  


static const Vec2 ksGiantPos(LEFT_BRIDGE_CENTER_X, RIVER_TOP_Y - 5.f);
static const Vec2 ksArcherPos(LEFT_BRIDGE_CENTER_X, 0.f);
bool isattacking = true;
std::vector<Entity*> OpponentBuildings;
std::vector<Entity*> OpponentMobs;
Game& g = Game::get();
Player& northPlayer = g.getPlayer(true);
Player& southPlayer = g.getPlayer(false);


// Getting the State of the player controlled by our AI
void Controller_AI_Rudra::getattackstatus()
{

    if (!OpponentMobs.empty())
    {
        isattacking = false;
    }
    
}


// Getting the opponent buildings
void Controller_AI_Rudra::GetOpponentBuildings()
{
    OpponentBuildings = southPlayer.getBuildings();
}


// Finding the nearest spawn point accoding to buidling health 
float Controller_AI_Rudra::nearestXforSpawn(float x, float x1)
{
    float xval;
    std::vector<Entity*> pbuild;
    for (Entity* otherbuilding : OpponentBuildings)
    {
       
        if (otherbuilding->getStats().getBuildingType() == iEntityStats::BuildingType::Princess)
        {
            pbuild.push_back(otherbuilding);
        }
       
    }

    if (pbuild[0]->getHealth() < pbuild[1]->getHealth() || pbuild[0]->getHealth() == pbuild[1]->getHealth())
    {
        float a, b;
        a = abs(pbuild[0]->getPosition().x - x);
        b = abs(pbuild[0]->getPosition().x - x1);
        if (a < b)
        {
            return x;
        }
        else
        {
            return x1;
        }
    }

    else if(pbuild[0]->getHealth() > pbuild[1]->getHealth())
    {
        float a, b;
        a = abs(pbuild[1]->getPosition().x - x);
        b = abs(pbuild[1]->getPosition().x - x1);
        if (a < b)
        {
            return x;
        }
        else
        {
            return x1;
        }
    }

    return x1;
    
}


float Controller_AI_Rudra::nearestXforDefenceGiant(float x, float x1)
{
    for (Entity* OMob : OpponentMobs)
    {
        if (!OMob->isDead())
        {
            if (OMob->getStats().getMobType() == iEntityStats::Giant)
            {
                if ((abs(OMob->getPosition().x - x)) < (abs(OMob->getPosition().x - x1)) || (abs(OMob->getPosition().x - x)) == (abs(OMob->getPosition().x - x1)))
                {
                    return x;
                }
                else if ((abs(OMob->getPosition().x - x)) > (abs(OMob->getPosition().x - x1)))
                {
                    return x1;
                }
            }
        }

        else
        {
            continue;
        }
    }

    return x1;
}

void Controller_AI_Rudra::GetEnemiesOnField()
{
    for (Entity* othermob : southPlayer.getMobs())
    {
        if (!othermob->isDead())
        {
            OpponentMobs.push_back(othermob);
        }
    }
}





void Controller_AI_Rudra::tick(float deltaTSec)
{
    assert(m_pPlayer);

    //Get Enemies on Field
    GetEnemiesOnField();

    // choose whether to attack or defend
    getattackstatus();

    // attack strategy
    if (isattacking)
    {

        if (m_pPlayer->getElixir() >= 8)
        {

        }


    }


    // defence strategy
    else
    {





    }



    //// wait for elixir
    //if (m_pPlayer->getElixir() >= 9)
    //{
    //    // convert the positions from player space to game space
    //    bool isNorth = m_pPlayer->isNorth();
    //    Vec2 giantPos_Game = ksGiantPos.Player2Game(isNorth);
    //    Vec2 archerPos_Game = ksArcherPos.Player2Game(isNorth);

    //    // Create two archers and a giant
    //    m_pPlayer->placeMob(iEntityStats::Giant, giantPos_Game);
    //    m_pPlayer->placeMob(iEntityStats::Archer, archerPos_Game);
    //    m_pPlayer->placeMob(iEntityStats::Archer, archerPos_Game);
    //}

    
}

