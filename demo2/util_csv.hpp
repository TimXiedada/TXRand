/*  SPDX-License-Identifier: GPL-3.0-or-later  */

/*
    A file of TXDarkForest++. It handles CSV files.
    Copyright (C) 2022-2023 Xie Youtian

    This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef _UTIL_CSV_H_
#define _UTIL_CSV_H_
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

typedef std::vector<std::vector<std::string>> csv_table;//表示一个二维字符串向量
typedef std::vector<std::string> csv_line; //表示一个一维字符串向量


int csv_read(std::ifstream csvin, const char sep,bool title, csv_table& tbl);

bool csv_writeln(std::ostream & csvout, const csv_line & csvln, const char sep = ',');
//用于将一行 csv_line 写入到 csvout 流中，使用 sep 字符作为分隔符。

int csv_write(std::ostream& csvin, csv_table& tbl, const char sep = ',');
#endif