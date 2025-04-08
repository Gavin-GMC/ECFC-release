//------------------------Description------------------------
// This file defines the learning strategy of individuals, which
// means how the individual learns from the goal and produces
// the offspring.
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
#include"individual.h"
#include"population.h"
#include"problem-handle.h"

namespace ECFC
{
	class LearningStrategy
	{
	public:
		LearningStrategy() {}

		virtual ~LearningStrategy()
		{

		}

		// ���Եĳ�ʼ��
		virtual void ini(ProblemHandle* problem_handle) {};

		virtual void setProblem(ProblemHandle* problem_handle) {}

		// ��ÿһ����ʼǰ��׼������
		virtual void preparation_s(Population& population) {};

		// ��ÿ�����忪ʼǰ��׼������
		virtual void preparation_i(Individual* individual, Solution** learning_object, Individual* child){}

		// ��ÿһά��ʼǰ��׼������
		virtual void preparation_d(const int decision_d, Individual* individual, Solution** learning_object, ProblemHandle* problem_handle, Individual* child) {}

		// ����ѧϰ��������һά��ֵ
		virtual double nextDecision(const int decision_d, Individual* individual, Solution** learning_object, ProblemHandle* problem_handle, Individual* child)
		{
			// ����
			sys_logger.error("Current learning strategy does not support dimensional-by-dimension construction!");
			// �˳�
			exit(-1);
		}

		// ��ÿһά���ɺ�ĸ��¹���
		virtual void update_d(Individual* child, const int decision_d) {};

		// ��ÿ���������ɲ�������ĸ��¹���
		virtual void update_i(Individual* child) {};

		// ���Ӵ����ɲ�ѡ���ĸ��¹���
		virtual void update_s(Population& population, Population& offspring) {};

		virtual void getNewIndividual(Individual* child, Individual* individual, Solution** learning_object, ProblemHandle* problem_handle)
		{
			preparation_i(individual, learning_object, child);
			for (int i = 0; i < child->getSolutionSize(); i++)
			{
				preparation_d(i, individual, learning_object, problem_handle, child);
				child->solution.result[i] = nextDecision(i, individual, learning_object, problem_handle, child);
				update_d(child, i);
			}
		}
	};
}