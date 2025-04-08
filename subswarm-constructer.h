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
	class SubswarmConstructer
	{
	public:
		SubswarmConstructer() {}

		virtual ~SubswarmConstructer()
		{

		}

		virtual void build(Subswarm** subswarms, int& swarm_number) = 0;

		virtual void ini(Subswarm** subswarms, int& swarm_number)
		{
			build(subswarms, swarm_number);
		};
	};
}
