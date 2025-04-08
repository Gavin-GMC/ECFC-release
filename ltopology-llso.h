//------------------------Description------------------------
// This file defines the learning topology of level-based pso, where
// individuals are divided into several levels, and the lower particles
//  are learning at two random high-level.
//-------------------------Reference-------------------------
//-------------------------Copyright-------------------------
// Copyright (c) 2024 ���������ƣ���ȷ�ϣ�, All Rights Reserved.
// You are free to use the ECFC����ȷ�ϣ� for research purposes.
// All publications which use this library or any code in the library
// should acknowledge the use of "ECFC" and reference 
// "δȷ��"
//-----------------------------------------------------------

#pragma once
#include"learning-topology.h"
#include"individual.h"

namespace ECFC
{
	class LLSOTopology : public InSwarmTopology
	{
	private:
		int _levels;
		int _order_size;
		int* _orders;

		void setSize(int size)
		{
			if (_order_size < size)
			{
				delete[] _orders;
				_order_size = size;
				_orders = new int[_order_size];
			}
		}

		void update(Population& swarm)
		{
			int swarm_size = swarm.getSize();
			setSize(swarm_size);
			for (int i = 0; i < swarm_size; i++)
			{
				_orders[i] = i;
			}

			// ��������(��Ϊ����ܿ���Ҳ�������õ�)
			for (int i = 1; i < swarm_size; i++)
			{
				int j = i - 1;
				int key = _orders[i];
				while ((j >= 0) && (swarm[key] < swarm[_orders[j]]))
				{
					_orders[j + 1] = _orders[j];
					j--;
				}
				_orders[j + 1] = key;
			}
		}

	public:
		LLSOTopology(int level_number = 4)
			:InSwarmTopology()
		{
			_levels = level_number;
			_order_size = 0;
			_orders = nullptr;
		}

		~LLSOTopology()
		{
			delete[] _orders;
		}

		static void preAssert(AssertList& list, double* paras)
		{
			// none
		}

		static void postAssert(AssertList& list, double* paras)
		{
			list.add(new Assert(ModuleType::T_learntopology, "objects", 2, MatchType::postAssert));
		}

		void ini()
		{

		}

		LearningGraph* getTopology(Population** subswarm, BestArchive** best_holder, const int swarm_number)
		{
			Population* cswarm = subswarm[0];
			int graph_size = cswarm->getSize();
			LearningGraph* back = new LearningGraph(graph_size, 2);

			int sid1, sid2;
			Individual* s1;
			Solution* s2;
			int level_capacity = ceil(double(graph_size) / _levels);

			// ��������
			update(*cswarm);

			// ���õ�һ�㼶
			for (int cid = 0; cid < level_capacity; cid++)
			{
				s1 = &cswarm[0][_orders[cid]];
				back->addStart(s1);

				back->addEnd(nullptr);
				back->addEnd(nullptr);
			}

			// ���ú����㼶
			int counter = level_capacity;
			for (int lid = 1; lid < _levels; lid++)
			{
				for (int cid = 0; cid < level_capacity && counter < graph_size; cid++) // ���������������һ�㲻ȫ����Խ��
				{
					// ��ȡѧϰ������
					sid1 = rand() % lid * level_capacity + rand() % level_capacity;
					sid2 = rand() % lid * level_capacity + rand() % level_capacity;

					// ��֤sid1����sid2
					if (sid2 < sid1)
						std::swap(sid1, sid2);

					// ��������ͼ
					s1 = &cswarm[0][counter];
					back->addStart(s1);
					counter++;

					// ��������ͼ
					s2 = &cswarm[0][_orders[sid1]].solution;
					back->addEnd(s2);
					s2 = &cswarm[0][_orders[sid2]].solution;
					back->addEnd(s2);
				}
			}

			return back;
		}
	};
}