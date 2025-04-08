#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <numeric>
#include <map>

namespace MetricLib
{
    bool has_repeat(double* sample1, int length1, double* sample2, int length2)
    {
        double adt_accu = std::abs(sample1[0] / 1e8);

        for (int i = 0; i < length1; i++)
        {
            for (int j = 0; j < length2; j++)
            {
                if (std::abs(sample1[i] - sample2[j]) < adt_accu)
                    return true;
            }
        }

        return false;
    }

    double* rankElements(double* input, int size, bool min_is_better = true)
    {
        // ����һ������������������
        std::vector<int> indices(size);
        for (int i = 0; i < size; ++i) {
            indices[i] = i;
        }

        // ��������ֵ����ͬʱ����ԭʼ����
        std::sort(indices.begin(), indices.end(), [input, min_is_better](int i, int j) {
            if (min_is_better) {
                return input[i] < input[j]; // ��������
            }
            else {
                return input[i] > input[j]; // ��������
            }
            });

        int begin_rank, end_rank;
        double average_rank;
        bool is_begin = true;
        double* back = new double[size];

        for (int i = 0; i < size - 1; i++)
        {
            if (std::abs(input[indices[i]] - input[indices[i + 1]]) < std::abs(input[indices[i]] * 1e-8))
            {
                if (is_begin)
                {
                    is_begin = false;
                    begin_rank = i;
                }
            }
            else
            {
                if (is_begin)
                {
                    back[indices[i]] = i;
                }
                else
                {
                    end_rank = i;
                    average_rank = begin_rank + end_rank;
                    average_rank /= 2;

                    for (int k = begin_rank; k < end_rank + 1; k++)
                        back[indices[k]] = average_rank;

                    is_begin = true;
                }
            }
        }
        if (is_begin)
        {
            back[indices[size - 1]] = size - 1;
        }
        else
        {
            end_rank = size - 1;
            average_rank = begin_rank + end_rank;
            average_rank /= 2;

            for (int k = begin_rank; k < end_rank + 1; k++)
                back[indices[k]] = average_rank;
        }

        return back;
    }

    std::map<double, int> appearCount(double* input, int size)
    {
        std::map<double, int> countMap; // ����һ��map���洢ÿ��Ԫ�ؼ�����ִ���

        // �������飬ͳ��ÿ��Ԫ�صĳ��ִ���
        for (int i = 0; i < size; ++i) {
            countMap[input[i]]++; // ���Ԫ���Ѵ��ڣ��������1����������ڣ����Զ����벢����Ϊ1
        }

        return countMap; // ����ͳ�ƽ��
    }

    // ����Mann-Whitney U�����Uֵ
    int mannWhitneyU(double* sample1, int length1, double* sample2, int length2)
    {
        double* rank_buffer = new double[length1 + length2];
        memcpy(rank_buffer, sample1, sizeof(double) * length1);
        memcpy(rank_buffer + length1, sample2, sizeof(double) * length2);
        double* ranks = rankElements(rank_buffer, length1 + length2);

        double rank1 = length1;
        for (int i = 0; i < length1; i++)
            rank1 += ranks[i];

        int U1 = rank1 - (length1 * (length1 + 1)) / 2;
        int U2 = length1 * length2 - rank1;

        delete[] rank_buffer;
        delete[] ranks;

        return std::max(U1, U2);
    }

    int mannWhitneyU(double* ranks, int length1, int length2)
    {
        double rank1 = length1;
        for (int i = 0; i < length1; i++)
            rank1 += ranks[i];

        return rank1 - (length1 * (length1 + 1)) / 2;
    }

    // ����Mann-Whitney U�����Zֵ
    double mannWhitneyZ(int U, int length1, int length2, double* ranks, bool continuity = true) {
        double meanU = (length1 * length2) / 2.0;
        double totla_length = length1 + length2;

        // ͬ�����
        double tie_term = 0;
        std::map<double, int> count_map = appearCount(ranks, length1 + length2);
        for (std::map<double, int>::iterator it = count_map.begin(); it != count_map.end(); it++)
        {
            tie_term += it->second * (it->second * it->second - 1);
        }
        double s = std::sqrt(length1 * length2 / 12.0 *
            ((totla_length + 1) - tie_term / (totla_length * (totla_length - 1))));

        double numerator = U - meanU;
        if (continuity)
            numerator -= 0.5;

        return numerator / s;
    }

    // ����Mann-Whitney U�����pֵ
    double mannWhitneyP_asymptotic(double Z)
    {
        return 0.5 * std::erfc(Z / std::sqrt(2));
    }

    double MannWhitneyUTest(double* sample1, int length1, double* sample2, int length2, bool two_side = true, bool isless = true)
    {
        double* rank_buffer = new double[length1 + length2];
        memcpy(rank_buffer, sample1, sizeof(double) * length1);
        memcpy(rank_buffer + length1, sample2, sizeof(double) * length2);
        double* ranks = rankElements(rank_buffer, length1 + length2);

        int u1 = mannWhitneyU(ranks, length1, length2);
        int u2 = length1 * length2 - u1;

        int u;
        if (two_side)
            u = std::max(u1, u2);
        else
            if (isless)
                u = u2;
            else
                u = u1;


        double z = mannWhitneyZ(u, length1, length2, ranks);
        double p = mannWhitneyP_asymptotic(z);

        if (two_side)
            p *= 2;

        return p;
    }
}