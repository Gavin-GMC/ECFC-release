//------------------------Description------------------------
// This file defines the repair methods of solutions, for repairing
// the illegal solutions
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
#include"problem-handle.h"

namespace ECFC
{
	class SolutionRepaireman
	{
	public:
		virtual double repair(const Solution& solution, int demensionId, ProblemHandle* problem_handle) = 0;
	};

	class BoundaryRepair final : public SolutionRepaireman
	{
	public:
		double repair(const Solution& solution, int demensionId, ProblemHandle* problem_handle)
		{
            double left = problem_handle->getBoundaryChoice(demensionId);
            if (solution.result[demensionId] < left)
                return left;
            return problem_handle->getBoundaryChoice(demensionId, false);
		}
	};

	class RandomValueRepair final : public SolutionRepaireman
	{
	public:
		double repair(const Solution& solution, int demensionId, ProblemHandle* problem_handle)
		{
			return problem_handle->getRandomChoiceInspace(demensionId);
		}
	};

	class GreedyRepair final : public SolutionRepaireman
	{
	public:
		double repair(const Solution& solution, int demensionId, ProblemHandle* problem_handle)
		{
			return problem_handle->getPrioriChoice(demensionId);
		}
	};

	class NoRepair final : public SolutionRepaireman
	{
	public:
		double repair(const Solution& solution, int demensionId, ProblemHandle* problem_handle)
		{
			return solution.result[demensionId];
		}
	};
}
