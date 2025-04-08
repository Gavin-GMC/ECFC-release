//------------------------Description------------------------
// This file defines the offspring generation framework for individuals.
// Parental individuals in the population construct offspring individuals
// based on this framework
//-------------------------Reference-------------------------
//-------------------------Copyright-------------------------
// Copyright (c) 2024 ���������ƣ���ȷ�ϣ�, All Rights Reserved.
// You are free to use the ECFC����ȷ�ϣ� for research purposes.
// All publications which use this library or any code in the library
// should acknowledge the use of "ECFC" and reference 
// "δȷ��"
//-----------------------------------------------------------

#pragma once
#include"solution.h"
#include"population.h"
#include"learning-topology.h"
#include"learning-strategy.h"
#include"problem-handle.h"
#include"best-archive.h"
#include"terminator.h"
#include"solution-repairman.h"

// Attention!����ǰ���ɹ�����δ������ֹ����������ʹ��������Ҳ����ɵ�ǰ���Ӵ�����

namespace ECFC
{
	class OffspringGenerator
	{
	protected:
		ProblemHandle* _problem_handle;
		InSwarmTopology* _inswarm_topology;
		LearningStrategy* _learning_strategy;
		Terminator* _terminator_pointer;
		SolutionRepaireman* _repairman;

		void evaluated_solution(Solution& solution, BestArchive* archive_pointer)
		{
			bool best_solution_updated;
			_problem_handle->solutionEvaluate(solution);
			best_solution_updated = archive_pointer->updateBest(solution);
			_terminator_pointer->update(best_solution_updated);
		}

	public:
		OffspringGenerator(InSwarmTopology* topology, LearningStrategy* strategy, SolutionRepaireman* repaire)
		{
			_inswarm_topology = topology;
			_learning_strategy = strategy;
			_repairman = repaire;
			_problem_handle = nullptr;
			_terminator_pointer = nullptr;
		}

		virtual ~OffspringGenerator()
		{
			delete _inswarm_topology;
			delete _learning_strategy;
			delete _repairman;
			delete _problem_handle;
		}

		static void preAssert(AssertList& list, double* paras)
		{
			// none
		}

		static void postAssert(AssertList& list, double* paras)
		{
			// none
		}

		virtual void setTerminator(Terminator* pointer)
		{
			_terminator_pointer = pointer;
		}

		virtual void ini()
		{
			_inswarm_topology->ini();
			_learning_strategy->ini(_problem_handle);
		}

		virtual void setProblem(ProblemHandle* problem_handle)
		{
			delete _problem_handle;
			_problem_handle = new ProblemHandle(*problem_handle);
			_learning_strategy->setProblem(problem_handle);
		}

		virtual void getOffspring(Population& offspring, Population** parent, BestArchive** archive, int subswarm_number = 1) = 0;

		virtual void update(Population& population, Population& offspring)
		{
			_learning_strategy->update_s(population, offspring);
		}

		void changeEnv(const double* values, int variableId)
		{
			_problem_handle->changeEnv(values, variableId);
		}
	};

	class GeneratorWithGenerationFramework final : public OffspringGenerator
	{
	public:
		GeneratorWithGenerationFramework(InSwarmTopology* topology, LearningStrategy* strategy, SolutionRepaireman* repaire)
			:OffspringGenerator(topology, strategy, repaire)
		{

		}

		~GeneratorWithGenerationFramework()
		{

		}

		static void preAssert(AssertList& list, double* paras)
		{
			// none
		}

		static void postAssert(AssertList& list, double* paras)
		{
			// none
		}

		void getOffspring(Population& offspring, Population** parent, BestArchive** archive, int subswarm_number = 1)
		{
			Individual* individual;
			Solution** learning_objects;
			bool best_solution_updated;
			LearningGraph* graph = _inswarm_topology->getTopology(parent, archive, subswarm_number);

			size_t graph_size = graph->getSize();
			if (offspring.getSize() < graph_size)
			{
				offspring.extend(int(graph_size));
			}

			_learning_strategy->preparation_s(*(parent[0]));

			// ����׼��
			int solution_size = offspring[0].getSolutionSize();
			Individual* child;

			for (int i = 0; i < graph_size; i++)
			{
				// ����׼��
				individual = graph->getStartPoint(i);
				learning_objects = graph->getEndPoint(i);
				child = &offspring[i];

				if (learning_objects[0] == nullptr && graph->getEndSize() != 0) // ��ѧϰ��ֱ�ӽ����Ӵ�
				{
					child->copy(*individual);
					continue;
				}

				// ����׼��
				_learning_strategy->preparation_i(individual, learning_objects, child); // �Ƚ��и���׼������Ϊ�ù����У����ֲ��Կ��ܸı��ڲ��ı����ڴ棬�������Ӵ���
				_problem_handle->setResult(child->solution); // ����ѡ��������������
				_problem_handle->constrainReset();

				// �Ӵ�����
				_learning_strategy->getNewIndividual(child, individual, learning_objects, _problem_handle);

				// Լ���޸�
				for (int d = 0; d < solution_size; d++)
				{
					// ������ֵת��ΪԤ��ľ���
					child->solution.result[d] = _problem_handle->choiceDiscretized(d, child->solution.result[d]);
					// Լ���޸�
					if (!_problem_handle->constrainCheck(d, child->solution.result[d]))
					{
						child->solution.result[d] = _repairman->repair(child->solution, d, _problem_handle);
					}
					_problem_handle->constrainChange(d, child->solution.result[d]);			
				}
				// �Ӵ�����
				_problem_handle->solutionEvaluate(child->solution);
				best_solution_updated = archive[0]->updateBest(child->solution);
				
				_learning_strategy->update_i(child);
				_terminator_pointer->update(best_solution_updated);
			}

			delete graph;
		}
	};

	class GeneratorWithNoCheckGeneration final : public OffspringGenerator
	{
	public:
		GeneratorWithNoCheckGeneration(InSwarmTopology* topology, LearningStrategy* strategy, SolutionRepaireman* repaire)
			:OffspringGenerator(topology, strategy, repaire)
		{

		}

		~GeneratorWithNoCheckGeneration()
		{

		}

		static void preAssert(AssertList& list, double* paras)
		{
			// none
		}

		static void postAssert(AssertList& list, double* paras)
		{
			// none
		}

		void getOffspring(Population& offspring, Population** parent, BestArchive** archive, int subswarm_number = 1)
		{
			Individual* individual;
			Solution** learning_objects;
			bool best_solution_updated;
			LearningGraph* graph = _inswarm_topology->getTopology(parent, archive, subswarm_number);

			size_t graph_size = graph->getSize();
			if (offspring.getSize() < graph_size)
				offspring.extend(int(graph_size));

			_learning_strategy->preparation_s(*(parent[0]));

			for (int i = 0; i < graph_size; i++)
			{
				// ����׼��
				individual = graph->getStartPoint(i);
				learning_objects = graph->getEndPoint(i);
				Individual* child = &offspring[i];

				if (learning_objects[0] == nullptr && graph->getEndSize() != 0) // ��ѧϰ��ֱ�ӽ����Ӵ�
				{
					offspring[i].copy(*individual);
					continue;
				}

				// ����׼��
				_learning_strategy->preparation_i(individual, learning_objects, child); // �Ƚ��и���׼������Ϊ�ù����У����ֲ��Կ��ܸı��ڲ��ı����ڴ棬�������Ӵ���
				_problem_handle->setResult(child->solution); // ����ѡ��������������

				// �Ӵ�����
				_learning_strategy->getNewIndividual(child, individual, learning_objects, _problem_handle);

				// �Ӵ�����
				_problem_handle->solutionEvaluate(child->solution);
				best_solution_updated = archive[0]->updateBest(offspring[i].solution);
				
				// �������������ĸ���
				_learning_strategy->update_i(&offspring[i]);
				_terminator_pointer->update(best_solution_updated);
			}

			delete graph;
		}
	};

	class GeneratorWithOrderedConstruct final : public OffspringGenerator
	{
	public:
		GeneratorWithOrderedConstruct(InSwarmTopology* topology, LearningStrategy* strategy, SolutionRepaireman* repaire)
			:OffspringGenerator(topology, strategy, repaire)
		{

		}

		~GeneratorWithOrderedConstruct()
		{

		}

		static void preAssert(AssertList& list, double* paras)
		{
			list.add(new Assert(ModuleType::T_learnstrategy, "constrcutive", 1, MatchType::equal));
		}

		static void postAssert(AssertList& list, double* paras)
		{
			// none
		}

		void getOffspring(Population& offspring, Population** parent, BestArchive** archive, int subswarm_number = 1)
		{
			Individual* individual;
			Solution** learning_objects;
			bool best_solution_updated;
			LearningGraph* graph = _inswarm_topology->getTopology(parent, archive, subswarm_number);

			int graph_size = graph->getSize();
			if (offspring.getSize() < graph_size)
				offspring.extend(graph_size);

			_learning_strategy->preparation_s(*(parent[0]));

			for (int i = 0; i < graph_size; i++)
			{
				// ����׼��
				_problem_handle->constrainReset();
				individual = graph->getStartPoint(i);
				learning_objects = graph->getEndPoint(i);

				// ����ֱ�ӽ����Ӵ�
				if (learning_objects[0] == nullptr && graph->getEndSize() != 0)
				{
					offspring[i].copy(*individual);
					continue;
				}

				// ����׼��
				Individual* child = &offspring[i];
				_learning_strategy->preparation_i(individual, learning_objects, child); // �Ƚ��и���׼������Ϊ�ù����У����ֲ��Կ��ܸı��ڲ��ı����ڴ棬�������Ӵ���
				_problem_handle->setResult(child->solution); // ����ѡ��������������

				// �Ӵ�����
				for (int d = 0; d < child->getSolutionSize(); d++)
				{
					_learning_strategy->preparation_d(d, individual, learning_objects, _problem_handle, child);

					child->solution.result[d] = _learning_strategy->nextDecision(d, individual, learning_objects, _problem_handle, child);
					
					// ������ֵת��ΪԤ��ľ���
					child->solution.result[d] = _problem_handle->choiceDiscretized(d, child->solution.result[d]);
					// Լ���޸�
					if (!_problem_handle->constrainCheck(d, child->solution.result[d]))
					{
						child->solution.result[d] = _repairman->repair(child->solution, d, _problem_handle);
					}

					_problem_handle->constrainChange(d, child->solution.result[d]);
					_learning_strategy->update_d(child, d);
				}

				// �Ӵ�����
				_problem_handle->solutionEvaluate(child->solution);
				best_solution_updated = archive[0]->updateBest(child->solution);

				// �������������ĸ���
				_learning_strategy->update_i(child);
				_terminator_pointer->update(best_solution_updated);
			}

			delete graph;
		}
	};

	class GeneratorWithParallelConstruct final : public OffspringGenerator
	{
	protected:
		ProblemHandle** _problem_handles;
		int _handle_buffer_number;

		void extend_buffer(int size)
		{
			if (_handle_buffer_number >= size)
				return;

			ProblemHandle** _new_handles = new ProblemHandle*[size];
			memcpy(_new_handles, _problem_handles, sizeof(ProblemHandle*) * _handle_buffer_number);

			for (int i = _handle_buffer_number; i < size; i++)
			{
				_new_handles[i] = new ProblemHandle(*_problem_handle);
			}

			delete[] _problem_handles;
			_problem_handles = _new_handles;
			_handle_buffer_number = size;
		}

		void delete_buffer()
		{
			for (int i = 0; i < _handle_buffer_number; i++)
			{
				delete _problem_handles[i];
			}
			delete[] _problem_handles;
			_handle_buffer_number = 0;
		}

	public:
		GeneratorWithParallelConstruct(InSwarmTopology* topology, LearningStrategy* strategy, SolutionRepaireman* repaire)
			:OffspringGenerator(topology, strategy, repaire)
		{
			_problem_handles = nullptr;
			_handle_buffer_number = 0;
		}

		~GeneratorWithParallelConstruct()
		{
			delete_buffer();
		}

		static void preAssert(AssertList& list, double* paras)
		{
			list.add(new Assert(ModuleType::T_learnstrategy, "constrcutive", 1, MatchType::equal));
		}

		static void postAssert(AssertList& list, double* paras)
		{
			// none
		}

		void setProblem(ProblemHandle* problem_handle)
		{
			delete_buffer();
			OffspringGenerator::setProblem(problem_handle);
		}

		void getOffspring(Population& offspring, Population** parent, BestArchive** archive, int subswarm_number = 1)
		{
			Individual* individual;
			Solution** learning_objects;
			bool best_solution_updated;
			LearningGraph* graph = _inswarm_topology->getTopology(parent, archive, subswarm_number);

			int graph_size = graph->getSize();
			if (offspring.getSize() < graph_size)
				offspring.extend(graph_size);

			_learning_strategy->preparation_s(*(parent[0]));

			// ����׼��
			extend_buffer(graph_size);
			for (int i = 0; i < graph_size; i++)
			{
				_problem_handles[i]->constrainReset();
			}
			for (int i = 0; i < graph_size; i++)
			{
				individual = graph->getStartPoint(i);
				learning_objects = graph->getEndPoint(i);
				_learning_strategy->preparation_i(individual, learning_objects, &offspring[i]);
				_problem_handles[i]->setResult(offspring[i].solution);
			}

			// �Ӵ�����
			Individual* child;
			ProblemHandle* corr_handle;
			int solution_size = offspring[0].getSolutionSize();
			for (int d = 0; d < solution_size; d++)
			{
				for (int i = 0; i < graph_size; i++)
				{
					// ����׼��
					individual = graph->getStartPoint(i);
					learning_objects = graph->getEndPoint(i);

					// ����ֱ�ӽ����Ӵ�
					if (learning_objects[0] == nullptr && graph->getEndSize() != 0)
					{
						continue;
					}

					// ����׼��
					child = &offspring[i];
					corr_handle = _problem_handles[i];
					_learning_strategy->preparation_d(d, individual, learning_objects, corr_handle, child);

					// ������һά
					child->solution.result[d] = _learning_strategy->nextDecision(d, individual, learning_objects, corr_handle, child);
					
					// ������ֵת��ΪԤ��ľ���
					child->solution.result[d] = corr_handle->choiceDiscretized(d, child->solution.result[d]);
					// Լ���޸�
					if (!corr_handle->constrainCheck(d, child->solution.result[d]))
					{
						child->solution.result[d] = _repairman->repair(child->solution, d, corr_handle);
					}

					// ����Լ��״̬��ѧϰ����
					corr_handle->constrainChange(d, child->solution.result[d]);
					_learning_strategy->update_d(child, d);
				}
			}

			// �Ӵ�����
			for (int i = 0; i < graph_size; i++)
			{
				individual = graph->getStartPoint(i);
				learning_objects = graph->getEndPoint(i);
				if (learning_objects[0] == nullptr)
					offspring[i].copy(*individual);
				else
				{
					_problem_handle->solutionEvaluate(offspring[i].solution);
					best_solution_updated = archive[0]->updateBest(offspring[i].solution);
					_terminator_pointer->update(best_solution_updated);
				}

				_learning_strategy->update_i(&offspring[i]);
			}

			delete graph;
		}
	};
}