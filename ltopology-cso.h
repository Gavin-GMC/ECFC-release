//------------------------Description------------------------
// This file defines the learning topology of cso, where
// the worse individual in the two individuals learn from the 
// better one.
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
#include"individual-pbest.h"

namespace ECFC
{
	class CSOTopology : public InSwarmTopology
	{
	private:
        int* id_list;
        int id_list_size;
        Solution population_mean;

        void cal_mean(Population& population)
        {
            population_mean.setSize(population[0].solution);

            double buffer;
            size_t p_size = population_mean.getSolutionSize();
            size_t s_size = population.getSize();
            for (size_t i = 0; i < p_size; i++)
            {
                buffer = 0;
                for(size_t j = 0; j < s_size; j++)
                {
                    buffer += population[j][i];
                }
                buffer /= s_size;
                population_mean[i] = buffer;
            }
        }

	public:
		CSOTopology()
			:InSwarmTopology()
		{
            id_list = nullptr;
            id_list_size = 0;
		}

		~CSOTopology()
		{

		}

		static void preAssert(AssertList& list, double* paras)
		{
			// none
		}

		static void postAssert(AssertList& list, double* paras)
		{
			list.add(new Assert(ModuleType::T_learntopology, "objects", 1, MatchType::postAssert));
			list.add(new Assert(ModuleType::T_learntopology, "coupled", 2, MatchType::postAssert)); // ����������໥ѧϰ
		}

		void ini()
		{

		}

		LearningGraph* getTopology(Population** subswarm, BestArchive** best_holder, const int swarm_number)
		{
			Population* cswarm = subswarm[0];
			int graph_size = cswarm->getSize();
			LearningGraph* back = new LearningGraph(graph_size, 2);

            cal_mean(*cswarm);

            if (id_list_size != graph_size)
            {
                delete[] id_list;
                id_list = new int[graph_size];
                for (int i = 0; i < graph_size; i++)
                    id_list[i] = i;
            }

            shuffle(id_list, graph_size);

            int winner, loser;
			Individual* s1;
			Solution* s2;
            for (int i = 0; i < graph_size; i++)
            {
                if (i == graph_size - 1)
                {
                    s1 = &cswarm[0][id_list[i]];
                    back->addStart(s1);
                    back->addEnd(nullptr); // ��ѧϰ����
                    continue;
                }
                winner = id_list[i];
                loser = id_list[i + 1];

                if (cswarm[0][loser] < cswarm[0][winner])
                    std::swap(loser, winner);

                // ��������ͼ
                s1 = &cswarm[0][winner];
                back->addStart(s1);
                back->addEnd(nullptr); // Ӯ�߲�ѧϰ

                s1 = &cswarm[0][loser];
                s2 = &cswarm[0][winner].solution;
                back->addStart(s1);
                back->addEnd(s2); // ��Ӯ��ѧϰ
                back->addEnd(&population_mean); // ����Ⱥ����ѧϰ
                i++;
            }

			return back;
		}
	};
}