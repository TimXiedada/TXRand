/*  SPDX-License-Identifier: GPL-3.0-or-later  */

/*
    A file of TXDarkForest++.
    Copyright (C) 2022-2023 Xie Youtian

    This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef _CIVILIZATION_H_
#define _CIVILIZATION_H_
#include "enums.hpp"
#include <vector>
#include <txrand.h>

class Civilization;

//������¼���ֱ�Ϊ����������˳�򣬷������ڴ��ַ����һ�����ڴ��ַ����������
typedef struct {
    size_t round;
    size_t i;
    Civilization* cv1;
    Civilization* cv2;
    IRESULT ir;
}
CVInteractRecord;


class Civilization
{
   /*
protected:
    static const Civilization* const * _cvArr;
    static const std::vector<CVInteractRecord>* _pCVIRecord;
    static const size_t* _pround,const *_pi;
    */
private:
    const CV_TYPE        _type;                   // ����
    const bool           _isPositive;             // �Ƿ��Ծ
    double               _mark,_maxmark=0.0,_minmark=0.0; // ��������߷֣���ͷ�
    bool                 _interactedFlag = false; // ��ֹС����ʳ
    const unsigned long  _serial;                 // ���к�
    void UpdateMaxMark() { 
        if (this->_mark >= _maxmark)_maxmark = this->_mark;
        if (this->_mark >= _minmark)_minmark = this->_mark;
    };
                                                  // ������߷ּ���ͷ�
public:
    struct CVStatInfo 
    // ͳ����Ϣ���ֱ�Ϊ�������������������������������������������ս���������ܻ�����������ɱ�����б���߷�����
    {
        size_t
            aliveRound,
            cooperation,
            cooperationAct,
            attack,
            fight,
            victory,
            interact;
        std::vector<Civilization*> kills;
        double maxScore,minScore;
    };
    //���캯��
    
    explicit Civilization(double initMark= uniform(8.0, 12.0), CV_TYPE civilizationType = (CV_TYPE)randbelow(3), bool isPositive = randbool(),unsigned long serial = randbelowul(0))
        :_type(civilizationType), _isPositive(isPositive), _mark(initMark), _serial(serial){
        // ��־��¼���ܼ���ʵ��
    }
    

    //��Ա����
    void            ClearFlag  (void)       { this->_interactedFlag = false; };
    CV_TYPE         Type       (void) const { return _type; };
    bool            IsPositive (void) const { return _isPositive; };
    bool            Alive      (void) const { return _mark > 0; };
    bool            Interacted (void) const { return _interactedFlag; };
    double          Mark       (void) const { return _mark; };
    unsigned long   Serial     (void) const { return _serial; };
    
    friend  IRESULT Fight      (Civilization& cv1, Civilization& cv2,bool attack);
    friend  IRESULT Cooperation(Civilization& cv1, Civilization& cv2);
    /*
    friend void SetUniverse(const Civilization**  cvArr,const std::vector<CVInteractRecord>* const cvIRecord,const size_t * const round,const size_t * const i)
    {
        cvArr[0]->_cvArr = cvArr;
        cvArr[0]->_pCVIRecord = cvIRecord;
        cvArr[0]->_pi = i;
        cvArr[0]->_pround = round;

    }
    */
    virtual IRESULT Interact(Civilization& cvt) ;
    Civilization::CVStatInfo StatInfo(const std::vector<CVInteractRecord>& ir) const;
    
    //�۹�����
    ~Civilization();
};


#endif /*_CIVILIZATION_H_*/
