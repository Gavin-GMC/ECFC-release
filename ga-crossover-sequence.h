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
#include<unordered_map>

namespace ECFC
{
	// �������������Ż������ӣ���չ��˵����ֳ���
	// 1. ͬһ���д�����ͬ�ı���
	// 2. �������д��ڶԷ������еı���
	// ��˱�����ƥ��Ҳ�߱��������
	// 1. 1��1ƥ��
	// 2. 1�Զ�ƥ��
	// 3. 1��0ƥ��
	// �ۺ���Щ���������������ѡ�ⳤ��һ�£��������µ��ж������
	// 1��1ƥ��ʱ�����������ӵļ��裬��������
	// 1�Զ�ʱ��ѡ����һδ����ƥ����Ķ��󣬶��ڶ�γ��ֵ�ͬһ��ֵӦ�ֱ�Դ�
	// 1��0ʱ����1�Զ൫����ƥ������þ�����ʱ���ȴ�δƥ�������ѡ�����Ӷ�ƥ�䵫δ�þ�������ѡ��
	// 
	// һ���ؼ�������ƥ�����Ĺ����ǻ����Ļ��������ģ�����Ҫ����һ����������ÿ�����������
	// Ϊ�˼�˸������Σ���֤��������³���ԣ����û�������ʽ
	// 
	// ���ڵ����ֲ���������Լ������һԼ������Ԫ����ͬ������ʱ���������еĽ������ӱ���͹�������
	// ��˲��ٹ���׷�����������Լ���Ϊ�������ԣ�������˳�����ƥ��
	// 1. Ѱ��ѧϰ������Ԫ��ƥ�䣬��δ���н����Ļ���λ
	// 2. ��λԪ�أ����ع鴫ͳ�ĵ㽻��/���Ƚ��棩
	// 3. ���δƥ��Ԫ��
	
	// ��������λƥ���ϵ����s1��������map_list�еĻ���λ��s2����ƥ�䣬��������洢��match_list�н��з��أ�
	void buildMap(double* s1, double* s2, int solution_size, int* map_list, int* match_list, int list_size)
	{
		bool* used = new bool[solution_size];
		for (int i = 0; i < solution_size; i++)
			used[i] = false;

		// Ѱ��ƥ��
		bool found;
		int s1_index;
		for (int i = 0; i < list_size; i++)
		{
			// ���Ҷ�Ӧλ��
			s1_index = map_list[i];
			found = false;
			int d;
			for (d = 0; d < solution_size; d++)
			{
				if (used[d]) // ��ǰλ���ѱ�ƥ��
					continue;

				if (equal(s1[s1_index], s2[d]))
				{
					used[d] = true;
					match_list[i] = d;
					found = true;
					break;
				}
			}

			// ���������ȶ�λ
			d = s1_index;
			while (!found)
			{
				if (!used[d])
				{
					used[d] = true;
					match_list[i] = d;
					found = true;
					break;
				}
				d = (d + rand() % solution_size) % solution_size;
			}
		}

		delete[] used;
	}

	void buildMap(double* s1, double* s2, int solution_size, int begin_index, int end_index, int* match_list)
	{
		bool* used = new bool[solution_size];
		for (int i = 0; i < solution_size; i++)
			used[i] = false;

		int list_size = end_index - begin_index;

		// Ѱ��ƥ��
		bool found;
		int s1_index;
		for (int i = 0; i < list_size; i++)
		{
			// ���Ҷ�Ӧλ��
			s1_index = begin_index + i;
			found = false;
			int d;
			for (d = 0; d < solution_size; d++)
			{
				if (used[d]) // ��ǰλ���ѱ�ƥ��
					continue;

				if (equal(s1[s1_index], s2[d]))
				{
					used[d] = true;
					match_list[i] = d;
					found = true;
					break;
				}
			}

			// ���������ȶ�λ
			d = s1_index;
			while (!found)
			{
				if (!used[d])
				{
					used[d] = true;
					match_list[i] = d;
					found = true;
					break;
				}
				d = (d + rand() % solution_size) % solution_size;
			}
		}

		delete[] used;
	}
	
	// ����ƥ�佻��
	class PartialMappedCrossover : public Crossover
	{
	private:
		int _buffer_size;
		int begin_index;
		int end_index;
		double* offspring1;
		double* offspring2;

		void buildPartialMap(double* s1, double* s2, int part_size, std::vector<std::pair<double, double>>& partial_map)
		{
			partial_map.clear();

			// �ߴ���
			for (int i = 0; i < part_size; i++)
			{
				partial_map.push_back({ s1[i],s2[i] });
			}

			// �ϲ�
			for (int i = 0; i < partial_map.size(); i++)
			{
				for (int j = i + 1; j < partial_map.size(); j++)
				{
					if (equal(partial_map[i].second, partial_map[j].first)) // ���ϲ�
					{
						partial_map[i].second = partial_map[j].second;
						partial_map.erase(partial_map.begin() + j);
						
						j = i + 1; // ���¼��
					}
					else if (equal(partial_map[i].first, partial_map[j].second)) // ��ǰ�ϲ�
					{
						partial_map[i].first = partial_map[j].first;
						partial_map.erase(partial_map.begin() + j);

						j = i + 1; // ���¼��
					}
				}
			}
		}

	public:
		PartialMappedCrossover(double cross_rate, bool coupled = true)
			:Crossover(cross_rate, coupled)
		{
			_buffer_size = 0;
			offspring1 = nullptr;
			offspring2 = nullptr;

			begin_index = -1;
			end_index = -1;
		}

		~PartialMappedCrossover()
		{
			delete[] offspring1;
			delete[] offspring2;
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
					delete[] offspring1;
					delete[] offspring2;
					_buffer_size = s->getSolutionSize();
					offspring1 = new double[_buffer_size];
					offspring2 = new double[_buffer_size];
				}

				begin_index = rand() % s->getSolutionSize();
				end_index = rand() % s->getSolutionSize();
				if (end_index < begin_index)
					std::swap(begin_index, end_index);
				end_index++;
			}
		}

		void apply(Solution* child, Solution* s1, Solution** s2, ProblemHandle* handle)
		{
			preparation(child);

			if (is_crossover)
			{
				if (new_pair)
				{
					int solution_size = child->getSolutionSize();

					memcpy(offspring1, s1->result, child->getSolutionSize() * sizeof(double));
					memcpy(offspring2, s2[0]->result, child->getSolutionSize() * sizeof(double));

					// ���������
					for (int i = begin_index; i < end_index; i++)
					{
						std::swap(offspring1[i], offspring2[i]);
					}

					// ��ȡ�����ӳ�价
					int part_size = end_index - begin_index;
					std::vector<std::pair<double, double>> partial_map;
					buildPartialMap(offspring1 + begin_index, offspring2 + begin_index, part_size, partial_map);
					
					// ִ��ӳ��
					bool used;
					// ��o1ӳ��
					for (int m = 0; m < partial_map.size(); m++)
					{
						used = false; // ÿ��ӳ��ÿ������ֻʹ��һ��
						for (int i = 0; i < begin_index; i++)
						{
							if (equal(offspring1[i], partial_map[m].first))
							{
								offspring1[i] = partial_map[m].second;
								used = true;
								break;
							}
						}

						if (used)
							continue;

						for (int i = end_index; i < solution_size; i++)
						{
							if (equal(offspring1[i], partial_map[m].first))
							{
								offspring1[i] = partial_map[m].second;
								break;
							}
						}
					}
					// ��o2ӳ��
					for (int m = 0; m < partial_map.size(); m++)
					{
						used = false; // ÿ��ӳ��ÿ������ֻʹ��һ��
						for (int i = 0; i < begin_index; i++)
						{
							if (equal(offspring2[i], partial_map[m].second))
							{
								offspring2[i] = partial_map[m].first;
								used = true;
								break;
							}
						}

						if (used)
							continue;

						for (int i = end_index; i < solution_size; i++)
						{
							if (equal(offspring2[i], partial_map[m].second))
							{
								offspring2[i] = partial_map[m].first;
								break;
							}
						}
					}

					// �����Ӵ�
					memcpy(child->result, offspring1, child->getSolutionSize() * sizeof(double));
				}
				else
				{
					memcpy(child->result, offspring2, child->getSolutionSize() * sizeof(double));
				}
			}
			else
			{
				memcpy(child->result, s1->result, child->getSolutionSize() * sizeof(double));
			}

			ending();
		}
	};

	// ѭ������
	class CycleCrossover : public Crossover
	{
	private:
		int _buffer_size;
		bool* used;
		int begin_index;
		double* offspring1;
		double* offspring2;

	public:
		CycleCrossover(double cross_rate, bool coupled = true)
			:Crossover(cross_rate, coupled)
		{
			_buffer_size = 0;
			used = nullptr;
			offspring1 = nullptr;
			offspring2 = nullptr;

			begin_index = -1;
		}

		~CycleCrossover()
		{
			delete[] used;
			delete[] offspring1;
			delete[] offspring2;
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
					delete[] used;
					delete[] offspring1;
					delete[] offspring2;
					_buffer_size = s->getSolutionSize();
					used = new bool[_buffer_size];
					offspring1 = new double[_buffer_size];
					offspring2 = new double[_buffer_size];
				}

				begin_index = rand() % s->getSolutionSize();
			}
		}

		void apply(Solution* child, Solution* s1, Solution** s2, ProblemHandle* handle)
		{
			preparation(child);

			if (is_crossover)
			{
				if (new_pair)
				{
					memcpy(offspring1, s1->result, child->getSolutionSize() * sizeof(double));
					memcpy(offspring2, s2[0]->result, child->getSolutionSize() * sizeof(double));
					for (int i = 0; i < s1->getSolutionSize(); i++)
						used[i] = false;

					int current_index = begin_index;
					bool found;
					double corr;
					while (true)
					{
						used[current_index] = true;
						std::swap(offspring1[current_index], offspring2[current_index]);

						// Ѱ�Ҷ�λ
						found = false;
						corr = s2[0]->result[current_index];
						for (int i = 0; i < s1->getSolutionSize(); i++)
						{
							if (used[i]) // ���ɣ��ӣ���
								continue;

							if (equal(corr, s1->result[i]))
							{
								found = true;
								current_index = i;
								break;
							}
						}

						if (!found) // �ﵽ�˵�
							break;
					}

					memcpy(child->result, offspring1, child->getSolutionSize() * sizeof(double));
				}
				else
				{
					memcpy(child->result, offspring2, child->getSolutionSize() * sizeof(double));
				}

			}
			else
			{
				memcpy(child->result, s1->result, child->getSolutionSize() * sizeof(double));
			}

			ending();
		}
	};

	// ˳�򽻲�
	class OrderCrossover : public Crossover
	{
	private:
		int _buffer_size;
		int* matched;
		int begin_index;
		int end_index;
		double* offspring1;
		double* offspring2;

	public:
		OrderCrossover(double cross_rate, bool coupled = true)
			:Crossover(cross_rate, coupled)
		{
			_buffer_size = 0;
			matched = nullptr;
			offspring1 = nullptr;
			offspring2 = nullptr;

			begin_index = -1;
			end_index = -1;
		}

		~OrderCrossover()
		{
			delete[] matched;
			delete[] offspring1;
			delete[] offspring2;
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
					delete[] matched;
					delete[] offspring1;
					delete[] offspring2;
					_buffer_size = s->getSolutionSize();
					matched = new int[_buffer_size];
					offspring1 = new double[_buffer_size];
					offspring2 = new double[_buffer_size];
				}

				int begin_index = rand() % s->getSolutionSize();
				int end_index = rand() % s->getSolutionSize();
				if (end_index < begin_index)
					std::swap(begin_index, end_index);
				end_index++;
			}
		}

		void apply(Solution* child, Solution* s1, Solution** s2, ProblemHandle* handle)
		{
			preparation(child);

			if (is_crossover)
			{
				if (new_pair)
				{
					int solution_size = child->getSolutionSize();

					memcpy(offspring1, s1->result, child->getSolutionSize() * sizeof(double));
					memcpy(offspring2, s2[0]->result, child->getSolutionSize() * sizeof(double));
					
					buildMap(s1->result, s2[0]->result, solution_size, begin_index, end_index, matched);

					int list_size = end_index - begin_index;
					std::sort(matched, matched + list_size);

					// ����
					int o1_index = 0;
					int o2_index = 0;
					int match_index = 0;
					while (true)
					{
						if (o1_index == begin_index)
							o1_index = end_index;
						if (o1_index >= solution_size) // �����жϷ��������ֹend_index�ǽ�Ľ�β
							break;

						while (match_index < list_size && o2_index == matched[match_index])
						{
							o2_index++;
							match_index++;
						}

						std::swap(offspring1[o1_index], offspring2[o2_index]);
						o1_index++;
						o2_index++;
					}

					memcpy(child->result, offspring1, child->getSolutionSize() * sizeof(double));
				}
				else
				{
					memcpy(child->result, offspring2, child->getSolutionSize() * sizeof(double));
				}
			}
			else
			{
				memcpy(child->result, s1->result, child->getSolutionSize() * sizeof(double));
			}

			ending();
		}
	};

	// ��·�����潻��
	class SubtourExchangeCrossover : public Crossover
	{
	private:
		int _buffer_size;
		int* matched;
		int begin_index;
		int end_index;
		double* offspring1;
		double* offspring2;

	public:
		SubtourExchangeCrossover(double cross_rate, bool coupled = true)
			:Crossover(cross_rate, coupled)
		{
			_buffer_size = 0;
			matched = nullptr;
			offspring1 = nullptr;
			offspring2 = nullptr;

			begin_index = -1;
			end_index = -1;
		}

		~SubtourExchangeCrossover()
		{
			delete[] matched;
			delete[] offspring1;
			delete[] offspring2;
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
					delete[] offspring1;
					delete[] offspring2;
					delete[] matched;
					_buffer_size = s->getSolutionSize();
					matched = new int[_buffer_size];
					offspring1 = new double[_buffer_size];
					offspring2 = new double[_buffer_size];
				}

				begin_index = rand() % s->getSolutionSize();
				end_index = rand() % s->getSolutionSize();
				if (end_index < begin_index)
					std::swap(begin_index, end_index);
				end_index++;
			}
		}

		void apply(Solution* child, Solution* s1, Solution** s2, ProblemHandle* handle)
		{
			preparation(child);

			if (is_crossover)
			{
				if (new_pair)
				{
					int solution_size = child->getSolutionSize();

					memcpy(offspring1, s1->result, child->getSolutionSize() * sizeof(double));
					memcpy(offspring2, s2[0]->result, child->getSolutionSize() * sizeof(double));
					
					buildMap(s1->result, s2[0]->result, solution_size, begin_index, end_index, matched);

					int list_size = end_index - begin_index;
					std::sort(matched, matched + list_size);

					for (int i = 0; i < list_size; i++)
					{
						std::swap(offspring1[i + begin_index], offspring2[matched[i]]);
					}

					memcpy(child->result, offspring1, child->getSolutionSize() * sizeof(double));
				}
				else
				{
					memcpy(child->result, offspring2, child->getSolutionSize() * sizeof(double));
				}
			}
			else
			{
				memcpy(child->result, s1->result, child->getSolutionSize() * sizeof(double));
			}

			ending();
		}
	};

	// ����λ�õĽ���
	class PositionBasedCrossover : public Crossover
	{
	private:
		int _buffer_size;

		int* genes;
		int* matched;
		int list_size;

		double* offspring1;
		double* offspring2;
		double _proportion;

	public:
		PositionBasedCrossover(double cross_rate, double select_gene_proportion, bool coupled = true)
			:Crossover(cross_rate, coupled)
		{
			_buffer_size = 0;
			_proportion = select_gene_proportion;

			genes = nullptr;
			matched = nullptr;
			list_size = 0;

			offspring1 = nullptr;
			offspring2 = nullptr;
		}

		~PositionBasedCrossover()
		{
			delete[] genes;
			delete[] matched;
			delete[] offspring1;
			delete[] offspring2;
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
					delete[] genes;
					delete[] matched;
					delete[] offspring1;
					delete[] offspring2;
					_buffer_size = s->getSolutionSize();
					genes = new int[_buffer_size];
					matched = new int[_buffer_size];
					offspring1 = new double[_buffer_size];
					offspring2 = new double[_buffer_size];
				}

				list_size = 0;
				for (int i = 0; i < s->getSolutionSize(); i++)
				{
					if (rand01() < _proportion)
					{
						genes[list_size] = i;
						list_size++;
					}
				}
			}
		}

		void apply(Solution* child, Solution* s1, Solution** s2, ProblemHandle* handle)
		{
			preparation(child);

			if (is_crossover)
			{
				if (new_pair)
				{
					int solution_size = child->getSolutionSize();

					memcpy(offspring1, s1->result, solution_size * sizeof(double));
					memcpy(offspring2, s2[0]->result, solution_size * sizeof(double));
					
					buildMap(s1->result, s2[0]->result, solution_size, genes, matched, list_size);
					
					// ѡ��Ļ����ѧϰ����ѡȡ������ֱ�ӽ����Ӵ�
					// (����ʵ��order-based�������������������Ӳ�û�б��ʲ�ͬ����order-based��orderX�������ƣ�����ʹ��position-based������)
					std::sort(matched, matched + list_size);
					int gene_index = 0;
					for (int i = 0; i < solution_size; i++)
					{
						if (i == genes[gene_index]) // ��ѡ��Ļ���λ
						{
							std::swap(offspring1[i], offspring2[matched[gene_index]]);
							gene_index++;
							if (gene_index == list_size)
								break;
						}
					}

					memcpy(child->result, offspring1, solution_size * sizeof(double));
				}
				else
				{
					memcpy(child->result, offspring2, child->getSolutionSize() * sizeof(double));
				}

			}
			else
			{
				memcpy(child->result, s1->result, child->getSolutionSize() * sizeof(double));
			}

			ending();
		}
	};
}