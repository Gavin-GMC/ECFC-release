//------------------------Description------------------------
// This file defines the learning topology of individuals, which
// represents the learning objects selection approaches of individuals.
//-------------------------Reference-------------------------
//-------------------------Copyright-------------------------
// Copyright (c) 2024 ���������ƣ���ȷ�ϣ�, All Rights Reserved.
// You are free to use the ECFC����ȷ�ϣ� for research purposes.
// All publications which use this library or any code in the library
// should acknowledge the use of "ECFC" and reference 
// "δȷ��"
//-----------------------------------------------------------

#pragma once
#include"population.h"
#include"best-archive.h"
#include"learning-graph.h"

namespace ECFC
{
	class InSwarmTopology
	{
	public:
		InSwarmTopology() {}

		virtual ~InSwarmTopology()
		{

		}

		virtual void ini() = 0;

		virtual LearningGraph* getTopology(Population** subswarm, BestArchive** best_holder, const int swarm_number) = 0;
	};
}
