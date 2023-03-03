// TXDarkForest++.cpp 
//
#define _CRT_SECURE_NO_WARNINGS
#include <cassert>
#include <stack>
#include <iostream>
#include "Civilization.hpp"
#include "Enums.hpp"
#include <string>
#include "txrand.h"



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
    UniverseMain(50000);
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
        std::printf("%llu,%llu,%llu,%llu,%d\n",e.round,e.i,e.cv1,e.cv2,(int)e.ir);
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

