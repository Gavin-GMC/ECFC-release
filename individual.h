//------------------------Description------------------------
// This file defines the basic individual object. It is the 
// fundamental component of ECFC optimizer, which maintains
// a candidate solution and providing operation such as 
// initialization and comparison.
//-------------------------Reference-------------------------
//-------------------------Copyright-------------------------
// Copyright (c) 2024 ���������ƣ���ȷ�ϣ�, All Rights Reserved.
// You are free to use the ECFC����ȷ�ϣ� for research purposes.
// All publications which use this library or any code in the library
// should acknowledge the use of "ECFC" and reference 
// "δȷ��"
//-----------------------------------------------------------

#pragma once
#include"module-type.h"
#include"solution.h"
#include"comparer.h"
#include"problem-handle.h"
#include"solution-initializer.h"
#include"assert.h"
#include"logger.h"

namespace ECFC
{
	class Individual
	{
	protected:
		Comparer* _comparer_pointer;
		SolutionInitializer* _initializer_pointer;
		int _age;

	public:
		Solution solution;

		Individual()
		{
			solution = Solution();
			_comparer_pointer = nullptr;
			_initializer_pointer = nullptr;
			_age = 0;
		}

		virtual ~Individual()
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

		int getSolutionSize()
		{
			return solution.getSolutionSize();
		}

		int getObjectNumber()
		{
			return solution.getObjectNumber();
		}

		virtual void setProblem(ProblemHandle* problem_handle)
		{
			solution.setSize(problem_handle->getProblemSize(), problem_handle->getObjectNumber());
			solution.setDecoder(problem_handle->getSolutionDecoder());
			_comparer_pointer = problem_handle->getSolutionComparer();
		}

		virtual void setProblem(const Individual& source)
		{
			solution.setSize(source.solution);
			solution.setDecoder(source.solution);
			_comparer_pointer = source._comparer_pointer;
		}

		void setInitializer(SolutionInitializer* initializer)
		{
			_initializer_pointer = initializer;
		}

		int getAge()
		{
			return _age;
		}

		void  rebirth()
		{
			_age = 0;
		}

		void ageing()
		{
			_age++;
		}

		virtual void ini(bool ini_solution = true, bool evaluate = true, bool ini_speciality = false)
		{
			if (ini_solution)
			{
				if (evaluate)
				{
					_initializer_pointer->ini_solution(solution);
				}
				else
				{
					_initializer_pointer->ini_solution(solution.result, solution.getSolutionSize());
				}
			}
			_age = 0;
		}

		virtual void copy(const Individual& copy_source)
		{
			setProblem(copy_source);
			//�Խ����������Ӧ��ֵ�������
			solution.copy(copy_source.solution);
			//���ó�ʼ����
			setInitializer(copy_source._initializer_pointer);
		}

		virtual void copy(const double* source_result, const double* source_fitness)
		{
			//�Խ����������Ӧ��ֵ�������
			solution.copy(source_result, source_fitness);
		}

		virtual void shallowCopy(const Individual& copy_source)
		{
			solution.shallowCopy(copy_source.solution);
			_comparer_pointer = copy_source._comparer_pointer;
			_initializer_pointer = copy_source._initializer_pointer;
		}

		virtual void shallowClear()
		{
			solution.shallowClear();
			_comparer_pointer = nullptr;
			_initializer_pointer = nullptr;
		}

		virtual void swap(Individual& copy_source)
		{
			solution.swap(copy_source.solution);
		}

		double& operator[](const int index)
		{
			return solution[index];
		}

		bool operator<(const Individual& a)const
		{
			return _comparer_pointer->isBetter(solution.fitness, a.solution.fitness);
		}

		bool operator==(const Individual& a)const
		{
			return solution == a.solution;
		}
	};
}
