#ifndef ALG_DOUBLE_H
#define ALG_DOUBLE_H
#include<algorithm>

namespace alglib {
    double EQACCURACY = 1e-10; // Ĭ�Ͼ���

    // ��������ֵ�ھ������Ƿ����
    bool equal(double a, double b, double accuracy = EQACCURACY)
    {
        return abs(a - b) <= accuracy;
    }

    // ��������ֵ�ھ�����ǰֵ�Ƿ���ں�ֵ
    bool large(double a, double b, double accuracy = EQACCURACY)
    {
        return a - b > accuracy;
    }

    // ��������ֵ�ھ�����ǰֵ�Ƿ�С�ں�ֵ
    bool less(double a, double b, double accuracy = EQACCURACY)
    {
        return b - a > accuracy;
    }

    // ��������ֵ�ھ�����ǰֵ�Ƿ񲻴��ں�ֵ
    bool notlarge(double a, double b, double accuracy = EQACCURACY)
    {
        return a - b <= accuracy;
    }

    // ��������ֵ�ھ�����ǰֵ�Ƿ�С�ں�ֵ
    bool notless(double a, double b, double accuracy = EQACCURACY)
    {
        return b - a <= accuracy;
    }

    // ����double���ȵ���ȡ�����(�����⣡����)
    int intelliTrunc(double value, double accuracy = EQACCURACY)
    {
        return int(value + accuracy);
    }
}

#endif // !ALG_DOUBLE_H



