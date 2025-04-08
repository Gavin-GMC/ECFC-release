//------------------------Description------------------------
// This file defines the learning topology used in gentic algorithms,
// including roulette, championship, uniform selection.
// individuals learn from gbest and its pbest.
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
#include"learning-topology.h"

namespace ECFC
{
	// ��Ŀ�굥��Ⱥ���̶�ѡ��
	class Roulette : public InSwarmTopology
	{
	private:
		double* _dart_board;
		int _board_size;
		
		int _objects;
		int* _id_list;

		void setSize(int size)
		{
			if (_board_size != size)
			{
				delete[] _dart_board;
				_board_size = size;
				_dart_board = new double[_board_size];
			}
		}

	public:
		Roulette(int object_number = 1)
			:InSwarmTopology()
		{
			_objects = object_number;
			_id_list = new int[object_number + 2];

			_dart_board = nullptr;
			_board_size = 0;
		}

		~Roulette()
		{
			delete[] _dart_board;
			delete[] _id_list;
		}

		
		static void preAssert(AssertList& list, double* paras)
		{
			// none
		}

		static void postAssert(AssertList& list, double* paras)
		{
			list.add(new Assert(ModuleType::T_learntopology, "objects", paras[0], MatchType::postAssert));
			list.add(new Assert(ModuleType::T_learntopology, "coupled", 1, MatchType::postAssert)); // ����������໥ѧϰ
		}

		void ini()
		{

		}

		LearningGraph* getTopology(Population** subswarm, BestArchive** best_holder, const int swarm_number)
		{
			Population* cswarm = subswarm[0];
			int graph_size = cswarm->getSize();
			setSize(graph_size);
			LearningGraph* back = new LearningGraph(graph_size, _objects);

			// ͳ�������Сֵ����һ��
			double max = -1;
			double min = ECFC_MAX;
			for (int i = 0; i < graph_size; i++)
			{
				if (max < cswarm[0][i].solution.fitness[0])
					max = cswarm[0][i].solution.fitness[0];
				if (min > cswarm[0][i].solution.fitness[0])
					min = cswarm[0][i].solution.fitness[0];
			}
			max *= 1.001; // ��֤��һ������0ֵ

			// ��һ��
			double total = 0;
			for (int i = 0; i < graph_size; i++)
			{
				_dart_board[i] = (max - cswarm[0][i].solution.fitness[0]) / (max - min);
				total += _dart_board[i];
			}
			for (int i = 0; i < graph_size; i++)
				_dart_board[i] /= total;

			// ���̶�
			double p0 = rand01();
			int id = 0;
			int counter = 0;
			Individual* s1;
			Solution* s2;
			while (counter < graph_size)
			{
				// ���̶�
				for (int i = 0; i < _objects + 1; i++)
				{
					while (p0 > 0)
					{
						id++;
						if (id == graph_size)
							id = 0;
						p0 -= _dart_board[id];
					}
					
					_id_list[i] = id;
					p0 += rand01();
				}

				// ��������ͼ
				s1 = &cswarm[0][_id_list[0]];
				back->addStart(s1);
				s2 = &cswarm[0][_id_list[1]].solution;
				back->addEnd(s2);
				for (int i = 1; i < _objects; i++)
				{
					s2 = &cswarm[0][_id_list[i + 1]].solution;
					back->addEnd(s2);
				}
				counter++;
				
				// ͼ����
				if (counter == graph_size)
					break;

				// ����Գƶ���
				s1 = &cswarm[0][_id_list[1]];
				back->addStart(s1);
				s2 = &cswarm[0][_id_list[0]].solution;
				back->addEnd(s2);
				for (int i = 1; i < _objects; i++)
				{
					s2 = &cswarm[0][_id_list[i + 1]].solution;
					back->addEnd(s2);
				}

				counter++;
			}

			return back;
		}
	};

	class Championship : public InSwarmTopology
	{
	private:
		int _competition_scale;

		int _objects_number;
		int* _id_list;

	public:
		Championship(int n = 2, int object_number = 1)
			:InSwarmTopology()
		{
			_competition_scale = n;

			_objects_number = object_number;
			_id_list = new int[object_number + 2];
		}

		~Championship()
		{
			delete[] _id_list;
		}

		
		static void preAssert(AssertList& list, double* paras)
		{
			// none
		}

		static void postAssert(AssertList& list, double* paras)
		{
			list.add(new Assert(ModuleType::T_learntopology, "objects", paras[1], MatchType::postAssert));
			list.add(new Assert(ModuleType::T_learntopology, "coupled", 1, MatchType::postAssert)); // ����������໥ѧϰ
		}

		void ini()
		{

		}

		LearningGraph* getTopology(Population** subswarm, BestArchive** best_holder, const int swarm_number)
		{
			Population* cswarm = subswarm[0]; // ��ǰ��Ⱥ
			int graph_size = cswarm->getSize();
			LearningGraph* back = new LearningGraph(graph_size, _objects_number);

			int counter = 0;
			int winner, challenger;
			Individual* s1;
			Solution* s2;
			while (counter < graph_size)
			{
				// ������
				for (int i = 0; i < _objects_number + 1; i++)
				{
					winner = rand() % graph_size;
					for (int i = 1; i < _competition_scale; i++)
					{
						challenger = rand() % graph_size;
						if ((*cswarm)[challenger] < (*cswarm)[winner])
							winner = challenger;
					}
					_id_list[i] = winner;
				}

				// ��������ͼ
				s1 = &(*cswarm)[_id_list[0]];
				back->addStart(s1);
				s2 = &(*cswarm)[_id_list[1]].solution;
				back->addEnd(s2);
				for (int i = 1; i < _objects_number; i++)
				{
					s2 = &cswarm[0][_id_list[i + 1]].solution;
					back->addEnd(s2);
				}
				counter++;

				// ͼ����
				if (counter == graph_size)
					break;

				// ����Գƶ���
				s1 = &(*cswarm)[_id_list[1]];
				back->addStart(s1);
				s2 = &(*cswarm)[_id_list[0]].solution;
				back->addEnd(s2);
				for (int i = 1; i < _objects_number; i++)
				{
					s2 = &cswarm[0][_id_list[i + 1]].solution;
					back->addEnd(s2);
				}
				counter++;
			}

			return back;
		}
	};

	class Uniform : public InSwarmTopology
	{
	private:
		int _objects_number;
		int* _id_list;

	public:
		Uniform(int object_number = 1)
			:InSwarmTopology()
		{
			_objects_number = object_number;
			_id_list = new int[object_number + 2];
		}

		~Uniform()
		{
			delete[] _id_list;
		}
		
		static void preAssert(AssertList& list, double* paras)
		{
			// none
		}

		static void postAssert(AssertList& list, double* paras)
		{
			list.add(new Assert(ModuleType::T_learntopology, "objects", paras[0], MatchType::postAssert));
			list.add(new Assert(ModuleType::T_learntopology, "coupled", 1, MatchType::postAssert)); // ����������໥ѧϰ
		}

		void ini()
		{

		}

		LearningGraph* getTopology(Population** subswarm, BestArchive** best_holder, const int swarm_number)
		{
			Population* cswarm = subswarm[0];
			int graph_size = cswarm->getSize();
			LearningGraph* back = new LearningGraph(graph_size, _objects_number);

			int counter = 0;
			Individual* s1;
			Solution* s2;
			while (counter < graph_size)
			{
				// ����ѡȡ
				for (int i = 0; i < _objects_number + 1; i++)
				{
					_id_list[i] = rand() % graph_size;
				}

				// ��������ͼ
				s1 = &cswarm[0][_id_list[0]];
				back->addStart(s1);
				s2 = &cswarm[0][_id_list[1]].solution;
				back->addEnd(s2);
				for (int i = 1; i < _objects_number; i++)
				{
					s2 = &cswarm[0][_id_list[i + 1]].solution;
					back->addEnd(s2);
				}
				counter++;

				// ͼ����
				if (counter == graph_size)
					break;

				// ����Գƶ���
				s1 = &cswarm[0][_id_list[1]];
				back->addStart(s1);
				s2 = &cswarm[0][_id_list[0]].solution;
				back->addEnd(s2);
				for (int i = 1; i < _objects_number; i++)
				{
					s2 = &cswarm[0][_id_list[i + 1]].solution;
					back->addEnd(s2);
				}
				counter++;
			}

			return back;
		}
	};
}