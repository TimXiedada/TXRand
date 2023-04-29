/*  SPDX-License-Identifier: GPL-3.0-or-later  */

/*
    A file of TXDarkForest++.
    Copyright (C) 2022-2023 Xie Youtian

    This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/
#ifndef _ENUMS_H
#define _ENUMS_H




enum CV_TYPE {  // 类型
    FRIENDLY,   // 友善
    NEUTRAL,    // 中立
    AGGRESIVE   // 好斗
};

enum IRESULT {
    IR_NOTHING,
    IR_COOPERATION,
    
    IR_PVICTORY,  IR_PVICTORYKILL,
    IR_PDEFEAT,   IR_PDEFEATDIED,
    IR_AVICTORY,  IR_AVICTORYKILL,
    IR_ADEFEAT,   IR_ADEFEATDIED
};

const char* const CV_TYPE_STR[] = {
    "FRIENDLY",
    "NEUTRAL",
    "AGGRESIVE"
};
const char* const IRESULT_STR[] = {

    "IR_NOTHING",
    "IR_COOPERATION",

    "IR_PVICTORY",  "IR_PVICTORYKILL",
    "IR_PDEFEAT",   "IR_PDEFEATDIED",
    "IR_AVICTORY",  "IR_AVICTORYKILL",
    "IR_ADEFEAT",   "IR_ADEFEATDIED"
};
//enum ITYPE {IT_NOINTERACT,IT_COOPERATION,IT_FIGHT};

//inline ITYPE IR2IT(const IRESULT IR) { return ((IR > 2) ? IT_FIGHT:(IR?IT_COOPERATION:IT_NOINTERACT)); };



#endif /*_ENUMS_H*/