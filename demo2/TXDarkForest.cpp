// TXDarkForest++.cpp 

/*  SPDX-License-Identifier: GPL-3.0-or-later  */

/*
    TXDarkForest++ is a re-implentation of a "Dark Forest" simulator made by BrackRat () ,testing TXRand in data-intensive conditions.
    Copyright (C) 2022-2023 Xie Youtian

    This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#define _CRT_SECURE_NO_WARNINGS

#include <cassert>
#include <stack>
#include <iostream>
#include <sstream>
#include <string>

#include "Civilization.hpp"
#include "Enums.hpp"
#include "txrand.h"
#include "util_csv.hpp"


inline std::string operator*(std::string str, size_t times) 
{
    std::string outstr;
    for (size_t i = 0; i < times; i++)outstr+=str;
    return outstr;
};

struct UniStatInfo {
    size_t nAlive, nAggresive, nFriendly, nPositive;
    const Civilization* champion;
    double maxMark;
};

UniStatInfo AnalyzeUniverse(
     Civilization** const cvpArray,
    std::stack<Civilization*>& acs,
    const size_t cvCount
);

void UniverseMain(size_t cvCount);
bool ExitCond(const UniStatInfo si) 
{ 
    return si.nAlive <= 1 || si.nAggresive <= 0 || si.nPositive <= 0; 
}

int main()
{
    size_t CvCount;
    std::cout << "CvCount:";
    std::cin >> CvCount;
    std::cout.put('\n');
    
    UniverseMain(CvCount);
}



void UniverseMain(const size_t cvCount) 
{
    using std::string;
    char fmtBuf[128];
    const char* header = "round,nAlive,nAggresive,nNeutral,nFriendly,nTotal,maxMark,champion";
    const char* fmt = "%u,%u,%u,%u,%u,%u,%g,%llu";
    Civilization** const cvpArray = new Civilization*[cvCount];
    for (size_t i = 0;i < cvCount;i++)cvpArray[i] = new Civilization;
    Civilization* pCurrentCV;
    std::stack <Civilization*> cvpInteractStack;
    //IRESULT ir;
    UniStatInfo statInfo;
    size_t round=0, i=0;
    std::vector<CVInteractRecord> interactRecord;
    //SetUniverse(cvArray,&interactRecord,&round,&i);
    //shuffle(cvpArray,sizeof(Civilization*),cvCount);
    //for (i = 0; i < cvCount; i++) {
    //    std::printf(
    //        "%llu,%d,%s,%g\n",
    //        cvpArray[i],
    //        cvpArray[i]->Type(),
    //        cvpArray[i]->IsPositive()?"true":"false",
    //        cvpArray[i]->Mark()
    //        );

    //}
    statInfo = AnalyzeUniverse(cvpArray, cvpInteractStack, cvCount);
    std::sprintf(
        fmtBuf,fmt,
        round,
        statInfo.nAlive,
        statInfo.nAggresive,
        statInfo.nAlive-statInfo.nAggresive-statInfo.nFriendly,
        statInfo.nFriendly,
        cvCount,
        statInfo.maxMark,
        statInfo.champion
    );
    std::cout << "Origin Uni:" << std::endl << header << std::endl << fmtBuf << std::endl << string("-") * 40;
    round = 1;
    std::cout << std::endl << header << std::endl;
    do 
    {
        
        while (! cvpInteractStack.empty()) 
        {
            i = 1;
            Civilization* pInteractCV = cvpInteractStack.top();
            if (!pInteractCV->Alive()) {
                cvpInteractStack.pop(); 
                continue;
            }
            GetCV:pCurrentCV = *((Civilization**)choice(cvpArray,sizeof(Civilization*),cvCount));
            if (
                   (!(pCurrentCV->Alive()))
                || (pCurrentCV->Interacted())
                || (pCurrentCV == pInteractCV)
               )
                goto GetCV;
            //ir = cvInteractStack.top()->Interact(*pCurrentCV);
            interactRecord.push_back(CVInteractRecord({round,i,cvpInteractStack.top(),pCurrentCV,pInteractCV->Interact(*pCurrentCV) }));
            i++;
            cvpInteractStack.pop();
        }
        statInfo = AnalyzeUniverse(cvpArray, cvpInteractStack, cvCount);
        std::cout.put('\n');
        std::cout.flush();
        std::sprintf(
            fmtBuf, fmt,
            round,
            statInfo.nAlive,
            statInfo.nAggresive,
            statInfo.nAlive - statInfo.nAggresive - statInfo.nFriendly,
            statInfo.nFriendly,
            cvCount,
            statInfo.maxMark,
            statInfo.champion
        );
        std::cout << fmtBuf ;
        shuffle(cvpArray, sizeof(Civilization*), cvCount);
        round++;
    } while (! ExitCond(statInfo));
    std::cout.put('\n');
    std::printf((string("-")*40).c_str());
    std::cout.put('\n');
    
    
    /*
    for (const auto & e : interactRecord) {
        csv_line lnbuf;  
        
        lnbuf.push_back((std::ostringstream() << e.cv1).str());
        lnbuf.push_back((std::ostringstream() << e.cv2).str());
        
       
        
    }
    */
    for (i = 0; i < cvCount; i++) {
        delete cvpArray[i];
    }
    delete[] cvpArray;
}

UniStatInfo AnalyzeUniverse(
    Civilization** const cvpArray,
    std::stack<Civilization*> &acs,
    const size_t cvCount
) 
{
    assert(acs.empty());
    UniStatInfo si = {0,0,0,0,nullptr,0.0};
    size_t i;
    for (i = 0; i < cvCount; i++) {
        double maxScore = 0.0;
        register const Civilization* currentCV = cvpArray[i];
        if (currentCV->Alive()) {
            
            si.nAlive++;
            const_cast<Civilization*>(currentCV)->ClearFlag();
            if (currentCV->Mark() >= maxScore)
            {
                si.maxMark = currentCV->Mark();
                si.champion = currentCV;
            }
            if (currentCV->IsPositive())si.nPositive++;
            switch (currentCV->Type())
            {
            case AGGRESIVE:
                si.nAggresive++;
                break;
            case FRIENDLY:
                si.nFriendly++;
                break;
            case NEUTRAL:
                break;
            default:
                abort();
            }

            if (currentCV->IsPositive()) acs.push(const_cast<Civilization*>(currentCV));
        }
    };
    return si;
}

