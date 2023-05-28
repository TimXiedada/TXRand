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
#include <cstdio>
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
    if (si.nAlive == si.nAggresive&& si.nAggresive == 2)return true;
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
    char fmtBuf[4096];
    const char* header = "round,nAlive,nAggresive,nNeutral,nFriendly,nTotal,maxMark,champion";
    const char* fmt = "%u,%u,%u,%u,%u,%u,%8.3f,%lu";
    Civilization** const cvpArray = new Civilization*[cvCount];
    for (size_t i = 0;i < cvCount;i++)cvpArray[i] = new Civilization;
    Civilization* pCurrentCV;
    std::stack <Civilization*> cvpInteractStack;
    //IRESULT ir;
    UniStatInfo statInfo;
    size_t round=0, i=0;
    std::vector<CVInteractRecord> interactRecord;
    // SetUniverse(cvpArray,&interactRecord,&round,&i);
    shuffle(cvpArray,sizeof(Civilization*),cvCount);
    FILE* pUniListCSV = fopen("unilist.csv", "w");
    std::fputs("Serial,Type,Is Positive,Initial Mark\n",pUniListCSV);
    for (i = 0; i < cvCount; i++) {
        std::fprintf(
            pUniListCSV,
            "%lu,%s,%s,%8.3f\n",
            cvpArray[i]->Serial(),
            CV_TYPE_STR[cvpArray[i]->Type()],
            cvpArray[i]->IsPositive()?"TRUE":"FALSE",
            cvpArray[i]->Mark()
            );

    }
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
        statInfo.champion->Serial()
    );
    std::puts("Origin Uni:\n"),std::puts(header),std::puts(fmtBuf),std::puts((string("-") * 40).c_str());
    round = 1;
    std::puts(header);

    do 
    {
        i = 1;
        while (! cvpInteractStack.empty()) 
        {
            
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
        //std::putchar('\n');
        std::fflush(stdout);
        std::sprintf(
            fmtBuf, fmt,
            round,
            statInfo.nAlive,
            statInfo.nAggresive,
            statInfo.nAlive - statInfo.nAggresive - statInfo.nFriendly,
            statInfo.nFriendly,
            cvCount,
            statInfo.maxMark,
            statInfo.champion->Serial()
        );
        std::puts(fmtBuf) ;
        shuffle(cvpArray, sizeof(Civilization*), cvCount);
        round++;
    } while (!ExitCond(statInfo));

    // std::cout.put('\n');
    std::printf((string("-")*40).c_str());
    std::cout.put('\n');
    // FILE* siCSVfp = fopen("statinfo.csv","w");
    FILE* irCSVfp = fopen("irecord.csv", "w");
    std::fputs("Round,In-Round No.,Civilization 1,Civilization 2,Result\n", irCSVfp);

    for (const auto & e : interactRecord) {
        std::sprintf(
            fmtBuf,
            "%llu,%llu,%lu,%lu,%s\n",
            e.round,
            e.i,
            e.cv1->Serial(),
            e.cv2->Serial(),
            IRESULT_STR[e.ir]
        );
        std::fputs(fmtBuf, irCSVfp);
    }
    
    i = 0;
    /*
    std::fputs("Serial,Alive Rounds,Cooperation,Requested Cooperation,Attack,Fight,Victory,Interact Count,Eliminates,Maximum Score\n",siCSVfp);
    for (Civilization::CVStatInfo si; i < cvCount; i++) {
        si = cvpArray[i]->StatInfo(interactRecord);
        std::sprintf(
            fmtBuf,
            "%ld,%llu,%llu,%llu,%llu,%llu,%llu,%llu,%llu,%llu,%8.3f,%8.3f\n",
            cvpArray[1]->Serial(),
            si.aliveRound,
            si.attack,
            si.cooperation,
            si.cooperationAct,
            si.attack,
            si.fight,
            si.victory,
            si.interact,
            si.kills.size(),
            si.maxScore,
            si.minScore

        );
        std::fputs(fmtBuf, siCSVfp);
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

