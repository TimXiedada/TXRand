/*  SPDX-License-Identifier: GPL-3.0-or-later  */

/*
    A file of TXDarkForest++.
    Copyright (C) 2022-2023 Xie Youtian

    This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/


#include "civilization.hpp"
#include <stdbool.h>
#include <txrand.h>
#include <cstdlib>
#include <cassert>
#include <cstring>
/*
Civilization::Civilization()
    :_type((CV_TYPE)randbelow(3)),_isPositive(randbool_b()),_mark(uniform(8.0,12.0)),_serial(randbelowul(0))
{
    // 日志记录功能即将实现
    //pos = this;
}
*/
/*
Civilization::Civilization(double initMark,CV_TYPE civilizationType, bool isPositive, unsigned long serial)
    :_type(civilizationType),_isPositive(isPositive),_mark(initMark), _serial(serial)
{
    // 日志记录功能即将实现
    //pos = this;
}
*/
/*
Civilization::Civilization(double initMark)
    :_type((CV_TYPE)randbelow(3)), _isPositive(randbool_b()), _mark(initMark), _serial(randbelowul(0))
{
    // 日志记录功能即将实现
    //pos = this;
}
*/
Civilization::~Civilization()
{
    // 日志记录功能即将实现
    //assert(pos == this);
}
IRESULT Civilization::Interact(Civilization& cvt)
{
    Civilization& me = (*this);
    assert(me.Alive());   //幽灵没有这个资格
    assert(this != &cvt); //不能自己和自己互动
    cvt._interactedFlag = true;
    //me._interactedFlag = true;

    if (me._type == AGGRESIVE)return Fight(me, cvt,true); //双方任意一个好斗，则直接战斗
    else {
        switch (cvt._type) {
        case AGGRESIVE:
            return Fight(cvt,me,false);// 双方任意一个好斗，则直接战斗
        case FRIENDLY:
            return Cooperation(cvt,me);// 如果双方都不好斗，且有任何一方友善
        case NEUTRAL:
            if (me._type == NEUTRAL) {
                //me._interactedFlag = false;
                cvt._interactedFlag = false;
                //
                // 日志记录
                return IR_NOTHING;
            } // 如果双方都中立... 不会互动
            else if (me._type == FRIENDLY)return Cooperation(me, cvt);// 如果双方都不好斗，且有任何一方友善
            else { assert(false); return IRESULT(-1); }

        default:
            assert(false);
            return IRESULT(-1);
        }
    }
}

Civilization::CVStatInfo Civilization::StatInfo(const std::vector<CVInteractRecord> & ir) const
{
    Civilization::CVStatInfo rsi;
    std::memset(&rsi,0,sizeof rsi);
    /*
    bool act;

    auto Related = [this, &act](Civilization* cv1, Civilization* cv2)
    {
        if (cv1 == this) { act = true; return true; }
        else if (cv2 == this) { act = false; return true; }
        else return false;
    };
    */
    for (const auto & ln : ir) {

        if (ln.cv1->_serial==this->_serial || ln.cv2->_serial == this->_serial) switch (ln.ir) {
            case IR_COOPERATION:
                (ln.cv1->_serial == this->_serial) ? (rsi.cooperationAct++, rsi.cooperation++) : rsi.cooperation++;
                break;

            case IR_AVICTORYKILL:
                rsi.fight++, rsi.attack++, rsi.victory++, rsi.kills.push_back(ln.cv2);
                break;
            case IR_AVICTORY:
                rsi.fight++, rsi.attack++, rsi.victory++;
                break;
            case IR_ADEFEAT:
                rsi.fight++, rsi.attack++;
                break;
            case IR_ADEFEATDIED:
                rsi.fight++, rsi.attack++;
                rsi.aliveRound = ln.round - 1;
                goto EndLoop; // 节省时间

            case IR_PVICTORYKILL:
                rsi.fight++, rsi.victory++, rsi.kills.push_back(ln.cv1);
                break;
            case IR_PVICTORY:
                rsi.fight++, rsi.victory++;
                break;
            case IR_PDEFEAT:
                rsi.fight++;
                break;
            case IR_PDEFEATDIED:
                rsi.fight++;
                rsi.aliveRound = ln.round - 1;
                goto EndLoop; // 节省时间
            }

    }
    EndLoop:;
    rsi.maxScore = this->_maxmark;
    rsi.minScore = this->_minmark;
    return rsi;
}
