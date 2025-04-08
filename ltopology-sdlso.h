//------------------------Description------------------------
// This file defines the learning topology of Stochastic Dominant
// Learning Swarm Optimizer, where learning objects are randomly 
// selected from the population and only learn if the object is
// superior to itself.
//-------------------------Reference-------------------------
// [1] Q. Yang, W. -N. Chen, T. Gu, H. Jin, W. Mao and J. Zhang, "An Adaptive Stochastic Dominant Learning Swarm Optimizer for High-Dimensional Optimization," 
// in IEEE Transactions on Cybernetics, vol. 52, no. 3, pp. 1960-1976, March 2022, doi: 10.1109/TCYB.2020.3034427. 
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
	// Stochastic Dominant Learning Swarm Optimizer
	class SDLTopology : public InSwarmTopology
	{
	public:
		SDLTopology()
			:InSwarmTopology()
		{
			
		}

		~SDLTopology()
		{

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

			for (int sid = 0; sid < graph_size; sid++)
			{
				// ������ʼ��
				back->addStart(&cswarm[0][sid]);

				// ���ѡ����������
				sid1 = rand() % graph_size;
				sid2 = rand() % graph_size;

				// �������Ӷ����������������
				if (cswarm[0][sid2] < cswarm[0][sid1])
				{
					std::swap(sid1, sid2);
				}
				// �ж��뵱ǰ��������ӹ�ϵ
				if (cswarm[0][sid] < cswarm[0][sid2])
				{
					// ����ѧϰ
					back->addEnd(nullptr);
				}
				else
				{
					// ��������ͼ
					back->addEnd(&cswarm[0][sid1].solution);
					back->addEnd(&cswarm[0][sid2].solution);
				}
			}

			return back;
		}
	};
}