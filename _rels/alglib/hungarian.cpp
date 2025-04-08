#include <algorithm>
#include"alg-const.h"
#include"hungarian.h"
#include <cstring>
#include<climits>

namespace alglib {
    bool _hungarian_find(int a, int size, bool** adjacency_matrix, bool* vistited, int* matched)
    {
        for (int i = 0; i < size; i++)
        {
            if (adjacency_matrix[a][i] == 1 && !vistited[i]) //����ڵ�i��a���ڲ���δ�����ҹ�
            {
                vistited[i] = true; //���iΪ�Ѳ��ҹ�

                if (matched[i] == -1 //���iδ��ǰһ��ƥ��M��
                    || _hungarian_find(matched[i], size, adjacency_matrix, vistited, matched)) //i��ƥ��M�У����Ǵ���i���ڵĽڵ��������������·
                {
                    matched[i] = a; //��¼���ҳɹ���¼

                    return true;//���ز��ҳɹ�
                }
            }
        }

        return false;
    }

    // ���ض���ͼ������ƥ��
    int hungarian(bool** adjacency_matrix, int left_size, int right_size, std::vector<int>* assignment)
    {
        bool* visited = new bool[right_size];
        int* matched = new int[right_size];
        for (int i = 0; i < right_size; i++)
            matched[i] = -1;

        int result = 0;
        for (int i = 0; i < left_size; i++)
        {
            memset(visited, 0, right_size * sizeof(bool));
            if (_hungarian_find(i, right_size, adjacency_matrix, visited, matched))
                result++;
        }

        if (assignment != nullptr)
        {
            assignment->resize(left_size, -1);
            for (int i = 0; i < right_size; i++)
            {
                if (matched[i] == -1)
                    continue;
                (*assignment)[matched[i]] = i;
            }
        }

        delete[] visited;
        delete[] matched;
        return result;
    }

    template<typename T>
    bool _kuhnmunkres_find(int u, int size, T** cost_matrix, bool* visited_left, bool* visited_right, T* mark_left, T* mark_right, int* matched, T* slack)
    {
        visited_left[u] = 1;//��ǽ�������·
        for (int v = 0; v < size; v++)
        {
            if (cost_matrix[u][v] >= 0 && !visited_right[v])//���Y���ĵ㻹û��������·,���Ҵ���·��
            {
                T t = mark_left[u] + mark_right[v] - cost_matrix[u][v];
                if (t == 0)//tΪ0˵���������ͼ
                {
                    visited_right[v] = 1;//��������·

                    //���Y���ĵ㻹δ����ƥ��,�����Ѿ�������ƥ�䣬���Դ�ԭ����ƥ�䷴���ҵ�����·���ǾͿ��Խ���ƥ��
                    if (matched[v] == -1 || _kuhnmunkres_find(matched[v], size, cost_matrix, visited_left, visited_right, mark_left, mark_right, matched, slack))
                    {
                        matched[v] = u;//����ƥ��
                        return true;
                    }
                }
                else if (t > 0)//�˴�tһ���Ǵ���0����Ϊ����֮��һ��>=��Ȩ
                {
                    slack[v] = std::min(slack[v], t); //slack[v]�����Y���ĵ���Ҫ��������ͼ����ֵ��С���Ӷ���
                }
            }
        }
        return false;
    }

    // ������������(cost_matrix��ֵ��С��0Ϊ���ɴ�)������������չ���Ƿ����Լ�������ȫ����������
    int kuhnmunkres(int** cost_matrix, int size, std::vector<int>* assignment)
    {
        // �ռ�����
        int* mark_left = new int[size];
        int* mark_right = new int[size];
        int* matched = new int[size]; // ÿ����ֵ��������ֵ
        bool* visited_left = new bool[size];
        bool* visited_right = new bool[size];
        int* slack = new int[size]; // ��Ȩ�Ͷ�����С�Ĳ�ֵ

        // ���ݳ�ʼֵ����
        memset(matched, -1, size * sizeof(int));
        memset(mark_left, 0, size * sizeof(int)); // �󼯵Ķ���Ϊ�õ����ӵıߵ����Ȩֵ
        memset(mark_right, 0, size * sizeof(int)); // �Ҽ��Ķ���Ϊ0
        for (int i = 0; i < size; i++) // Ԥ���������ֵ
        {
            for (int j = 0; j < size; j++)
            {
                if (cost_matrix[i][j] < 0) // ���ɷ���
                    continue;
                mark_left[i] = std::max(mark_left[i], cost_matrix[i][j]);
            }
        }

        for (int i = 0; i < size; i++)//ö��X���ĵ�
        {
            for (int s = 0; s < size; s++)
                slack[s] = INT_MAX;
            while (1)
            {
                memset(visited_left, 0, size * sizeof(bool));
                memset(visited_right, 0, size * sizeof(bool));
                if (_kuhnmunkres_find(i, size, cost_matrix, visited_left, visited_right, mark_left, mark_right, matched, slack))
                    break;//�Ѿ�ƥ����ȷ

                // �ҳ���û�����ĵ��У���Ҫ��������ͼ����С�������ӵĶ���ֵ
                int minz = INT_MAX;
                for (int j = 0; j < size; j++)
                    if (!visited_right[j] && minz > slack[j])
                        minz = slack[j];


                // ��δƥ�䣬��X���Ķ����ȥminz��Y���Ķ������minz
                for (int j = 0; j < size; j++)
                    if (visited_left[j])
                        mark_left[j] -= minz;
                for (int j = 0; j < size; j++)
                    if (visited_right[j])
                        mark_right[j] += minz;
                    else
                        slack[j] -= minz; // �޸Ķ����Ҫ�����в��ڽ������е�Y�����slackֵ����ȥminz
            }
        }

        if (assignment != nullptr)
        {
            assignment->resize(size);
            for (int i = 0; i < size; i++)
            {
                if (matched[i] == -1)
                    continue;
                (*assignment)[matched[i]] = i;
            }
        }

        double back = 0;//����ͼ����ƥ��Ȩֵ
        for (int i = 0; i < size; i++)
        {
            if (matched[i] == -1)
                continue;
            back += cost_matrix[matched[i]][i];
        }

        // �����ڴ�
        delete[] mark_left;
        delete[] mark_right;
        delete[] matched;
        delete[] visited_left;
        delete[] visited_right;
        delete[] slack;

        return back;
    };

    // ������������(cost_matrix��ֵ��С��0Ϊ���ɴ�)������������չ���Ƿ����Լ�������ȫ����������
    double kuhnmunkres(double** cost_matrix, int size, std::vector<int>* assignment)
    {
        // �ռ�����
        double* mark_left = new double[size];
        double* mark_right = new double[size];
        int* matched = new int[size]; // ÿ����ֵ��������ֵ
        bool* visited_left = new bool[size];
        bool* visited_right = new bool[size];
        double* slack = new double[size]; // ��Ȩ�Ͷ�����С�Ĳ�ֵ

        // ���ݳ�ʼֵ����
        memset(matched, -1, size * sizeof(int));
        memset(mark_left, 0, size * sizeof(double)); // �󼯵Ķ���Ϊ�õ����ӵıߵ����Ȩֵ
        memset(mark_right, 0, size * sizeof(double)); // �Ҽ��Ķ���Ϊ0
        for (int i = 0; i < size; i++) // Ԥ���������ֵ
        {
            for (int j = 0; j < size; j++)
            {
                if (cost_matrix[i][j] < 0) // ���ɷ���
                    continue;
                mark_left[i] = std::max(mark_left[i], cost_matrix[i][j]);
            }
        }

        for (int i = 0; i < size; i++)//ö��X���ĵ�
        {
            for (int s = 0; s < size; s++)
                slack[s] = INT_MAX;
            while (1)
            {
                memset(visited_left, 0, size * sizeof(bool));
                memset(visited_right, 0, size * sizeof(bool));
                if (_kuhnmunkres_find(i, size, cost_matrix, visited_left, visited_right, mark_left, mark_right, matched, slack))
                    break;//�Ѿ�ƥ����ȷ

                // �ҳ���û�����ĵ��У���Ҫ��������ͼ����С�������ӵĶ���ֵ
                int minz = INT_MAX;
                for (int j = 0; j < size; j++)
                    if (!visited_right[j] && minz > slack[j])
                        minz = slack[j];


                // ��δƥ�䣬��X���Ķ����ȥminz��Y���Ķ������minz
                for (int j = 0; j < size; j++)
                    if (visited_left[j])
                        mark_left[j] -= minz;
                for (int j = 0; j < size; j++)
                    if (visited_right[j])
                        mark_right[j] += minz;
                    else
                        slack[j] -= minz; // �޸Ķ����Ҫ�����в��ڽ������е�Y�����slackֵ����ȥminz
            }
        }

        if (assignment != nullptr)
        {
            assignment->resize(size);
            for (int i = 0; i < size; i++)
            {
                if (matched[i] == -1)
                    continue;
                (*assignment)[matched[i]] = i;
            }
        }

        double back = 0;//����ͼ����ƥ��Ȩֵ
        for (int i = 0; i < size; i++)
        {
            if (matched[i] == -1)
                continue;
            back += cost_matrix[matched[i]][i];
        }

        // �����ڴ�
        delete[] mark_left;
        delete[] mark_right;
        delete[] matched;
        delete[] visited_left;
        delete[] visited_right;
        delete[] slack;

        return back;
    };
}