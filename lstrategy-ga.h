//------------------------Description------------------------
// This file defines the learning strategy of ga. It realizes
// the generation of child individuals through crossover and 
// mutation operators.
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
#include"crossover-factory.h"
#include"mutation-factory.h"

namespace ECFC
{
	// ��ѡ�������龰��
	// ����ʽ��ֱ�Ӳ��������⣬
	// ˳����ʽ����ά���������⣬��Ҫ��¼�Ƿ���н�����죬��������������Ϣ
	// ���й���ʽ��ÿ������������һ��ά�ȣ���Ҫ��¼������Ⱥ��������Ϣ
	// 
	// ����ʽ�͹���ʽ����Ĳ��죺
	// ��������в�����Լ������Ӧ�õĻ��ڣ��������д���
	// ���ǹ���ʽ�Ἣ�����ӿ�ܺ����ӵ�����Ѷ�
	// ͬʱ���������Ӳ���֧�ְ�λ����
	// 
	// ���ۣ�ֻ֧������ʽ�����ṩ����ʽ��λ���ɵķ��������Կ��Ǻ�������ӣ�
	// 


	// ��������ͱ�����Ŵ��㷨ѧϰ���Կ��
	class GAStrategy : public LearningStrategy
	{
	private:
		Mutation* _mutation_func;
		Crossover* _crossover_func;
	public:
		GAStrategy(Mutation* mutation_func, Crossover* crossover_func)
			:LearningStrategy()
		{
			_mutation_func = mutation_func;
			_crossover_func = crossover_func;
		}

		~GAStrategy()
		{
			delete _mutation_func;
			delete _crossover_func;
		}

		static void preAssert(AssertList& list, double* paras)
		{
			CrossoverType ctype = CrossoverType(paras[0]);
			AssertList* clist = CrossoverFactory::preAssert(ctype, paras + 1);
			for (int i = 0; i < clist->getSize(); i++)
			{
				list.add(&(*clist)[i]);
			}
			delete clist;

			MutationType mtype = MutationType(paras[SUBPARA]);
			AssertList* mlist = MutationFactory::preAssert(mtype, paras + 1 + SUBPARA);
			for (int i = 0; i < mlist->getSize(); i++)
			{
				list.add(&(*mlist)[i]);
			}
			delete mlist;
		}

		static void postAssert(AssertList& list, double* paras)
		{
			CrossoverType ctype = CrossoverType(paras[0]);
			AssertList* clist = CrossoverFactory::postAssert(ctype, paras + 1);
			for (int i = 0; i < clist->getSize(); i++)
			{
				list.add(&(*clist)[i]);
			}
			delete clist;

			MutationType mtype = MutationType(paras[SUBPARA]);
			AssertList* mlist = MutationFactory::postAssert(mtype, paras + 1 + SUBPARA);
			for (int i = 0; i < mlist->getSize(); i++)
			{
				list.add(&(*mlist)[i]);
			}
			delete mlist;
		}

		void setProblem(ProblemHandle* problem_handle)
		{
			LearningStrategy::setProblem(problem_handle);
			_crossover_func->setProblem(problem_handle);
			_mutation_func->setProblem(problem_handle);
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