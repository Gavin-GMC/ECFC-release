//------------------------Description------------------------
// This file defines environment selection object. It represents 
// the process by which offspring and parents form new populations 
// together. In this process, individuals who are not adapted to
//  their environment may be eliminated, thus promoting population
//  evolution and optimization.
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
#include"sort-helper.h"
#include"solution.h"
#include"population.h"

namespace ECFC
{
	// ��������cpp
	void replacer(Individual& offspring, Individual& parent)
	{
		parent.swap(offspring);
		parent.rebirth();
	}

	void betterReplacer(Individual& offspring, Individual& parent)
	{
		if (offspring < parent)
		{
			replacer(offspring, parent);
			parent.rebirth();
		}
	}

	class EnvirSelect
	{
	protected:
		void (*replace)(Individual& offspring, Individual& parent);
	public:
		EnvirSelect(bool better_replace)
		{
			if (better_replace)
				replace = betterReplacer;
			else
				replace = replacer;
		}

		virtual ~EnvirSelect()
		{

		}

		virtual void update_subswarm(Population& parent, Population& offspring) = 0;
	};
}
