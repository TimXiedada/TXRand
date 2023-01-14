#include "civilization.hpp"
#include <stdbool.h>
#include <txrand.h>
#include <cstdlib>
#include <cassert>

Civilization::Civilization()
	:_type((CV_TYPE)randbelow(3)),_isPositive(randbool_b()),_mark(uniform(8000.0,12000.0))
{
	// 日志记录功能即将实现
	pos = this;
}

Civilization::Civilization(CV_TYPE civilizationType, bool isPositive, double initMark)
	:_type(civilizationType),_isPositive(isPositive),_mark(initMark)
{
	// 日志记录功能即将实现
	pos = this; 
}

Civilization::Civilization(double initMark)
	:_type((CV_TYPE)randbelow(3)), _isPositive(randbool_b()), _mark(initMark)
{
	// 日志记录功能即将实现
	pos = this;
}
Civilization::~Civilization() 
{
	// 日志记录功能即将实现
	assert(pos == this);
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