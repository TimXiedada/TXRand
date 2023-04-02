/*  SPDX-License-Identifier: GPL-3.0-or-later  */

/*
    A file of TXDarkForest++. It handles CSV files.
    Copyright (C) 2022-2023 Xie Youtian

    This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/


#include "util_csv.hpp"
/*
int csv_read(std::ifstream csvin, const char sep, bool title, csv_table& tbl)
{
    std::string line_buf;
    std::getline(csvin,line_buf);
    return 0;
}
*/

//
//
//
bool csv_writeln(std::ostream & csvout, const csv_line & csvln, const char sep)
{
    auto csvln_iter = csvln.cbegin();
    while (csvout.good()) {
        if (csvln_iter->find(sep) != std::string::npos)
            csvout << "'" << *csvln_iter << "'"; //如果 csv_line 中的任何一个字符串包含了 sep 字符，则将该字符串用单引号括起来。
        else csvout << *csvln_iter;
        csvln_iter++;
        if (csvln.cend() == csvln_iter) {
            csvout << std::endl;
            return true;
        }
        else csvout.put(sep);
    }
    return false;
}

int csv_write(std::ostream & csvout, csv_table& tbl, const char sep)
{
    int lncount = 0;
    for (const csv_line& ln : tbl) {
        if (!csv_writeln(csvout, ln, sep))return -1;
        else lncount++;
    }
    return lncount;
}
