/*
** $Id: 2009/02/07, Taras Tovchenko $
** Unit Tests
** See Copyright Notice in dCocoa.h
*/

#ifndef _TESTDATA_H_
#define _TESTDATA_H_

#include <iostream>


class TestValue {
public:
    void setValue(dInteger val) { _val = val; }
    dInteger getValue() const { return _val; }

    dInteger	_val;
};

class TestKey {
public:
    void  setValue(const char* str) {
        strcpy(_str, str);
    }
    const char* getValue() const {
        return _str;
    }
    dInteger hashValue() const {
        dInteger hash = 0;
        const char* str = _str;
        for (;;) {
            dInteger a = *str++;
            if (0 == a) break;
            hash += (hash << 8) + a;
        }
        return hash;
    }

    char _str[255];
};

#endif // _TESTDATA_H_