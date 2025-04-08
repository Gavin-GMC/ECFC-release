//------------------------Description------------------------
// This file defines the population consisted by individuals.
// It provides an object that manages the set of candidate
// solutions.
//-------------------------Reference-------------------------
//-------------------------Copyright-------------------------
// Copyright (c) 2024 ���������ƣ���ȷ�ϣ�, All Rights Reserved.
// You are free to use the ECFC����ȷ�ϣ� for research purposes.
// All publications which use this library or any code in the library
// should acknowledge the use of "ECFC" and reference 
// "δȷ��"
//-----------------------------------------------------------

#pragma once
#include"individual.h"
#include"individual-factory.h"
#include<algorithm>

namespace ECFC
{
	class Population
	{
	private:
		int _individual_buffer_size;
		int _population_size;
		Individual** _individuals;
		IndividualType _individual_type;

	public:
		Population(IndividualType type = IndividualType::F_default, int size = 1)
		{
			_individual_buffer_size = size;
			_population_size = size;
			_individual_type = type;

			_individuals = IndividualFactory::newIndividualArray(_individual_type, size);
		}

		~Population()
		{
			for (int i = 0; i < _individual_buffer_size; i++)
				delete _individuals[i];
			
			delete[] _individuals;
		}

		void ini()
		{
			for (int i = 0; i < _individual_buffer_size; i++)
			{
				_individuals[i]->ini(true, true, true);
			}
		}

		int getSize()
		{
			return _population_size;
		}

		Individual** getIndividuals()
		{
			return _individuals;
		}

		// �����ж��𻵵��쳣����������������
		void extend(int target_size)
		{
			if (target_size <= _individual_buffer_size)
			{
				_population_size = target_size;
			}
			else
			{
				int array_size = _population_size;//��������������Ƶ������չ����

				//��������Ⱥ�ĳ���
				while (array_size < target_size)
				{
					array_size += _population_size;
				}

				// �����µĿռ�
				Individual** individuals_buffer = _individuals;
				_individuals = IndividualFactory::newIndividualArray(_individual_type, array_size);

				//��ԭ��Ⱥ����������Ⱥ
				for (int i = 0; i < _individual_buffer_size; i++)
				{
					// ͨ�����������������������
					std::swap(_individuals[i], individuals_buffer[i]);
				}

				for (int i = 0; i < _individual_buffer_size; i++)
				{
					delete individuals_buffer[i];
				}
				delete[] individuals_buffer;

				//�������������Ⱥʣ�����
				for (int i = _individual_buffer_size; i < array_size; i++)
				{
					_individuals[i]->setProblem(**_individuals);
				}

				//����size������ʵ��һ��
				_individual_buffer_size = array_size;
				_population_size = target_size;
			}
		}

		void append(Individual* individuals, int size = 1)
		{
			//����ڴ泤�Ȳ����ͽ�����չ
			if (_population_size + size > _individual_buffer_size)
			{
				extend(_population_size + size);
				_population_size -= size;
			}
			//���¸������������ȫ�������������Ϊ��֪�������δ����״̬���������Կ�������������������һ�㣩
			for (int i = 0; i < size; i++)
			{
				_individuals[_population_size++]->copy(individuals[i]);
			}
		}

		void remove(Individual* remove_individuals, int size = 1)
		{
			for (int i = 0; i < size; i++)
			{
				for (int j = 0; j < _population_size; j++)
				{
					//����Ҫ�Ƴ��ĸ���
					if (remove_individuals[i] == *(_individuals[j]))
					{
						//�����������β���������鳤�ȼ�¼
						std::swap(_individuals[j], _individuals[--_population_size]);
					}
				}
			}
		}

		void remove(int* remove_individuals_index, int size = 1)
		{
			//��������������
			std::sort(remove_individuals_index, remove_individuals_index + size, std::greater<int>());
			//�Ӵ���С�����������β����
			int last_index = -1;
			for (int i = 0; i < size; i++)
			{
				//�쳣����Ĵ����ų��Ƿ������Լ��ظ���ɾ��
				if (remove_individuals_index[i] < 0)
					break;
				if (remove_individuals_index[i] >= _population_size || remove_individuals_index[i] == last_index)
					continue;

				//�����������β���������鳤�ȼ�¼
				_population_size--;
				std::swap(_individuals[remove_individuals_index[i]], _individuals[_population_size]);
				//��¼����
				last_index = remove_individuals_index[i];
			}
		}

		void sort()
		{
            int n = _population_size;
            bool swapped;
            for (int i = 0; i < n - 1; ++i) {
                swapped = false;
                for (int j = 0; j < n - 1 - i; ++j) {
                    // �����ǰԪ�ش�����һ��Ԫ�أ��򽻻�����
                    if (*_individuals[j + 1] < *_individuals[j]) {
                        _individuals[j]->swap(*_individuals[j + 1]);
                        swapped = true;
                    }
                }
                // �������һ����û�з�����������˵�������Ѿ����򣬿�����ǰ��������
                if (!swapped) {
                    break;
                }
            }
		}

		void setProblem(ProblemHandle* problem_handle)
		{
			for (int i = 0; i < _individual_buffer_size; i++)
			{
				_individuals[i]->setProblem(problem_handle);
			}
		}

		Individual& operator[](int i)
		{
			return *(_individuals[i]);
		}

	};
}