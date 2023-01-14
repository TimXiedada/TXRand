#include "civilization.hpp"
#include <stdbool.h>
#include <txrand.h>
#include <cstdlib>
#include <cassert>

Civilization::Civilization()
	:_type((CV_TYPE)randbelow(3)),_isPositive(randbool_b()),_mark(uniform(8000.0,12000.0))
{
	// ��־��¼���ܼ���ʵ��
	pos = this;
}

Civilization::Civilization(CV_TYPE civilizationType, bool isPositive, double initMark)
	:_type(civilizationType),_isPositive(isPositive),_mark(initMark)
{
	// ��־��¼���ܼ���ʵ��
	pos = this; 
}

Civilization::Civilization(double initMark)
	:_type((CV_TYPE)randbelow(3)), _isPositive(randbool_b()), _mark(initMark)
{
	// ��־��¼���ܼ���ʵ��
	pos = this;
}
Civilization::~Civilization() 
{
	// ��־��¼���ܼ���ʵ��
	assert(pos == this);
}
IRESULT Civilization::Interact(Civilization& cvt) 
{
	Civilization& me = (*this);
	assert(me.Alive());
	assert(this != &cvt);
	cvt._interactedFlag = true;
	//me._interactedFlag = true;
	if (me._type == AGGRESIVE)return Fight(me, cvt,true); //˫������һ���ö�����ֱ��ս��
	else {
		switch (cvt._type) {
		case AGGRESIVE:
			return Fight(cvt,me,false);// ˫������һ���ö�����ֱ��ս��
		case FRIENDLY:
			return Cooperation(cvt,me);// ���˫�������ö��������κ�һ������
		case NEUTRAL:
			if (me._type == NEUTRAL) {
				//me._interactedFlag = false;
				cvt._interactedFlag = false;
				//
				// ��־��¼
				return IR_NOTHING;
			} // ���˫��������... ���ụ��
			else if (me._type == FRIENDLY)return Cooperation(me, cvt);// ���˫�������ö��������κ�һ������
			else { assert(false); return IRESULT(-1); }

		default:
			assert(false);
			return IRESULT(-1);
		}
	}
}

//CVStatInfo Civilization::StatInfo(void) const {}