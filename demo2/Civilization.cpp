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

Civilization::Civilization()
	:_type((CV_TYPE)randbelow(3)),_isPositive(randbool_b()),_mark(uniform(8000.0,12000.0)),_serial(randbelowul(0))
{
	// 日志记录功能即将实现
	//pos = this;
}

Civilization::Civilization(CV_TYPE civilizationType, bool isPositive, double initMark,unsigned long serial)
	:_type(civilizationType),_isPositive(isPositive),_mark(initMark), _serial(serial)
{
	// 日志记录功能即将实现
	//pos = this; 
}

Civilization::Civilization(double initMark)
	:_type((CV_TYPE)randbelow(3)), _isPositive(randbool_b()), _mark(initMark), _serial(randbelowul(0))
{
	// 日志记录功能即将实现
	//pos = this;
}
Civilization::~Civilization() 
{
	// 日志记录功能即将实现
	//assert(pos == this);
}
IRESULT Civilization::Interact(Civilization& cvt) 
{
	Civilization& me = (*this);
	assert(me.Alive());
	assert(this != &cvt);
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

//CVStatInfo Civilization::StatInfo(void) const {}