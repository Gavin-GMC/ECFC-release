//------------------------Description------------------------
// This file is the factory class of BestArchive classes,
// which is used to build objects from types
//-------------------------Reference-------------------------
//-------------------------Copyright-------------------------
// Copyright (c) 2024 ���������ƣ���ȷ�ϣ�, All Rights Reserved.
// You are free to use the ECFC����ȷ�ϣ� for research purposes.
// All publications which use this library or any code in the library
// should acknowledge the use of "ECFC" and reference 
// "δȷ��"
//-----------------------------------------------------------

#pragma once
#include"ecfc-constant.h"
#include"bestarchive-type.h"
#include"best-archive.h"

namespace ECFC
{
	class ArchiveFactory
	{
	public:
		static BestArchive* produce(BestArchiveType type)
		{
			switch (type)
			{
			case ECFC::BestArchiveType::F_normal:
				return new BasicArchive();
			case ECFC::BestArchiveType::F_multiobjective:
				return new MultiobjectArchive();
			case ECFC::BestArchiveType::F_multimodal:
				return new MultimodalArchive();
			case ECFC::BestArchiveType::F_default:
			case ECFC::BestArchiveType::F_end:
			default:
				return nullptr;
			}
		}

		static size_t typeSize(BestArchiveType type)
		{
			switch (type)
			{
			case ECFC::BestArchiveType::F_normal:
				return sizeof(BasicArchive);
			case ECFC::BestArchiveType::F_multiobjective:
				return sizeof(MultiobjectArchive);
			case ECFC::BestArchiveType::F_multimodal:
				return sizeof(MultimodalArchive);
			case ECFC::BestArchiveType::F_default:
			case ECFC::BestArchiveType::F_end:
			default:
				return 0;
			}
		}
	};
}
