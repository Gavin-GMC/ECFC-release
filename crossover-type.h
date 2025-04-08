//------------------------Description------------------------
// This file is consist of all Crossover types that is 
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
	enum class CrossoverType
	{
		F_default,
		F_point,
		F_SBX,
		F_uniform,
		F_difference,
		F_no,
		F_partialmap,
		F_cycle,
		F_order,
		F_subtourExchange,
		F_positionBased,
		F_end
	};
}