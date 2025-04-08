//------------------------Description------------------------
// This file defines the solution comparer in ECFC. They support
// candidate solution comparison considering target priority.
//-------------------------Reference-------------------------
//-------------------------Copyright-------------------------
// Copyright (c) 2024 ���������ƣ���ȷ�ϣ�, All Rights Reserved.
// You are free to use the ECFC����ȷ�ϣ� for research purposes.
// All publications which use this library or any code in the library
// should acknowledge the use of "ECFC" and reference 
// "δȷ��"
//-----------------------------------------------------------

#pragma once
#include"basicfunc.h"
#include"sort-helper.h"
#include"objective.h"

namespace ECFC
{
	class Comparer
	{
	protected:
		int _object_level; // Ŀ�����ȼ�������
		int* _object_number; // ÿһ����Ŀ����Ŀ
		int* _priority; // �����ȼ������Ŀ������
		bool* _min_is_better; // Ŀ������С��/��󻯱�ʶ
	public:
		Comparer(Objective* objectives, const int objective_number)
		{
			_priority = new int[objective_number];
			_min_is_better = new bool[objective_number];
			sortHelper<int, int>* sort_buffer = new sortHelper<int, int>[objective_number];
			for (int i = 0; i < objective_number; i++)
			{
				sort_buffer[i] = sortHelper<int, int>(i, -1 * objectives[i].priority());
				_min_is_better[i] = objectives[i].IsMin();
			}
			sort(sort_buffer, sort_buffer + objective_number, false);

			_object_level = 0;
			_object_number = new int[objective_number];
			int pre_priority = sort_buffer[0].value;
			_object_number[0] = 0;
			for (int i = 0; i < objective_number; i++)
			{
				_priority[i] = sort_buffer[i].id;
				if (sort_buffer[i].value != pre_priority)
				{
					//���ȼ�С��0��Ŀ�겻����Ƚ�
					if (sort_buffer[i].value > 0)
					{
						break;
					}
					_object_level++;
					_object_number[_object_level] = 0;
					_priority[i] = sort_buffer[i].id;
					pre_priority = sort_buffer[i].value;
				}
				_object_number[_object_level]++;
			}

			_object_level++;
			delete[] sort_buffer;
		}

		virtual ~Comparer()
		{
			delete[] _object_number;
			delete[] _priority;
			delete[] _min_is_better;
		}

		// ��������Ŀ�����������ӹ�ϵ(f1�Ƿ����f2)
		virtual bool isBetter(const double f1[], const double f2[])
		{
			int object_id;
			int counter;
			
			// ��ֵ�ų�
			counter = 0;
			for (int i = 0; i < _object_level; i++)
			{
				for (int j = 0; j < _object_number[i]; j++)
				{
					object_id = _priority[counter++];

					if (f1[object_id] == EMPTYVALUE)
						return false; // f1����δ����Ŀ�꣬�ǲ��
					if (f2[object_id] == EMPTYVALUE)
						return true; // f2����δ����Ŀ�꣬�ǲ��
				}
			}

			// �����ȼ��ıȽ�
			bool is_better;
			bool is_worse;
			counter = 0;
			bool outperformed;
			for (int i = 0; i < _object_level; i++)
			{
				is_better = true;
				is_worse = true;
				for (int j = 0; j < _object_number[i]; j++)
				{
					object_id = _priority[counter++];
					if (f1[object_id] == f2[object_id])
						continue;
					outperformed = f1[object_id] > f2[object_id];
					outperformed = outperformed ^ _min_is_better[object_id];

					if (outperformed)
						is_worse = false;
					else
						is_better = false;
				}

				if (is_better && !is_worse)
					return true;
				if (!is_better && is_worse)
					return false;
			}
			return false;
		}

		// ������ȼ�Ŀ���е�������
		virtual double diff_max_priori(const double f1[], const double f2[])
		{
			int o_id = _priority[0];
			double result = abs(f1[o_id] - f2[o_id]);

			double buffer;
			for (int i = 1; i < _object_number[0]; i++)
			{
				o_id = _priority[i];
				buffer = abs(f1[o_id] - f2[o_id]);
				if (buffer > result)
					result = buffer;
			}

			return result;
		}
	};

	// ��һ��Ŀ���µıȽ����ػ������Խ�Լ�Ƚ�ʱ��
	class SingleObjectComparer final : public Comparer
	{
	private:
		int _key_id;
		bool _key_is_min;

	public:
		SingleObjectComparer(Objective* objectives, const int objective_number)
			:Comparer(objectives, objective_number)
		{
			_key_id = _priority[0];
			_key_is_min = _min_is_better[_key_id];
		}

		~SingleObjectComparer()
		{

		}

		bool isBetter(const double f1[], const double f2[])
		{
			bool smaller = f1[_key_id] < f2[_key_id];
			return smaller ^ _key_is_min;
		}

		double diff_max_priori(const double f1[], const double f2[])
		{
			return f1[0] - f2[0];
		}
	};
}
