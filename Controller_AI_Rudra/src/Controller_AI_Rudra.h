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

#pragma once

#include "/Game ai/New folder/CrashLoyal-master/Interface/src/iController.h"
#include "/Game ai/New folder/CrashLoyal-master/Game/src/Entity.h"
#include "/Game ai/New folder/CrashLoyal-master/Game/src/Game.h"
#include "/Game ai/New folder/CrashLoyal-master/Game/src/Player.h"





class Controller_AI_Rudra : public iController
{
public:
    Controller_AI_Rudra() {}
    virtual ~Controller_AI_Rudra() {}

    void tick(float deltaTSec);

    void getattackstatus();

    void GetEnemiesOnField();

    bool isGiantPresent();

    void GetOpponentBuildings();

    float nearestXforSpawn(float x, float x1);

    float nearestXforDefence(float x, float x1, Entity* otherMob);
    
    float isOpponentMobPresent(iEntityStats::MobType mobType);


   



};