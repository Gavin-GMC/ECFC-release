//------------------------Description------------------------
// This file defines the crossover operator of evolationary algorithms.
// It realizes the generation of new candidate solutions by exchanging
//  fragments of different individuals in a specific way.
//-------------------------Reference-------------------------
//-------------------------Copyright-------------------------
// Copyright (c) 2024 ���������ƣ���ȷ�ϣ�, All Rights Reserved.
// You are free to use the ECFC����ȷ�ϣ� for research purposes.
// All publications which use this library or any code in the library
// should acknowledge the use of "ECFC" and reference 
// "δȷ��"
//-----------------------------------------------------------

#pragma once
#include"ga-crossover.h"

namespace ECFC
{
	// �㽻�棬���������λ֮���Ƭ��
	class PointCrossover : public Crossover
	{
	private:
		int _point_number;
		int* points;

		// ����Ƿ�����ظ��Ľ���λ��
		bool isRepeat(int index)
		{
			for (int i = 0; i < index; i++)
			{
				if (points[i] == points[index])
					return true;
			}
			return false;
		}

	public:
		PointCrossover(int point_number = 1, double cross_rate = 0.9, bool coupled = true)
			:Crossover(cross_rate, coupled)
		{
			points = new int[point_number];
			_point_number = point_number;
		}

		~PointCrossover()
		{
			delete[] points;
		}

		static void preAssert(AssertList& list, double* paras)
		{
			Crossover::preAssert(list, paras);
			list.add(new Assert(ModuleType::T_learntopology, "objects", 1, MatchType::notLessButNotice)); // ��Ҫ1��ѧϰĿ��
			if (paras[1])
				list.add(new Assert(ModuleType::T_learntopology, "coupled", 1, MatchType::anyButNotice)); // ����������໥ѧϰ
		}

		static void postAssert(AssertList& list, double* paras)
		{
			Crossover::postAssert(list, paras);
		}

		void preparation(Solution* s)
		{
			Crossover::preparation(s);

			if (new_pair && is_crossover)// �µ��ױ����Լ���Ҫ����
			{
				// ���ɽ����λ���У����ظ�������������У�
				for (int i = 0; i < _point_number; i++)
				{
					points[i] = rand() % s->getSolutionSize();
					//�ظ����������ɣ������ں�ѡ���ģԶ���ڽ������Ŀ�����
					while (isRepeat(i))
					{
						points[i] = rand() % s->getSolutionSize();
					}
				}
				std::sort(points, points + _point_number);
			}
		}

		void apply(Solution* child, Solution* s1, Solution** s2, ProblemHandle* handle)
		{
			preparation(s1);

			if (is_crossover)
			{
				int position = 0;
				int length;
				bool is_s1 = true;
				// �����������ױ��н���ѧϰ
				for (int i = 0; i < _point_number; i++)
				{
					length = points[i] - position;
					if (is_s1)
					{
						memcpy(child->result + position, s1->result + position, length * sizeof(double));
					}
					else
					{
						memcpy(child->result + position, s2[0]->result + position, length * sizeof(double));
					}
					position = points[i];
					is_s1 = !is_s1;
				}

				// ���һ����Ҫ�ر����
				length = child->getSolutionSize() - position;
				if (is_s1)
				{
					memcpy(child->result + position, s1->result + position, length * sizeof(double));
				}
				else
				{
					memcpy(child->result + position, s2[0]->result + position, length * sizeof(double));
				}

#if defined(DEBUGMODE) || defined(CROSSDEBUG)
				sys_logger.debug("AFTER crossover:\t" + child->decoder_pointer->toString(child->result, child->fitness, false));
#endif
			}
			else // ������ֱ�Ӽ̳��ױ� 
			{
				memcpy(child->result, s1->result, child->getSolutionSize() * sizeof(double));
			}

			ending();
		}
	};

	class UniformCrossover : public Crossover
	{
	private:
		bool* _is_s1;
		int _buffer_size;

	public:
		UniformCrossover(double cross_rate, bool coupled = true)
			:Crossover(cross_rate, coupled)
		{
			_buffer_size = 0;
			_is_s1 = nullptr;
		}

		~UniformCrossover()
		{
			delete[] _is_s1;
		}

		static void preAssert(AssertList& list, double* paras)
		{
			Crossover::preAssert(list, paras);
			list.add(new Assert(ModuleType::T_learntopology, "objects", 1, MatchType::notLessButNotice)); // ��Ҫ1��ѧϰĿ��
			if (paras[1])
				list.add(new Assert(ModuleType::T_learntopology, "coupled", 1, MatchType::anyButNotice)); // ����������໥ѧϰ
		}

		static void postAssert(AssertList& list, double* paras)
		{
			Crossover::postAssert(list, paras);
		}

		void preparation(Solution* s)
		{
			Crossover::preparation(s);

			if (new_pair && is_crossover)// �µ��ױ����Լ���Ҫ����
			{
				if (_buffer_size != s->getSolutionSize()) // ���»����С
				{
					delete[] _is_s1;
					_buffer_size = s->getSolutionSize();
					_is_s1 = new bool[_buffer_size];
				}

				for (int i = 0; i < s->getSolutionSize(); i++)
				{
					_is_s1[i] = rand01() < 0.5;
				}
			}
		}

		void apply(Solution* child, Solution* s1, Solution** s2, ProblemHandle* handle)
		{
			preparation(child);

			if (is_crossover)
			{
				for (int i = 0; i < child->getSolutionSize(); i++)
				{
					if (_is_s1[i])
						child->result[i] = s1->result[i];
					else
						child->result[i] = s2[0]->result[i];
				}
			}
			else
			{
				memcpy(child->result, s1->result, child->getSolutionSize() * sizeof(double));
			}

			ending();
		}
	};

	//Simulated Binary Crossover operator[1]
	class SBXCrossover : public Crossover
	{
	private:
		double _eta;
		double _r;
		double _belta;
		double* _c2_buffer;
		int buffer_size;

	public:
		SBXCrossover(double eta, double cross_rate, bool coupled = true)
			:Crossover(cross_rate, coupled)
		{
			_eta = eta;
			buffer_size = 0;
			
			_r = 0;
			_belta = 0;
			_c2_buffer = nullptr;
		}

		~SBXCrossover()
		{
			delete[] _c2_buffer;
		}

		static void preAssert(AssertList& list, double* paras)
		{
			Crossover::preAssert(list, paras);
			list.add(new Assert(ModuleType::T_learntopology, "objects", 1, MatchType::notLessButNotice)); // ��Ҫ1��ѧϰĿ��
			if (paras[1])
				list.add(new Assert(ModuleType::T_learntopology, "coupled", 1, MatchType::anyButNotice)); // ����������໥ѧϰ
		}

		static void postAssert(AssertList& list, double* paras)
		{
			Crossover::postAssert(list, paras);
		}

		void preparation(Solution* s)
		{
            Crossover::preparation(s);

			if (new_pair && is_crossover)// �µ��ױ����Լ���Ҫ����
			{
				// ���û����С
				if (buffer_size != s->getSolutionSize())
				{
					delete[] _c2_buffer;
					buffer_size = s->getSolutionSize();
					_c2_buffer = new double[buffer_size];
				}
			}
		}

		void apply(Solution* child, Solution* s1, Solution** s2, ProblemHandle* handle)
		{
			preparation(child);

			if (is_crossover)
			{
				if (new_pair) // ���ױ��ԣ���Ҫ����
				{
					for (int i = 0; i < child->getSolutionSize(); i++)
					{
						if (rand01() < 0.5)// ��Ƭ�β����н���
						{
							_c2_buffer[i] = s2[0]->result[i];
							child->result[i] = s1->result[i];
						}
						else {
							_r = rand01_();
							if (_r > 0.5)
								_belta = pow(2 - _r * 2, -1 / (1 + _eta));
							else _belta = pow(_r * 2, 1 / (1 + _eta));

							if (rand01() > 0.5)
								_belta *= -1;


							child->result[i] = 0.5 * ((1 + _belta) * s1->result[i] + (1 - _belta) * s2[0]->result[i]);
							_c2_buffer[i] = 0.5 * ((1 - _belta) * s1->result[i] + (1 + _belta) * s2[0]->result[i]);
						}
					}
				}
				else
				{
					memcpy(child->result, _c2_buffer, sizeof(double) * child->getSolutionSize());
				}
			}
			 else
			{
			    memcpy(child->result, s1->result, child->getSolutionSize() * sizeof(double));
			}

			ending();
		}
	};

	// ��ֽ���
	class DifferenceCrossover : public Crossover
	{
	private:
		double _zoom_factor;
		bool adaptive_factor;
		Comparer* _comparer_pointer;

	public:
		DifferenceCrossover(double cross_rate = 0.9, double factor = 0.5, bool coupled = true)
			:Crossover(cross_rate, coupled)
		{
			_zoom_factor = factor;
			adaptive_factor = _zoom_factor == EMPTYVALUE;
		}

		~DifferenceCrossover()
		{
		}

		static void preAssert(AssertList& list, double* paras)
		{
			Crossover::preAssert(list, paras);
			list.add(new Assert(ModuleType::T_learntopology, "objects", 2, MatchType::notLessButNotice)); // ��Ҫ2��ѧϰĿ��
		}

		static void postAssert(AssertList& list, double* paras)
		{
			Crossover::postAssert(list, paras);
		}

		void setProblem(ProblemHandle* problem_handle)
		{
			_comparer_pointer = problem_handle->getSolutionComparer();
		}

		void preparation(Solution* s)
		{
			Crossover::preparation(s);
		}

		void apply(Solution* child, Solution* s1, Solution** s2, ProblemHandle* handle)
		{
			preparation(child);

			if (is_crossover)
			{
				Solution* s_best = s1;
				Solution* s_middle = s2[0];
				Solution* s_worst = s2[1];

				// ����
				if (_comparer_pointer->isBetter(s_middle->fitness, s_best->fitness))
					std::swap(s_middle, s_best);
				if (_comparer_pointer->isBetter(s_worst->fitness, s_middle->fitness))
					std::swap(s_worst, s_middle);
				if (_comparer_pointer->isBetter(s_middle->fitness, s_best->fitness))
					std::swap(s_middle, s_best);

				if (adaptive_factor) // ������������Ӧ����
				{
					//Fi = Fl+(Fu-Fl)*(fm-fb)/(fw-fb)
					_zoom_factor = 0.1 + 0.8 * (s_middle->fitness[0] - s_best->fitness[0]) / (s_worst->fitness[0] - s_best->fitness[0]);
				}

				for (int i = 0; i < child->getSolutionSize(); i++)
					child->result[i] = s_best->result[i] +
					_zoom_factor * (s_middle->result[i] - s_worst->result[i]);
			}
			else
			{
				memcpy(child->result, s1->result, child->getSolutionSize() * sizeof(double));
			}

			ending();
		}
	};

	// ������
	class NoCrossover : public Crossover
	{
	public:
		NoCrossover()
			:Crossover(0, 1)
		{

		}

		~NoCrossover()
		{

		}

		static void preAssert(AssertList& list, double* paras)
		{
			Crossover::preAssert(list, paras);
			list.add(new Assert(ModuleType::T_learntopology, "objects", 0, MatchType::notLessButNotice)); // ��Ҫ2��ѧϰĿ��
		}

		static void postAssert(AssertList& list, double* paras)
		{
			Crossover::postAssert(list, paras);
		}

		void preparation(Solution* s)
		{
			Crossover::preparation(s);
		}

		double apply(Solution* s1, Solution** s2, const int demension, ProblemHandle* handle)
		{
			return (*s1)[demension];
		}

		void apply(Solution* child, Solution* s1, Solution** s2, ProblemHandle* handle)
		{
			child->copy(*s1);
		}
	};
}