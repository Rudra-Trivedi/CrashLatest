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

#include "Mob.h"

#include "Constants.h"
#include "Game.h"

#include <algorithm>
#include <vector>

#include<Player.h>


Mob::Mob(const iEntityStats& stats, const Vec2& pos, bool isNorth)
    : Entity(stats, pos, isNorth)
    , m_pWaypoint(NULL)
{
    assert(dynamic_cast<const iEntityStats_Mob*>(&stats) != NULL);
}

void Mob::tick(float deltaTSec)
{
    // Tick the entity first.  This will pick our target, and attack it if it's in range.
    Entity::tick(deltaTSec);

    // if our target isn't in range, move towards it.
    if (!targetInRange())
    {
        move(deltaTSec);
    }
}

void Mob::move(float deltaTSec)
{
    // If we have a target and it's on the same side of the river, we move towards it.
    //  Otherwise, we move toward the bridge.
    bool bMoveToTarget = false;
    if (!!m_pTarget)
    {    
        bool imTop = m_Pos.y < (GAME_GRID_HEIGHT / 2);
        bool otherTop = m_pTarget->getPosition().y < (GAME_GRID_HEIGHT / 2);

        if (imTop == otherTop)
        {
            bMoveToTarget = true;
        }
    }

    Vec2 destPos;
    if (bMoveToTarget)
    { 
        m_pWaypoint = NULL;
        destPos = m_pTarget->getPosition();
    }
    else
    {
        if (!m_pWaypoint)
        {
            m_pWaypoint = pickWaypoint();
        }
        destPos = m_pWaypoint ? *m_pWaypoint : m_Pos;
    }

    // Actually do the moving
    Vec2 moveVec = destPos - m_Pos;
    float distRemaining = moveVec.normalize();
    float moveDist = m_Stats.getSpeed() * deltaTSec;

    // if we're moving to m_pTarget, don't move into it
    if (bMoveToTarget)
    {
        assert(m_pTarget);
        distRemaining -= (m_Stats.getSize() + m_pTarget->getStats().getSize()) / 2.f;
        distRemaining = std::max(0.f, distRemaining);
    }

    if (moveDist <= distRemaining)
    {
        m_Pos += moveVec * moveDist;
    }
    else
    {
        m_Pos += moveVec * distRemaining;

        // if the destination was a waypoint, find the next one and continue movement
        if (m_pWaypoint)
        {
            m_pWaypoint = pickWaypoint();
            destPos = m_pWaypoint ? *m_pWaypoint : m_Pos;
            moveVec = destPos - m_Pos;
            moveVec.normalize();
            m_Pos += moveVec * distRemaining;
        }
    }

    // PROJECT 3: This is where your collision code will be called from
    std::vector<Entity*> otherMob = checkCollision();
    if (!otherMob.empty())
    {
        for (std::size_t i = 0; i < otherMob.size(); i++)
        {
            processCollision(otherMob[i], deltaTSec);
        }
    }

  
  
}

const Vec2* Mob::pickWaypoint()
{
    float smallestDistSq = FLT_MAX;
    const Vec2* pClosest = NULL;

    for (const Vec2& pt : Game::get().getWaypoints())
    {
        // Filter out any waypoints that are behind (or barely in front of) us.
        // NOTE: (0, 0) is the top left corner of the screen
        float yOffset = pt.y - m_Pos.y;
        if ((m_bNorth && (yOffset < 1.f)) ||
            (!m_bNorth && (yOffset > -1.f)))
        {
            continue;
        }

        float distSq = m_Pos.distSqr(pt);
        if (distSq < smallestDistSq) {
            smallestDistSq = distSq;
            pClosest = &pt;
        }
    }

    return pClosest;
}

// PROJECT 3: 
//  1) return a vector of mobs that we're colliding with
//  2) handle collision with towers & river 
std::vector<Entity*> Mob::checkCollision() 
{
    //for (const Mob* pOtherMob : Game::get().getMobs())
    //{
    //    if (this == pOtherMob) 
    //    {
    //        continue;
    //    }

    //    // PROJECT 3: YOUR CODE CHECKING FOR A COLLISION GOES HERE
    //}
    //return NULL;

    std::vector<Entity*> othermobvec;
    Game& game = Game::get();
    Player& northPlayer = game.getPlayer(true);
    Player& southPlayer = game.getPlayer(false);
    

    if (this->getPosition().y + this->getStats().getSize()/2 > (RIVER_TOP_Y) /*- ((this->getStats().getSize())/2)*/ && this->getPosition().y - this->getStats().getSize()/2 < (RIVER_BOT_Y) /*+ ((this->getStats().getSize()) /2)*/)
    {
        if (!((this->getPosition().x - this->getStats().getSize())>= (LEFT_BRIDGE_CENTER_X - BRIDGE_WIDTH) && (this->getPosition().x + this->getStats().getSize())<= (LEFT_BRIDGE_CENTER_X + BRIDGE_WIDTH)) && !(((this->getPosition().x - this->getStats().getSize()) >= (RIGHT_BRIDGE_CENTER_X - BRIDGE_WIDTH) && ((this->getPosition().x + this->getStats().getSize()) <= (RIGHT_BRIDGE_CENTER_X + BRIDGE_WIDTH)))))
        {
            if (this->isNorth())
            {
                this->m_Pos.y = RIVER_TOP_Y - this->getStats().getSize()/2;
                
            }
            else
            {
                this->m_Pos.y = RIVER_BOT_Y + this->getStats().getSize() / 2;
            }
        }
    }

    // collision inside bridge

    /*if ((this->m_Pos.y > RIVER_TOP_Y) && (this->m_Pos.y < RIVER_BOT_Y))
    {
        if (this->m_Pos.x < (RIGHT_BRIDGE_CENTER_X - BRIDGE_WIDTH))
        {
            this->m_Pos.x = RIGHT_BRIDGE_CENTER_X - BRIDGE_WIDTH;
        }

        else if (this->m_Pos.x > (RIGHT_BRIDGE_CENTER_X + BRIDGE_WIDTH))
        {
            this->m_Pos.x = RIGHT_BRIDGE_CENTER_X + BRIDGE_WIDTH;

        }

        else if (this->m_Pos.x > (LEFT_BRIDGE_CENTER_X - BRIDGE_WIDTH))
        {
            this->m_Pos.x = LEFT_BRIDGE_CENTER_X - BRIDGE_WIDTH;

        }

        else if (this->m_Pos.x < (LEFT_BRIDGE_CENTER_X + BRIDGE_WIDTH))
        {
            this->m_Pos.x = LEFT_BRIDGE_CENTER_X + BRIDGE_WIDTH;

        }
    }*/

    

    for (Entity* otherBuilding : northPlayer.getBuildings())
    {
        if (!otherBuilding->isDead())
        {
            if (this->m_Pos.x >= (otherBuilding->getPosition().x - otherBuilding->getStats().getSize() / 2) && this->m_Pos.x <= (otherBuilding->getPosition().x + otherBuilding->getStats().getSize() / 2) && this->m_Pos.y <= (otherBuilding->getPosition().y + otherBuilding->getStats().getSize() / 2) && this->m_Pos.y >= (otherBuilding->getPosition().y - otherBuilding->getStats().getSize() / 2))
            {

                if (!this->isNorth())
                {
                    this->m_Pos.y = otherBuilding->getPosition().y + otherBuilding->getStats().getSize()/2;
                }

                else
                {
                    if (otherBuilding->getPosition().x > this->getPosition().x)
                    {

                        this->m_Pos.x = otherBuilding->getPosition().x - otherBuilding->getStats().getSize()/2;

                    }
                    else
                    {
                        this->m_Pos.x = otherBuilding->getPosition().x + otherBuilding->getStats().getSize()/2;

                    }
                    m_pWaypoint = pickWaypoint();

                }


            }
        }
    }
    
    for (Entity* otherBuilding : southPlayer.getBuildings())
    {
        if (!otherBuilding->isDead())
        {
            if (this->m_Pos.x >= (otherBuilding->getPosition().x - (otherBuilding->getStats().getSize()/2 + otherBuilding->getStats().getSize() / 4)) && this->m_Pos.x <= (otherBuilding->getPosition().x + (otherBuilding->getStats().getSize()/2 + otherBuilding->getStats().getSize() / 4)) && this->m_Pos.y <= (otherBuilding->getPosition().y + (otherBuilding->getStats().getSize()/2 + otherBuilding->getStats().getSize() / 4)) && this->m_Pos.y >= (otherBuilding->getPosition().y - (otherBuilding->getStats().getSize()/2 + otherBuilding->getStats().getSize() / 4)))

            {
                if (this->isNorth())
                {
                    this->m_Pos.y = otherBuilding->getPosition().y - (otherBuilding->getStats().getSize() / 2 + otherBuilding->getStats().getSize() / 4);
                    
                }

                else
                {
                    if (otherBuilding->getPosition().x > this->getPosition().x)
                    {

                        this->m_Pos.x = otherBuilding->getPosition().x - (otherBuilding->getStats().getSize() / 2 + otherBuilding->getStats().getSize() / 4);

                    }
                    else
                    {
                        this->m_Pos.x = otherBuilding->getPosition().x + (otherBuilding->getStats().getSize() / 2 + otherBuilding->getStats().getSize() / 4);

                    }
                    m_pWaypoint = pickWaypoint();

                }
            }
        }
    }


    for (Entity* otherMob : northPlayer.getMobs()) {
        // don't collide with yourself
        if (this == otherMob)
        {
            continue;
        }

        else
        {
            
           othermobvec.push_back(otherMob);
        }

        // PROJECT 3: YOUR CODE CHECKING FOR A COLLISION GOES HERE
    }

    for (Entity* otherMob : southPlayer.getMobs()) {
        // don't collide with yourself
        if (this == otherMob)
        {
            continue;
        }

        else
        {
           // return otherMob;
            othermobvec.push_back(otherMob);
        }

        // PROJECT 3: YOUR CODE CHECKING FOR A COLLISION GOES HERE
    }
    //return std::shared_ptr<Mob>(nullptr);
    return othermobvec;

}

void Mob::processCollision(Entity* otherMob, float deltaTSec) 
{
    int c = 0;
    if (this->isNorth() && otherMob->isNorth())
    {
        c = 1;
    }
    else if (!(this->isNorth()) && !(this->isNorth()))
    {
        c = 2;
    }
    
    if (!otherMob->isDead() && c>0)
    {
        Vec2 OtherMobPos = otherMob->getPosition();
        float x = OtherMobPos.x;
        float y = OtherMobPos.y;
        float size = otherMob->getStats().getSize();
        int MassSelect = 0;
        if (this->getStats().getMass() > otherMob->getStats().getMass())
        {
            MassSelect = 1;
        }

        else if (this->getStats().getMass() == otherMob->getStats().getMass())
        {
            MassSelect = 2;
        }

        // Getting the coordinates of the corners of the othermob 
        /*Point TL(x - (size / 2), y - (size / 2));
        Point BL(x - (size / 2), y + (size / 2));
        Point TR(x + (size / 2), y - (size / 2));
        Point BR(x + (size / 2), y + (size / 2));*/

        Vec2 TL(x - (size / 2), y + (size / 2));
        Vec2 BL(x - (size / 2), y - (size / 2));
        Vec2 TR(x + (size / 2), y + (size / 2));
        Vec2 BR(x + (size / 2), y - (size / 2));

        // Getting the coordinates of this mob's corners
        /*Point thisTL(this->getPosition()->x - (this->GetSize() / 2), this->getPosition()->y - (this->GetSize() / 2));
        Point thisBL(this->getPosition()->x - (this->GetSize() / 2), this->getPosition()->y + (this->GetSize() / 2));
        Point thisTR(this->getPosition()->x + (this->GetSize() / 2), this->getPosition()->y - (this->GetSize() / 2));
        Point thisBR(this->getPosition()->x + (this->GetSize() / 2), this->getPosition()->y + (this->GetSize() / 2));*/

        Vec2 thisTL(this->getPosition().x - (this->getStats().getSize() / 2), this->getPosition().y + (this->getStats().getSize() / 2));
        Vec2 thisBL(this->getPosition().x - (this->getStats().getSize() / 2), this->getPosition().y - (this->getStats().getSize() / 2));
        Vec2 thisTR(this->getPosition().x + (this->getStats().getSize() / 2), this->getPosition().y + (this->getStats().getSize() / 2));
        Vec2 thisBR(this->getPosition().x + (this->getStats().getSize() / 2), this->getPosition().y - (this->getStats().getSize() / 2));


       // if (TL.insideOf((this->getPosition()), this->getStats().getSize()))
        
        
            if (TL.x <= thisTR.x && TL.x >= thisTL.x && TL.y <= thisTR.y && TL.y >= thisBR.y)
            {
                Vec2 other(((this->getPosition().x) + ((this->getStats().getSize()) / 2)), this->getPosition().y);
                float dis = TL.dist(other);
                float disx = thisTR.x - TL.x;
                float disy = TL.y - thisBR.y;
                if (MassSelect == 1)
                {

                    // otherMob->SetPosX(otherMob->getPosition().x - (dis + (dis / 2)));
                    if (otherMob->isNorth())
                    {
                        otherMob->SetPosY(otherMob->getPosition().y - disy );  // this->getPosition().y - this->getStats().getSize() - disy); // -otherMob->getStats().getSize()); //(dis + (dis / 2)));
                    }

                    else
                    {
                        otherMob->SetPosY(otherMob->getPosition().y  + disy); //this->getPosition().y + this->getStats().getSize() + disy);// +otherMob->getStats().getSize());//(dis + (dis / 2)));

                    }

                }
                else if (MassSelect == 2)
                {
                    this->m_Pos.x -= dis + (dis / 2); // -
                    this->m_Pos.y += disy;// dis + (dis / 2);

                }

                std::cout << "Top right collision" << "COLLIDING WITH OTHER <<<<<<<<<<<<<<" << std::endl;

            }

            // else if (TR.insideOf((this->getPosition()), this->getStats().getSize()))
            else if (TR.x >= thisTL.x && TR.x <= thisTR.x && TR.y <= thisTL.y && TR.y >= thisBL.y)
            {
                Vec2 other(((this->getPosition().x) - ((this->getStats().getSize()) / 2)), this->getPosition().y);
                float dis = TR.dist(other);
                float disx = TR.x - thisTL.x;
                float disy = TR.y - thisBL.y;
                if (MassSelect == 1)
                {

                    // otherMob->SetPosX(otherMob->getPosition().x + (dis + (dis / 2)));
                    if (otherMob->isNorth())
                    {
                        otherMob->SetPosY(otherMob->getPosition().y - disy );//this->getPosition().y - this->getStats().getSize() -disy);// -otherMob->getStats().getSize());//(dis + (dis / 2)));
                    }

                    else
                    {
                        otherMob->SetPosY(otherMob->getPosition().y  + disy );//this->getPosition().y + this->getStats().getSize() + disy);// +otherMob->getStats().getSize());//(dis + (dis / 2)));

                    }


                }

                else if (MassSelect == 2)
                {
                    this->m_Pos.x += ( dis + (dis / 2));
                    this->m_Pos.y += disy;// dis + (dis / 2);
                }

                /*this->pos.x += dis + (dis / 2);
                this->pos.y += dis + (dis / 2);*/

                std::cout << "Top left Collision" << "COLLIDING WITH OTHER <<<<<<<<<<<<<<" << std::endl;
            }

            //else if (BL.insideOf((this->getPosition()), this->getStats().getSize()))
            else if (BL.x >= thisTL.x && BL.x <= thisTR.x && BL.y <= thisTL.y && BL.y >= thisBL.y)
            {
                Vec2 other(((this->getPosition().x) + ((this->getStats().getSize()) / 2)), this->getPosition().y);
                float dis = BL.dist(other);
                float disx = thisTR.x - BL.x;
                float disy = thisTR.y - BL.y;
                if (MassSelect == 1)
                {

                    // otherMob->SetPosX(otherMob->getPosition().x - (dis + (dis / 2)));
                    if (otherMob->isNorth())
                    {
                        otherMob->SetPosY(otherMob->getPosition().y  - disy); //this->getPosition().y - this->getStats().getSize() -disy);// -otherMob->getStats().getSize());//(dis + (dis / 2)));
                    }

                    else
                    {
                        otherMob->SetPosY(otherMob->getPosition().y  + disy); //this->getPosition().y + this->getStats().getSize() + disy);// +otherMob->getStats().getSize());//(dis + (dis / 2)));

                    }
                }

                else if (MassSelect == 2)
                {
                    this->m_Pos.x -= dis + (dis / 2);
                    this->m_Pos.y -= disy;// dis + (dis / 2);
                }

                /*this->pos.x -= dis + (dis / 2);
                this->pos.y -= dis + (dis / 2);*/

                std::cout << "Top Bottom Left Collision" << "COLLIDING WITH OTHER <<<<<<<<<<<<<<" << std::endl;
            }


            // else if (BR.insideOf((this->getPosition()), this->getStats().getSize()))
            else if (BR.x >= thisTL.x && TR.x <= thisTR.x && BR.y <= thisTL.y && BR.y >= thisBL.y)
            {
                Vec2 other(((this->getPosition().x) - ((this->getStats().getSize()) / 2)), this->getPosition().y);
                float dis = BR.dist(other);
                float disx = BR.x - thisTL.x;
                float disy = thisTL.y - BR.y;
                if (MassSelect == 1)
                {

                    // otherMob->SetPosX(otherMob->getPosition().x + (dis + (dis / 2)));
                    if (otherMob->isNorth())
                    {
                        otherMob->SetPosY(otherMob->getPosition().y  - disy ); // this->getPosition().y - this->getStats().getSize() - disy);// -otherMob->getStats().getSize());//(dis + (dis / 2)));
                    }

                    else
                    {
                        otherMob->SetPosY(otherMob->getPosition().y + disy ); //this->getPosition().y + this->getStats().getSize() + disy);// +otherMob->getStats().getSize());//(dis + (dis / 2)));

                    }
                }

                else if (MassSelect == 2)
                {
                    this->m_Pos.x += dis+(dis / 2);
                    this->m_Pos.y -= disy;// dis + (dis / 2);
                }

                /*this->pos.x += dis + (dis / 2);
                this->pos.y -= dis + (dis/2);*/
                std::cout << "Bottom Right Collision" << "COLLIDING WITH OTHER <<<<<<<<<<<<<<" << std::endl;
            }
        
       
    }
}

