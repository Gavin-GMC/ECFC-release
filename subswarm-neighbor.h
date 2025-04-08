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

namespace ECFC
{
	struct NeighborSwarm
	{
	private:
		int neighbor_number;
		Subswarm** neighborhood;
	public:
		NeighborSwarm()
		{
			neighbor_number = -1;
			neighborhood = nullptr;
		}

		~NeighborSwarm()
		{
			delete[] neighborhood;
		}

		void setSize(int size)
		{
			if (neighbor_number > 0)
			{
				delete[] neighborhood;
			}
			neighbor_number = size;
			if (neighbor_number > 0)
				neighborhood = new Subswarm * [size];
		}

		int getSize()
		{
			return neighbor_number;
		}

		Subswarm** getNSet()
		{
			return neighborhood;
		}

		Subswarm*& operator[](int i)
		{
			return neighborhood[i];
		}
	};
}