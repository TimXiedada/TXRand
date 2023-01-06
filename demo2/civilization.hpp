#ifndef _CIVILIZATION_H_
#define _CIVILIZATION_H_
#include "enums.hpp"
#include <vector>

class Civilization;
typedef struct {
    size_t round;
    size_t i;
    Civilization* cv1;
    Civilization* cv2;
    IRESULT ir;
}
CVInteractRecord;
//������¼���ֱ�Ϊ����������˳�򣬷������ڴ��ַ����һ�����ڴ��ַ����������
struct CVStatInfo {
    unsigned long
        aliveRound,
        cooperation,
        cooperationAct,
        attack,
        fight,
        victory,
        interact;
    std::vector<Civilization*> kills;
    double maxScore;
};
class Civilization
{
    /*
protected:
    static const Civilization* _cvArr;
    static const std::vector<CVInteractRecord>* _pCVIRecord;
    static const size_t* _pround, *_pi;
    */
private:
    const CV_TYPE  _type;    //����
    const bool     _isPositive; //�Ƿ��Ծ
          double   _mark;           //����
    bool           _interactedFlag = false;
    
public:
    //���캯��
    Civilization();
    Civilization(CV_TYPE civilizationType, bool isPositive, double initMark);
    explicit Civilization(double initMark);

    //��Ա����
    void            ClearFlag  (void)       { this->_interactedFlag = false; };
    CV_TYPE         Type       (void) const { return _type; };
    bool            IsPositive (void) const { return _isPositive; };
    bool            Alive      (void) const { return _mark > 0; };
    bool            Interacted (void) const { return _interactedFlag; };
    double          Mark       (void) const { return _mark; };
    
    
    friend  IRESULT Fight      (Civilization& cv1, Civilization& cv2);
    friend  IRESULT Cooperation(Civilization& cv1, Civilization& cv2);
    /*
    friend  void    SetUniverse(const Civilization* const cvArr,const std::vector<CVInteractRecord>* const cvIRecord,const size_t * const round,const size_t * const i)
    {
        cvArr[0]._cvArr = cvArr;
        cvArr[0]._pCVIRecord = cvIRecord;
        cvArr[0]._pi = i;
        cvArr[0]._pround = round;

    }
    */
    virtual IRESULT Interact(Civilization& cvt) ;
    CVStatInfo      StatInfo(void) const;
    
    //�۹�����
    ~Civilization();
};


#endif /*_CIVILIZATION_H_*/
