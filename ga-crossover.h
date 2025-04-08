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
#include"ecfc-constant.h"
#include"module-type.h"
#include"solution.h"
#include"problem-handle.h"
#include"assert.h"

namespace ECFC
{
	class Crossover
	{
	protected:
		double _cross_rate;
		bool new_pair;
		bool is_crossover;
		bool _coupled;

	public:
		Crossover(double cross_rate, double coupled = true)
		{
			_cross_rate = cross_rate;
			_coupled = coupled;
			new_pair = true;
			is_crossover = false;
		}

		virtual ~Crossover() {}

		static void preAssert(AssertList& list, double* paras)
		{
			// none
		}

		static void postAssert(AssertList& list, double* paras)
		{
			// none
		}

		virtual void setProblem(ProblemHandle* problem_handle)
		{

		}

		virtual void ini()
		{
			new_pair = true;
		}

		// ����ǰ��Ԥ������
		virtual void preparation(Solution* s)
		{
			if (new_pair)
				is_crossover = _cross_rate > rand01_();
		}

		// ��������β����
		virtual void ending()
		{
			if(_coupled)
				new_pair = !new_pair;
		}

		// ����ά�ȵĽ���
		virtual double apply(Solution* s1, Solution** s2, const int demension, ProblemHandle* handle)
		{
			// ����

			// �˳�
			exit(-1);
		}

		// ���ڽ�����Ľ���
		virtual void apply(Solution* child, Solution* s1, Solution** s2, ProblemHandle* handle) = 0;
	};
}