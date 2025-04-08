//------------------------Description------------------------
// This file is consist of all offspring generator model types that is 
// supported by the library
//-------------------------Reference-------------------------
//-------------------------Copyright-------------------------
// Copyright (c) 2024 ���������ƣ���ȷ�ϣ�, All Rights Reserved.
// You are free to use the ECFC����ȷ�ϣ� for research purposes.
// All publications which use this library or any code in the library
// should acknowledge the use of "ECFC" and reference 
// "δȷ��"
//-----------------------------------------------------------
#pragma once

namespace ECFC
{
	enum class OffspringGeneratorType
	{
		F_default,
		F_generation_nocheck,
		F_generation,
		F_construct_order,
		F_construct_parallel,
		F_end
	};
}