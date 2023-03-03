/*  SPDX-License-Identifier: GPL-3.0-or-later  */

/*
	TXDarkForest++ is a re-implentation of a "Dark Forest" simulator made by BrackRat () ,testing TXRand in data-intensive conditions.
	Copyright (C) 2022-2023 Xie Youtian

	This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

	You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/
#include "enums.hpp"
#include "civilization.hpp"
#include <cstdlib>
#include <txrand.h>
#include "ptdef.hpp"
#include "cassert"
//using std::abort;
IRESULT Fight(Civilization& cv1, Civilization& cv2,bool attack) {
	int resultFlag = 0;
	//cv1._statInfo.fight++;
	//cv2._statInfo.fight++;
	assert(!(cv1._mark <= 0.0 || cv2._mark <= 0.0));
	//	return IR_NOTHING;
	//cv1._statInfo.fight++;
	//cv2._statInfo.fight++;
    if (cv1._mark > cv2._mark) {
	m0:;
		double deltam = (cv1._mark + cv2._mark) * POINT_FIGHTWIN;
		cv1._mark += deltam * POINT_FIGHTWIN;
		cv2._mark -= deltam * POINT_FIGHTLOSE;
		//日志记录功能
		//cv1._statInfo.victory++;
		if (!cv2.Alive()) {
			//日志记录功能
			//cv1._statInfo.kills.push_back(cv2);
			resultFlag = 2;
		}
		else resultFlag = 1;
	}
	else if (cv1._mark < cv2._mark) {
	m1:;
		double deltam = (cv1._mark + cv2._mark) * POINT_FIGHTWIN;
		cv2._mark += deltam * POINT_FIGHTWIN;
		cv1._mark -= deltam * POINT_FIGHTLOSE;
		//cv2._statInfo.victory++;
		//日志记录功能
		if (!cv1.Alive()) {
			//日志记录功能
			//cv2._statInfo.kills.push_back(cv1);
			resultFlag = -2;
		}
		else resultFlag = -1;
	}
	else if (cv1._mark == cv2._mark) {
		if (randbool_b())goto m1;
		else goto m0;
	}
	switch (resultFlag) {
	case 2:
		return attack ? IR_AVICTORYKILL : IR_PVICTORYKILL;
	case 1:
		return attack ? IR_AVICTORY : IR_PVICTORY;
	case -1:
		return attack ? IR_ADEFEAT : IR_PDEFEAT;
	case -2:
		return attack ? IR_ADEFEATDIED : IR_PDEFEATDIED;
	default:
		std::abort();
	}
}

IRESULT Cooperation(Civilization& cv1, Civilization& cv2) {
	double deltam;
	deltam = cv1._mark + cv2._mark;
	cv1._mark += deltam;
	cv2._mark += deltam;
	return IR_COOPERATION;
}
