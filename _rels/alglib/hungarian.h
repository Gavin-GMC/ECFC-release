#include <vector>
#include <algorithm>
#include"alg-const.h"

namespace alglib {
    // ���ض���ͼ������ƥ��
    int hungarian(bool** adjacency_matrix, int left_size, int right_size, std::vector<int>* assignment = nullptr);

    // ������������(cost_matrix��ֵ��С��0Ϊ���ɴ�)������������չ���Ƿ����Լ�������ȫ����������
    int kuhnmunkres(int** cost_matrix, int size, std::vector<int>* assignment = nullptr);

    // ������������(cost_matrix��ֵ��С��0Ϊ���ɴ�)������������չ���Ƿ����Լ�������ȫ����������
    double kuhnmunkres(double** cost_matrix, int size, std::vector<int>* assignment = nullptr);
}