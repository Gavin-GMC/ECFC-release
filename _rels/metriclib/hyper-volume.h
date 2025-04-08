#pragma once
#include<cmath>
#include<algorithm>
#include"funcs.h"

namespace MetricLib
{
	double _hv_iter(double* input, int size, int demension)
	{
		if (demension == 1)
		{
			double back = input[0];
			for (int i = 0; i < size; i++)
			{
				if (back < input[i])
					back = input[i];
			}
			return back;
		}
		else
		{
			double back = 0;
			double* buffer = new double[size * (demension - 1)];
			sortHelper<int, double>* index_buffer = new sortHelper<int, double>[size];
			
			//�Ե���ά��������(�����ظ�������������˷�)
			for (int i = 0; i < size; i++)
			{
				index_buffer[i].id = i;
				index_buffer[i].value = input[i * demension];
			}
			std::sort(index_buffer, index_buffer + size);

			// �����
			int new_demension = demension - 1;
			for (int i = 0; i < size; i++)
			{
				memcpy(input + index_buffer[i].id * demension + 1, buffer + i * new_demension, sizeof(double) * new_demension);
			}

			// ���㳬���
			int current_index = 0;
			int current_h = 0;
			while (current_index != size)
			{
				if (index_buffer[current_index].value <= current_h)
				{
					current_index++;
					continue;
				}
				// �����Ƭ����ά����
				back += (index_buffer[current_index].value - current_h) * _hv_iter(buffer + current_index * new_demension, size - current_index, new_demension);
				current_h = index_buffer[current_index].value;
			}
			
			delete[] index_buffer;
			delete[] buffer;
			return back;
		}
	}

	double hv_math(double* input, double* refer_point, int size, int object_number)
	{
		double back = 0;
		
		// ����ϵ�ƶ�
		double* buffer = new double[size * object_number];
		memcpy(buffer, input, size * object_number * sizeof(double));
		int index = 0;
		for (int i = 0; i < size; i++)
		{
			for (int o = 0; o < object_number; o++)
			{
				buffer[index] -= refer_point[o];
				index++;
			}
		}

		// ������������
		int* belongs = new int[size];
		for (int i = 0; i < size; i++)
		{
			belongs[i] = 0;
			for (int o = 0; o < object_number; o++)
			{
				belongs[i] *= 2;
				if (buffer[index] < 0)
				{
					belongs[i]++;
					buffer[index] *= -1;
				}
			}
		}
		
		// �����޼��㳬���
		int quadrants = std::pow(2, object_number);
		double* points = new double[size * object_number];
		int counter;
		for (int b = 0; b < quadrants; b++)
		{
			counter = 0;
			for (int i = 0; i < size; i++)
			{
				if (belongs[i] == b)
				{
					memcpy(points + counter * object_number, buffer + i * object_number, object_number * sizeof(double));
					counter++;
				}
			}
			back += _hv_iter(points, counter, object_number); //������Ƭ�������㳬���
		}

		delete[] buffer;
		delete[] points;
		delete[] belongs;

		return back;
	}

	double hv_montecarlo(double* input, double* refer_point, int size, int object_number)
	{
		//ͳ�ƿռ䷶Χ
		double* o_max = new double[object_number];
		double* o_min = new double[object_number];
		for (int o = 0; o < object_number; o++) // ��ʼ��
		{
			o_min[o] = refer_point[o];
			o_max[o] = refer_point[o];
		}
		int index = 0;
		for (int i = 0; i < size; i++)
		{
			for (int o = 0; o < object_number; o++)
			{
				if (input[index] < o_min[o])
					o_min[o] = input[index];
				else if (input[index] > o_max[o])
					o_max[o] = input[index];

				index++;
			}
		}

		// �����ռ��������
		double space_volume = 1;
		for (int o = 0; o < object_number; o++)
			space_volume *= (o_max[o] - o_min[o]);

		// ���ؿ������
		int sampled = 5e4;
		int counter = 0;
		bool isin;
		double* s_point = new double[object_number];
		for (int s = 0; s < sampled; s++)
		{
			//����
			for (int i = 0; i < object_number; i++)
			{
				s_point[i] = rand01() * (o_max[i] - o_min[i]) + o_min[i];
			}

			// �����ж�
			for (int i = 0; i < size; i++)
			{
				isin = true;
				index = i * object_number;
				for (int o = 0; o < object_number; o++)
				{
					if ((s_point[o] < refer_point[o] && s_point[o] < input[index]) ||
						(s_point[o] > refer_point[o] && s_point[o] > input[index]))//���ڸó���������
					{
						isin = false;
						break;
					}
					index++;
				}
				if (isin)
				{
					counter++;
					break;
				}
			}
		}

		delete[] o_max;
		delete[] o_min;
		delete[] s_point;

		return (double)counter / sampled * space_volume;
	}
}