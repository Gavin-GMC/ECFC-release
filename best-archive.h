//------------------------Description------------------------
// This file defines the best archive of optimizer and swarms,
// which records all optimal or acceptable solutions.
//-------------------------Reference-------------------------
//-------------------------Copyright-------------------------
// Copyright (c) 2024 ���������ƣ���ȷ�ϣ�, All Rights Reserved.
// You are free to use the ECFC����ȷ�ϣ� for research purposes.
// All publications which use this library or any code in the library
// should acknowledge the use of "ECFC" and reference 
// "δȷ��"
//-----------------------------------------------------------

#pragma once
#include"ecfc-constant.h"
#include"solution.h"
#include"comparer.h"
#include"problem-handle.h"

namespace ECFC
{
	class BestArchive
	{
	protected:
		Solution* _best_solution;
		Comparer* _comparer_pointer;
		int _best_size;

	public:
		BestArchive()
		{
			_best_solution = nullptr;
			_comparer_pointer = nullptr;
			_best_size = 0;
		};

		virtual ~BestArchive() {
			
		};

		virtual Solution* getBest()
		{
			return _best_solution;
		}

		virtual Solution* getElite() = 0;

		virtual bool updateBest(const Solution& new_best) = 0;

		virtual int getBestSize()
		{
			return _best_size;
		}

		virtual void clear()
		{
			_best_size = 0;
		}

		virtual void setProblem(ProblemHandle* handle) = 0;
	};

	// �������Ž������
	class BasicArchive final :public BestArchive
	{
	public:
		BasicArchive() 
		{
			_best_solution = new Solution();
		}

		~BasicArchive() 
		{
			delete _best_solution;
		}

		Solution* getElite()
		{
			return _best_solution;
		}

		bool updateBest(const Solution& new_best)
		{
			if (_best_size == 0)
			{
				_best_solution->copy(new_best);
				_best_size = 1;
				return true;
			}
			if (_comparer_pointer->isBetter(new_best.fitness, _best_solution->fitness))
			{
				_best_solution->copy(new_best);
				return true;
			}
			return false;
		}

		void setProblem(ProblemHandle* handle)
		{
			_comparer_pointer = handle->getSolutionComparer();
		}
	};

	// ƫ�����Ž⵵�����������Զ���ߴ磩
	class HistoricArchive final : public BestArchive
	{
	private:
		int _max_size;
		int _worst_id;

		void updateWorstId()
		{
			_worst_id = 0;
			for (int i = 0; i < _best_size; i++)
			{
				if (_comparer_pointer->isBetter(_best_solution[_worst_id].fitness, _best_solution[i].fitness))
				{
					_worst_id = i;
				}
			}
		}

		void updateBestSolution(int challenger_id)
		{
			if (_comparer_pointer->isBetter(_best_solution[challenger_id].fitness, _best_solution[0].fitness))
			{
				_best_solution[0].swap(_best_solution[challenger_id]);
			}
		}

	public:
		HistoricArchive(int max_size)
		{
			_max_size = max_size;
			_worst_id = -1;
			_best_solution = new Solution[_max_size + 1];
		}

		~HistoricArchive()
		{
			delete[] _best_solution;
		}

		Solution* getElite()
		{
			return _best_solution + (rand() % _best_size);
		}

		bool updateBest(const Solution& new_best)
		{
			// ��ǰ����δ���������β
			if (_best_size < _max_size)
			{
				_best_solution[_best_size].copy(new_best);
				updateBestSolution(_best_size);
				_best_size++;

				if (_best_size == _max_size)
				{
					updateWorstId();
				}
				return true;
			}
			else // ������������������бȽ��滻
			{
				if (_comparer_pointer->isBetter(new_best.fitness, _best_solution[_worst_id].fitness))
				{
					_best_solution[_worst_id].copy(new_best);
					updateBestSolution(_worst_id);
					updateWorstId();
					return true;
				}
			}

			return false;
		}

		void setProblem(ProblemHandle* handle)
		{

		}
	};

	// ��Ŀ�����Ž������
	class MultiobjectArchive final :public BestArchive
	{
	public:
		MultiobjectArchive()
		{
			_best_solution = new Solution[PFSIZE + 1];
			_best_size = 0;
		}

		~MultiobjectArchive()
		{
			delete[] _best_solution;
		}

		Solution* getElite()
		{
			return _best_solution + (rand() % _best_size);
		}

		bool updateBest(const Solution& new_best)
		{
			//����Pareto��
			Solution* temp = (Solution*)malloc(sizeof(Solution));
			for (int i = 0; i < _best_size; i++)
			{
				// �����н�֧��������н���ͬ����������
				if (_comparer_pointer->isBetter(_best_solution[i].fitness, new_best.fitness) || _best_solution[i] == new_best)
					return false;

				// ֧�����н⣬�Ƴ���֧���
				if (_comparer_pointer->isBetter(new_best.fitness, _best_solution[i].fitness))
				{
					*temp = std::move(_best_solution[i]);
					_best_solution[i] = std::move(_best_solution[_best_size - 1]);
					_best_solution[_best_size - 1] = std::move(*temp);

					i--;
					_best_size--;
				}
			}

			free(temp);

			//Pareto������
			if (_best_size == (PFSIZE - 1))
				return false;

			_best_solution[_best_size++].copy(new_best);
			return true;
		}

		void setProblem(ProblemHandle* handle)
		{
			// ��һĿ��ʱ����־��¼

			_comparer_pointer = handle->getSolutionComparer();
		}
	};

	// ��Ŀ���ģ̬���Ž������
	class MultimodalArchive final :public BestArchive
	{
	private:
		double _accuracy_object;
		double _accuracy_decision;
		int _worst_id;

		bool isEffect(const Solution& a, const Solution b)
		{
			// �������Բ��������ͬ��
			return _comparer_pointer->diff_max_priori(a.fitness, b.fitness) > _accuracy_object || eu_distance(a.result, b.result) < _accuracy_decision;
		}

	public:
		MultimodalArchive(double accuracy_object = 0.01, double accuracy_decision = 1)
		{
			_best_solution = new Solution[PKSIZE + 1];
			_best_size = 0;
			_accuracy_object = accuracy_object;
			_accuracy_decision = accuracy_decision;
			_worst_id = -1;
		}

		~MultimodalArchive()
		{
			delete[] _best_solution;
		}

		Solution* getElite()
		{
			return _best_solution + (rand() % _best_size);
		}

		bool updateBest(const Solution& new_best)
		{
			Solution* temp = (Solution*)malloc(sizeof(Solution));
			// ������ֵ��
			for (int i = 0; i < _best_size; i++)
			{
				// ��ǰ�����Բ��ڷ�ֵ������ͬ���ֵ��
				if (_comparer_pointer->isBetter(_best_solution[i].fitness, new_best.fitness))
				{
					if (isEffect(_best_solution[i], new_best))
						return false;
				}
				// ��ǰ�����Ժ��ڷ�ֵ������ͬ���ֵ��
				else
				{
					if (isEffect(_best_solution[i], new_best))
					{
						*temp = std::move(_best_solution[i]);
						_best_solution[i] = std::move(_best_solution[_best_size - 1]);
						_best_solution[_best_size - 1] = std::move(*temp);

						i--;
						_best_size--;
					}
				}
			}
			free(temp);
			// ��ֵ������
			if (_best_size == (PKSIZE - 1))
				return false;

			_best_solution[_best_size++].copy(new_best);
			return true;
		}

		void setProblem(ProblemHandle* handle)
		{
			_comparer_pointer = handle->getSolutionComparer();
		}
	};
}