//------------------------Description------------------------
// This file defines the factory object of individual initializers.
// It can product different individual initializer based on types.
//-------------------------Reference-------------------------
//-------------------------Copyright-------------------------
// Copyright (c) 2024 ���������ƣ���ȷ�ϣ�, All Rights Reserved.
// You are free to use the ECFC����ȷ�ϣ� for research purposes.
// All publications which use this library or any code in the library
// should acknowledge the use of "ECFC" and reference 
// "δȷ��"
//-----------------------------------------------------------

#pragma once
#include"initializer-type.h"
#include"solution-initializer.h"

namespace ECFC
{
	class InitializerFactory
	{
	public:
		static SolutionInitializer* newInitializer(InitializerType type)
		{
			switch (type)
			{
			case ECFC::InitializerType::F_random:
				return new RandomInitializer();
			case ECFC::InitializerType::F_greedy:
				return new GreedyInitializer();
			case ECFC::InitializerType::F_no:
				return new NoInitializer();
			case ECFC::InitializerType::F_default:
			case ECFC::InitializerType::F_end:
			default:
				return nullptr;
			}
		}

		static size_t typeSize(InitializerType type)
		{
			switch (type)
			{
			case ECFC::InitializerType::F_random:
				return sizeof(RandomInitializer);
			case ECFC::InitializerType::F_greedy:
				return sizeof(GreedyInitializer);
			case ECFC::InitializerType::F_no:
				return sizeof(NoInitializer);
			case ECFC::InitializerType::F_default:
			case ECFC::InitializerType::F_end:
			default:
				return 0;
			}
		}

	};
}
