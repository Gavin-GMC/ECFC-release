//------------------------Description------------------------
// 
//-------------------------Reference-------------------------
//-------------------------Copyright-------------------------
// Copyright (c) 2022 ���������ƣ���ȷ�ϣ�, All Rights Reserved.
// You are free to use the myEC����ȷ�ϣ� for research purposes.
// All publications which use this library or any code in the library
// should acknowledge the use of "myEC" and reference "δȷ��"
//-----------------------------------------------------------

#pragma once
#include"subswarm.h"
#include"subswarm-neighbor.h"

namespace ECFC
{
	/// <summary>
	/// ����ģ��
	/// </summary>
	class TopologyModel
	{
	public:
		NeighborSwarm* neighborhoods;

		TopologyModel()
		{
			neighborhoods = nullptr;
		}

		virtual ~TopologyModel()
		{
			delete[] neighborhoods;
		}

		virtual void build(Subswarm** subswarms, int& swarm_number) = 0;

		virtual void ini(Subswarm** subswarms, int& swarm_number) = 0;

		virtual void update(Subswarm** subswarms, int& swarm_number) {}
	};
}