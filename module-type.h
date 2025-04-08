//------------------------Description------------------------
// This file is consist of all optimizer module types that 
// support user-defined in the library
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
	enum class ModuleType {
		T_individual,
		T_learnstrategy,
		T_learntopology,
		T_offspringgenerator,
		T_selector,
		T_subswarbuilder,
		T_subswarmtopology,
		T_subswarmmanager,
		T_bestarchive
	};
}
