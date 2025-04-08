//------------------------Description------------------------
// This file defines the learning strategy of evolution strategy. 
// It realizes the generation of child individuals through crossover 
// and mutation operators.
//-------------------------Reference-------------------------
//-------------------------Copyright-------------------------
// Copyright (c) 2024 ���������ƣ���ȷ�ϣ�, All Rights Reserved.
// You are free to use the ECFC����ȷ�ϣ� for research purposes.
// All publications which use this library or any code in the library
// should acknowledge the use of "ECFC" and reference 
// "δȷ��"
//-----------------------------------------------------------

#pragma once
#include"learning-strategy.h"
#include"ga-crossover.h"
#include"ga-mutation.h"

namespace ECFC
{
	// ��������ͱ�����Ŵ��㷨ѧϰ���Կ��
	class ESStrategy : public LearningStrategy
	{
	private:
		Mutation* _mutation_func;
		Crossover* _crossover_func;
	public:
		ESStrategy(Mutation* mutation_func, Crossover* crossover_func)
			:LearningStrategy()
		{
			_mutation_func = mutation_func;
			_crossover_func = crossover_func;
		}

		~ESStrategy()
		{
			delete _mutation_func;
			delete _crossover_func;
		}

		static void preAssert(AssertList& list)
		{

		}

		static void postAssert(AssertList& list)
		{

		}

		void preparation_s(Population& population)
		{
			_crossover_func->ini();
		};

		void getNewIndividual(Individual* child, Individual* individual, Solution** learning_object, ProblemHandle* problem_handle)
		{
			problem_handle->constrainReset();
			_crossover_func->apply(&child->solution, &individual->solution, learning_object, problem_handle);
			_mutation_func->apply(&child->solution, problem_handle);
		}
	};
}