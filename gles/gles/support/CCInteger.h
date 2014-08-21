#ifndef __CCINTEGER_H__
#define __CCINTEGER_H__

#include "CCObject.h"

/**
 * @addtogroup data_structures
 * @{
 */

class CCInteger : public CCObject
{
public:
    CCInteger(int v)
        : m_nValue(v) {}
    int getValue() const {return m_nValue;}

    static CCInteger* create(int v)
    {
        CCInteger* pRet = new CCInteger(v);
        pRet->autorelease();
        return pRet;
    }
private:
    int m_nValue;
};

// end of data_structure group
/// @}

#endif /* __CCINTEGER_H__ */