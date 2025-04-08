//------------------------Description------------------------
// This file is consisted of some system funcions of ECFC.
//-------------------------Reference-------------------------
//-------------------------Copyright-------------------------
// Copyright (c) 2024 ���������ƣ���ȷ�ϣ�, All Rights Reserved.
// You are free to use the ECFC����ȷ�ϣ� for research purposes.
// All publications which use this library or any code in the library
// should acknowledge the use of "ECFC" and reference 
// "δȷ��"
//-----------------------------------------------------------

#pragma once
#include"basicfunc.h"
#include"logger.h"

namespace ECFC
{
	//system environment initialization
	//ϵͳ������ʼ��
	void sys_environment_ini()
	{
		try
		{
			dir_create("_log");
			dir_create("_config");
			dir_create("_resl");
		}
		catch(std::exception e)
		{
			sys_logger.error("Failing create system dictionary!");
		}

	}
}