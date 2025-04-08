#ifndef ALG_RAND_H
#define ALG_RAND_H
#include<algorithm>

namespace alglib {
    //return a value between 0-1
    double rand01()
    {
        return double(rand()) / RAND_MAX;
    }

    //return a value between 0-1 but not be 0 or 1
    double rand01_()
    {
        return double(rand() + 1) / (double(RAND_MAX) + 2);
    }
}

#endif // !ALG_RAND_H



