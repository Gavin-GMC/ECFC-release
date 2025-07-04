#pragma once
#include<cmath>
#include"funcs.h"

namespace MetricLib
{
	// 找到的最优峰值的比例
	double peakRatio(double* input, double* refer_point, int size, int refer_size, int demension, double accuracy_d = 1e-3)
	{
		double* solution_pointer;
		double* aim_pointer;
		double distance;

		int counter = 0;
		for (int i = 0; i < refer_size; i++)
		{
			aim_pointer = refer_point + i * demension;
			for (int j = 0; j < size; j++)
			{
				solution_pointer = input + j * demension;
				distance = eu_distance(solution_pointer, aim_pointer, demension);
				if (distance < accuracy_d)
				{
					counter++;
					break;
				}
			}
		}

		return double(counter) / refer_size;
	}

	// 每个候选解到其距离最近的峰值的距离
	double peakDistance(double* input, double* refer_point, int size, int refer_size, int demension)
	{
		double back = 0;

		double* solution_pointer;
		double* aim_pointer;
		double distance_min;
		double distance_buffer;

		for (int i = 0; i < size; i++)
		{
			solution_pointer = input + i * demension;
			distance_min = eu_distance(refer_point, solution_pointer, demension);
			for (int j = 1; j < refer_size; j++)
			{
				aim_pointer = refer_point + j * demension;
				distance_buffer = eu_distance(solution_pointer, aim_pointer, demension);
				if (distance_buffer < distance_min)
					distance_min = distance_buffer;
			}
			back += distance_min;
		}
		back /= size;;

		return back;
	}

	// 每个候选解到其距离最近的峰值的适应度差距期望
	double peakInaccuracy(double* input, double* refer_point, int size, int refer_size, int demension)
	{
		double back = 0;
		// 输入处理
		double* input_object = input;
		double* input_decision = input + size;
		double* refer_object = refer_point;
		double* refer_decision = refer_point + refer_size;

		// 寻找最近峰并进行指标计算
		double* solution_pointer;
		double* aim_pointer;
		int min_id;
		double distance_min;
		double distance_buffer;

		for (int i = 0; i < size; i++)
		{
			solution_pointer = input_decision + i * demension;
			distance_min = eu_distance(refer_decision, solution_pointer, demension);
			min_id = 0;
			for (int j = 1; j < refer_size; j++)
			{
				aim_pointer = refer_decision + j * demension;
				distance_buffer = eu_distance(solution_pointer, aim_pointer, demension);
				if (distance_buffer < distance_min)
				{
					min_id = j;
					distance_min = distance_buffer;
				}
			}
			back += std::abs(refer_object[min_id] - input_object[i]);
		}

		back /= size;;

		return back;
	}
}