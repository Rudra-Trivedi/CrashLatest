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


// X for giant spawn area

static const float GiantLeftSpawnX = (LEFT_BRIDGE_CENTER_X); 

static const float GiantRightSpawnX = (RIGHT_BRIDGE_CENTER_X );

// Y for giant spawn area
static const float GiantSpawnY = (RIVER_TOP_Y - 2.f);

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


Game& g = Game::get();
//Player& northPlayer = g.getPlayer(m_pPlayer->isNorth());
//Player& southPlayer = g.getPlayer(!(m_pPlayer->isNorth()));





std::vector<Entity*> OpponentBuildings;
std::vector<Entity*> OpponentMobs;


// Returns true if there's already a giant present
bool Controller_AI_Rudra::isGiantPresent()
{
    for (Entity* pMob : g.getPlayer(m_pPlayer->isNorth()).getMobs())
    {
        if (!pMob->isDead())
        {
            if (pMob->getStats().getMobType() == iEntityStats::Giant)
            {
                return true;
            }
        }
        else
        {
            continue;
        }
    }

    return false;
}

bool Controller_AI_Rudra::isKnightPresent()
{
    for (Entity* pMob : g.getPlayer(m_pPlayer->isNorth()).getMobs())
    {
        if (!pMob->isDead())
        {
            if (pMob->getStats().getMobType() == iEntityStats::Swordsman)
            {
                return true;
            }
        }
        else
        {
            continue;
        }
    }

    return false;
}
bool Controller_AI_Rudra::isArcherPresent()
{
    for (Entity* pMob : g.getPlayer(m_pPlayer->isNorth()).getMobs())
    {
        if (!pMob->isDead())
        {
            if (pMob->getStats().getMobType() == iEntityStats::Archer)
            {
                return true;
            }
        }
        else
        {
            continue;
        }
    }

    return false;
}

float Controller_AI_Rudra::isOpponentMobPresent(iEntityStats::MobType mobType)
{
    for (Entity* othermob : g.getPlayer(!(m_pPlayer->isNorth())).getMobs())
    {
        if (!othermob->isDead())
        {
            if (othermob->getStats().getMobType() == mobType)
            {
                return othermob->getPosition().x;
            }
        }
    }

    return -1;
}


// Getting the State of the player controlled by our AI
void Controller_AI_Rudra::getattackstatus()
{

    if (!g.getPlayer(!(m_pPlayer->isNorth())).getMobs().empty())
    {
        isattacking = false;
    }
    else
    {
        isattacking = true;
    }
    
}


// Getting the opponent buildings
void Controller_AI_Rudra::GetOpponentBuildings()
{
    OpponentBuildings = g.getPlayer(!(m_pPlayer->isNorth())).getBuildings();
}


// Finding the nearest spawn point accoding to buidling health 
float Controller_AI_Rudra::nearestXforSpawn(float x, float x1)
{
    
    std::vector<Entity*> pbuild;
    for (Entity* otherbuilding : OpponentBuildings)
    {
       
        if (otherbuilding->getStats().getBuildingType() == iEntityStats::BuildingType::Princess)
        {
            pbuild.push_back(otherbuilding);
        }
       
    }
    
        if (pbuild[0]->getHealth() < pbuild[1]->getHealth())
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

        else if (pbuild[0]->getHealth() > pbuild[1]->getHealth())
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



float Controller_AI_Rudra::nearestXforDefence(float x, float x1, Entity* OMob)
{
    if (!OMob->isDead())
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

    else
    {
        return x1;
    }
}



void Controller_AI_Rudra::GetEnemiesOnField()
{
    for (Entity* othermob : g.getPlayer(!(m_pPlayer->isNorth())).getMobs())
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

    // Get opponent buildings

    GetOpponentBuildings();

    // choose whether to attack or defend
    getattackstatus();

    // attack strategy
    if (isattacking)
    {

        if (m_pPlayer->getElixir() >= 9)
        {
            if (!isGiantPresent())
            {
                
                Vec2 GiantSpawnWorld(nearestXforSpawn(GiantLeftSpawnX, GiantRightSpawnX), GiantSpawnY);
                Vec2 GiantGamePos = GiantSpawnWorld.Player2Game(m_pPlayer->isNorth());
                m_pPlayer->placeMob(iEntityStats::Giant, GiantGamePos);

                Vec2 ArcherSpawnWorld(GiantSpawnWorld.x, ArcherSpawnY);
                Vec2 ArcherGamePos = ArcherSpawnWorld.Player2Game(m_pPlayer->isNorth());
                m_pPlayer->placeMob(iEntityStats::Archer, ArcherGamePos);
                m_pPlayer->placeMob(iEntityStats::Archer, ArcherGamePos);

            }

            else if (isGiantPresent())
            {
                Vec2 ArcherSpawnWorld(nearestXforSpawn(ArcherLeftSpawnX,ArcherRightSpawnX), ArcherSpawnY);
                Vec2 ArcherGamePos = ArcherSpawnWorld.Player2Game(m_pPlayer->isNorth());
                m_pPlayer->placeMob(iEntityStats::Archer, ArcherGamePos);
                m_pPlayer->placeMob(iEntityStats::Archer, ArcherGamePos);
            }

        }

        else if (m_pPlayer->getElixir() >=6 && m_pPlayer->getElixir() < 9)
        {
            if (isGiantPresent())
            {
                Vec2 ArcherSpawnWorld(nearestXforSpawn(ArcherLeftSpawnX, ArcherRightSpawnX), ArcherSpawnY);
                Vec2 ArcherGamePos = ArcherSpawnWorld.Player2Game(m_pPlayer->isNorth());
                m_pPlayer->placeMob(iEntityStats::Archer, ArcherGamePos);
                m_pPlayer->placeMob(iEntityStats::Archer, ArcherGamePos);
            } 
            else
            {
                Vec2 GiantSpawnWorld(nearestXforSpawn(GiantLeftSpawnX, GiantRightSpawnX), GiantSpawnY);
                Vec2 GiantGamePos = GiantSpawnWorld.Player2Game(m_pPlayer->isNorth());
                m_pPlayer->placeMob(iEntityStats::Giant, GiantGamePos);
            }

        }

        else if(m_pPlayer->getElixir() >= 4 && m_pPlayer->getElixir() < 6)
        {
             if (isGiantPresent())
            {
                Vec2 ArcherSpawnWorld(nearestXforSpawn(ArcherLeftSpawnX, ArcherRightSpawnX), ArcherSpawnY);
                Vec2 ArcherGamePos = ArcherSpawnWorld.Player2Game(m_pPlayer->isNorth());
                m_pPlayer->placeMob(iEntityStats::Archer, ArcherGamePos);
                m_pPlayer->placeMob(iEntityStats::Archer, ArcherGamePos);
            } 
        }


    }


    // defence strategy
    else
    {
        int RightTroops = 0;
        int LeftTroops = 0;
        for (Entity* othermob : g.getPlayer(!(m_pPlayer->isNorth())).getMobs())
        {
            if (othermob->getPosition().x > KingX)
            {
                RightTroops++;
            }
            else
            {
                LeftTroops++;
            }
        }
        if (m_pPlayer->getElixir() >= 8)
        {
            std::cout << "Enough ELixir to Spawn Strong Defense" << std::endl;


            if (RightTroops > LeftTroops)
            {
                std::cout << "More troops on the right" << std::endl;

                Vec2 KnightSpawnWorld(KnightRightSpawnX, KnightSpawnY);
                Vec2 KnightGamePos = KnightSpawnWorld.Player2Game(m_pPlayer->isNorth());
                m_pPlayer->placeMob(iEntityStats::Swordsman, KnightGamePos);

                Vec2 ArcherSpawnWorld(ArcherRightSpawnX , KnightSpawnY - 1.f);
                Vec2 ArcherGamePos = ArcherSpawnWorld.Player2Game(m_pPlayer->isNorth());
                m_pPlayer->placeMob(iEntityStats::Archer, ArcherGamePos);
                m_pPlayer->placeMob(iEntityStats::Archer, ArcherGamePos);
            }

            else
            {
                std::cout << "More troops on the left" << std::endl;

                Vec2 KnightSpawnWorld(KnightLeftSpawnX, KnightSpawnY );
                Vec2 KnightGamePos = KnightSpawnWorld.Player2Game(m_pPlayer->isNorth());
                m_pPlayer->placeMob(iEntityStats::Swordsman, KnightGamePos);

                Vec2 ArcherSpawnWorld(ArcherLeftSpawnX , KnightSpawnY - 1.f);
                Vec2 ArcherGamePos = ArcherSpawnWorld.Player2Game(m_pPlayer->isNorth());
                m_pPlayer->placeMob(iEntityStats::Archer, ArcherGamePos);
                m_pPlayer->placeMob(iEntityStats::Archer, ArcherGamePos);
            }
        }

        else if (m_pPlayer->getElixir() >= 5 && m_pPlayer->getElixir() < 8)
        {
            float archx = isOpponentMobPresent(iEntityStats::Archer);
            float knightx = isOpponentMobPresent(iEntityStats::Swordsman);
            float giantx = isOpponentMobPresent(iEntityStats::Giant);
            std::cout << "Enough ELixir to Spawn counter defense" << std::endl;

            if (archx > 0)
            {
                if (archx > KingX)
                {
                    std::cout << "Spawned defense against Archers on the right" << std::endl;

                    Vec2 ArcherSpawnWorld(ArcherRightSpawnX, ArcherSpawnY);
                    Vec2 ArcherGamePos = ArcherSpawnWorld.Player2Game(m_pPlayer->isNorth());
                    m_pPlayer->placeMob(iEntityStats::Archer, ArcherGamePos);
                    m_pPlayer->placeMob(iEntityStats::Archer, ArcherGamePos);
                }
                else
                {
                    std::cout << "Spawned defense against Archers on the left" << std::endl;

                    Vec2 ArcherSpawnWorld(ArcherLeftSpawnX, ArcherSpawnY);
                    Vec2 ArcherGamePos = ArcherSpawnWorld.Player2Game(m_pPlayer->isNorth());
                    m_pPlayer->placeMob(iEntityStats::Archer, ArcherGamePos);
                    m_pPlayer->placeMob(iEntityStats::Archer, ArcherGamePos);
                }
            }

            else if (knightx > 0)
            {
                if (knightx > KingX)
                {
                    std::cout << "Spawned defense against Knight on the right" << std::endl;

                    Vec2 KnightSpawnWorld(KnightRightSpawnX, KnightSpawnY);
                    Vec2 KnightGamePos = KnightSpawnWorld.Player2Game(m_pPlayer->isNorth());
                    m_pPlayer->placeMob(iEntityStats::Swordsman, KnightGamePos);
                }
                else
                {

                    std::cout << "Spawned defense against Knight on the left" << std::endl;

                    Vec2 KnightSpawnWorld(KnightLeftSpawnX, KnightSpawnY);
                    Vec2 KnightGamePos = KnightSpawnWorld.Player2Game(m_pPlayer->isNorth());
                    m_pPlayer->placeMob(iEntityStats::Swordsman, KnightGamePos);
                }

            }

            else if (giantx > KingX)
            {
                if (giantx > KingX)
                {
                    std::cout << "Spawned defense against Giant on the right" << std::endl;

                    Vec2 KnightSpawnWorld(KnightRightSpawnX, KnightSpawnY);
                    Vec2 KnightGamePos = KnightSpawnWorld.Player2Game(m_pPlayer->isNorth());
                    m_pPlayer->placeMob(iEntityStats::Swordsman, KnightGamePos);
                }
                else
                {
                    std::cout << "Spawned defense against Giant on the left" << std::endl;

                    Vec2 KnightSpawnWorld(KnightLeftSpawnX, KnightSpawnY);
                    Vec2 KnightGamePos = KnightSpawnWorld.Player2Game(m_pPlayer->isNorth());
                    m_pPlayer->placeMob(iEntityStats::Swordsman, KnightGamePos);
                }
            }

        }

        else if (m_pPlayer->getElixir() >= 3 && m_pPlayer->getElixir() < 5)
        {
        std::cout << "Spawning defense to stall attack" << std::endl;

            if (!isKnightPresent())
            {
                 if (RightTroops > LeftTroops)
                 {
                     std::cout << "Spawned knight on right to stall attack" << std::endl;

                     Vec2 KnightSpawnWorld(KnightRightSpawnX, KnightSpawnY);
                     Vec2 KnightGamePos = KnightSpawnWorld.Player2Game(m_pPlayer->isNorth());
                     m_pPlayer->placeMob(iEntityStats::Swordsman, KnightGamePos);
                 }

                else
                {
                    std::cout << "Spawned knight on left to stall attack" << std::endl;

                    Vec2 KnightSpawnWorld(KnightLeftSpawnX, KnightSpawnY);
                    Vec2 KnightGamePos = KnightSpawnWorld.Player2Game(m_pPlayer->isNorth());
                    m_pPlayer->placeMob(iEntityStats::Swordsman, KnightGamePos);

                }
            }

            else if(!isArcherPresent())
            {
                if (RightTroops > LeftTroops)
                {
                    std::cout << "Knight Present sending Archers on right" << std::endl;

                    Vec2 ArcherSpawnWorld(ArcherRightSpawnX, ArcherSpawnY);
                    Vec2 ArcherGamePos = ArcherSpawnWorld.Player2Game(m_pPlayer->isNorth());
                    m_pPlayer->placeMob(iEntityStats::Archer, ArcherGamePos);
                    m_pPlayer->placeMob(iEntityStats::Archer, ArcherGamePos);
                }

                else
                {
                    std::cout << "Sending archers to supoort knight on the right" << std::endl;

                    Vec2 ArcherSpawnWorld(ArcherLeftSpawnX, ArcherSpawnY);
                    Vec2 ArcherGamePos = ArcherSpawnWorld.Player2Game(m_pPlayer->isNorth());
                    m_pPlayer->placeMob(iEntityStats::Archer, ArcherGamePos);
                    m_pPlayer->placeMob(iEntityStats::Archer, ArcherGamePos);
                }
            }
        }
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

